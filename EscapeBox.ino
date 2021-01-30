// ********************************************************************************************* //
// ********************************************************************************************* //
// ***************************************** ESCAPEBOX ***************************************** //
// ********************************************************************************************* //
// ********************************************************************************************* //
// AUTOR:   Imanol Padillo
// DATE:    21/12/2020
// VERSION: 1.0

// Board: Arduino Mega
// Processor: Atmega 2560 (Mega 2560)
// Port: /dev/cu.wchusbserial1420

// Arduino project for developing an escape box, including the following steps:
// Game_step_0:
//   Place in order a set of wires with the clue of some leds
// Game_step_1:
//

 

// ********************************************************************************************* //
// INCLUDES 
// ********************************************************************************************* //
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <TinyGPS++.h>
#include "Morse.h"
#include "Melody.h"
#include "LedControl.h"
#include "TM1637.h"

// ********************************************************************************************* //
// PASSWORDS
// ********************************************************************************************* //
// KEYPAD COMMANDS
#define CMD_RESET_RECORDING_TIME          "A311252"
/*#define CMD_GOTO_1                        "A000000"
#define CMD_GOTO_2                        "A667750"
#define CMD_GOTO_3                        "A566777"
#define CMD_GOTO_4                        "A756778"
#define CMD_GOTO_5                        "A465887"
#define CMD_GOTO_6                        "A954644"
#define CMD_GOTO_7                        "A173499"
#define CMD_GOTO_8                        "A249433"
#define CMD_GOTO_9                        "A445346"
#define CMD_GOTO_END                      "A592288"*/
#define CMD_GOTO_1                        "A111111"
#define CMD_GOTO_2                        "A222222"
#define CMD_GOTO_3                        "A333333"
#define CMD_GOTO_4                        "A444444"
#define CMD_GOTO_5                        "A555555"
#define CMD_GOTO_6                        "A666666"
#define CMD_GOTO_7                        "A777777"
#define CMD_GOTO_8                        "A888888"
#define CMD_GOTO_9                        "A999999"
#define CMD_GOTO_END                      "AAAAAAA"
// GAME STEP 1: simon
#define SIMON_SEQUENCE_0                  "GGBB"
#define SIMON_SEQUENCE_1                  "GGGBBB"
#define SIMON_SEQUENCE_2                  "GGGBBB"
#define SIMON_SEQUENCE_3                  "GGGBBB"
// GAME STEP 2: rfid
#define RFID_WHITE                        "3221477164"
#define RFID_BLUE                         "575574115"
// GAME STEP 3: ledmatrix
#define GS3_PASSWORD                      "1234"
// GAME STEP 4: joystick   
const String GS4_PASSWORD  =              "LLRRUD";
// GAME STEP 5: encoder
#define YEAR_0                            1510
#define YEAR_1                            1511
#define YEAR_2                            1512
#define YEAR_3                            1513
// GAME STEP 6: display
#define GS7_PASSWORD                      "DIGNIDAD"
// GAME STEP 7: gps
const double GPS_TARGET_LATITUDE =          42.841465;    
const double GPS_TARGET_LONGITUDE =         -2.66903;           
   

// ********************************************************************************************* //
// GPIO
// ********************************************************************************************* //
// CLUES
#define GPIO_CLUE                         2
#define GPIO_BUZZER                       38
#define GPIO_TIMER_CLK                    48
#define GPIO_TIMER_DIO                    49
// GAME STEP LEDS
#define GPIO_LED_1                        61
#define GPIO_LED_2                        62
#define GPIO_LED_3                        63
#define GPIO_LED_4                        64
#define GPIO_LED_5                        65
#define GPIO_LED_6                        66
#define GPIO_LED_7                        67
#define GPIO_LED_8                        68
#define GPIO_LED_9                        69
#define GPIO_MOTOR                        16
#define GPIO_LOCKER                       17
// GAME STEP 0: wires + leds
#define GPIO_PULSE                        3
#define GPIO_LED_R                        22
#define GPIO_LED_G                        24
#define GPIO_LED_B                        26
#define GPIO_LED_W                        28
#define GPIO_WIRE_INPUT                   30
// GAME STEP 1: simon
#define GPIO_SIMON_G_L                    8
#define GPIO_SIMON_G_P                    9
#define GPIO_SIMON_B_L                    10
#define GPIO_SIMON_B_P                    11
#define GPIO_SIMON_Y_L                    57
#define GPIO_SIMON_Y_P                    58
#define GPIO_SIMON_R_L                    59
#define GPIO_SIMON_R_P                    60
// GAME STEP 2: rfid
#define GPIO_RFID_RST                     34
#define GPIO_RFID_SDA                     36
//#define GPIO_RFID_SCK                   52  
//#define GPIO_RFID_MOSI                  51 
//#define GPIO_RFID_MISO                  50
// GAME STEP 3: led matrix
#define GPIO_KP_C1                        23
#define GPIO_KP_C2                        25
#define GPIO_KP_C3                        27
#define GPIO_KP_C4                        29
#define GPIO_KP_R1                        31
#define GPIO_KP_R2                        33
#define GPIO_KP_R3                        35
#define GPIO_KP_R4                        37
#define GPIO_LMATRIX_CS                   53  // SS    
//#define GPIO_LMATRIX_CLK                52  // SCK
//#define GPIO_LMATRIX_DIN                51  // MOSI 
// GAME STEP 4: joystick   
#define GPIO_JOY_X                        A0
#define GPIO_JOY_Y                        A1       
// GAME STEP 5: rotary encoder
#define GPIO_ENC_CLK                      19
#define GPIO_ENC_DT                       18   
#define GPIO_ENC_SW                       40  
#define GPIO_PLAY_SONG                    32
#define GPIO_DATE1_CLK                    41
#define GPIO_DATE1_DIO                    39
#define GPIO_DATE2_CLK                    47
#define GPIO_DATE2_DIO                    45
#define GPIO_DATE3_CLK                    5
#define GPIO_DATE3_DIO                    4
#define GPIO_DATE4_CLK                    7
#define GPIO_DATE4_DIO                    6
// GAME STEP 6: display
#define GPIO_DISPLAY_DIN                  42
#define GPIO_DISPLAY_CS                   44
#define GPIO_DISPLAY_CLK                  46
// GAME STEP 7: switches
#define GPIO_SWITCHES_INPUT               12
// GAME STEP 8: gps
#define GPIO_MICROPHONE                   A2
#define GPIO_GPS_TX                       14
#define GPIO_GPS_RX                       15
#define GPIO_GPS_LED                      43

