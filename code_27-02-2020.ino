#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SoftwareSerial.h>

#define probe_length 500 // unit = mm
#define radius 50 // unit = mm
#define TX_ENABLE_PIN 25

AsyncWebServer server(80);

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "FUELBUDDY";
const char* password = "Greenfuel@098";

// declare variable to store data
String header;
char data;
int response;
String res;
String resp;
int i=0;
int finish=0;
char dep[10];
double capacity = 0;
double capacity2 = 0;
String mystring;
String mystring2;
String mystring3;
int period = 5000;
unsigned long time_now = 0;
const char* PARAM_INPUT_1 = "input1";
const char* PARAM_INPUT_2 = "input2";
String inputMessage = " ";
String inputMessage1 = "";
String inputParam;
String inputParam1;
int  r=0, t=0;
String A;
String myString;

String output26State = "Lock";
String output27State = "Lock";
String output25State = "off";

// Assign output variables to GPIO pins
const int output2 = 2;
const int output26 = 26;
const int output27 = 27;
const int output25 = 25;

byte read_sale []    =  {0x01, 0x41, 0x52, 0x7F, 0x6D};
byte pump_start []   =  {0x01, 0x41, 0x4F, 0x7F, 0x70};
byte read_preset []  =  {0x01, 0x41, 0x48, 0x7F, 0x77};
byte authorize []    =  {0x01, 0x41, 0x41, 0x7F, 0x7E};
byte clear_sale []   =  {0x01, 0x41, 0x46, 0x7F, 0x79};
byte suspend_sale [] =  {0x01, 0x41, 0x44, 0x7F, 0x7B};
byte pump_stop []    =  {0x01, 0x41, 0x5A, 0x7F, 0x65};
byte read_qty []     =  {0x01, 0x41, 0x52, 0x7F, 0x6D};
byte check_nozzle_totalizer [] =  {0x01, 0x41, 0x53, 0x7F, 0x6D};

// HTML web page to handle 3 input fields (input1, input2, input3)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
<title>ESP Input Form</title>
<meta name="viewport" content="width=device-width, initial-scale=1"> 

</head><body>
<form action="/get">
input1: <input type="text" name="input1">
<input type="submit" value="Submit">
</form><br>
<form action="/get">
input2: <input type="text" name="input2">
<input type="submit" value="Submit">
</form><br>
</body></html>)rawliteral";

void notFound(AsyncWebServerRequest *request) 
{
 request->send(404, "text/plain", "kuch nhi mila");
}

int printHex(unsigned int numberToChange)
{
   if(numberToChange>0){
    return(numberToChange+30);
   }
}

void setup() 
{
 Serial.begin(9600);
 Serial2.begin(9600);

 // Initialize the output variables as outputs
 pinMode(output2, OUTPUT);
 pinMode(output26, OUTPUT);
 pinMode(output27, OUTPUT);
 pinMode(output25, OUTPUT);
 pinMode(TX_ENABLE_PIN, OUTPUT);
 
 // Set outputs to LOW
 digitalWrite(output2, LOW);
 digitalWrite(output26, LOW);
 digitalWrite(output27, LOW);
 digitalWrite(output25, LOW);
  
 capacity = (3.14*(radius)*(radius)*probe_length)/1000000; // capacity = litre
 
 WiFi.mode(WIFI_STA);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) 
 {
  delay(500);
  Serial.print(".");
  digitalWrite(output2, HIGH);
  delay(100);
  digitalWrite(output2, LOW);
 }
  
 Serial.println();
 Serial.print("IP Address: ");
 Serial.println(WiFi.localIP());

  // Send web page with input fields to client
 server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
 request->send_P(200, "text/html", index_html);
 });
 // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
 server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
 
 // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
 if (request->hasParam(PARAM_INPUT_1)) 
 {
  inputMessage = request->getParam(PARAM_INPUT_1)->value();
  inputParam = PARAM_INPUT_1;
 }
 // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
 else if (request->hasParam(PARAM_INPUT_2)) 
 {
  inputMessage1 = request->getParam(PARAM_INPUT_2)->value();
  inputParam1 = PARAM_INPUT_2;
 }
 else 
 {
  inputMessage = "No message sent";
  inputParam = "none";
  inputMessage1 = "No message sent";
  inputParam1 = "none";
 }
 Serial.println(inputMessage);
 Serial.println(inputMessage1);
 request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
                                     + inputParam + ") with value: " + inputMessage + 
                                     "<br><a href=\"/\">Return to Home Page</a>");
 Serial.println(inputMessage1);
 request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
                                     + inputParam1 + ") with value: " + inputMessage1 +
                                     "<br><a href=\"/\">Return to Home Page</a>");
 });
 server.onNotFound(notFound);
 server.begin();

}

