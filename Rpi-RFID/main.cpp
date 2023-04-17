#include <stdio.h>
#include <thread>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "MFRC522.h"
#include "bcm2835.h"
#include <iostream>
#include <condition_variable>
#include <string>
#include <sstream>
#include <iomanip>


#define PORT 8008

using namespace std;

class supporter{
public:
  bool interrupt = 0;
  string rfid;
  


  void tcp_msg(int client_fd)
  {
	
	//client_fd = socket(AF_INET, SOCK_STREAM,0);
	
    while(1){
      if(interrupt){
	//cout<<"here1"<<endl;
        write(client_fd, const_cast<char *>(rfid.data()), strlen(const_cast<char *>(rfid.data())));
        //cout<<"here"<<endl;
        interrupt = 0;
        rfid = "";
    }
    }
    // closing the connected socket
    close(client_fd);
  }
  void rfid_read(){
	cout<<"bhak rfid"<<endl;
    MFRC522 mfrc;

	mfrc.PCD_Init();
	//8A 44
	while (true) {
		if (interrupt)
			continue;
		if (!mfrc.PICC_IsNewCardPresent())
			continue;

		if (!mfrc.PICC_ReadCardSerial())
			continue;
		//auto rfid = make_hex_string(std::begin(mfrc.uid.uidByte), std::end(mfrc.uid.uidByte), true, true);
		for (byte i = 0; i < mfrc.uid.size; ++i) {
			
			if (mfrc.uid.uidByte[i] < 0x10) {
				stringstream stream;
				int j = static_cast<int>(mfrc.uid.uidByte[i]);
			        stream <<  setfill ('0') << setw(sizeof(int)*2) << hex  <<  j;
				string s(stream.str());
				rfid.append(" "+ s);
			}
			else {
				
				stringstream stream;
				int j = static_cast<int>(mfrc.uid.uidByte[i]);
			        stream <<  setfill ('0') << setw(sizeof(int)*2) << hex  <<  j;
				string s(stream.str());
			        //s.append( reinterpret_cast<const char*>(mfrc.uid.uidByte[i]));
				//cout<<"reading"<<endl;
				//string rf(rfid,rfid+strlen(rfid));
				//cout<<"reading"<<endl;
				rfid.append(" "+ s);
				//rfid = comb.data());
				
				//rfid = const_cast<char*>(comb.c_str());
				//cout<<"reading2"<<endl;
			}
		}
		//cout<<"reading"<<endl;
		interrupt=1;
		delay(1000);
	}
  }
  thread m1Thread(){
	  return thread([=] {rfid_read(); });
  }
  thread m2Thread(int client){
	  return thread([=] {tcp_msg(client);});
  }
};
int main() {
	supporter support;
	//cout<<"poison"<<endl;
	support.rfid.reserve(10);
	
	int client_fd = socket(AF_INET, SOCK_STREAM,0);
	int status, new_socket,c;//, client_fd;
	struct sockaddr_in serv_addr,client;
	if (client_fd<0){
	  printf("\n Socket creation error \n");
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr= INADDR_ANY;
	serv_addr.sin_port = htons(PORT);
	status = bind(client_fd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
	//c = sizeof(struct sockaddr_in);
	//new_socket = accept(client_fd, (struct sockaddr *)&client, (socklen_t*)&c);
	//if (new_socket < 0) {
	//    printf("\nConnection Failed \n");
	//}
	thread t1 = support.m1Thread();
	//cout<<"rfid start"<<endl;
	thread t2 = support.m2Thread(status);
	//cout<<"tcp start"<<endl;
	t1.join();
	//cout<<"bhak rfid join"<<endl;
	t2.join();
	cout<<"bhak tcp join"<<endl;
}