// ********************************************************************************************* //
// CONSTANTS
// ********************************************************************************************* //
#define MAX_STEPS                         10
#define MAX_CLUES                         3
#define RECORDING_MS                      60000
// lcd
#define LCD_ADDRESS                       0x27
#define LCD_SPEED                         300
#define LCD_BUFFER                        40
#define LCD_COLS                          16
#define LCD_ROWS                          2
#define TIMEOUT_SHORT                     2000
#define TIMEOUT_LARGE                     3500
// buzzer
#define BUZZER_KEY                        0
#define BUZZER_ERROR                      1
#define BUZZER_SUCCESS                    2
// led matrix
#define LMATRIX_H_DISPLAYS                1
#define LMATRIX_V_DISPLAYS                1
#define LMATRIX_SPACER                    1
#define LMATRIX_WIDTH                     6
// encoder
#define ENCODER_MAX_VAL_GS5               2200
#define ENCODER_MIN_VAL_GS5               1500
#define ENCODER_MAX_VAL_GS6               25
#define ENCODER_MIN_VAL_GS6               0
// gps
#define GPS_LED_TIMEOUT                   500
#define GPS_TARGET_DISTANCE               500  //meters
// microphone
#define MICROPHONE_LEVEL                  900
// buzzer
int melody_0[] = {
  NOTE_E5, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_C5,8,  NOTE_B4,8,
  NOTE_A4, 4,  NOTE_A4,8,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, -4,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,8,  NOTE_A4,4,  NOTE_B4,8,  NOTE_C5,8,
  NOTE_D5, -4,  NOTE_F5,8,  NOTE_A5,4,  NOTE_G5,8,  NOTE_F5,8,
  NOTE_E5, -4,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,4, REST, 4,
};
int melody_1[] = {
  NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8, 
  REST,8, NOTE_E5,8, REST,8, NOTE_E5,8, NOTE_GS5,8, NOTE_GS5,8, NOTE_A5,8, NOTE_B5,8,
  NOTE_A5,8, NOTE_A5,8, NOTE_A5,8, NOTE_E5,8, REST,8, NOTE_D5,8, REST,8, NOTE_FS5,8, 
  REST,8, NOTE_FS5,8, REST,8, NOTE_FS5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8,
  NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8, 
  };

int melody_2[] = {
  REST,2, REST,4, REST,8, NOTE_DS4,8, 
  NOTE_E4,-4, REST,8, NOTE_FS4,8, NOTE_G4,-4, REST,8, NOTE_DS4,8,
  NOTE_E4,-8, NOTE_FS4,8,  NOTE_G4,-8, NOTE_C5,8, NOTE_B4,-8, NOTE_E4,8, NOTE_G4,-8, NOTE_B4,8,   
  NOTE_AS4,2, NOTE_A4,-16, NOTE_G4,-16, NOTE_E4,-16, NOTE_D4,-16, 
  NOTE_E4,2,
  };
String morse_0 = ".-.. --- ... / .--. .. ... - --- .-.. . .-. --- ... / -.. . .-.. / . -.-. .-.. .. .--. ... .";

int melody_end[] = {
  NOTE_AS4,8, NOTE_AS4,8, NOTE_AS4,8,//1
  NOTE_F5,2, NOTE_C6,2,
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4,  
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4,  
  NOTE_AS5,8, NOTE_A5,8, NOTE_AS5,8, NOTE_G5,2, NOTE_C5,8, NOTE_C5,8, NOTE_C5,8,
  NOTE_F5,2, NOTE_C6,2,
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4,  
  
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4, //8  
  NOTE_AS5,8, NOTE_A5,8, NOTE_AS5,8, NOTE_G5,2, NOTE_C5,-8, NOTE_C5,16, 
  NOTE_D5,-4, NOTE_D5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  NOTE_F5,8, NOTE_G5,8, NOTE_A5,8, NOTE_G5,4, NOTE_D5,8, NOTE_E5,4,NOTE_C5,-8, NOTE_C5,16,
  NOTE_D5,-4, NOTE_D5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  
  NOTE_C6,-8, NOTE_G5,16, NOTE_G5,2, REST,8, NOTE_C5,8,//13
  NOTE_D5,-4, NOTE_D5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  NOTE_F5,8, NOTE_G5,8, NOTE_A5,8, NOTE_G5,4, NOTE_D5,8, NOTE_E5,4,NOTE_C6,-8, NOTE_C6,16,
  NOTE_F6,4, NOTE_DS6,8, NOTE_CS6,4, NOTE_C6,8, NOTE_AS5,4, NOTE_GS5,8, NOTE_G5,4, NOTE_F5,8,
  NOTE_C6,1
  
  };

String LMATRIX_TAPE = "www.escapebox.com";
// Messages                                1234567890123456789012345678901234567890
#define MSG_INIT_1                        "   ESCAPE BOX   "
#define MSG_INIT_2                        "  Mai y Mario   "

const String MSG_GAMESTEP[MAX_STEPS][LCD_ROWS] = {
  {"Prueba 1        ", "Cables y leds   "},
  {"Prueba 2        ", "Simon           "},
  {"Prueba 3        ", "rfid            "},
  {"Prueba 4        ", "led matrix      "},
  {"Prueba 5        ", "joystick        "},
  {"Prueba 6        ", "dates           "},
  {"Prueba 7        ", "dignity         "},
  {"Prueba 8        ", "switches        "},
  {"Prueba 9        ", "gps             "},
  {"Felicidades!", "Tiempo: " + String(EEPROM.read(0)*1000 + EEPROM.read(1)*100 + EEPROM.read(2)*10 + 
      EEPROM.read(3)) + " min"}
};
const String MSG_CLUE_1[MAX_STEPS][MAX_CLUES] = {
  {"Prueba1 - Pista1", "Prueba1 - Pista2", "Prueba1 - Pista3"},
  {"Prueba2 - Pista1", "Prueba2 - Pista2", "Prueba2 - Pista3"},
  {"Prueba3 - Pista1", "Prueba3 - Pista2", "Prueba3 - Pista3"},
  {"Prueba4 - Pista1", "Prueba4 - Pista2", "Prueba4 - Pista3"},
  {"Prueba5 - Pista1", "Prueba5 - Pista2", "Prueba5 - Pista3"},
  {"Prueba6 - Pista1", "Prueba6 - Pista2", "Prueba6 - Pista3"},
  {"Prueba7 - Pista1", "Prueba7 - Pista2", "Prueba7 - Pista3"},
  {"Prueba8 - Pista1", "Prueba8 - Pista2", "Prueba8 - Pista3"},
  {"Prueba9 - Pista1", "Prueba9 - Pista2", "Prueba9 - Pista3"},
  {"Prueba10 - Pista1", "Prueba10 - Pista2", "Prueba10 - Pista3"}
};
const String MSG_CLUE_2[MAX_STEPS][MAX_CLUES] = {
  {"Prueba1 - Pista1", "Prueba1 - Pista2", "Prueba1 - Pista3"},
  {"Prueba2 - Pista1", "Prueba2 - Pista2", "Prueba2 - Pista3"},
  {"Prueba3 - Pista1", "Prueba3 - Pista2", "Prueba3 - Pista3"},
  {"Prueba4 - Pista1", "Prueba4 - Pista2", "Prueba4 - Pista3"},
  {"Prueba5 - Pista1", "Prueba5 - Pista2", "Prueba5 - Pista3"},
  {"Prueba6 - Pista1", "Prueba6 - Pista2", "Prueba6 - Pista3"},
  {"Prueba7 - Pista1", "Prueba7 - Pista2", "Prueba7 - Pista3"},
  {"Prueba8 - Pista1", "Prueba8 - Pista2", "Prueba8 - Pista3"},
  {"Prueba9 - Pista1", "Prueba9 - Pista2", "Prueba9 - Pista3"},
  {"Prueba10 - Pista1", "Prueba10 - Pista2", "Prueba10 - Pista3"}
};
// ********************************************************************************************* //
// VARIABLES
// ********************************************************************************************* //
// lcd
LiquidCrystal_I2C lcd(LCD_ADDRESS, 16, 2);
String lcd_msg_1;
String lcd_msg_2;
bool lcd_scroll_flag = false;
long lcd_scroll_ms=millis();
// keypad
const char hexaKeys[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[4] = {GPIO_KP_R1, GPIO_KP_R2, GPIO_KP_R3, GPIO_KP_R4}; 
byte colPins[4] = {GPIO_KP_C1, GPIO_KP_C2, GPIO_KP_C3, GPIO_KP_C4}; 
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, 4, 4); 
// encoder
int encoder_max_val;
int encoder_min_val;
// display 8-digits
LedControl lc=LedControl(GPIO_DISPLAY_DIN,GPIO_DISPLAY_CLK,GPIO_DISPLAY_CS,1);
// display 4-digits
TM1637 recording_time(GPIO_TIMER_CLK,GPIO_TIMER_DIO);
// general
unsigned long loop_ms = millis();
unsigned long total_ms;
int game_step;
String last_clue;
bool flag_playing_clue = false;
//game_step 0
bool flag_playing_leds = false;
//game_step 1
int simon_step = 0;
bool simon_played = false;
String simon_input = "";
String simon_sequence;
char simon_last_led_actived = '\0';
//game_step 2
MFRC522 mfrc522(GPIO_RFID_SDA, GPIO_RFID_RST); 
//game_step 3
String password_val;
Max72xxPanel matrix = Max72xxPanel(GPIO_LMATRIX_CS, LMATRIX_H_DISPLAYS, LMATRIX_V_DISPLAYS);
int lmatrix_index = 0;
//game_step 4
char joy_last_state = ' ';
String joy_val="";
//game_step 5
const int timeThreshold = 5;
long timeCounter = 0;
int encoder_val = ENCODER_MIN_VAL_GS5;
int step_minus = 0;
int step_plus = 0;
Morse morse(GPIO_BUZZER);
Melody melody(GPIO_BUZZER);
int song_index=0;


