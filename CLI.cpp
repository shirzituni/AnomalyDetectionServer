// shir zituni	316537992

#include "CLI.h"

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    this->arr_commands[0]= new Command1(dio);
    this->arr_commands[1]= new Command2(dio);
    this->arr_commands[2]= new Command3(dio);
    this->arr_commands[3]= new Command4(dio);
    this->arr_commands[4]= new Command5(dio);
    this->arr_commands[5]= new Command6(dio);
}

void CLI::start(){
    string r;
    //show menu
     this->dio->write("Welcome to the Anomaly Detection Server.\nPlease choose an option:\n");
    //run on the array and show all the commands
    for(int i = 0; i< 6; i++){
        this->dio->write(arr_commands[i]->description);
    }
    r = this->dio->read();
    //while we did not get 6
    while (r.compare("6")!=0){
        
        if(r.compare("1")==0){
            arr_commands[0]->execute();
        }
        else if(r.compare("2")==0){
            arr_commands[1]->execute();
        }
        else if(r.compare("3")==0){
            arr_commands[2]->execute();
        }
        else if(r.compare("4")==0){
            arr_commands[3]->execute();
        }
        else if(r.compare("5")==0){
            arr_commands[4]->execute();
        }
        //show menu;   
        this->dio->write("Welcome to the Anomaly Detection Server.\nPlease choose an option:\n");
        //run on the array and show all the commands
        for(int i = 0; i< 6; i++){
            this->dio->write(arr_commands[i]->description);
        }
        r = this->dio->read();
    }
}
CLI::~CLI() {
}

