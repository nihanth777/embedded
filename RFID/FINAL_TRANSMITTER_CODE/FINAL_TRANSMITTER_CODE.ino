/*
 * Initial Author: ryand1011 (https://github.com/ryand1011)
 *
 * Reads data written by a program such as "rfid_write_personal_data.ino"
 *
 * See: https://github.com/miguelbalboa/rfid/tree/master/examples/rfid_write_personal_data
 *
 * Uses MIFARE RFID card using RFID-RC522 reader
 * Uses MFRC522 - Library
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
*/

#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>

SoftwareSerial ch12(4, 3); // RX, TX

#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

//*****************************************************************************************//
void setup() {
  Serial.begin(115200);  
   ch12.begin(9600);
 // pinMode(Relay, OUTPUT); digitalWrite(Relay, HIGH);// Initialize serial communications with the PC
  SPI.begin();                                                  // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
   // Enhance the MFRC522 Receiver Gain to maximum value of some 48 dB
   //mfrc522.PCD_SetRegisterBitMask(mfrc522.RFCfgReg, (0x07<<4));
    // mfrc522.PCD_ClearRegisterBitMask(mfrc522.RFCfgReg, (0x07<<4));
   //mfrc522.PCD_SetRegisterBitMask(mfrc522.RFCfgReg, (0x00<<4));
  Serial.println("Please scan your card");
}
void PrintHex(uint8_t *data, uint8_t length) // prints 8-bit data in hex with leading zeroes
{
     char tmp[16];
       for (int i=0; i<length; i++) { 
         sprintf(tmp, "0x%.2X",data[i]); 
         Serial.print(tmp); Serial.print(" ");
       }
}
uint8_t buf[10]= {};
MFRC522::Uid id;
MFRC522::Uid id2;
bool is_card_present = false;
//*****************************************************************************************//

void cpid(MFRC522::Uid *id){
  memset(id, 0, sizeof(MFRC522::Uid));
  memcpy(id->uidByte, mfrc522.uid.uidByte, mfrc522.uid.size);
  id->size = mfrc522.uid.size;
  id->sak = mfrc522.uid.sak;
}

bool cmpid(MFRC522::Uid *id1, MFRC522::Uid *id2){
  return memcmp(id1, id2, sizeof(MFRC522::Uid));
}

void deregister_card(){
  is_card_present = false;
  memset(&id,0, sizeof(id));
}
uint8_t control = 0x00;
void loop() {

  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
  MFRC522::StatusCode status;

  //-------------------------------------------

  // Look for new cards
  if ( !mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if ( !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  String content= "";
  byte letter;
for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
    
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
content.toUpperCase();
Serial.println(content);
ch12.println(content);

// if (content.substring(1) == "DD 85 AB 29" || content.substring(1) == "EB 57 12 11" ) // change here the UID of the card/cards that you want to give access
//  {
//  
//  //PrintHex(id.uidByte, id.size);
//  //Serial.println("hello");
//  bool result = true;
//  uint8_t buf_len=4;
//  cpid(&id);
//  Serial.print("NewCard ");
//  //ch12.print("valid");
//  //ch12.print(content);
//  //digitalWrite(Relay, LOW);
//  //PrintHex(id.uidByte, id.size);
//  Serial.println("");
//  while(true){
//    control=0;
//    for(int i=0; i<3; i++){
//      if(!mfrc522.PICC_IsNewCardPresent()){
//        if(mfrc522.PICC_ReadCardSerial()){
//          //Serial.print('a');
//          control |= 0x16;
//        }
//        if(mfrc522.PICC_ReadCardSerial()){
//          //Serial.print('b');
//          control |= 0x16;
//        }
//        //Serial.print('c');
//          control += 0x1;
//      }
//      //Serial.print('d');
//      control += 0x4;
//    }
//    
//    //Serial.println(control);
//    if(control == 13 || control == 14){
//      //card is still there
//    } 
//    
//    else {
//      break;
//    }
//  }
//  Serial.println("CardRemoved");
//  ch12.println("CardRemoved");
// // digitalWrite(Relay, HIGH);
// // delay(500); //change value if you want to read cards faster
//
//  mfrc522.PICC_HaltA();
//  mfrc522.PCD_StopCrypto1();
//}
//else
//{
//  Serial.println("Invalid card");
//  ch12.println("invalid");

 while(true){
    control=0;
    for(int i=0; i<3; i++){
      if(!mfrc522.PICC_IsNewCardPresent()){
        if(mfrc522.PICC_ReadCardSerial()){
          //Serial.print('a');
          control |= 0x16;
        }
        if(mfrc522.PICC_ReadCardSerial()){
          //Serial.print('b');
          control |= 0x16;
        }
        //Serial.print('c');
          control += 0x1;
      }
      //Serial.print('d');
      control += 0x4;
    }
    
    //Serial.println(control);
    if(control == 13 || control == 14){
      //card is still there
    } 
    
    else {
      break;
    }
  }
  Serial.println("CardRemoved");
  ch12.println("CardRemoved");
  Serial.flush();
  
}


//*****************************************************************************************//a
