#include <iostream>
#include "topology.h"
#include <set>
#include "myZmq.h"
#include <string>
#include "tree.h"

int main(){
    std::string command;
    TNode node(-1, -1, -1);
    Tree tree;
    std::string answer;
    while(std::cin >> command){
        if(command == "create"){
            int child;
            std::cin >> child;
            if(tree.exist(child)){
                std::cout << "Error: child already existed!\n";
            }
            else{
                while(true){
                    int idParent = tree.findId();
                    if(idParent == node.id){
                        answer = node.createChild(child);
                        tree.addElem(child, idParent);
                        break;
                    }
                    else{
                        std::string message = "create " + std::to_string(child);
                        answer = node.sendStr(message, idParent);
                        if(answer == "Error: id is not found"){
                            tree.notAvailable(idParent);
                        }
                        else{
                            tree.addElem(child, idParent);
                            break;
                        }
                    }
                }
                std::cout << answer << std::endl;
            }
        }
        else if(command == "ping"){
            int child;
            std::cin >> child;
            if(!tree.exist(child)){
                std::cout << "Error: child is not existed!\n";
            }
            else if(node.leftId == child || node.rightId == child){
                answer = node.Ping(child);
                std::cout << answer << std::endl;
            }
            else{
                std::string message = "ping " + std::to_string(child);
                answer = node.sendStr(message, child);
                if(answer == "Error: id is not found"){
                    answer = "OK: 0";
                }
                std::cout << answer << std::endl;
            }
        }
        else if(command == "exec"){
            std::string str;
            int child;
            std::cin >> child;
            getline(std::cin, str);
            if(!tree.exist(child)){
                std::cout << "Error: child is not existed!\n";
            }else{
                std::string message = "exec " + str;
                answer = node.sendStr(message, child);
                std::cout << answer << std::endl;
            }
        }
        else if(command == "remove"){
            int child;
            std::cin >> child;
            std::string message = "remove";
            if(!tree.exist(child)){
                std::cout << "Error: child is not existed!\n";
            }else{
                answer = node.sendStr(message, child);
                if(answer != "Error: id is not found"){
                    tree.Remove(child);
                    if(child == node.leftId){
                        unbind(node.left, node.leftPort);
                        node.leftId = -2;
                        answer = "OK";
                    }
                    else if(child == node.rightId){
                        node.rightId = -2;
                        unbind(node.right, node.rightPort);
                        answer = "OK";
                    }
                    else{
                        message = "clear " + std::to_string(child);
                        answer = node.sendStr(message, std::stoi(answer));
                    }
                    std::cout << answer << std::endl;
                }
            }
        }
    }
    node.removeElem();
    return 0;
}