TM1637 date1(GPIO_DATE1_CLK,GPIO_DATE1_DIO);
TM1637 date2(GPIO_DATE2_CLK,GPIO_DATE2_DIO);
TM1637 date3(GPIO_DATE3_CLK,GPIO_DATE3_DIO);
TM1637 date4(GPIO_DATE4_CLK,GPIO_DATE4_DIO);

//game_step 6: display
String display_val="";
char letter;

//game_step 7: gps
TinyGPSPlus gps;
bool gps_led_status = false;
bool gps_in_target = false;
long gps_led_ms = 0;
double latitude;
double longitude;

// game_step finsih
bool melody_end_flag = false;

// ********************************************************************************************* //
// SETUP
// ********************************************************************************************* //
void setup() {
  // Setup digital pins
  pinMode(GPIO_CLUE, INPUT_PULLUP);  
  attachInterrupt(digitalPinToInterrupt(GPIO_CLUE), activate_flag_playing_clue, FALLING);
  pinMode(GPIO_BUZZER, OUTPUT); 
  // GAME STEP leds
  pinMode(GPIO_LED_1, OUTPUT);
  pinMode(GPIO_LED_2, OUTPUT);
  pinMode(GPIO_LED_3, OUTPUT);
  pinMode(GPIO_LED_4, OUTPUT);
  pinMode(GPIO_LED_5, OUTPUT);
  pinMode(GPIO_LED_6, OUTPUT);
  pinMode(GPIO_LED_7, OUTPUT);
  pinMode(GPIO_LED_8, OUTPUT);
  pinMode(GPIO_MOTOR, OUTPUT);
  pinMode(GPIO_LOCKER, OUTPUT);
  // GAME STEP 0: wires + leds
  pinMode(GPIO_LED_R, OUTPUT);  
  pinMode(GPIO_LED_G, OUTPUT);  
  pinMode(GPIO_LED_B, OUTPUT);  
  pinMode(GPIO_LED_W, OUTPUT);  
  pinMode(GPIO_PULSE, INPUT_PULLUP);  
  attachInterrupt(digitalPinToInterrupt(GPIO_PULSE), activate_flag_playing_leds, FALLING);
  pinMode(GPIO_WIRE_INPUT, INPUT_PULLUP);
  // GAME STEP 1: simon
  pinMode(GPIO_SIMON_G_L, OUTPUT);
  pinMode(GPIO_SIMON_G_P, INPUT);
  pinMode(GPIO_SIMON_B_L, OUTPUT);
  pinMode(GPIO_SIMON_B_P, INPUT);
  pinMode(GPIO_SIMON_Y_L, OUTPUT);
  pinMode(GPIO_SIMON_Y_P, INPUT);
  pinMode(GPIO_SIMON_R_L, OUTPUT);
  pinMode(GPIO_SIMON_R_P, INPUT);
  // GAME STEP 2: rfid
  // GAME STEP 3: led matrix
  pinMode(GPIO_KP_R1, INPUT);
  pinMode(GPIO_KP_R2, INPUT);
  pinMode(GPIO_KP_R3, INPUT);
  pinMode(GPIO_KP_R4, INPUT);
  pinMode(GPIO_KP_C1, INPUT);
  pinMode(GPIO_KP_C2, INPUT);
  pinMode(GPIO_KP_C3, INPUT);
  pinMode(GPIO_KP_C4, INPUT);
  // GAME STEP 4: joystick 
  pinMode(GPIO_JOY_X, INPUT);
  pinMode(GPIO_JOY_Y, INPUT);
  // GAME STEP 5: rotary encoder
  pinMode(GPIO_ENC_CLK, INPUT_PULLUP);
  pinMode(GPIO_ENC_DT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(GPIO_ENC_CLK), doEncodeCLK, CHANGE);
  attachInterrupt(digitalPinToInterrupt(GPIO_ENC_DT), doEncodeDT, CHANGE);
  pinMode(GPIO_ENC_SW, INPUT);
  digitalWrite(GPIO_ENC_SW, HIGH);
  pinMode(GPIO_PLAY_SONG, INPUT);
  // GAME STEP 7: switches
  pinMode(GPIO_SWITCHES_INPUT, INPUT_PULLUP);
  // GAME STEP 8: gps
  pinMode(GPIO_GPS_LED, OUTPUT);
  
  // Setup serial comm
  Serial.begin(9600);  

  // Setup Uno serial comm
  //Serial2.begin(9600);

  // Setup gps serial comm
  Serial3.begin(9600);

  // Setup SPI and rfid
  SPI.begin();
  mfrc522.PCD_Init();

  // Init lcd
  lcd.begin();                      
  lcd.backlight();
  lcd_print(MSG_INIT_1,MSG_INIT_2,TIMEOUT_LARGE);

  // Init led matrix
  matrix.fillScreen(0);
  matrix.write();

  // Init display 8-digits
  //lc.shutdown(0,false);      // switch off energy saver
  lc.setIntensity(0,2);      // set brightness
  lc.clearDisplay(0);        // clear screen

  // Init display 4-digits
  recording_time.init();
  recording_time.set(BRIGHT_TYPICAL);
  date1.init();
  date1.set(BRIGHT_TYPICAL);
  date2.init();
  date2.set(BRIGHT_TYPICAL);
  date3.init();
  date3.set(BRIGHT_TYPICAL);
  date4.init();
  date4.set(BRIGHT_TYPICAL);
  
  // set game_step
  read_game_step();
  play_game_step_leds();

  // show time
  increment_recording_time(0);
  
}