void loop() 
{
 while (WiFi.status() != WL_CONNECTED) 
 {
  delay(500);
  Serial.print(".");
  digitalWrite(output2, HIGH);
  delay(100);
  digitalWrite(output2, LOW);
 }
 digitalWrite(output2, LOW);
 digitalWrite(output2, HIGH);

 int mes_length = inputMessage1.length();

 if (mes_length > 0){
  A = inputMessage1;
  Serial.print(A);
  inputMessage1 = "";
  mes_length = 0;
  resp = "";
  resp = "QTY Received"+A;
  server.on("/qty_response" ,HTTP_GET, [](AsyncWebServerRequest *request){
  AsyncWebServerResponse *response1 = request->beginResponse(200, "text/plain", resp);
  request->send(response1);
  });
 }

 // turns the GPIOs UnLock and Lock
 if (inputMessage == "lock")  
 {
  output26State = "UnLock";
  digitalWrite(output26, HIGH);
  inputMessage = " ";
  resp = "";
  resp = "Unlocked";
  server.on("/relay_response" ,HTTP_GET, [](AsyncWebServerRequest *request){
  AsyncWebServerResponse *response1 = request->beginResponse(200, "text/plain", resp);
  request->send(response1);
  });
 } 
 else if (inputMessage == "unlock") 
 {
  output26State = "Lock";
  digitalWrite(output26,LOW);
  inputMessage = " ";
  resp = "";
  resp = "Locked";
  server.on("/relay_response" ,HTTP_GET, [](AsyncWebServerRequest *request){
  AsyncWebServerResponse *response1 = request->beginResponse(200, "text/plain", resp);
  request->send(response1);
  });
 } 

 else if (inputMessage == "pump_on") 
 {
  output25State = "off";
  digitalWrite(output25, HIGH);
  inputMessage = " ";
  resp = "";
  resp = "Pump Started";
  server.on("/relay_response" ,HTTP_GET, [](AsyncWebServerRequest *request){
  AsyncWebServerResponse *response1 = request->beginResponse(200, "text/plain", resp);
  request->send(response1);
  });
 } 
 else if (inputMessage == "pump_off") 
 {
  output25State = "on";
  digitalWrite(output25, LOW);
  inputMessage = " ";
  resp = "";
  resp = "Pump Stopped";
  server.on("/relay_response" ,HTTP_GET, [](AsyncWebServerRequest *request){
  AsyncWebServerResponse *response1 = request->beginResponse(200, "text/plain", resp);
  request->send(response1);
  });
 }
 else if (inputMessage == "read_sale") 
 {
  read_sale_F();
  inputMessage = " ";
 }
 else if (inputMessage == "pump_start")
 {
  pump_start_F(); 
  inputMessage = " ";
 }
 else if (inputMessage == "read_preset")
 {
  read_preset_F(); 
  inputMessage = " ";
 }
 else if (inputMessage == "authorize")
 {
  authorize_F();
  inputMessage = " ";
 }
 else if (inputMessage == "clear_sale")
 {
  clear_sale_F();
  inputMessage = " ";
 }
 else if (inputMessage == "suspend_sale")
 {
  suspend_sale_F();
  inputMessage = " ";
 }
 else if (inputMessage == "pump_stop")
 {
  pump_stop_F();
  inputMessage = " ";
 }
 else if (inputMessage == "read_qty")
 {
  read_qty_F();
  inputMessage = " ";
 }
 else if (inputMessage == "send_preset")
 {
  send_preset_F();
  inputMessage = " ";
 }
 else if (inputMessage == "check_nozzle_totalizer")
 {
  check_nozzle_totalizer_F();
  inputMessage = " ";
 }
 
}

