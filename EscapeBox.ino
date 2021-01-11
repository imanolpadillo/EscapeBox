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
#include "Morse.h"
#include "Melody.h"

// ********************************************************************************************* //
// PASSWORDS
// ********************************************************************************************* //
// GAME STEP 1: keyboard
#define GS1_PASSWORD                      "1234"
#define RESET_RECORDING_TIME              "311252"
// GAME STEP 2: rfid
#define RFID_WHITE                        "3221477164"
#define RFID_BLUE                         "575574115"
// GAME STEP 4: joystick   
const String GS4_PASSWORD  =              "LLRRUD";
// GAME STEP 5: encoder
#define YEAR_0                            1510
#define YEAR_1                            1511
#define YEAR_2                            1512
#define YEAR_3                            1513

   

// ********************************************************************************************* //
// GPIO
// ********************************************************************************************* //
// CLUES
#define GPIO_CLUE                         2
#define GPIO_BUZZER                       35
// GAME STEP 0: wires + leds
#define GPIO_PULSE                        3
#define GPIO_LED_R                        22
#define GPIO_LED_G                        23
#define GPIO_LED_B                        24
#define GPIO_LED_W                        25
#define GPIO_WIRE_INPUT                   26
// GAME STEP 1: keyboard
#define GPIO_KP_R1                        27
#define GPIO_KP_R2                        28
#define GPIO_KP_R3                        29
#define GPIO_KP_R4                        30
#define GPIO_KP_C1                        31
#define GPIO_KP_C2                        32
#define GPIO_KP_C3                        33
#define GPIO_KP_C4                        34
// GAME STEP 2: rfid
#define GPIO_RFID_RST                     8
#define GPIO_RFID_SDA                     9
//#define GPIO_RFID_SCK                   52  
//#define GPIO_RFID_MOSI                  51 
//#define GPIO_RFID_MISO                  50
// GAME STEP 3: led matrix
#define GPIO_LMATRIX_CS                   53  // SS    
//#define GPIO_LMATRIX_CLK                52  // SCK
//#define GPIO_LMATRIX_DIN                51  // MOSI 
// GAME STEP 4: joystick   
#define GPIO_JOY_X                        A0
#define GPIO_JOY_Y                        A1       
// GAME STEP 5: rotary encoder
#define GPIO_ENC_CLK                      19
#define GPIO_ENC_DT                       18   
#define GPIO_ENC_SW                       4  
#define GPIO_PLAY_SONG                    5

// ********************************************************************************************* //
// CONSTANTS
// ********************************************************************************************* //
#define MAX_STEPS                         9
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
const int ENCODER_MAX_VAL =               2200;
const int ENCODER_MIN_VAL =               1500;

String LMATRIX_TAPE = "www.escapebox.com";
// Messages                                1234567890123456789012345678901234567890
#define MSG_INIT_1                        "   ESCAPE BOX   "
#define MSG_INIT_2                        "  Mai y Mario   "
const String MSG_GAMESTEP[MAX_STEPS][LCD_ROWS] = {
  {"Prueba 1", "Cables y leds"},
  {"Prueba 2", "keypad"},
  {"Prueba 3", "rfid"},
  {"Prueba 4", "led matrix"},
  {"Prueba 5", "joystick"},
  {"Prueba 6", "Prueba6 - Descripcion"},
  {"Prueba 7", "Prueba7 - Descripcion"},
  {"Prueba 8", "Prueba8 - Descripcion"},
  {"Prueba 9", "Prueba9 - Descripcion"}
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
  {"Prueba9 - Pista1", "Prueba9 - Pista2", "Prueba9 - Pista3"}
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
  {"Prueba9 - Pista1", "Prueba9 - Pista2", "Prueba9 - Pista3"}
};
// ********************************************************************************************* //
// VARIABLES
// ********************************************************************************************* //
// lcd
LiquidCrystal_I2C lcd(LCD_ADDRESS, 16, 2);
String lcd_msg_1;
String lcd_msg_2;
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
// general
unsigned long loop_ms = millis();
unsigned long total_ms;
int game_step = 5;
String last_clue;
bool flag_playing_clue = false;
//game_step 0
bool flag_playing_leds = false;
//game_step 1
String password_val;
//game_step 2
MFRC522 mfrc522(GPIO_RFID_SDA, GPIO_RFID_RST); 
//game_step 3
Max72xxPanel matrix = Max72xxPanel(GPIO_LMATRIX_CS, LMATRIX_H_DISPLAYS, LMATRIX_V_DISPLAYS);
int lmatrix_index = 0;
//game_step 3
char joy_last_state = ' ';
//game_step 4
const int timeThreshold = 5;
long timeCounter = 0;
int encoder_val = ENCODER_MIN_VAL;
int step_minus = 0;
int step_plus = 0;
Morse morse(GPIO_BUZZER);
Melody melody(GPIO_BUZZER);
int song_index=0;

