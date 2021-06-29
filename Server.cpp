// shir zituni	316537992

#include "Server.h"

Server::Server(int port)throw (const char*) {
	//send system call to linux op, ask to open network channel
	fd = socket(AF_INET,SOCK_STREAM,0);
	if(fd<0){
		throw "socket failed";
	}
	// working with IPV4
	server.sin_family = AF_INET; 
	//server's IP
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);
	//bind
	if(bind(fd,(struct sockaddr *) &server, sizeof(server))<0){
		throw "bind failure";
	}
	//ask to listen to the socketdd
	if(listen(fd, 3) < 0){
		throw "listen failure";
	}
	member_stop = true;	
}
//deal with few clients together- put the accept in loop , handle and close outside
void Server::start(ClientHandler& ch)throw(const char*){
	//ADD- loop while(!stop) accept after accept - bool flag, and after that, close

	t = new thread([&ch,this](){
		while(member_stop){
			//cout<< "waiting for a client"<< endl;
			alarm(5);
			socklen_t clientSize= sizeof(client);
			int aClient = accept(fd,(struct sockaddr*)&client, &clientSize);
			//alarm(0);
			if(aClient < 0){
				throw "accept failure";
			}
			//cout<< "client connected" << endl;
			ch.handle(aClient);
		}
		close(fd);
	});
}

void Server::stop(){
	//flag that chcek when to stop 
	member_stop = false;
	t->join(); 
}

Server::~Server() {
	delete t;
}