void tank1_sensor()
{
 int cmd=analogRead(32);
 int percentage=map(cmd, 0, 1023, 0, 100);
 float z= (percentage*capacity)/100; 
 mystring = String(z);
 
 server.on("/tank1_sensor" ,HTTP_GET, [](AsyncWebServerRequest *request){
 AsyncWebServerResponse *response1 = request->beginResponse(200, "text/plain", mystring);
 request->send(response1);
 }); 
}

void tank2_sensor()
{
 int cmd2=analogRead(33);
 int percentage2=map(cmd2, 0, 1023, 0, 100);
 float z2= (percentage2*capacity)/100;
 mystring2 = String(z2);

 server.on("/tank2_sensor" ,HTTP_GET, [](AsyncWebServerRequest *request){
 AsyncWebServerResponse *response1 = request->beginResponse(200, "text/plain", mystring2);
 request->send(response1);
 });
}

void temp_sensor()
{
 while(Serial.available()>0)
 {
  data = Serial.read();
  if(data !=0x0D && finish ==0)
  {
   dep[i++]= data;
  }
  if(data ==0x0D)
  {
   finish=1;
  }
 }
 if(finish)
 {
  finish=0;
  i=0;
 }
 mystring3 = String(dep); 

 server.on("/temp_sensor" ,HTTP_GET, [](AsyncWebServerRequest *request){
 AsyncWebServerResponse *response1 = request->beginResponse(200, "text/plain", mystring3);
 request->send(response1);
 });
}

void dover_response()
{
 res = "";
 response = 0;
 time_now = millis();
 digitalWrite(TX_ENABLE_PIN, LOW);
 while (Serial2.available()> 0 ) 
 {
  response = Serial2.read();
  Serial.write(response);
  String response_str = String(response);
  res = res+response_str;
 }
 res = "43 4D 59 40 6F" ;

// delay(500);
 server.on("/response" ,HTTP_GET, [](AsyncWebServerRequest *request){
 AsyncWebServerResponse *response1 = request->beginResponse(200, "text/plain", res);
 request->send(response1);
 });
}

String read_sale_F()
{
 digitalWrite (TX_ENABLE_PIN, HIGH);     // enable transmit driver
 Serial2.write(read_sale, sizeof(read_sale));
 Serial.write(read_sale, sizeof(read_sale));
 Serial.println();
 digitalWrite(TX_ENABLE_PIN, LOW);       // disable transmit driver
 delay(500);
 dover_response();
}

void suspend_sale_F()
{
 digitalWrite (TX_ENABLE_PIN, HIGH);     // enable transmit driver
 Serial2.write(suspend_sale, sizeof(suspend_sale));
 Serial.write(suspend_sale, sizeof(suspend_sale));
 Serial.println();
 digitalWrite(TX_ENABLE_PIN, LOW);       // disable transmit driver
 delay(500);
 dover_response();
}

void clear_sale_F()
{
 digitalWrite (TX_ENABLE_PIN, HIGH);     // enable transmit driver
 Serial2.write(clear_sale, sizeof(clear_sale));
 Serial.write(clear_sale, sizeof(clear_sale));
 Serial.println();
 digitalWrite(TX_ENABLE_PIN, LOW);       // disable transmit driver
 delay(500);
 dover_response();
}

void read_preset_F()
{
 digitalWrite (TX_ENABLE_PIN, HIGH);     // enable transmit driver
 Serial2.write(read_preset, sizeof(read_preset));
 Serial.write(read_preset, sizeof(read_preset));
 Serial.println();
 digitalWrite(TX_ENABLE_PIN, LOW);       // disable transmit driver
 delay(500);
 dover_response();
}

