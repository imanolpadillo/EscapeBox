#include <TinyGPS++.h>
TinyGPSPlus gps;
void setup() {
  // Setup serial comm
  Serial.begin(9600);  
  // Setup gps serial comm
  Serial3.begin(9600);
}

void loop() {
  while (Serial3.available()>0) {
      char c = Serial3.read();
      //Serial.write(c);
      gps.encode(c);
      //Serial.write(Serial3.read());
      //gps.encode(Serial3.read());
    }
    Serial.print("Sentences that failed checksum=");
    Serial.println(gps.failedChecksum());

}
