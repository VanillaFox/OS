#include <iostream>
#include "myzmq.h"
#include <string>

#define ADDRESS_A "tcp://127.0.0.1:5555"
#define ADDRESS_B "tcp://127.0.0.1:5557"

int main(){
    zmq::context_t context;
    std::string str;
    zmq::socket_t B(context,ZMQ_REQ);
    zmq::socket_t A(context,ZMQ_REP);

    B.connect(ADDRESS_B);
    A.bind(ADDRESS_A);
    std::string message, size, answer;
    
    while(1){
        message = receive_message(A);
        if(message == "close$"){
            break;
        }
        std::cout << message << std::endl;
        send_message(A, "String received");
        size = std::to_string(message.size());
        send_message(B, size);
        answer = receive_message(B);
        if(answer != "Good"){
            break;
        }
    }

    B.disconnect(ADDRESS_B);
    A.unbind(ADDRESS_A);
    A.close();
    B.close();
    return 0;
}