int melody_0[] = {

  //Based on the arrangement at https://www.flutetunes.com/tunes.php?id=192
  
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

  // Take on me, by A-ha
  // Score available at https://musescore.com/user/27103612/scores/4834399
  // Arranged by Edward Truong

  NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8, 
  REST,8, NOTE_E5,8, REST,8, NOTE_E5,8, NOTE_GS5,8, NOTE_GS5,8, NOTE_A5,8, NOTE_B5,8,
  NOTE_A5,8, NOTE_A5,8, NOTE_A5,8, NOTE_E5,8, REST,8, NOTE_D5,8, REST,8, NOTE_FS5,8, 
  REST,8, NOTE_FS5,8, REST,8, NOTE_FS5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8,
  NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8, 
  
  };

  int melody_2[] = {

  // Pink Panther theme
  // Score available at https://musescore.com/benedictsong/the-pink-panther
  // Theme by Masato Nakamura, arranged by Teddy Mason

  REST,2, REST,4, REST,8, NOTE_DS4,8, 
  NOTE_E4,-4, REST,8, NOTE_FS4,8, NOTE_G4,-4, REST,8, NOTE_DS4,8,
  NOTE_E4,-8, NOTE_FS4,8,  NOTE_G4,-8, NOTE_C5,8, NOTE_B4,-8, NOTE_E4,8, NOTE_G4,-8, NOTE_B4,8,   
  NOTE_AS4,2, NOTE_A4,-16, NOTE_G4,-16, NOTE_E4,-16, NOTE_D4,-16, 
  NOTE_E4,2,
 
  };

  int morse_0 = ".-.. --- ... / .--. .. ... - --- .-.. . .-. --- ... / -.. . .-.. / . -.-. .-.. .. .--. ... .";



// ********************************************************************************************* //
// SETUP
// ********************************************************************************************* //
void setup() {
  // Setup digital pins
  pinMode(GPIO_CLUE, INPUT_PULLUP);  
  attachInterrupt(digitalPinToInterrupt(GPIO_CLUE), activate_flag_playing_clue, FALLING);
  pinMode(GPIO_BUZZER, OUTPUT); 
  // GAME STEP 0: wires + leds
  pinMode(GPIO_LED_R, OUTPUT);  
  pinMode(GPIO_LED_G, OUTPUT);  
  pinMode(GPIO_LED_B, OUTPUT);  
  pinMode(GPIO_LED_W, OUTPUT);  
  pinMode(GPIO_PULSE, INPUT_PULLUP);  
  attachInterrupt(digitalPinToInterrupt(GPIO_PULSE), activate_flag_playing_leds, FALLING);
  pinMode(GPIO_WIRE_INPUT, INPUT);
  // GAME STEP 1: keyboard
  pinMode(GPIO_KP_R1, INPUT);
  pinMode(GPIO_KP_R2, INPUT);
  pinMode(GPIO_KP_R3, INPUT);
  pinMode(GPIO_KP_R4, INPUT);
  pinMode(GPIO_KP_C1, INPUT);
  pinMode(GPIO_KP_C2, INPUT);
  pinMode(GPIO_KP_C3, INPUT);
  pinMode(GPIO_KP_C4, INPUT);
  // GAME STEP 2: rfid
  // GAME STEP 3: led matrix
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
  // Setup serial port
  Serial.begin(9600);  

  // Setup SPI and rfid
  SPI.begin();
  mfrc522.PCD_Init();

  // Init lcd
  lcd.begin();                      
  lcd.backlight();
  lcd_print(MSG_INIT_1,MSG_INIT_2,TIMEOUT_LARGE);

  // step leds
  play_game_step_leds(-1);

  // empty led matrix
  matrix.write();
  
}