// ********************************************************************************************* //
// MAIN
// ********************************************************************************************* //
void loop() {

  
  // Clue
  if (flag_playing_clue == true){
      play_clue(); 
  }

  //lcd info
  lcd_print(MSG_GAMESTEP[game_step][0],MSG_GAMESTEP[game_step][1],0);  

  //read keypad
  char customKey = customKeypad.getKey();
  if (isAlphaNumeric(customKey) or isDigit(customKey)
    or customKey == '*'){
    play_buzzer(BUZZER_KEY);
    password_val = password_val + customKey;
  }
  else if(customKey == '#'){
    if (check_password("")==true){
      customKey = '\0';
    }
  }

  int aaa=analogRead(GPIO_MICROPHONE);
  if (aaa>500){
    Serial.println(aaa);
  }
  
  // GAME_STEP 0
  if (game_step == 0)
  {
    if (digitalRead(GPIO_WIRE_INPUT) == LOW){
      increment_game_step();
    }
    else if (flag_playing_leds == true){
      play_leds(); 
    }
  }
  // GAME_STEP_1
  else if (game_step == 1)
  {
    if (simon_played == false){
      if (simon_step == 0){
        simon_sequence = SIMON_SEQUENCE_0;
        play_simon(300,300);
        simon_played = true;
      }
      else if (simon_step == 1){
        simon_sequence = SIMON_SEQUENCE_1;
        play_simon(150,150);
        simon_played = true;
      }
      else if (simon_step == 2){
        simon_sequence = SIMON_SEQUENCE_2;
        play_simon(120,120);
        simon_played = true;
      }
      else if (simon_step == 3){
        simon_sequence = SIMON_SEQUENCE_3;
        play_simon(100,100);
        simon_played = true;
      }
    }
    else{
      if(check_simon()==true){
          simon_step = simon_step + 1;
          simon_played = false;
          delay(1000);
      }
      if (simon_step == 4){
        increment_game_step();
      }
    }
  }
  // GAME_STEP_2
  else if (game_step == 2)
  {
    if (read_rfid() == true){
        increment_game_step();
    }
  }
  // GAME_STEP_3
  else if (game_step == 3)
  {
    if (lmatrix_index < LMATRIX_WIDTH * LMATRIX_TAPE.length() + matrix.width() - 1 - LMATRIX_SPACER) {
      delay(100);
      lmatrix_write(lmatrix_index);
      lmatrix_index++;
    }
    else{
      lmatrix_index = 0;
    }
    // read keypad
    if (customKey == '#'){
      if (check_password(GS3_PASSWORD) == true){
        increment_game_step();
        // empty led matrix
        matrix.fillScreen(0);
        matrix.write();
      }
    }
  }
  // GAME_STEP_4
  else if (game_step == 4)
  {
    if (check_joystick() == true){
        increment_game_step();
    }
  }
  // GAME_STEP_5
  else if (game_step == 5)
  {
    encoder_max_val = ENCODER_MAX_VAL_GS5;
    encoder_min_val = ENCODER_MIN_VAL_GS5;
    if (check_date() == true){
       encoder_val=0;
       increment_game_step();
    }
  }
  // GAME_STEP_6
  else if (game_step == 6)
  {
    if (encoder_max_val != ENCODER_MAX_VAL_GS6){
      set_digit8_password();
    }
    
    if (encoder_val>encoder_max_val){
      encoder_val = encoder_min_val;
    }
    if (check_word() == true){
      encoder_val=0;
      lc.clearDisplay(0);   
      matrix.fillScreen(0);
      matrix.write();
      increment_game_step();
    }
  }

  // GAME_STEP_7
  else if (game_step == 7)
  {
    if (digitalRead(GPIO_SWITCHES_INPUT) == LOW){
      increment_game_step();
    }
  }

  // GAME_STEP_8
  else if (game_step == 8)
  {
    if(gps_in_target == false){
      play_gps_led();
      while (Serial3.available()>0) {
        char c = Serial3.read();
        //Serial.write(c);
        gps.encode(c);
      }
      //Serial.print("Sentences that failed checksum=");
      //Serial.println(gps.failedChecksum());

      if (check_gps() == true){
        gps_in_target = true;
      }
    }

    if (gps_in_target == true and analogRead(GPIO_MICROPHONE)>MICROPHONE_LEVEL){
      increment_game_step();
      //Serial.println("End");
    }
  }

  // FINISH
  else if (game_step == MAX_STEPS-1 and melody_end_flag == false)
  {
    MSG_GAMESTEP[MAX_STEPS-1][0] = "Felicidades!";
    MSG_GAMESTEP[MAX_STEPS-1][1] = "Tiempo: " + String(EEPROM.read(0)*1000 + EEPROM.read(1)*100 + EEPROM.read(2)*10 + 
      EEPROM.read(3)) + " min";
    lcd_print(MSG_GAMESTEP[game_step][0],MSG_GAMESTEP[game_step][1],0); 
    melody.play_melody(melody_end, sizeof(melody_end));
    melody_end_flag = true;
  }

  // Increment recording time
  if((millis()-loop_ms>=RECORDING_MS) and (game_step<MAX_STEPS-1)){
    increment_recording_time(1);
  }

  // check error keypad command
  if (password_val != "" and customKey == '#'){
    password_val = "";
    play_buzzer(BUZZER_ERROR);
  }

}


// ********************************************************************************************* //
// ********************************************************************************************* //
// FUNCTIONS
// ********************************************************************************************* //
// ********************************************************************************************* //

// ********************************************************************************************* //
// GENERAL
// ********************************************************************************************* //

// Increment recording time
void increment_recording_time(int minutes)
{
  if (game_step < MAX_STEPS-1)
  {  
    loop_ms = millis();
    
    int current_minutes = EEPROM.read(0)*1000 + EEPROM.read(1)*100 + EEPROM.read(2)*10 + 
      EEPROM.read(3) + minutes;
    
    int i=0;
    char digits[4];  
    digits[3]=0;
    digits[2]=0;
    digits[1]=0;
    digits[0]=0;
    while(current_minutes>0 && i<4){
      digits[i++] = current_minutes % 10; // you can change the 10 here and below to any number base
      current_minutes /= 10;
    }

    //Serial.print("Recording time(m): ");
    //Serial.print(digits[3],DEC);
    //Serial.print(digits[2],DEC);
    //Serial.print(digits[1],DEC);
    //Serial.println(digits[0],DEC);
    recording_time.display(0,int8_t(digits[3]));
    recording_time.display(1,int8_t(digits[2]));
    recording_time.display(2,int8_t(digits[1]));
    recording_time.display(3,int8_t(digits[0]));

    EEPROM.write(0,digits[3]);
    EEPROM.write(1,digits[2]);
    EEPROM.write(2,digits[1]);
    EEPROM.write(3,digits[0]);
  }
}


// Save game step
void write_game_step()
{
    EEPROM.write(4,game_step);
    //Serial.print("Game_stepW:");
    //Serial.println(game_step,DEC);
}

// Read game step
void read_game_step()
{
    game_step=EEPROM.read(4);
    //Serial.print("Game_stepR:");
    //Serial.println(game_step,DEC);
}


// increment game step
void increment_game_step ()
{
  game_step = game_step + 1;
  activate_game_step();
}

void activate_game_step()
{
  if (game_step > 0){
    play_buzzer(BUZZER_SUCCESS);
  }
  // A led will be played when a step is finished
  play_game_step_leds();
  write_game_step();
}

