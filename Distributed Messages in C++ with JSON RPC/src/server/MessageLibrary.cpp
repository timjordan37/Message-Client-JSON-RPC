#include "MessageLibrary.hpp"
#include "Message.hpp"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cstddef>
#include <json/json.h>
#include <json/writer.h>

/**
 * Copyright (c) 2019 Tim Jordan
 * Software Engineering,
 * Arizona State University at the Polytechnic campus
 * <p/>
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation version 2
 * of the License.
 * <p/>
 * This program is distributed in the hope that it will be useful,
 * but without any warranty or fitness for a particular purpose.
 * <p/>
 * Please review the GNU General Public License at:
 * http://www.gnu.org/licenses/gpl-2.0.html
 * see also: https://www.gnu.org/licenses/gpl-faq.html
 * so you are aware of the terms and your rights with regard to this software.
 * Or, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,USA
 * <p/>
 * Purpose: C++ FLTK client UI for Message management.
 * This class extends the Gui component class MessageGui and demonstrates
 * sample control functions that respond to button clicks drop-down selects.
 * This software is meant to run on Linux and MacOS using g++.
 * <p/>
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 *
 * @author Tim Jordan tsjorda1@asu.edu, Software Engineering.
 * @file    MessageLibrary.cpp
 * @date    September, 2019
 **/

using namespace std;

/*
* Constructor
*/
MessageLibrary::MessageLibrary() {}

void MessageLibrary::initMessageLibraryFromJsonFile(string fileName) {
    ifstream jsonFile(fileName.c_str());
    string line;
    cout << "The content of the Json file as a string: " << endl;
    if (jsonFile.is_open()) {
        while (getline(jsonFile, line)) {
            cout << line << endl;
        }
    } else {
        cout << "Json file not opened properly" << endl;
    }
    jsonFile.close();
    Json::Reader reader;
    Json::Value root;
    ifstream json(fileName.c_str(), ifstream::binary); // read the file
    bool parseSuccess = reader.parse(json, root, false);
    if (parseSuccess) {
        cout << "successful parse" << endl;
        Json::Value::Members userCollection = root.getMemberNames(); // get list of members (std::vector<std::string>)
        for (vector<string>::const_iterator i = userCollection.begin(); i != userCollection.end(); i++) {
            cout << *i << " " << endl;
            Json::Value user = root[*i];
            string nameStr = "name";
            if (nameStr.compare(*i) == 0) {
                name = user.asString();
                cout << "found name: " << *i << " value: " << name << endl;
            } else {
                cout << "found " << *i << endl;
                for ( int index = 0; index < user.size(); index++) {
                    const Json::Value msgFields = user[index]; //get fields objects for each user
                    string to = msgFields["MessageTo"].asString();
                    string from = msgFields["MessageFrom"].asString();
                    string subject = msgFields["MessageSubject"].asString();
                    string date = msgFields["MessageDate"].asString();
                    string content = msgFields["MessageContent"].asString();

                    Message msg(to, from, subject, date, content);
                    //messages.push_back(msg);
                    database[*i].push_back(msg); // add msg under current user
                } // end index loop
            } // end else
        } // end i loop
    } // end if

}

// Destructor
MessageLibrary::~MessageLibrary() {
}

/*
* getMessageFromHeaders returns a string array of message headers being sent to userName.
* Headers returned are of the form: (from user name @ server and message date)
* e.g., a message from J Buffett with header: Jimmy.Buffet  Tue 18 Dec 5:32:29 2018
* @param userName current user
*/
Json::Value MessageLibrary::getMessageFromHeaders(string userName) {
    Json::Value ret(Json::arrayValue);
    vector<string> headerVect;
    vector<Message> messages = database.at(userName);

    for (int i = 0; i < messages.size(); i++) {
        if (messages.at(i).findMessageTo(userName)) {
            headerVect.push_back(messages.at(i).getMessageFrom() + " " + messages.at(i).getMessageDate());
        }
    }
    for (vector<string>::iterator it = headerVect.begin(); it != headerVect.end(); it++) {
        ret.append(Json::Value(*it));
    }

    return ret;
}