// ********************************************************************************************* //
// MAIN
// ********************************************************************************************* //
void loop() {
  // waste
  //char led[2]="5";
  //Serial.write(led,1);
  
  // Clue
  if (flag_playing_clue == true){
      play_clue(); 
  }

  //lcd info
  lcd_print(MSG_GAMESTEP[game_step][0],MSG_GAMESTEP[game_step][1],0);
  
  // GAME_STEP 0
  if (game_step == 0)
  {
    if (digitalRead(GPIO_WIRE_INPUT) == HIGH){
      play_game_step_leds(game_step);
      game_step = game_step + 1;
    }
    else if (flag_playing_leds == true){
      play_leds(); 
    }
  }
  // GAME_STEP_1
  else if (game_step == 1)
  {
    char customKey = customKeypad.getKey();
    if (isAlphaNumeric(customKey) or isDigit(customKey)
      or customKey == '*' or customKey == '#'){
      play_buzzer(BUZZER_KEY);
      if (check_password(customKey) == true){
        play_game_step_leds(game_step);
        game_step = game_step + 1;
      }
    }
  }
  // GAME_STEP_2
  else if (game_step == 2)
  {
    if (read_rfid() == true){
        play_game_step_leds(game_step);
        game_step = game_step + 1;
    }
  }
  // GAME_STEP_3
  else if (game_step == 3)
  {
    if (lmatrix_index < LMATRIX_WIDTH * LMATRIX_TAPE.length() + matrix.width() - 1 - LMATRIX_SPACER) {
      lmatrix_write(lmatrix_index);
      lmatrix_index++;
    }
    else{
      lmatrix_index = 0;
    }
  }
  // GAME_STEP_4
  else if (game_step == 4)
  {
    if (check_joystick() == true){
        play_game_step_leds(game_step);
        game_step = game_step + 1;
    }
  }
  // GAME_STEP_5
  else if (game_step == 5)
  {
    if (check_encoder() == true){
        play_game_step_leds(game_step);
        game_step = game_step + 1;
    }
  }

  // Increment recording time
  increment_recording_time();
  


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
void increment_recording_time()
{
  if(millis()-loop_ms>=RECORDING_MS){
    loop_ms = millis();
    
    int current_minutes = EEPROM.read(0)*1000 + EEPROM.read(1)*100 + EEPROM.read(2)*10 + 
      EEPROM.read(3) + 1;
    
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

    EEPROM.write(0,digits[3]);
    EEPROM.write(1,digits[2]);
    EEPROM.write(2,digits[1]);
    EEPROM.write(3,digits[0]);
  }
}

// A led will be played when a step is finished
void play_game_step_leds (int level)
{
  if (level != -1){
    play_buzzer(BUZZER_SUCCESS);
  }
  //Serial.print(level+1,DEC);
  //Serial.print('A');
}
// Play buzzer tone
void play_buzzer (int option)
{
  if (option == BUZZER_KEY){
    tone(GPIO_BUZZER, 1000); 
    delay(300);        
    noTone(GPIO_BUZZER);     
    delay(300);
  }
  else if (option == BUZZER_SUCCESS){
    tone(GPIO_BUZZER, 3000); 
    delay(800);        
    noTone(GPIO_BUZZER);     
    delay(300);
  }
  else if (option == BUZZER_ERROR){
    tone(GPIO_BUZZER, 200); 
    delay(300);        
    noTone(GPIO_BUZZER);     
    delay(300);
  }
   
}

// Print data in lcd display
void lcd_print(String line1, String line2, int timeout)
{
  if (lcd_msg_1 != line1 and lcd_msg_2 != line2) {
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
    lcd_scroll();
  }
  
}

// Scroll left lcd if text is longer than 16 characters
void lcd_scroll()
{
  if (lcd_msg_1.length()>16 or lcd_msg_2.length()>16){
    lcd.scrollDisplayLeft();
    delay(LCD_SPEED);
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

// Interrupt function when pressing pulse
bool check_password(char key)
{
  if (key=='#'){
    if (password_val == GS1_PASSWORD){
      password_val = "";
      return true;
    }
    else if (password_val == RESET_RECORDING_TIME){
      //EEPROM reset recording time
      EEPROM.write(0,0);
      EEPROM.write(1,0);
      EEPROM.write(2,0);
      EEPROM.write(3,0);
      play_buzzer(BUZZER_SUCCESS);
      password_val = "";
    }
    else{
      play_buzzer(BUZZER_ERROR);
      password_val = "";
    }
  }
  else{
    password_val = password_val + key;
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
    password_val = password_val + joy_current_state;
    play_buzzer(BUZZER_KEY);
  }

  //Serial.println ("joy_current_state: " + String(joy_current_state) + ", joy_last_state: " + String(joy_last_state) + ", password_val: " + password_val);

  if (password_val.length() == GS4_PASSWORD.length()){
    if (password_val != GS4_PASSWORD){
      play_buzzer(BUZZER_ERROR);
      password_val = "";
    }
    else{
      password_val = "";
      return true;
    }
  }

  return false;
}

// ********************************************************************************************* //
// GAME STEP 5
// ********************************************************************************************* //

// Check encoder data
bool check_encoder()
{
  Serial.println(encoder_val);
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
      morse.dot(); morse.dot(); morse.dot();
    }
  }
  return false;
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
        if(encoder_val>ENCODER_MIN_VAL){
          encoder_val=encoder_val-1;
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
        if(encoder_val<ENCODER_MAX_VAL){
          encoder_val=encoder_val+1;
        }
      }
    }
    timeCounter = millis();
  }
}
