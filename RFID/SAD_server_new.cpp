
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <cstring>
#include <thread>
#include <stdio.h>
#include <ctime>


using namespace std;

#define PORT_R 8008   // Port to receive RFID number
#define PORT_AA 8007  // Port to send GET balance
#define PORT_S 8009   // Port to recieve fuel discharge and receive fuel amount & cost
#define PORT_JA  8010  // Port to send fuel discharge and receive fuel amount & cost to Server
#define PORT_DI 8011
//#define MAX_BUF 4096
string validRFIDs[2][2] = {{"0000008a 00000044 000000fa 000000af","BD27069"},{ "00000063 0000000a 0000006d 000000ad","GL25108"}};
int numValidRFIDs = 2;
char *server_reply,*android_reply,*server_reply_rc ;
string VehicleNmber;
//const char *message;
int interrupt1 = 0, interrupt2 = 0, interrupt3 = 0, interrupt4 = 0, interrupt5 = 0,interrupt6 = 0;
string *Final_data;
class supporter
{
public:


  void tcp_msg1(int connect_server)
  {

	
    while(true)
    {
        int rfid_recv = recv(connect_server, server_reply , 4096 , 0);
	
	      
	
        if (rfid_recv > 0)
        {
          
            interrupt1 = 0;
           
	    continue;
	
        }

        else
        {
            
            
	    interrupt1 = 1;
	    cout<<"Authorised1"<<endl;
	    string server_reply = "0000008a 00000044 000000fa 000000af";// Comment out Once RFID data can be sent	    
	    for(int i=0; i<numValidRFIDs ; i++)
	    {
	      cout<<"Test"<<endl;
	      if(server_reply == validRFIDs[i][0])
	      {
		cout<<"Authorised2"<<endl;
		VehicleNmber =  validRFIDs[i][1];
		interrupt1 = 1;
		
	      }
	    }
	  continue;
         
        }
      
      
	}
 
  }


   void tcp_msg2(int client_android)
  {
    cout<<"in tcp_msg2"<<endl;
    
      if(interrupt1==1)
      {
      cout<<"i tcp_msg2in interrupt"<<endl;
      char* message = "GET BALANCE";
      //int message = 77;
      
      int sendRes = send(client_android , &message , sizeof(message), 0);
     
	if( sendRes < 0)
        {
		interrupt2=0;
		
		cout<<"Send failed in T2"<<endl;
	}else {
	    cout<<"Send complete in T2"<<endl;
	    interrupt2=1;
	}*/
	//cout<<"Data Send in T2\n"<<endl;
      }
  }

 void tcp_msg3(int client_app)
  {
   
    if(interrupt2==1)
    {	
	if (recv(client_app, android_reply , 4096, 0) < 0){
		cout<<"recv failed in T3"<<endl;
		interrupt3=0;
	}
	else
	{
	  cout<<"Android reply sent successfully";
	  interrupt3=1;
	}
      }
  }


   void tcp_msg4(int client_disp)
  {
   
    if(interrupt3==1)
    {
	android_reply ="22";
	int sendRes1 = write(client_disp, android_reply  , strlen(android_reply));

	if(sendRes1 <0)
	{
	  interrupt4 =0;
	}
	else
	{
	  interrupt4=1;
	}
      }


  }


   void tcp_msg5(int client_receipt)
  {
    interrupt4==1;
  
   if(interrupt4==1)
   {
      int rfid_recv = recv(client_receipt, server_reply_rc , 4096 , 0);

        if (rfid_recv <= 0)
        {
            
            interrupt5 = 0;
            	    
        }
        else
        {
		interrupt5 = 1;
	    }
    }
  }


   void tcp_msg6(int client_appdata)
  {
	 if(interrupt5==1)
   {
     time_t now = time(0);
     char* dt = ctime(&now);
     const char *Final_data1 = ((VehicleNmber +"," ).c_str());
     char *Finaldata=new char[strlen(Final_data1)+strlen(server_reply_rc)+strlen(dt)+1];
     strcpy(Finaldata,Final_data1);
     strcpy(Finaldata, server_reply_rc);
     strcpy(Finaldata, dt);
     
     
     
     int sendRes2 = send(client_appdata, Final_data  , 4096, 0);
  }

  }

  thread m1Thread(int client_rfid){
	  return thread([=] {tcp_msg1(client_rfid);});
  }

  thread m2Thread(int client_android){
    	  return thread([=] {tcp_msg2(client_android);});  
  }
  thread m3Thread(int client_app){
    	  return thread([=] {tcp_msg3(client_app);});  
  }
  thread m4Thread(int client_disp){	  
   return thread([=] {tcp_msg4(client_disp);}); 
    }

  thread m5Thread(int client_receipt){	  
    return thread([=] {tcp_msg5(client_receipt);});  
    }