/*
* getMessage returns the Message having the corresponding header. Assume headers are unique.
* As above, the header has includes (from user name - server and message date)
* @param header the header to search
* @param userName current user
*/
Json::Value MessageLibrary::getMessage(string header, string userName) {
    vector<Message> messages = database.at(userName);
    Message msg;
    string name;
    string date;
    // find the matching MessageFrom and MessageDate
    for (int i = 0; i < messages.size(); i++) {
        size_t pos = header.find_first_of(" ");
        if (pos != string::npos) {
            //cout << "pos: " << pos << endl;
            name = header.substr(0, pos);
            date = header.substr(pos + 1, header.size());
            //cout << "ML debug 4-----" << "name: " << name << " date: " << date << endl;
            if (name.compare(messages.at(i).getMessageFrom()) == 0) {
                if (date.compare(messages.at(i).getMessageDate()) == 0) {
                    msg = messages.at(i);
                } // end date compare
            } // end name compare
        } // end pos compare
    } // end i loop
    return msg.toJson();
}

/*
* deletes the message for current user
* @header
* @userName
*/
bool MessageLibrary::deleteMessage(string header, string userName) {
    Message msg;
    string name;
    string date;

    // if does not contain the userName
    map<string, vector<Message>>::iterator it = database.find(userName);
    if (it == database.end()) {
        cout << "Fail to delete the message, user: " << userName << " does not exist." << endl;
        return false;
    }

    // find the matching MessageFrom and MessageDate
    vector<Message> * messages = &(database.at(userName));
    for (int i = 0; i < (*messages).size(); i++) {
        size_t pos = header.find_first_of(" ");
        if (pos != string::npos) {
            name = header.substr(0, pos);
            date = header.substr(pos + 1, header.size());
            if (name.compare((*messages).at(i).getMessageFrom()) == 0) {
                if (date.compare((*messages).at(i).getMessageDate()) == 0) {
                    (*messages).erase((*messages).begin() + i);
                    cout << "Successfully delete message for " << userName << endl;
                    return true;
                } // end date compare
            } // end name compare
        } // end pos compare
    } // end i loop
    cout << "Fail to delete the message, " << userName << " does not contain this message." << endl;
    return false;
}

/**
* This method will store the received jsonStr to JSON file.
* @param jsonStr json string of Message object
* @param toUser the user to store the message
*/
bool MessageLibrary::sendClearText(string jsonStr, string toUser) {
    Message msg(jsonStr);
    cout << "ML::sendClearText: " << msg.toJsonString() << endl;
    //vector<Message> msgVect = database.at(toUser);
    database.at(toUser).push_back(msg);
    this->saveToJsonFile();

    return true;
}

/**
* This method is not used.
*/
Json::Value MessageLibrary::getMail(string userName) {
    Json::Value ret = getMessageFromHeaders(userName);
    return ret;
}

/**
* Save the current data into JSON file.
*/
bool MessageLibrary::saveToJsonFile() {
    string jsonFileName = "Messages.json";
    bool ret = false;
    Json::Value jsonDatabase;
    for (map<string, vector<Message>>::iterator it = database.begin(); it != database.end(); it++) {
        Json::Value jsonMessageVect(Json::arrayValue);
        string key = it->first;
        vector<Message> msgVect = it->second;
        for (int i = 0; i < msgVect.size(); i++) {
            jsonMessageVect.append(msgVect.at(i).toJson());
        }
        jsonDatabase[key] = jsonMessageVect;
    }
    ret = true;
    Json::StyledStreamWriter ssw(" ");
    ofstream jsonOutFile(jsonFileName.c_str(), ofstream::binary);
    ssw.write(jsonOutFile, jsonDatabase);
    return ret;
}