// Play game step leds
void play_game_step_leds()
{
  //Serial2.print(game_step,DEC);
 if (game_step==0){
    digitalWrite(GPIO_LED_1, LOW);
    digitalWrite(GPIO_LED_2, LOW);
    digitalWrite(GPIO_LED_3, LOW);
    digitalWrite(GPIO_LED_4, LOW);
    digitalWrite(GPIO_LED_5, LOW);
    digitalWrite(GPIO_LED_6, LOW);
    digitalWrite(GPIO_LED_7, LOW);
    digitalWrite(GPIO_LED_8, LOW);
    digitalWrite(GPIO_LED_9, LOW);
    digitalWrite(GPIO_MOTOR, LOW);
    digitalWrite(GPIO_LOCKER, LOW);
  } else if (game_step==1){
    digitalWrite(GPIO_LED_1, HIGH);
    digitalWrite(GPIO_LED_2, LOW);
    digitalWrite(GPIO_LED_3, LOW);
    digitalWrite(GPIO_LED_4, LOW);
    digitalWrite(GPIO_LED_5, LOW);
    digitalWrite(GPIO_LED_6, LOW);
    digitalWrite(GPIO_LED_7, LOW);
    digitalWrite(GPIO_LED_8, LOW);  
    digitalWrite(GPIO_LED_9, LOW);
    digitalWrite(GPIO_MOTOR, LOW);
    digitalWrite(GPIO_LOCKER, LOW);
  } else if (game_step==2){
    digitalWrite(GPIO_LED_1, HIGH);
    digitalWrite(GPIO_LED_2, HIGH);
    digitalWrite(GPIO_LED_3, LOW);
    digitalWrite(GPIO_LED_4, LOW);
    digitalWrite(GPIO_LED_5, LOW);
    digitalWrite(GPIO_LED_6, LOW);
    digitalWrite(GPIO_LED_7, LOW);
    digitalWrite(GPIO_LED_8, LOW); 
    digitalWrite(GPIO_LED_9, LOW);
    digitalWrite(GPIO_MOTOR, LOW);
    digitalWrite(GPIO_LOCKER, LOW);
  } else if (game_step==3){
    digitalWrite(GPIO_LED_1, HIGH);
    digitalWrite(GPIO_LED_2, HIGH);
    digitalWrite(GPIO_LED_3, HIGH);
    digitalWrite(GPIO_LED_4, LOW);
    digitalWrite(GPIO_LED_5, LOW);
    digitalWrite(GPIO_LED_6, LOW);
    digitalWrite(GPIO_LED_7, LOW);
    digitalWrite(GPIO_LED_8, LOW);
    digitalWrite(GPIO_LED_9, LOW);
    digitalWrite(GPIO_MOTOR, LOW);
    digitalWrite(GPIO_LOCKER, LOW);
  } else if (game_step==4){
    digitalWrite(GPIO_LED_1, HIGH);
    digitalWrite(GPIO_LED_2, HIGH);
    digitalWrite(GPIO_LED_3, HIGH);
    digitalWrite(GPIO_LED_4, HIGH);
    digitalWrite(GPIO_LED_5, LOW);
    digitalWrite(GPIO_LED_6, LOW);
    digitalWrite(GPIO_LED_7, LOW);
    digitalWrite(GPIO_LED_8, LOW);
    digitalWrite(GPIO_LED_9, LOW);
    digitalWrite(GPIO_MOTOR, LOW);
    digitalWrite(GPIO_LOCKER, LOW);
  } else if (game_step==5){
    digitalWrite(GPIO_LED_1, HIGH);
    digitalWrite(GPIO_LED_2, HIGH);
    digitalWrite(GPIO_LED_3, HIGH);
    digitalWrite(GPIO_LED_4, HIGH);
    digitalWrite(GPIO_LED_5, HIGH);
    digitalWrite(GPIO_LED_6, LOW);
    digitalWrite(GPIO_LED_7, LOW);
    digitalWrite(GPIO_LED_8, LOW);  
    digitalWrite(GPIO_LED_9, LOW);
    digitalWrite(GPIO_MOTOR, LOW);
    digitalWrite(GPIO_LOCKER, LOW); 
  } else if (game_step==6){
    digitalWrite(GPIO_LED_1, HIGH);
    digitalWrite(GPIO_LED_2, HIGH);
    digitalWrite(GPIO_LED_3, HIGH);
    digitalWrite(GPIO_LED_4, HIGH);
    digitalWrite(GPIO_LED_5, HIGH);
    digitalWrite(GPIO_LED_6, HIGH);
    digitalWrite(GPIO_LED_7, LOW);
    digitalWrite(GPIO_LED_8, LOW);
    digitalWrite(GPIO_LED_9, LOW);
    digitalWrite(GPIO_MOTOR, LOW);
    digitalWrite(GPIO_LOCKER, LOW); 
  } else if (game_step==7){
    digitalWrite(GPIO_LED_1, HIGH);
    digitalWrite(GPIO_LED_2, HIGH);
    digitalWrite(GPIO_LED_3, HIGH);
    digitalWrite(GPIO_LED_4, HIGH);
    digitalWrite(GPIO_LED_5, HIGH);
    digitalWrite(GPIO_LED_6, HIGH);
    digitalWrite(GPIO_LED_7, HIGH);
    digitalWrite(GPIO_LED_8, LOW);
    digitalWrite(GPIO_LED_9, LOW);
    digitalWrite(GPIO_MOTOR, LOW);
    digitalWrite(GPIO_LOCKER, LOW);
  } else if (game_step==8){
    digitalWrite(GPIO_LED_1, HIGH);
    digitalWrite(GPIO_LED_2, HIGH);
    digitalWrite(GPIO_LED_3, HIGH);
    digitalWrite(GPIO_LED_4, HIGH);
    digitalWrite(GPIO_LED_5, HIGH);
    digitalWrite(GPIO_LED_6, HIGH);
    digitalWrite(GPIO_LED_7, HIGH);
    digitalWrite(GPIO_LED_8, HIGH); 
    digitalWrite(GPIO_LED_9, LOW);
    digitalWrite(GPIO_MOTOR, LOW);
    digitalWrite(GPIO_LOCKER, LOW);
  } else if (game_step==MAX_STEPS - 1){
    digitalWrite(GPIO_LED_1, HIGH);
    digitalWrite(GPIO_LED_2, HIGH);
    digitalWrite(GPIO_LED_3, HIGH);
    digitalWrite(GPIO_LED_4, HIGH);
    digitalWrite(GPIO_LED_5, HIGH);
    digitalWrite(GPIO_LED_6, HIGH);
    digitalWrite(GPIO_LED_7, HIGH);
    digitalWrite(GPIO_LED_8, HIGH); 
    digitalWrite(GPIO_LED_9, HIGH);
    digitalWrite(GPIO_MOTOR, HIGH);
    digitalWrite(GPIO_LOCKER, HIGH);
  }
  
}

// Play buzzer tone
void play_buzzer (int option)
{
  if (option == BUZZER_KEY){
    tone(GPIO_BUZZER, 1000); 
    delay(300);        
    noTone(GPIO_BUZZER);     
  }
  else if (option == BUZZER_SUCCESS){
    tone(GPIO_BUZZER, 3000); 
    delay(800);        
    noTone(GPIO_BUZZER);     
  }
  else if (option == BUZZER_ERROR){
    tone(GPIO_BUZZER, 200); 
    delay(300);        
    noTone(GPIO_BUZZER);     
  }
   
}

// Print data in lcd display
void lcd_print(String line1, String line2, int timeout)
{
  if (lcd_msg_1 != line1 and lcd_msg_2 != line2) {
    lcd.clear();
    lcd_msg_1 = line1;
    lcd_msg_2 = line2;
    lcd.home();
    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
    if (timeout != 0){
      delay(timeout);  
      lcd.clear();
    }
  }
  else {
    if (lcd_scroll_flag==true && (millis()-lcd_scroll_ms)>LCD_SPEED)
    {
      lcd_scroll_flag=false;
      lcd_scroll_ms=millis();
    }
    else if (lcd_scroll_flag==false)
    {
      lcd_scroll_flag=true;
      lcd_scroll();
    }
  }
  
}

// Scroll left lcd if text is longer than 16 characters
void lcd_scroll()
{
  if (lcd_msg_1.length()>16 or lcd_msg_2.length()>16){
    lcd.scrollDisplayLeft();
    //delay(LCD_SPEED);
  }
}

// Get clue when interrupt is pressed
void activate_flag_playing_clue()
{
  if (flag_playing_clue==false){
    flag_playing_clue = true;
  }
}

