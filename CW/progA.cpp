#include <iostream>
#include "myzmq.h"
#include <string>

#define ADDRESS_C "tcp://127.0.0.1:5555"
#define ADDRESS_B "tcp://127.0.0.1:5556"

int main(){
    zmq::context_t context;
    std::string str;
    zmq::socket_t B(context,ZMQ_REQ);
    zmq::socket_t C(context,ZMQ_REQ);
    B.connect(ADDRESS_B);
    C.connect(ADDRESS_C);
    std::string message, answer;
    while(std::getline(std::cin, str)){
        message = str;
        send_message(C, message);
        std::string size = std::to_string(message.size());
        answer = receive_message(C);
        if(answer != "String received"){
            break;
        }
        send_message(B, size);
        answer = receive_message(B);
        if(answer != "Good"){
            break;
        }
    }

    send_message(C, "close$");
    send_message(B, "end");

    C.disconnect(ADDRESS_C);
    B.disconnect(ADDRESS_B);
    C.close();
    B.close();
    return 0;
}
