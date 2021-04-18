#include <iostream>
#include "myzmq.h"
#include <string>

#define ADDRESS_A "tcp://127.0.0.1:5556"
#define ADDRESS_C "tcp://127.0.0.1:5557"

int main(){
    zmq::context_t context;
    std::string str;
    zmq::socket_t A(context,ZMQ_REP);
    zmq::socket_t C(context,ZMQ_REP);

    A.bind(ADDRESS_A);
    C.bind(ADDRESS_C);

    std::string message;
    while(1){
        message = receive_message(A);
        if(message == "end"){
            break;
        }
        std::cout << "A: " << message << std::endl;
        send_message(A, "Good");
        message = receive_message(C);
        std::cout << "C: " << message << std::endl;
        send_message(C, "Good");
        std::cout << std::endl;
    }

    C.unbind(ADDRESS_C);
    A.unbind(ADDRESS_A);
    A.close();
    C.close();
    return 0;
}