void play_clue ()
{  
  String clue_1;
  String clue_2;
  int index_last_clue = -1;
  int index_next_clue;
  
  for (int i=0; i<MAX_CLUES; i++){
    if (last_clue == MSG_CLUE_1[game_step][i]){
      index_last_clue = i;
      break;
    }
  }

  if (index_last_clue == -1){
    index_next_clue = 0;
  }
  else if (index_last_clue == MAX_CLUES-1){
    index_next_clue = 0;
  }
  else{
    index_next_clue = index_last_clue + 1;
  }

  
  clue_1 = MSG_CLUE_1[game_step][index_next_clue];
  clue_2 = MSG_CLUE_2[game_step][index_next_clue];
  last_clue = clue_1;
  
  if (clue_2.length()>LCD_COLS){
    for (int i=0; i<=LCD_BUFFER; i++){
      lcd_print(clue_1, clue_2, 0);
    }
  }
  else{
    lcd_print(clue_1, clue_2, TIMEOUT_LARGE);
  }
  flag_playing_clue = false;

  //increment current time -> penalization (1,2 or 3 minutes)
  increment_recording_time(index_next_clue+1);
  play_buzzer(BUZZER_KEY);
  
}

// Check keypad entered password
bool check_password(String password)
{
  // Game step password
  if(password != ""){
    if (password_val == password){
      password_val = "";
      return true;
    }
    else{
      play_buzzer(BUZZER_ERROR);
      password_val = "";
    }
  }
  // Key command
  else{
    if (password_val == CMD_RESET_RECORDING_TIME){
      //EEPROM reset recording time
      EEPROM.write(0,0);
      EEPROM.write(1,0);
      EEPROM.write(2,0);
      EEPROM.write(3,0);
      play_buzzer(BUZZER_SUCCESS);
      password_val = "";
      return true;
    }
    else if (password_val == CMD_GOTO_1){
      game_step = 0;
      activate_game_step();
      password_val = "";
      return true;
    }
    else if (password_val == CMD_GOTO_2){
      game_step = 1;
      activate_game_step();
      password_val = "";
      return true;
    }
    else if (password_val == CMD_GOTO_3){
      game_step = 2;
      activate_game_step();
      password_val = "";
      return true;
    }
    else if (password_val == CMD_GOTO_4){
      game_step = 3;
      activate_game_step();
      password_val = "";
      return true;
    }
    else if (password_val == CMD_GOTO_5){
      game_step = 4;
      activate_game_step();
      password_val = "";
      return true;
    }
    else if (password_val == CMD_GOTO_6){
      game_step = 5;
      activate_game_step();
      password_val = "";
      return true;
    }
    else if (password_val == CMD_GOTO_7){
      game_step = 6;
      activate_game_step();
      password_val = "";
      return true;
    }
    else if (password_val == CMD_GOTO_8){
      game_step = 7;
      activate_game_step();
      password_val = "";
      return true;
    }
    else if (password_val == CMD_GOTO_9){
      game_step = 8;
      activate_game_step();
      password_val = "";
      return true;
    }
    else if (password_val == CMD_GOTO_END){
      game_step = MAX_STEPS-1;
      activate_game_step();
      password_val = "";
      return true;
    }
  }
  return false;
}

// Reset all outputs
void reset_outputs()
{
  password_val = "";
  // GAME STEP 0 - WIRES
  // GAME STEP 1 - SIMON
  simon_input = "";
  digitalWrite(GPIO_SIMON_R_L,LOW);
  digitalWrite(GPIO_SIMON_G_L,LOW);
  digitalWrite(GPIO_SIMON_B_L,LOW);
  digitalWrite(GPIO_SIMON_Y_L,LOW);
  // GAME STEP 2 - RFID
  // GAME STEP 3 - LED MATRIX
  matrix.fillScreen(0);
  matrix.write();
  // GAME STEP 4 - JOYSTICK
  joy_val = "";
  // GAME STEP 5 - DATES
  display_val = "";
  date1.clearDisplay();
  date2.display(0,int8_t({0x0}));
  date2.display(1,int8_t({0x0}));
  date2.display(2,int8_t({0x0}));
  date2.display(3,int8_t({0x0}));
  date3.display(0,int8_t({0x0}));
  date3.display(1,int8_t({0x0}));
  date3.display(2,int8_t({0x0}));
  date3.display(3,int8_t({0x0}));
  date4.display(0,int8_t({0x0}));
  date4.display(1,int8_t({0x0}));
  date4.display(2,int8_t({0x0}));
  date4.display(3,int8_t({0x0}));
  // GAME STEP 6 - DIGNITY
  //display_val = "";
  //matrix.fillScreen(0);
  //matrix.write();
  lc.clearDisplay(0); 
  // GAME STEP 7 - SWITCHES
  // GAME STEP 8 - GPS
  gps_in_target = false;
  digitalWrite(GPIO_GPS_LED,LOW);
}

// ********************************************************************************************* //
// GAME STEP 0
// ********************************************************************************************* //

// Interrupt function when pressing pulse
void activate_flag_playing_leds()
{
  if (flag_playing_leds==false){
    flag_playing_leds = true;
  }
}

// Play leds with random timeout
void play_leds ()
{
    long timeout;
    timeout=random(50,800);
    // 1/10
    digitalWrite(GPIO_LED_R, HIGH);  // 2
    digitalWrite(GPIO_LED_G, HIGH);  // 3
    digitalWrite(GPIO_LED_B, HIGH);  // 1
    digitalWrite(GPIO_LED_W, HIGH);  // 5
    delay(timeout);
    // 2/10
    digitalWrite(GPIO_LED_W, LOW);
    delay(timeout);
    // 3/10
    digitalWrite(GPIO_LED_G, LOW);
    digitalWrite(GPIO_LED_R, LOW); 
    digitalWrite(GPIO_LED_W, HIGH);
    delay(timeout);
    // 4/10
    digitalWrite(GPIO_LED_G, HIGH);
    digitalWrite(GPIO_LED_W, LOW);
    delay(timeout);
    // 5/10
    digitalWrite(GPIO_LED_W, HIGH);
    delay(timeout);
    // 6/10
    digitalWrite(GPIO_LED_R, HIGH); 
    digitalWrite(GPIO_LED_G, LOW);
    digitalWrite(GPIO_LED_B, LOW); 
    digitalWrite(GPIO_LED_W, LOW);
    delay(timeout);
    // 7/10
    digitalWrite(GPIO_LED_W, HIGH);
    digitalWrite(GPIO_LED_G, HIGH);
    delay(timeout);
    // 8/10
    digitalWrite(GPIO_LED_R, LOW); 
    digitalWrite(GPIO_LED_W, LOW);
    delay(timeout);
    // 9/10
    digitalWrite(GPIO_LED_G, HIGH);
    digitalWrite(GPIO_LED_W, HIGH);
    delay(timeout);
    // 10/10
    digitalWrite(GPIO_LED_G, LOW);
    digitalWrite(GPIO_LED_W, LOW);
    delay(timeout);
  
    digitalWrite(GPIO_LED_R, LOW);  
    digitalWrite(GPIO_LED_G, LOW);  
    digitalWrite(GPIO_LED_B, LOW);  
    digitalWrite(GPIO_LED_W, LOW); 
    flag_playing_leds = false; 
}

// ********************************************************************************************* //
// GAME STEP 1
// ********************************************************************************************* //

