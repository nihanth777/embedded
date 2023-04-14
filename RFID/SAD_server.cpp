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
//#define PORT_S 15   // Port to send fuel discharge and receive fuel amount & cost to Server
//#define MAX_BUF 4096

 
int main()
{
    string validRFIDs[] = {"8A 44 FA AF", "63 0A 6D AD"};
    int numValidRFIDs = sizeof(validRFIDs) / sizeof(validRFIDs[0]);
    string ipAddress = "127.0.0.1";
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
    inet_pton(AF_INET, "0.0.0.0", &hint_s.sin_addr);
 
    bind(sock_s, (sockaddr*)&hint_s, sizeof(hint_s));
    char buf[4096];
    

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    //	Connect to the server on the socket
    int connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));
    
    
    while(1)
    {
    	
        memset(buf, 0, 4096);
        
		
		#PORT 13 Receive Code 
        // Wait for client to send data
        int bytesReceived = recv(clientSocket_r, buf, 4096, 0);
		 
        cout << string(buf, 0, bytesReceived) << endl;
        char string(buf, 0, bytesReceived); 
 
        // Echo message back to client
        send(clientSocket, buf, bytesReceived + 1, 0);   	
        
	    bool found = false;
	    for (int i = 0; i < numValidRFIDs; i++) {
	        if (validRFIDs[i] == string(buf, 0, bytesReceived)) {
	            found = true;
	            break;
	        }
	    }
	    
	    if(found) 
	    {
	    	string message = "SGAOLEADH_CONNAIDH";
	    	int sendRes = send(sock_s, message.c_str(), message.size() + 1, 0);
		}
		else 
		{
			string message = "RFID not Valid";
			int sendRes = send(sock_s, message.c_str(), message.size() + 1, 0);
		}
		
		//		Wait for response
		memset(buf, 0, 4096);
        int bytesReceived = recv(sock, buf, 4096, 0);
        if (bytesReceived == -1)
        {
            cout << "There was an error getting response from server\r\n";
        }
        else
        {
            //		Display response
            cout << "SERVER> " << string(buf, bytesReceived) << "\r\n";
        }
        
        
        
        
        
        
    	
	}       
    return 0;
}

