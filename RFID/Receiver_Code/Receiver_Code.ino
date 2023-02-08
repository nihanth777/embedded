#include <SoftwareSerial.h>
SoftwareSerial ch12(3,4); // RX, TX

void setup() {
  Serial.begin(115200);
  ch12.begin(9600);
 // ch12.print(F("AT+B115200"));
} 


void loop() {
 if (ch12.available() > 0) { // checks to see if HC-12 is receiving data.
   // run once. Keep in mind transmitter actually sends twice.
    // Serial.print("OK");
     String cmd = ch12.readString(); 
     Serial.println(cmd);
     cmd="";
    
 }

}