  thread m6Thread(int client_appdata){	  return thread([=] {tcp_msg6(client_appdata);});  }

};
int main() {
	supporter *support;
	VehicleNmber.reserve(40);
	
	////////////////////////////////////////
	//1. Connection with Dispensor
	int client_rfid = socket(AF_INET, SOCK_STREAM,0);

	//Prepare the sockaddr_in structure
	sockaddr_in server_rfid;
	server_rfid.sin_family = AF_INET;
	server_rfid.sin_addr.s_addr = INADDR_ANY;
	server_rfid.sin_port = htons(PORT_R);
	char ip_str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(server_rfid.sin_addr), ip_str, INET_ADDRSTRLEN);

	//Bind
	
	 int bind_server = bind(client_rfid,(struct sockaddr *)&server_rfid , sizeof(server_rfid));
	 
	 int connect_server = connect(client_rfid , (struct sockaddr *)&server_rfid , sizeof(server_rfid)) ;
	 

	 if (connect_server< 0)	 { 		cout<<"connect error"<<endl; 	 }
	else	{	    cout<<"connection made"<<endl;	}

	if(bind_server < 0)	{	    cout<<"bind failed"<<endl;	}
	else	{	     cout<<"binding completed"<<endl;	}
	
	
	////////////////////////////////////////////////////////////////////
	//2. Connection with App Server
	int client_android = socket(AF_INET, SOCK_STREAM,0);
      if (client_android< 0)	 { 		cout<<"App server connect error"<<endl; 	 }
	else	{	    cout<<"App server connection made"<<endl;	}
	    //Prepare the sockaddr_in structure
	sockaddr_in server_android;
	server_android.sin_family = AF_INET;
	server_android.sin_addr.s_addr = inet_addr("192.168.29.177");
	server_android.sin_port = htons(PORT_AA);
	

	//Bind
	 int bind_android = bind(client_android,(struct sockaddr *)&server_android , sizeof(server_android));
	 int connect_android = connect(client_android, (struct sockaddr *)&server_android , sizeof(server_android)) ;

	
	////////////////////////////////
	//3. Receive data from App
	int client_app = socket(AF_INET, SOCK_STREAM,0);
	     if (client_app< 0)	 { 		cout<<"client_app connect error"<<endl; 	 }
	else	{	    cout<<"client_app connection made"<<endl;	}

	//Prepare the sockaddr_in structure
	sockaddr_in server_app;
	server_app.sin_family = AF_INET;
	server_app.sin_addr.s_addr = INADDR_ANY;
	server_app.sin_port = htons(8009);
	

	//Bind
	
	 int bind_app = bind(client_app,(struct sockaddr *)&server_app , sizeof(server_app));
	 int connect_app = connect(client_app , (struct sockaddr *)&server_app , sizeof(server_app)) ;
	 if (connect_app< 0)	 { 		cout<<"connect_app connect error"<<endl; 	 }
	else	{	    cout<<"connect_app connection made"<<endl;	}

		
	
	////////////////////////////////////////////////////////////////////
	//4. Connection with Dispensor
	int client_disp = socket(AF_INET, SOCK_STREAM,0);
      if (client_disp < 0)	 { 		cout<<"App server connect error"<<endl; 	 }
	else	{	    cout<<"App server connection made"<<endl;	}
	    //Prepare the sockaddr_in structure
	sockaddr_in server_disp;
	server_disp.sin_family = AF_INET;
	server_disp.sin_addr.s_addr = INADDR_ANY;
	server_disp.sin_port = htons(8009);
	

	
	 int bind_disp = bind(client_disp,(struct sockaddr *)&server_disp , sizeof(server_disp));
	 int connect_disp = connect(client_disp, (struct sockaddr *)&server_disp , sizeof(server_disp)) ;
    if (bind_disp < 0)	 { 		cout<<"bind_disp connect error"<<endl; 	 }
	else	{	    cout<<"bind_disp connection made"<<endl;	}
	if (connect_disp < 0)	 { 		cout<<"connect_disp connect error"<<endl; 	 }
	else	{	    cout<<"connect_disp connection made"<<endl;	}
	  
      ////////////////////////////////////////
	//5. Connection with Final Receipt
	int client_receipt= socket(AF_INET, SOCK_STREAM,0);

	//Prepare the sockaddr_in structure
	sockaddr_in server_receipt;
	server_receipt.sin_family = AF_INET;
	server_receipt.sin_addr.s_addr = INADDR_ANY;
	server_receipt.sin_port = htons(8009);
	

	//Bind
	 int bind_receipt = bind(client_receipt,(struct sockaddr *)&server_receipt , sizeof(server_receipt));
	 int server_receipt= accept(client_receipt, (sockaddr*)&server_receipt, sizeof(server_receipt));
	 int connect_receipt = connect(client_receipt , (struct sockaddr *)&server_receipt , sizeof(server_receipt)) ;

	
	
	////////////////////////////////////////
	//6. Connection with Updating App with data
	int client_appdata= socket(AF_INET, SOCK_STREAM,0);

	//Prepare the sockaddr_in structure
	sockaddr_in server_appdata;
	server_appdata.sin_family = AF_INET;
	server_appdata.sin_addr.s_addr = INADDR_ANY;
	server_appdata.sin_port = htons(PORT_R);
	

	//Bind
	 int bind_appdata = bind(client_appdata,(struct sockaddr *)&server_appdata , sizeof(server_appdata));
	 int server_appdata= accept(client_receipt, (sockaddr*)&server_receipt, sizeof(server_receipt));
	 int connect_appdata = connect(client_appdata , (struct sockaddr *)&server_appdata , sizeof(server_appdata)) ;

	
	  
	thread t1 = support->m1Thread(connect_server);
	thread t2 = support->m2Thread(connect_android);
	thread t3 = support->m3Thread(connect_app);
	thread t4 = support->m4Thread(connect_disp);
	thread t5 = support->m5Thread(connect_receipt);
	thread t6 = support->m6Thread(connect_appdata);

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
}
