#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
 
using namespace std;

//#define PORT_R 13   // Port to receive RFID number
//#define PORT_S 14   // Port to send fuel discharge and receive fuel amount & cost
//#define PORT_JA 15   // Port to send fuel discharge and receive fuel amount & cost to Server
//#define MAX_BUF 4096

 
int main()
{
    string validRFIDs[] = {"8A 44 FA AF", "63 0A 6D AD"};
    int numValidRFIDs = sizeof(validRFIDs) / sizeof(validRFIDs[0]);
    string ipAddress = "127.0.0.1";
    char buf[4096];

    //Port 13
    // Create a socket for Port 13
    int sock_r = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_r == -1)
    {
        cerr << "Can't create a socket! Quitting" << endl;
        return -1;
    }
 
    // Bind the ip address and port to a socket
    sockaddr_in hint_r;
    hint_r.sin_family = AF_INET;
    hint_r.sin_port = htons(13);
    inet_pton(AF_INET, "0.0.0.0", &hint_r.sin_addr);
 
    bind(sock_r, (sockaddr*)&hint_r, sizeof(hint_r));
    listen(sock_r, SOMAXCONN);
    sockaddr_in client_r;
    socklen_t clientSize_r = sizeof(client_r);
    int clientSocket_r = accept(sock_r, (sockaddr*)&client, &clientSize);
	char buf_r[4096];
    
    
    //Port 14 
    // Create a socket for Port 14
    
    int sock_s = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_s == -1)
    {
        cerr << "Can't create a socket! Quitting" << endl;
        return -1;
    }
    
    // Bind the ip address and port to a socket
    sockaddr_in hint_s;
    hint_s.sin_family = AF_INET;
    hint_s.sin_port = htons(14);
    inet_pton(AF_INET, ipAddress.c_str(), &hint_s.sin_addr);
 
    bind(sock_s, (sockaddr*)&hint_s, sizeof(hint_s));
    char buf_s[4096];
    
    //	Connect to the server on the socket
    int connectRes_s = connect(sock_s, (sockaddr*)&hint_s, sizeof(hint_s));
    
    
 //Port 15 
    // Create a socket for Port 15
    
    int sock_ja = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_ja == -1)
    {
        cerr << "Can't create a socket! Quitting" << endl;
        return -1;
    }
    
    // Bind the ip address and port to a socket
    sockaddr_in hint_ja;
    hint_ja.sin_family = AF_INET;
    hint_ja.sin_port = htons(15);
    inet_pton(AF_INET, ipAddress.c_str(), &hint_ja.sin_addr);
 
    bind(sock_ja, (sockaddr*)&hint_ja, sizeof(hint_ja));
       
    //	Connect to the server on the socket
    int connectRes_ja = connect(sock_ja, (sockaddr*)&hint_ja, sizeof(hint_ja));        
    char buf_ja[4096];

    
    
    while(1)
    {
    	
        memset(buf, 0, 4096);
        
		
		#PORT 13 Receive Code 
        // Wait for client to send data
        int bytesReceived_r = recv(clientSocket_r, buf, 4096, 0);
		 
        cout << string(buf, 0, bytesReceived_r) << endl;
        char string(buf, 0, bytesReceived_r); 
 
        // Echo message back to client
    //    send(clientSocket_r, buf_r, bytesReceived_r + 1, 0);   	
        //Authorise RFID code
	    bool found = false;
	    for (int i = 0; i < numValidRFIDs; i++) {
	        if (validRFIDs[i] == string(buf_r, 0, bytesReceived_r)) {
	            found = true;
	            break;
	        }
	    }
	    
	    if(found) 
	    {
	    	string message = "SGAOLEADH_CONNAIDH";
	    	int sendRes_s = send(sock_s, message.c_str(), message.size() + 1, 0);
		}
		else 
		{
			string message = "RFID not Valid";
			int sendRes = send(sock_s, message.c_str(), message.size() + 1, 0);
		}
		
		//		Wait for response
		memset(buf_s, 0, 4096);
        int bytesReceived_s = recv(sock_s, buf_s, 4096, 0);
        if (bytesReceived_s == -1)
        {
            cout << "There was an error getting response from server\r\n";
        }
        else
        {
            //		Display response
            cout << "SERVER> " << string(buf_s, bytesReceived_s) << "\r\n";
        }         
        
        int sendRe_jas = send(sock_ja, string(buf_s, bytesReceived_s), message.size() + 1, 0);
		
		 	
	}       
    return 0;
}
