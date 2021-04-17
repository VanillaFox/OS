#pragma once
#include <zmq.hpp>
#include <iostream>
#include <string>

const int MAIN_PORT = 4040;

void send_message(zmq::socket_t &socket, const std::string &msg) {
    zmq::message_t message(msg.size());
    memcpy(message.data(), msg.c_str(), msg.size());
    socket.send(message);
}

std::string receive_message(zmq::socket_t &socket) {
    zmq::message_t message;
    int chars_read;
    try {
        chars_read = (int)socket.recv(&message);
    }
    catch (...) {
        chars_read = 0;
    }
    if (chars_read == 0) {
        throw -1;
    }
    std::string received_msg(static_cast<char*>(message.data()), message.size());
    return received_msg;
}

void connect(zmq::socket_t &socket, int port) {
    std::string adress = "tcp://127.0.0.1:" + std::to_string(port);
    socket.connect(adress);
}

void disconnect(zmq::socket_t &socket, int port) {
    std::string adress = "tcp://127.0.0.1:" + std::to_string(port);
    socket.disconnect(adress);
}

int bind(zmq::socket_t &socket, int id) {
    int port = MAIN_PORT + id;
    std::string adress = "tcp://127.0.0.1:" + std::to_string(port);
    while(1){
        try{
            socket.bind(adress);
            break;
        }
        catch(...){
            port++;
        }
    }
    return port;
}

void unbind(zmq::socket_t &socket, int port) {
    std::string adress = "tcp://127.0.0.1:" + std::to_string(port);
    socket.unbind(adress);
}
