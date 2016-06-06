#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int client, server;
    int portNum = 1500;
    bool isExit = false;
    int buffsize = 1024;
    char buffer[buffsize];
    
    struct sockaddr_in serverAddress;
    socklen_t size;
    
    // init socket;
    
    client = socket(AF_INET, SOCK_STREAM,0);
    
    if(client < 0)
    {
        std::cout << "Error establishing connection" << std::endl;
        exit(1);
    }
    std::cout<< "Server socket connection created\n";
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htons(INADDR_ANY);
    serverAddress.sin_port = htons(portNum);
    
    //binding socket
    if(bind(client,(struct sockaddr*)&serverAddress,sizeof(serverAddress)) < 0)
    {
        std::cout<<"Error binding socket..."<<std::endl;
        exit(1);
    }
    size = sizeof(serverAddress);
    std::cout << "Looking for clients..."<<std::endl;
    
    // listening 
    listen(client , 1);
    
    //accept client
    server = accept(client, (struct sockaddr*)&serverAddress,&size);
    if(server < 0)
    {
        std::cout<<"Error on accepting..."<<std::endl;
        exit(1);
    }
    
    while(server > 0)
    {
        strcpy(buffer, "Server Connected..\n");
        send(server,buffer,buffsize,0);
        
        std::cout<<"Connected with client.."<<std::endl;
        std::cout<< "Enter # to end the connection"<<std::endl;
        std::cout <<"Cllient: ";
        
        do {
            recv(server, buffer, buffsize, 0);
            std::cout << buffer << " ";
            if (*buffer == '#') {
                *buffer = '*';
                isExit = true;
            }
        } while (*buffer != '*');
 
        do {
            std::cout << "\nServer: ";
            do {
                std::cin >> buffer;
                send(server, buffer, buffsize, 0);
                if (*buffer == '#') {
                    send(server, buffer, buffsize, 0);
                    *buffer = '*';
                    isExit = true;
                }
            } while (*buffer != '*');
 
            std::cout << "Client: ";
            do {
                recv(server, buffer, buffsize, 0);
                std::cout << buffer << " ";
                if (*buffer == '#') {
                    *buffer == '*';
                    isExit = true;
                }
            } while (*buffer != '*');
        } while (!isExit);
        
        std::cout << "\n\n=> Connection terminated with IP " << inet_ntoa(serverAddress.sin_addr);
        close(server);
        std::cout << "\nGoodbye..." << std::endl;
        isExit = false;
        exit(1);
    }
    
    close(client);
    return 0;
    
}