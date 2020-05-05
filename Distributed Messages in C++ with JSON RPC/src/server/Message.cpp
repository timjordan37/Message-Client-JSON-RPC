#include "Message.hpp"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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
 * @file    Message.cpp
 * @date    September, 2019
 **/

using namespace std;

// constructor 1.
Message::Message() {
    messageTo = "";
    messageFrom = "";
    messageSubject = "";
    messageDate = "";
    messageContent = "";
}

// constructor 2, constructor that store all the fields.
Message::Message(string to, string from, string subject, string date, string content) {
    messageTo = to;
    messageFrom = from;
    messageSubject = subject;
    messageDate = date;
    messageContent = content;
}

// constructor 3
Message::Message(const Json::Value& jsonMsg) {
    messageTo = jsonMsg["MessageTo"].asString();
    messageFrom = jsonMsg["MessageFrom"].asString();
    messageSubject = jsonMsg["MessageSubject"].asString();
    messageDate = jsonMsg["MessageDate"].asString();
    messageContent = jsonMsg["MessageContent"].asString();
}

// constructor 4
Message::Message(string jsonString) {
    Json::Reader reader;
    Json::Value root;
    bool parseSuccess = reader.parse(jsonString, root, false);
    if (parseSuccess) {
        messageTo = root["MessageTo"].asString();
        messageFrom = root["MessageFrom"].asString();
        messageSubject = root["MessageSubject"].asString();
        messageDate = root["MessageDate"].asString();
        messageContent = root["MessageContent"].asString();
    }
}

// deconstructor
Message::~Message() {

}

string Message::toJsonString() {
    string ret = "{}";
    Json::Value jsonMsg;
    jsonMsg["MessageTo"] = messageTo;
    jsonMsg["MessageFrom"] = messageFrom;
    jsonMsg["MessageSubject"] = messageSubject;
    jsonMsg["MessageDate"] = messageDate;
    jsonMsg["MessageContent"] = messageContent;
    ret = jsonMsg.toStyledString();
    return ret;
}

Json::Value Message::toJson() {
    Json::Value jsonMsg;
    jsonMsg["MessageTo"] = messageTo;
    jsonMsg["MessageFrom"] = messageFrom;
    jsonMsg["MessageSubject"] = messageSubject;
    jsonMsg["MessageDate"] = messageDate;
    jsonMsg["MessageContent"] = messageContent;
    return jsonMsg;
}

// Check if the messageTo matches the input 'to'.
bool Message::findMessageTo(std::string to) {
    if (Message::messageTo.compare(to) == 0) {
        return true;
    }
    return false;
}

// Getters
string Message::getMessageTo() {
    return messageTo;
}
string Message::getMessageFrom() {
    return messageFrom;
}
string Message::getMessageSubject() {
    return messageSubject;
}
string Message::getMessageDate() {
    return messageDate;
}
string Message::getMessageContent() {
    return messageContent;
}

// Setters
void Message::setMessageTo(string to) {
    messageTo = to;
}
void Message::setMessageFrom(string from) {
    messageFrom = from;
}
void Message::setMessageSubject(string subject) {
    messageSubject = subject;
}
void Message::setMessageDate(string date) {
    messageDate = date;
}
void Message::setMessageContent(string content) {
    messageContent = content;
}
