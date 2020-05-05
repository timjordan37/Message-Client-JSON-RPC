#include "MessageLibrary.hpp"
#include "msglibserverstub.h"

#include <jsonrpccpp/server.h>
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <json/json.h>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <csignal>

/**
 * Copyright 2019 Tim Jordan,
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * <p/>
 * Purpose: C++ class which serves as server for a collection of students. This
 * class is part of a student collection distributed application that uses JsonRPC.
 * Meant to run on OSX, Debian Linux, and Raspberry Pi Debian.
 * <p/>
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Jordan tsjorda1@asu.edu
 *         Software Engineering
 * @version September 2019
 */

using namespace jsonrpc;
using namespace std;

class MessageServer : public msglibserverstub {
    public:
    MessageServer(AbstractServerConnector &connector, int port);
    virtual Json::Value getMessageFromHeaders(const string& userName);
    virtual Json::Value getMessage(const string& header, const string& userName);
    virtual bool deleteMessage(const string& header, const string& userName);
    virtual bool sendClearText(const string& jsonStr, const string& toUser);
    virtual Json::Value getMail(const string& userName);

    private:
    MessageLibrary * messageLib;
    int portNum;
};

MessageServer::MessageServer(AbstractServerConnector &connector, int port) : msglibserverstub(connector) {
    messageLib = new MessageLibrary();
    messageLib->initMessageLibraryFromJsonFile("Messages.json");
    portNum = port;
}

Json::Value MessageServer::getMessageFromHeaders(const string& userName) {
    cout << "Server::getMessageFromHeaders: " << userName << endl;
    return messageLib->getMessageFromHeaders(userName);
}

Json::Value MessageServer::getMessage(const string& header, const string& userName) {
    cout << "Server::getMessage: " << header << " userName: " << userName << endl;
    return messageLib->getMessage(header, userName);
}

bool MessageServer::deleteMessage(const string& header, const string& userName) {
    cout << "Server::deleteMessage: " << header << " userName: " << userName << endl;
    if (messageLib->deleteMessage(header, userName)) {
        return true;
    }
    return false;
}

bool MessageServer::sendClearText(const string& jsonStr, const string& toUser) {
    cout << "Server::sendClearText: " << jsonStr << " toUser: " << toUser << endl;
    if (messageLib->sendClearText(jsonStr, toUser)) {
        return true;
    }
    return false;
}

Json::Value MessageServer::getMail(const string& userName) {
    cout << "Server::getMail: " << userName << endl;
    return messageLib->getMail(userName);
}

int main (int argc, char * argv[]) {
    int port = 1099;
    if (argc > 1) {
        port = atoi(argv[1]);
    }
    HttpServer httpserver(port);
    MessageServer sv(httpserver, port);
    // to use tcp sockets instead of http uncomment below (comment above), and the include
    // for tcpsocketserver.h above. If not running locally, you will need to input ip & port
    // from command line for both server and client programs.
    //TcpSocketServer tcpserver("localhost",port);
    //Server ss(tcpserver, port);
    //std::atexit(exiting);
    auto ex = [] (int i) {
        cout << "server terminating with signal " << i << endl;
        //ss.StopListening();
        exit(0);
        //return 0;
    };
    //^C
    std::signal(SIGINT, ex);
    // abort()
    std::signal(SIGABRT, ex);
    // sent by kill command
    std::signal(SIGTERM, ex);
    // ^Z
    std::signal(SIGTSTP, ex);
    cout << "Student collection server listening on port " << port
        //<< " press return/enter to quit." << endl;
        << " use ps to get pid. To quit: kill -9 pid " << endl;
    sv.StartListening();
    while (true) {
    }
    int c = getchar();
    sv.StopListening();
    return 0;
}
