//
// File:   quanser.h
// Author: Leonardo Dantas de Oliveira
//
// Created on 6 de Março de 2008, 20:23
//

#ifndef _QUANSER_H
#define	_QUANSER_H
#include <unistd.h>
#include <stdlib.h>
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <sstream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <QDebug>

using std::string;
using std::stringstream;

class Quanser {
private:
   char* server;
   int tcpPort;
   int    sockfd;
   char   hostaddress[32];
   struct sockaddr_in address;
   bool status = false;

   /**
    * Tenta conectar ao servidor, se conseguir conectar, retorna 0, se não
    * conseguir, retorna 1
    */
    int connectServer () {
        this->sockfd  = socket(AF_INET, SOCK_STREAM,0);  // criacao do socket

        qDebug() << "sockfd: " << this->sockfd;
        if(this->sockfd < 0) {
            perror("Socket");
            status = false;
            return 1;
        }

        this->address.sin_family = AF_INET;

        this->address.sin_addr.s_addr = inet_addr(this->server);
        this->address.sin_port = htons(this->tcpPort);

        int len = sizeof(this->address);

        /* Ta esperando infinitamente quando nao encontra o ip*/
        int result = connect(this->sockfd, (struct sockaddr *) &this->address, len);

        qDebug() << "4" << result;

        if (result == -1)  {
            perror ("Houve erro no cliente");
            status = false;
            return 1;
        }
        else {
            status = true;
            return 0;
        }
    }

   /**
    *Converte de inteiro para string
    */
    string itoa(int _toConvert){
        stringstream ss;
        string str;
        ss << _toConvert;
        ss >> str;
        return str;
    }
   /**
    *Converte de float para string
    */
    string ftoa(float _toConvert){
        stringstream ss;
        string str;
        ss << _toConvert;
        ss >> str;
        return str;
    }

    string receiveData() {
        char  ch = ' ';
        string _received = "";
        int _count = 0;
        do {
            read(this->sockfd,&ch,1);
            _received.append(1,ch);
            _count++;
        } while (ch != '\n' || _count < 3); //Assumo que nao receberei mensagens menores que 3

        return _received;
    }

    int sendData(string _toSend) {
        int _tamanho = _toSend.length();
        write(this->sockfd,_toSend.c_str(),_tamanho);
        return 0;
    }

public:
    Quanser (const char* _server, int _tcpPort) {
        this->server = new char[strlen(_server)+1];
        this->tcpPort = _tcpPort;
        strcpy(this->server,_server);
        this->connectServer();
    }

    ~Quanser(void){
        delete this->server;
        close(this->sockfd);
    }

   /**
    *Grava a tensao especificada no parametro no canal DA
    */
    int writeDA(int _channel, float _volts) {
        string _toSend = "WRITE ";
        _toSend.append(itoa(_channel));
        _toSend.append(" ");
        _toSend.append(ftoa(_volts));
        _toSend.append("\n");
        this->sendData(_toSend);
        string _rec = this->receiveData();

        if (_rec.find("ACK",0) > _rec.length() ) return -1 ; //erro
        else return 0;
    }


   /**
    *Le o valor de tensao que esta no canal AD especificado
    */
    double readAD(int _channel) {
        string _toSend = "READ ";
        _toSend.append(itoa(_channel));
        _toSend.append("\n");
        this->sendData(_toSend);
        string _rec = this->receiveData();
        return atof(_rec.c_str());
    }

    bool getStatus() {
        return status;
    }
};

#endif	/* _QUANSER_H */
