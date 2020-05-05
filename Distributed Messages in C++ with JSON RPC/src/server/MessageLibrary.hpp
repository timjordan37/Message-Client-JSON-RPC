#ifndef MESSAGELIBRARY_HPP_
#define MESSAGELIBRARY_HPP_

#include "Message.hpp"
#include <iostream>
#include <string>
#include <map>
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
 * @file    MessageLibrary.hpp
 * @date    September, 2019
 **/
//using namespace std;

class MessageLibrary {
    private:
    std::string name; // the name of this MessageLibrary
    std::map<std::string, std::vector<Message>> database; // the members of this MessageLibrary

    public:
    //Constructor
    MessageLibrary();

    // This method reads JSON file
    void initMessageLibraryFromJsonFile(std::string fileName);

    // deconstructor
    ~MessageLibrary();

    // getMessageFromHeaders returns a string array of message headers being sent to toAUserName.
    // Headers returned are of the form: (from user name @ server and message date)
    // e.g., a message from J Buffett with header: Jimmy.Buffet  Tue 18 Dec 5:32:29 2018
    Json::Value getMessageFromHeaders(std::string userName);

    // getMessage returns the Message having the corresponding header. Assume headers are unique.
    // As above, the header has includes (from user name - server and message date)
    Json::Value getMessage(std::string header, std::string userName);

    // deletes the message for this user
    bool deleteMessage(std::string header, std::string userName);

    // Not implement sendClearText in Assign 4.
    // receive clear text from client use java RMI, server will store the message for
    // the corresponding receiver, and delete the message from sender
    bool sendClearText(std::string jsonStr, std::string toUser);

    // get all the emails that belongs to the user who requires it
    Json::Value getMail(std::string userName);

    // save data to JSON file
    bool saveToJsonFile();

};

#endif