// play simon sequence
void play_simon(int led_on_ms,int led_off_ms) {
  for (int i=0;i<simon_sequence.length(); i++){
    int led_gpio;
    if (simon_sequence[i] == 'R'){
      led_gpio = GPIO_SIMON_R_L;
    }
    else if (simon_sequence[i] == 'G'){
      led_gpio = GPIO_SIMON_G_L;
    }
    else if (simon_sequence[i] == 'B'){
      led_gpio = GPIO_SIMON_B_L;
    }
    else {
      led_gpio = GPIO_SIMON_Y_L;
    }
    digitalWrite(led_gpio,HIGH);
    delay(led_on_ms);
    digitalWrite(led_gpio,LOW);
    delay(led_off_ms);
  }
}

bool check_simon()
{
  if (simon_sequence == simon_input){
    play_buzzer(BUZZER_SUCCESS);
    simon_input = "";
    return true;
  }
  else{
    if (digitalRead(GPIO_SIMON_R_P)==HIGH and simon_last_led_actived == '\0')
    {
      simon_input = simon_input + 'R'; 
      simon_last_led_actived = 'R'; //R
      if (simon_sequence[simon_input.length()-1] == simon_input[simon_input.length()-1]){
        digitalWrite(GPIO_SIMON_R_L,HIGH);
        delay(200);
        digitalWrite(GPIO_SIMON_R_L,LOW);
        play_buzzer(BUZZER_KEY);
      }
      else{
        play_buzzer(BUZZER_ERROR);
        simon_input = "";
        simon_played = false;
        delay(1000);
      }
    }

    if (digitalRead(GPIO_SIMON_G_P)==HIGH and simon_last_led_actived == '\0')
    {
      simon_input = simon_input + 'G'; 
      simon_last_led_actived = 'G'; //G
      if (simon_sequence[simon_input.length()-1] == simon_input[simon_input.length()-1]){
        digitalWrite(GPIO_SIMON_G_L,HIGH);
        delay(200);
        digitalWrite(GPIO_SIMON_G_L,LOW);
        play_buzzer(BUZZER_KEY);
      }
      else{
        play_buzzer(BUZZER_ERROR);
        simon_input = "";
        simon_played = false;
        delay(1000);
      }
    }

    if (digitalRead(GPIO_SIMON_B_P)==HIGH and simon_last_led_actived == '\0')
    {
      simon_input = simon_input + 'B'; 
      simon_last_led_actived = 'B'; //B
      if (simon_sequence[simon_input.length()-1] == simon_input[simon_input.length()-1]){
        digitalWrite(GPIO_SIMON_B_L,HIGH);
        delay(200);
        digitalWrite(GPIO_SIMON_B_L,LOW);
        play_buzzer(BUZZER_KEY);
      }
      else{
        play_buzzer(BUZZER_ERROR);
        simon_input = "";
        simon_played = false;
        delay(1000);
      }
    }

    if (digitalRead(GPIO_SIMON_Y_P)==HIGH and simon_last_led_actived == '\0')
    {
      simon_input = simon_input + 'Y'; 
      simon_last_led_actived = 'Y'; //Y
      if (simon_sequence[simon_input.length()-1] == simon_input[simon_input.length()-1]){
        digitalWrite(GPIO_SIMON_Y_L,HIGH);
        delay(200);
        digitalWrite(GPIO_SIMON_Y_L,LOW);
        play_buzzer(BUZZER_KEY);
      }
      else{
        play_buzzer(BUZZER_ERROR);
        simon_input = "";
        simon_played = false;
        delay(1000);
      }
    }
    
    else{
      simon_last_led_actived = '\0';
    }
  }
  return false;
}

// ********************************************************************************************* //
// GAME STEP 2
// ********************************************************************************************* //

// Print array data
String printArray(byte *buffer, byte bufferSize) {
   String rfid_data;
   for (byte i = 0; i < bufferSize; i++) {
      //Serial.print(buffer[i] < 0x10 ? " 0" : " ");
      //Serial.print(buffer[i], HEX);
      rfid_data = rfid_data + buffer[i];
   }
   return rfid_data;
}

// Read rfid data
bool read_rfid()
{
  String rfid_data;
 if (mfrc522.PICC_IsNewCardPresent())
   {
      if (mfrc522.PICC_ReadCardSerial())
      {
         rfid_data=printArray(mfrc522.uid.uidByte, mfrc522.uid.size);
         //Serial.println(rfid_data);
         // Stop reading
         mfrc522.PICC_HaltA();
      }
   }
   delay(250);
   if (rfid_data == RFID_WHITE or rfid_data == RFID_BLUE){
    return true;
   }
   else{
    return false;
   }
}

// ********************************************************************************************* //
// GAME STEP 3
// ********************************************************************************************* //

// Print led matrix
void lmatrix_write(int i)
{
  matrix.fillScreen(LOW);
  int letter = i / LMATRIX_WIDTH;
  int x = (matrix.width() - 1) - i % LMATRIX_WIDTH;
  int y = (matrix.height() - 8) / 2; // center the text vertically

  while (x + LMATRIX_WIDTH - LMATRIX_SPACER >= 0 && letter >= 0) {
     if (letter < LMATRIX_TAPE.length()) {
        matrix.drawChar(x, y, LMATRIX_TAPE[letter], HIGH, LOW, 1);
     }

     letter--;
     x -= LMATRIX_WIDTH;
  }
  matrix.write(); // Send bitmap to display
}

// ********************************************************************************************* //
// GAME STEP 4
// ********************************************************************************************* //

// Check joystick data
bool check_joystick()
{
  int Xval = 0;
  int Yval = 0;
  char joy_current_state;
  Xval = analogRead(GPIO_JOY_X);
  delay(100);                 //es necesaria una pequeña pausa entre lecturas analógicas
  Yval = analogRead(GPIO_JOY_Y);
  if (Xval>800 and Yval>200 and Yval<800){
    joy_current_state = 'U';
  }
  else if (Xval<200 and Yval>200 and Yval<800){
    joy_current_state = 'D';
  }
  else if (Yval>800 and Xval>200 and Xval<800){
    joy_current_state = 'R';
  }
  else if (Yval<200 and Xval>200 and Xval<800){
    joy_current_state = 'L';
  }
  else {
    joy_current_state = ' ';
    joy_last_state = ' ';
  }

  if (joy_last_state == ' ' and joy_current_state != ' '){
    joy_last_state = joy_current_state;
    joy_val = joy_val + joy_current_state;
    play_buzzer(BUZZER_KEY);
  }

  //Serial.println ("joy_current_state: " + String(joy_current_state) + ", joy_last_state: " + String(joy_last_state) + ", joy_val: " + password_val);

  if (joy_val.length() == GS4_PASSWORD.length()){
    if (joy_val != GS4_PASSWORD){
      play_buzzer(BUZZER_ERROR);
      joy_val = "";
    }
    else{
      joy_val = "";
      return true;
    }
  }

  return false;
}

// ********************************************************************************************* //
// GAME STEP 5
// ********************************************************************************************* //

// Check encoder data
bool check_date()
{
  //print date
  if (song_index==0){
    print_date(date1);
  } else if (song_index==1){
    print_date(date2);
  } else if (song_index==2){
    print_date(date3);
  } else if (song_index==3){
    print_date(date4);
  }
  
  //Serial.println(encoder_val);
  if (!digitalRead(GPIO_ENC_SW)){
    if (song_index == 0){
      if (encoder_val == YEAR_0){
        play_buzzer(BUZZER_SUCCESS);
        song_index = song_index + 1;
      }
      else{
        play_buzzer(BUZZER_ERROR);
      }
    }
    else if (song_index == 1){
      if (encoder_val == YEAR_1){
        play_buzzer(BUZZER_SUCCESS);
        song_index = song_index + 1;
      }
      else{
        play_buzzer(BUZZER_ERROR);
      }
    }
    else if (song_index == 2){
      if (encoder_val == YEAR_2){
        play_buzzer(BUZZER_SUCCESS);
        song_index = song_index + 1;
      }
      else{
        play_buzzer(BUZZER_ERROR);
      }
    }
    else if (song_index == 3){
      if (encoder_val == YEAR_3){
        play_buzzer(BUZZER_SUCCESS);
        song_index = 0;
        return true;
      }
      else{
        play_buzzer(BUZZER_ERROR);
      }
    }
  }
  if (digitalRead(GPIO_PLAY_SONG) == LOW){
    if (song_index == 0){
      melody.play_melody(melody_0, sizeof(melody_0));
    }
    else if (song_index == 1){
      melody.play_melody(melody_1, sizeof(melody_1));
    }
    else if (song_index == 2){
      melody.play_melody(melody_2, sizeof(melody_2));
    }
    else if (song_index == 3){
      play_morse(morse_0);
    }
  }
  return false;
}

