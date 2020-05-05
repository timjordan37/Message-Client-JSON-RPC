#ifndef MESSAGE_HPP_
#define MESSAGE_HPP_

#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <json/json.h>

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
 * @file    Message.hpp
 * @date    September, 2019
 **/

class Message {
    private:
    std::string messageTo;
    std::string messageFrom;
    std::string messageSubject;
    std::string messageDate;
    std::string messageContent;

    public:
    // constructor 1, null constructor.
    Message();

    // constructor 2, constructor that store all the fields.
    Message(std::string to, std::string from, std::string subject, std::string date, std::string content);

    // deconstructor
    ~Message();

    Message(const Json::Value& jsonObj);
    Message(std::string jsonString);
    std::string toJsonString();
    Json::Value toJson();
    //void fromJson(Json::Value json);

    // Check if the messageTo matches the input 'to'.
    bool findMessageTo(std::string to);

    // Getters
    std::string getMessageTo();
    std::string getMessageFrom();
    std::string getMessageSubject();
    std::string getMessageDate();
    std::string getMessageContent();

    // Setters
    void setMessageTo(std::string to);
    void setMessageFrom(std::string from);
    void setMessageSubject(std::string subject);
    void setMessageDate(std::string date);
    void setMessageContent(std::string content);
};

#endif
