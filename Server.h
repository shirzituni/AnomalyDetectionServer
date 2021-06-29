// shir zituni	316537992

#ifndef SERVER_H_
#define SERVER_H_

#include "commands.h"
#include "CLI.h"

#include <thread> 
#include <pthread.h>
#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

using namespace std;

// edit your ClientHandler interface here:
class ClientHandler{
public:
    virtual void handle(int clientID)=0;
};

// you can add helper classes here and implement on the cpp file


// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler{
public:
    virtual void handle(int clientID){
		socketIO* socket = new socketIO(clientID);
		CLI* cli= new CLI(socket);
		cli->start();
		delete cli;
		delete socket;
    }
};


// implement on Server.cpp
class Server {
	thread* t; // the thread to run the start() method in
	int fd; 	//file descriptor
	bool member_stop=false;
	sockaddr_in server;
	sockaddr_in client;
	

public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch)throw(const char*);
	void stop();
};

#endif /* SERVER_H_ */
