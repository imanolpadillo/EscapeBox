char mystr[1]; //Initialized variable to store recieved data
#define GPIO_LED_1                  2
#define GPIO_LED_2                  3
#define GPIO_LED_3                  4
#define GPIO_LED_4                  5
#define GPIO_LED_5                  6
#define GPIO_LED_6                  7
#define GPIO_LED_7                  8
#define GPIO_LED_8                  9
#define GPIO_LED_9                  10
#define GPIO_LED_10                 11

void setup() {
  // Begin the Serial at 9600 Baud
  Serial.begin(9600);
  // GPIO
  pinMode(GPIO_LED_1, OUTPUT); 
  pinMode(GPIO_LED_2, OUTPUT); 
  pinMode(GPIO_LED_3, OUTPUT); 
  pinMode(GPIO_LED_4, OUTPUT); 
  pinMode(GPIO_LED_5, OUTPUT); 
  pinMode(GPIO_LED_6, OUTPUT); 
  pinMode(GPIO_LED_7, OUTPUT); 
  pinMode(GPIO_LED_8, OUTPUT); 
  pinMode(GPIO_LED_9, OUTPUT); 
}
void loop() {
  Serial.readBytes(mystr,1); //Read the serial data and store in var
  //Serial.println(mystr[0]); //Print data on Serial Monitor
  if (mystr[0]=='0'){
    digitalWrite(GPIO_LED_1, LOW);
    digitalWrite(GPIO_LED_2, LOW);
    digitalWrite(GPIO_LED_3, LOW);
    digitalWrite(GPIO_LED_4, LOW);
    digitalWrite(GPIO_LED_5, LOW);
    digitalWrite(GPIO_LED_6, LOW);
    digitalWrite(GPIO_LED_7, LOW);
    digitalWrite(GPIO_LED_8, LOW);
    digitalWrite(GPIO_LED_9, LOW);
    digitalWrite(GPIO_LED_10, LOW);
  } else if (mystr[0]=='1'){
    digitalWrite(GPIO_LED_1, HIGH);
    digitalWrite(GPIO_LED_2, LOW);
    digitalWrite(GPIO_LED_3, LOW);
    digitalWrite(GPIO_LED_4, LOW);
    digitalWrite(GPIO_LED_5, LOW);
    digitalWrite(GPIO_LED_6, LOW);
    digitalWrite(GPIO_LED_7, LOW);
    digitalWrite(GPIO_LED_8, LOW);
    digitalWrite(GPIO_LED_9, LOW); 
    digitalWrite(GPIO_LED_10, LOW);   
  } else if (mystr[0]=='2'){
    digitalWrite(GPIO_LED_1, HIGH);
    digitalWrite(GPIO_LED_2, HIGH);
    digitalWrite(GPIO_LED_3, LOW);
    digitalWrite(GPIO_LED_4, LOW);
    digitalWrite(GPIO_LED_5, LOW);
    digitalWrite(GPIO_LED_6, LOW);
    digitalWrite(GPIO_LED_7, LOW);
    digitalWrite(GPIO_LED_8, LOW);
    digitalWrite(GPIO_LED_9, LOW); 
    digitalWrite(GPIO_LED_10, LOW);   
  } else if (mystr[0]=='3'){
    digitalWrite(GPIO_LED_1, HIGH);
    digitalWrite(GPIO_LED_2, HIGH);
    digitalWrite(GPIO_LED_3, HIGH);
    digitalWrite(GPIO_LED_4, LOW);
    digitalWrite(GPIO_LED_5, LOW);
    digitalWrite(GPIO_LED_6, LOW);
    digitalWrite(GPIO_LED_7, LOW);
    digitalWrite(GPIO_LED_8, LOW);
    digitalWrite(GPIO_LED_9, LOW);    
    digitalWrite(GPIO_LED_10, LOW);
  } else if (mystr[0]=='4'){
    digitalWrite(GPIO_LED_1, HIGH);
    digitalWrite(GPIO_LED_2, HIGH);
    digitalWrite(GPIO_LED_3, HIGH);
    digitalWrite(GPIO_LED_4, HIGH);
    digitalWrite(GPIO_LED_5, LOW);
    digitalWrite(GPIO_LED_6, LOW);
    digitalWrite(GPIO_LED_7, LOW);
    digitalWrite(GPIO_LED_8, LOW);
    digitalWrite(GPIO_LED_9, LOW); 
    digitalWrite(GPIO_LED_10, LOW);   
  } else if (mystr[0]=='5'){
    digitalWrite(GPIO_LED_1, HIGH);
    digitalWrite(GPIO_LED_2, HIGH);
    digitalWrite(GPIO_LED_3, HIGH);
    digitalWrite(GPIO_LED_4, HIGH);
    digitalWrite(GPIO_LED_5, HIGH);
    digitalWrite(GPIO_LED_6, LOW);
    digitalWrite(GPIO_LED_7, LOW);
    digitalWrite(GPIO_LED_8, LOW);
    digitalWrite(GPIO_LED_9, LOW); 
    digitalWrite(GPIO_LED_10, LOW);   
  } else if (mystr[0]=='6'){
    digitalWrite(GPIO_LED_1, HIGH);
    digitalWrite(GPIO_LED_2, HIGH);
    digitalWrite(GPIO_LED_3, HIGH);
    digitalWrite(GPIO_LED_4, HIGH);
    digitalWrite(GPIO_LED_5, HIGH);
    digitalWrite(GPIO_LED_6, HIGH);
    digitalWrite(GPIO_LED_7, LOW);
    digitalWrite(GPIO_LED_8, LOW);
    digitalWrite(GPIO_LED_9, LOW); 
    digitalWrite(GPIO_LED_10, LOW);   
  } else if (mystr[0]=='7'){
    digitalWrite(GPIO_LED_1, HIGH);
    digitalWrite(GPIO_LED_2, HIGH);
    digitalWrite(GPIO_LED_3, HIGH);
    digitalWrite(GPIO_LED_4, HIGH);
    digitalWrite(GPIO_LED_5, HIGH);
    digitalWrite(GPIO_LED_6, HIGH);
    digitalWrite(GPIO_LED_7, HIGH);
    digitalWrite(GPIO_LED_8, LOW);
    digitalWrite(GPIO_LED_9, LOW);  
    digitalWrite(GPIO_LED_10, LOW);  
  } else if (mystr[0]=='8'){
    digitalWrite(GPIO_LED_1, HIGH);
    digitalWrite(GPIO_LED_2, HIGH);
    digitalWrite(GPIO_LED_3, HIGH);
    digitalWrite(GPIO_LED_4, HIGH);
    digitalWrite(GPIO_LED_5, HIGH);
    digitalWrite(GPIO_LED_6, HIGH);
    digitalWrite(GPIO_LED_7, HIGH);
    digitalWrite(GPIO_LED_8, HIGH);
    digitalWrite(GPIO_LED_9, LOW);  
    digitalWrite(GPIO_LED_10, LOW);  
  } else if (mystr[0]=='9'){
    digitalWrite(GPIO_LED_1, HIGH);
    digitalWrite(GPIO_LED_2, HIGH);
    digitalWrite(GPIO_LED_3, HIGH);
    digitalWrite(GPIO_LED_4, HIGH);
    digitalWrite(GPIO_LED_5, HIGH);
    digitalWrite(GPIO_LED_6, HIGH);
    digitalWrite(GPIO_LED_7, HIGH);
    digitalWrite(GPIO_LED_8, HIGH);
    digitalWrite(GPIO_LED_9, HIGH); 
    digitalWrite(GPIO_LED_10, LOW);   
  } else if (mystr[0]=='A'){
    digitalWrite(GPIO_LED_1, HIGH);
    digitalWrite(GPIO_LED_2, HIGH);
    digitalWrite(GPIO_LED_3, HIGH);
    digitalWrite(GPIO_LED_4, HIGH);
    digitalWrite(GPIO_LED_5, HIGH);
    digitalWrite(GPIO_LED_6, HIGH);
    digitalWrite(GPIO_LED_7, HIGH);
    digitalWrite(GPIO_LED_8, HIGH);
    digitalWrite(GPIO_LED_9, HIGH); 
    digitalWrite(GPIO_LED_10, HIGH);   
  }
  delay(1000);
}