void print_date(TM1637 date_item)
{
  int i=0;
  int date_val = encoder_val;
  char digits[4];  
  digits[3]=0;
  digits[2]=0;
  digits[1]=0;
  digits[0]=0;
  while(date_val>0 && i<4){
    digits[i++] = date_val % 10; // you can change the 10 here and below to any number base
    date_val /= 10;
  }
  date_item.display(0,int8_t(digits[3]));
  date_item.display(1,int8_t(digits[2]));
  date_item.display(2,int8_t(digits[1]));
  date_item.display(3,int8_t(digits[0]));
}

void play_morse(String data)
{
  for(int i=0; i<= data.length();i++){
    //Serial.print(data[i]);
    if (data[i]=='.'){
      morse.dot();
    }
    else if (data[i]=='-'){
      morse.dash();
    }
    else if (data[i]==' '){
      delay(1000);
    }
    else if (data[i]=='/'){
      delay(500);
    }
  }
}

void doEncodeCLK()
{
  if (millis() > timeCounter + timeThreshold)
  {
    if (digitalRead(GPIO_ENC_CLK) != digitalRead(GPIO_ENC_DT))
    {
      step_minus = step_minus+1;
      if (step_minus >= 2){
        step_minus=0;
        if(encoder_val>encoder_min_val){
          encoder_val=encoder_val-1;
        }
        else if(encoder_val<=encoder_min_val){
          encoder_val=encoder_max_val;
        }
      }
    }
    timeCounter = millis();
  }
}

void doEncodeDT()
{
  if (millis() > timeCounter + timeThreshold)
  {
    if (digitalRead(GPIO_ENC_CLK) != digitalRead(GPIO_ENC_DT))
    {
      step_plus = step_plus+1;
      if (step_plus >= 2){
        step_plus=0;
        if(encoder_val<encoder_max_val){
          encoder_val=encoder_val+1;
        }
        else if(encoder_val>=encoder_max_val){
          encoder_val=encoder_min_val;
        }
      }
    }
    timeCounter = millis();
  }
}


// ********************************************************************************************* //
// GAME STEP 6
// ********************************************************************************************* //

// Set digit-8 password
void set_digit8_password(){
  encoder_max_val = ENCODER_MAX_VAL_GS6;
  encoder_min_val = ENCODER_MIN_VAL_GS6;  
  lc.shutdown(0,false);
  lc.clearDisplay(0);
  
  delay(250);
  lc.setDigit(0,3,(byte)9,false);  //I
  delay(250);
  lc.setDigit(0,2,(byte)4,false);  //D
  delay(250);
  lc.setDigit(0,1,(byte)1,false);  //A
  delay(250);
  lc.setDigit(0,0,(byte)4,false);  //D
  delay(250);
  lc.setDigit(0,7,(byte)4,false);  //D
  delay(250);
  lc.setDigit(0,6,(byte)9,false);  //I
  delay(250);
  lc.setDigit(0,5,(byte)7,false);  //G
  delay(250);
  lc.setChar(0,4,'E',false);       //N
}

// Check encoder data
bool check_word()
{
  if (!digitalRead(GPIO_ENC_SW)){
    display_val = display_val + letter;
    //Serial.println(display_val);
    if (display_val.length()==8){
      if(display_val==GS7_PASSWORD){
        display_val="";
        return true;
      }
      else{
        play_buzzer(BUZZER_ERROR);
        display_val="";
      }
    }
    else{
      play_buzzer(BUZZER_KEY);
    }
  }
  write_letter(encoder_val);
  return false;
}

void write_letter(int option)
{
  
  if (option == 0){
    letter = 'A';
  }
  else if (option == 1){
    letter = 'B';
  }
  else if (option == 2){
    letter = 'C';
  }
  else if (option == 3){
    letter = 'D';
  }
  else if (option == 4){
    letter = 'E';
  }
  else if (option == 5){
    letter = 'F';
  }
  else if (option == 6){
    letter = 'G';
  }
  else if (option == 7){
    letter = 'H';
  }
  else if (option == 8){
    letter = 'I';
  }
  else if (option == 9){
    letter = 'J';
  }
  else if (option == 10){
    letter = 'K';
  }
  else if (option == 11){
    letter = 'L';
  }
  else if (option == 12){
    letter = 'M';
  }
  else if (option == 13){
    letter = 'N';
  }
  else if (option == 14){
    letter = 'O';
  }
  else if (option == 15){
    letter = 'P';
  }
  else if (option == 16){
    letter = 'Q';
  }
  else if (option == 17){
    letter = 'R';
  }
  else if (option == 18){
    letter = 'S';
  }
  else if (option == 19){
    letter = 'T';
  }
  else if (option == 20){
    letter = 'U';
  }
  else if (option == 21){
    letter = 'V';
  }
  else if (option == 22){
    letter = 'W';
  }
  else if (option == 23){
    letter = 'X';
  }
  else if (option == 24){
    letter = 'Y';
  }
  else if (option == 25){
    letter = 'Z';
  }
  matrix.fillScreen(LOW);
  matrix.drawChar(1, 0, letter, HIGH, LOW, 1);
  matrix.write();
}

// ********************************************************************************************* //
// GAME STEP 7
// ********************************************************************************************* //
// Return a rounded float. Number of decimals is a function input.
float DecimalRound(float input, int decimals)
{
  float scale=pow(10,decimals);
  return round(input*scale)/scale;
}

bool check_gps()
{
  latitude = DecimalRound(gps.location.lat(),6);
  longitude = DecimalRound(gps.location.lng(),6);
  //Serial.println("Location:" + String(latitude,6) + "; " + String(longitude,6));
  if (int(latitude) != 0 and int(longitude) != 0){
    double distance = TinyGPSPlus::distanceBetween(
      GPS_TARGET_LATITUDE, GPS_TARGET_LONGITUDE, latitude, longitude);
    Serial.print("Distance to target:");
    Serial.println(distance);
    if (distance < GPS_TARGET_DISTANCE){
      digitalWrite(GPIO_GPS_LED, HIGH);
      return true;
    }
  }
  return false;
}

void play_gps_led()
{
  if (int(latitude) == 0 or int(longitude) == 0){
    // blink led
    if (gps_led_status == true and (millis()-gps_led_ms>GPS_LED_TIMEOUT)){
      gps_led_status = false;
      gps_led_ms = millis();
      digitalWrite(GPIO_GPS_LED, LOW);
    }else if(gps_led_status == false and (millis()-gps_led_ms>GPS_LED_TIMEOUT)){
      gps_led_status = true;
      gps_led_ms = millis();
      digitalWrite(GPIO_GPS_LED, HIGH);
    }
  }
  else {
    digitalWrite(GPIO_GPS_LED, HIGH);
    gps_led_status=true;
  }
}
