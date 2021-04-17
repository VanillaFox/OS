#include <iostream>
#include "myZmq.h"
#include <string>
#include <sstream>
#include "unistd.h"

class TNode{
    private:
        zmq::context_t context;
    public:
        zmq::socket_t left, right, parent;
        int id, leftId = -2, rightId = -2, parentId;
        int leftPort, rightPort, parentPort;

        TNode(int idElem, int parentP, int parID): id(idElem), parentPort(parentP), parentId(parID),
        left(context,ZMQ_REQ), right(context,ZMQ_REQ), parent(context,ZMQ_REP){
            if(idElem != -1){
                connect(parent, parentPort);
            }
        }
        ~TNode(){}

        std::string createChild(int childId){
            int port;
            bool isleft = false;
            if(leftId == -2){
                leftPort = bind(left, childId);
                leftId = childId;
                port = leftPort;
                isleft = true;
            }else if(rightId == -2){
                rightPort = bind(right, childId);
                rightId = childId;
                port = rightPort;
            }else{
                return "Error create";
            }
            int pid = fork();
            if(pid == 0){
                execl("./server", "server", std::to_string(childId).c_str(), std::to_string(port).c_str(), std::to_string(id).c_str(), (char*)NULL);
                std::cout << "Error execl" << std::endl;
                exit(1);
            }
            else{
                std::string pidChild;
                try{
                    if(isleft){
                        left.setsockopt(ZMQ_SNDTIMEO, 3000);
                        send_message(left, "pid");
                        pidChild = receive_message(left);
                    }
                    else{
                        right.setsockopt(ZMQ_SNDTIMEO, 3000);
                        send_message(right, "pid");
                        pidChild = receive_message(right);
                    }
                    return "OK: " + pidChild;
                }
                catch(int){
                    return "Error: can't connect to child";
                }
            }
        }

        std::string Ping(int idElem){
            std::string answer = "OK: 0";
            if(idElem == id){
                answer = "OK: 1";
                return answer;
            }
            else if(leftId == idElem){
                std::string message = "ping " + std::to_string(idElem);
                send_message(left, message);
                try{
                    message = receive_message(left);
                    if(message == "OK: 1"){
                        answer = message;
                    }
                }
                catch(int){}
            }
            else if(rightId == idElem){
                std::string message = "ping " + std::to_string(idElem);
                send_message(right, message);
                try{
                    message = receive_message(right);
                    if(message == "OK: 1"){
                        answer = message;
                    }
                }
                catch(int){}
            }
            return answer;
        }

        std::string sendStr(std::string str, int idElem){
            std::string answer = "Error: id is not found";

            if(leftId == -2 && rightId == -2){
                return answer;
            }
            else if(leftId == idElem){
                if(Ping(leftId) == "OK: 1"){
                    send_message(left, str);
                    try{
                        answer = receive_message(left);
                    }
                    catch(int){}
                }
            }
            else if(rightId == idElem){
                if(Ping(rightId) == "OK: 1"){
                    send_message(right, str);
                    try{
                        answer = receive_message(right);
                    }
                    catch(int){}
                }
            }
            else{                
                if(Ping(leftId) == "OK: 1"){
                    std::string message = "send " + std::to_string(idElem) + " " + str;
                    send_message(left, message);
                    try{
                        message = receive_message(left);
                    }
                    catch(int){
                        message = "Error: id is not found";
                    }
                    if(message != "Error: id is not found"){
                        answer = message;
                    }
                }
                if(Ping(rightId) == "OK: 1"){
                    std::string message = "send " + std::to_string(idElem) + " " + str;
                    send_message(right, message);
                    try{
                        message = receive_message(right);
                    }
                    catch(int){
                        message = "Error: id is not found";
                    }
                    if(message != "Error: id is not found"){
                        answer = message;
                    }
                }
            }
            return answer;
        }

        std::string exec(std::string str){
            std::istringstream numbers(str);
            int sum = 0;
            int count, number;
            numbers >> count;
            for(int i = 0; i < count; i++){
                numbers >> number;
                sum += number;
            }
            std::string answer = "OK: " + std::to_string(id) + ": " + std::to_string(sum);
            return answer;
        }

        std::string clearChild(int child){
            if(leftId == child){
                leftId = -2;
                unbind(left, leftPort);
            }
            else{
                rightId = -2;
                unbind(right, rightPort);
            }
            return "OK";
        }

        std::string removeElem(){
            //std::string answer;
            if(leftId != -2){
                if(Ping(leftId) == "OK: 1"){
                    std::string message = "remove";
                    send_message(left, message);
                    try{
                        message = receive_message(left);
                    }
                    catch(int){}
                    unbind(left, leftPort);
                    left.close();
                }
            }
            if(rightId != -2){
                if(Ping(rightId) == "OK: 1"){
                    std::string message = "remove";
                    send_message(right, message);
                    try{
                        message = receive_message(right);
                    }
                    catch(int){}
                    unbind(right, rightPort);
                    right.close();
                }
            }
            return std::to_string(parentId);
        }
};