void pump_start_F()
{
 digitalWrite (TX_ENABLE_PIN, HIGH);     // enable transmit driver
 Serial2.write(pump_start, sizeof(pump_start));
 Serial.write(pump_start, sizeof(pump_start));
 Serial.println();
 digitalWrite(TX_ENABLE_PIN, LOW);       // disable transmit driver
 delay(500);
 dover_response();
}

void pump_stop_F()
{
 digitalWrite (TX_ENABLE_PIN, HIGH);     // enable transmit driver
 Serial2.write(pump_stop, sizeof(pump_stop));
 Serial.write(pump_stop, sizeof(pump_stop));
 Serial.println();
 digitalWrite(TX_ENABLE_PIN, LOW);       // disable transmit driver
 delay(500);
 dover_response();
}

void authorize_F()
{
 digitalWrite (TX_ENABLE_PIN, HIGH);        // enable transmit driver
 Serial2.write(authorize, sizeof(authorize));
 Serial.write(authorize, sizeof(authorize));
 Serial.println();
 digitalWrite(TX_ENABLE_PIN, LOW);       // disable transmit driver
 delay(500);
 dover_response();
}

void read_qty_F()
{
 digitalWrite (TX_ENABLE_PIN, HIGH);     // enable transmit driver
 Serial2.write(read_qty, sizeof(read_qty));
 Serial.write(read_qty, sizeof(read_qty));
 Serial.println();
 digitalWrite(TX_ENABLE_PIN, LOW);       // disable transmit driver
 delay(500);
 dover_response();
}

void send_preset_F()
{

 int J,K,L,P,set;
  
 set = A.toInt();
       if(set <10){
         J=0;
         K=0;
         L=0;
         P=set;
         Serial.print("Recieved1-");
       }
       if(set >9 && set <100){
         J=0;
         K=0;
         L=((set/10));
         P=(set%10);
         Serial.print("Recieved2-");
       }
       if(set >99 && set <1000){
         J=0;
         K=((set/100));
         L=((set/10)%10);
         P=(set%10);
         Serial.print("Recieved3-");
       }
       if(set >999 && set <10000){
         J=(set/1000);;
         K=((set/100)%10);
         L=((set/10)%10);
         P=(set%10);
         Serial.print("Recieved4-");
       }
      
       J = printHex(J);
       K = printHex(K);
       L = printHex(L);
       P = printHex(P);
       
       byte one=J;
       byte two=K;
       byte three=L;
       byte four=P;
       byte five=0;
       byte six=0;
       byte seven=0;

 byte BCC[]={0x01,0x41,0x50,0x31,one,two,three,four,five,six,seven,0x7F};
 byte result=BCC[0]^BCC[1]^BCC[2]^BCC[3]^BCC[4]^BCC[5]^BCC[6]^BCC[7]^BCC[8]^BCC[9]^BCC[10]^BCC[11];
 byte volume[]={0x01,0x41,0x50,0x31,one,two,three,four,five,six,seven,0x7F,result};

 digitalWrite (TX_ENABLE_PIN, HIGH);     // enable transmit driver
 Serial2.write(volume, sizeof(volume));
 Serial.write(volume, sizeof(volume));
 Serial.println();
 digitalWrite(TX_ENABLE_PIN, LOW);       // disable transmit driver
 delay(500);
  
 dover_response();

}

void check_nozzle_totalizer_F() {
 digitalWrite (TX_ENABLE_PIN, HIGH);     // enable transmit driver
 Serial2.write(check_nozzle_totalizer, sizeof(check_nozzle_totalizer));
 Serial.write(check_nozzle_totalizer, sizeof(check_nozzle_totalizer));
 Serial.println();
 digitalWrite(TX_ENABLE_PIN, LOW);       // disable transmit driver
 delay(500);
 dover_response();
}
