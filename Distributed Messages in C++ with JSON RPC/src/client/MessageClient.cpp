#include "MessageGui.cpp"
#include <jsonrpccpp/client/connectors/httpclient.h>
#include <json/json.h>
#include "messagelibrarystub.h"
#include "../server/Message.hpp"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Input_Choice.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_Check_Browser.H>

#include <stdio.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <vector>


/**
 * Copyright (c) 2019 Tim Lindquist, Tim Jordan 
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
 * Purpose: C++ FLTK Client UI for Message management.
 * This class extends the Gui component class MessageGui and demonstrates
 * sample control functions that respond to button clicks drop-down selects.
 * This software is meant to run on Linux and MacOS using g++.
 * <p/>
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist (Tim.Lindquist@asu.edu) CIDSE - Software Engineering,
 *                       IAFSE, ASU at the Polytechnic campus
 * @author Tim Jordan tsjorda1@asu.edu, Software Engineering.
 * @file    MessageClient.cpp
 * @date    September, 2019
 **/

using namespace jsonrpc;
using namespace std;

class MessageClient : public MessageGui {

    /** ClickedX is one of the callbacks for GUI controls.
    * Callbacks need to be static functions.
    * @param w
    * @param userdata
    */
    static void ClickedX(Fl_Widget * w, void * userdata) {
        std::cout << "You clicked Exit" << std::endl;
        exit(1);
    }

    /*
    * This method retrieves data from backend and populates the GUI
    * @param w
    * @param userdata
    */
    static void ClickedGet(Fl_Widget * w, void * userdata) {
        // since this is a static method, it does not have access to any instance variables.
        // get access through the userdata argument which is the instance.
        MessageClient* anInstance = (MessageClient*)userdata; // create a pointer to Client class
        std::cout << "You clicked Get" << std::endl;

        // set up short names
        Fl_Multiline_Input * messageFMI = anInstance->messageFMI;
        Fl_Input * dateFI = anInstance->dateFI;
        Fl_Input * subjectFI = anInstance->subjectFI;
        Fl_Input * toFI = anInstance->toFI;
        Fl_Input * fromFI = anInstance->fromFI;
        Fl_Check_Browser * headers = anInstance->headerFCB;
        string userId = anInstance->userId;
        HttpClient httpclient(anInstance->urlServer);
        messagelibrarystub * messageLib = new messagelibrarystub(httpclient);

        // clear all existing entries from the list of message headers
        headers->clear();

        // get headers
        Json::Value jsonHeaders = messageLib->getMessageFromHeaders(userId);
        vector<string> msgHeaders;
        for (int i = 0; i < jsonHeaders.size(); i++) {
            msgHeaders.push_back(jsonHeaders[i].asString());
        }

        if (msgHeaders.size() == 0) { // if not message available for current user
            toFI->value("");
            fromFI->value("");
            dateFI->value("");
            subjectFI->value("");
            messageFMI->value("");
        } else {
            for (int i = 0; i < msgHeaders.size(); i++) {
                headers->add(trimMe(msgHeaders.at(i)).c_str());
            }
            headers->checked(1,1); // check first item

            // populate data to GUI
            Json::Value jsonMsg = (*messageLib).getMessage(msgHeaders.at(0), userId);
            Message msg(jsonMsg);
            toFI->value(msg.getMessageTo().c_str());
            fromFI->value(msg.getMessageFrom().c_str());
            dateFI->value(msg.getMessageDate().c_str());
            subjectFI->value(msg.getMessageSubject().c_str());
            messageFMI->value(msg.getMessageContent().c_str());
        }
    }

    /*
    * This method delete the corresponding message from selected header
    * @param w
    * @param userdata
    */
    static void ClickedDelete(Fl_Widget * w, void * userdata) {
        MessageClient* anInstance = (MessageClient*)userdata; // create a pointer to Client class

        // set up shorter name
        //Fl_Input * from = anInstance->fromFI;
        Fl_Check_Browser * headers = anInstance->headerFCB;
        Fl_Multiline_Input * messageFMI = anInstance->messageFMI;
        Fl_Input * dateFI = anInstance->dateFI;
        Fl_Input * subjectFI = anInstance->subjectFI;
        Fl_Input * toFI = anInstance->toFI;
        Fl_Input * fromFI = anInstance->fromFI;
        string userId = anInstance->userId;
        HttpClient httpclient(anInstance->urlServer);
        messagelibrarystub * messageLib = new messagelibrarystub(httpclient);

        int howMany = headers->nitems(); // get the number of items
        int which = headers->value(); // get the index of current selected item

        if(howMany == 0) { // when no item left, and delete button is clicked
            toFI->value("");
            fromFI->value("");
            dateFI->value("");
            subjectFI->value("");
            messageFMI->value("");
        } else { 
            if (howMany > 0 && which == 0) { // when no selection, but still have items left, and delete button is clicked
                which = 1;
            }

            // delete the selected message from database
            bool isDeleted = (*messageLib).deleteMessage(headers->text(which), userId);
            if (isDeleted) {
                cout << "Message deleted from database. headers->text(which): " << headers->text(which) << endl;
            }

            // clear all existing entries from the list of message headers
            headers->clear();

            // reload all the messages to GUI
            Json::Value jsonHeaders = messageLib->getMessageFromHeaders(userId);
            vector<string> msgHeaders;
            for (int i = 0; i < jsonHeaders.size(); i++) {
                msgHeaders.push_back(jsonHeaders[i].asString());
            }
            
            if (msgHeaders.size() == 0) { // if last item just deleted
                toFI->value("");
                fromFI->value("");
                dateFI->value("");
                subjectFI->value("");
                messageFMI->value("");
            } else {
                for (int i = 0; i < msgHeaders.size(); i++) {
                    headers->add(trimMe(msgHeaders.at(i)).c_str());
                }
                Json::Value jsonMsg = (*messageLib).getMessage(msgHeaders.at(0), userId);
                Message msg(jsonMsg);
                headers->checked(1,1); // check the first item

                // populate data to GUI
                toFI->value(msg.getMessageTo().c_str());
                fromFI->value(msg.getMessageFrom().c_str());
                dateFI->value(msg.getMessageDate().c_str());
                subjectFI->value(msg.getMessageSubject().c_str());
                messageFMI->value(msg.getMessageContent().c_str());
            }
        }
        std::string fr(fromFI->value());
        std::cout << "You clicked Delete" << std::endl;

    }

    /*
    * This method select the header when user click an existing header on left panel
    * @param w
    * @param userdata
    */
    static void SelectedHeader(Fl_Widget * w, void * userdata) {
        MessageClient* anInstance = (MessageClient*)userdata; // create a pointer to Client class

        // set up shorter name
        Fl_Check_Browser * headers = anInstance->headerFCB;
        Fl_Multiline_Input * messageFMI = anInstance->messageFMI;
        Fl_Input * dateFI = anInstance->dateFI;
        Fl_Input * subjectFI = anInstance->subjectFI;
        Fl_Input * toFI = anInstance->toFI;
        Fl_Input * fromFI = anInstance->fromFI;
        string userId = anInstance->userId;
        HttpClient httpclient(anInstance->urlServer);
        messagelibrarystub * messageLib = new messagelibrarystub(httpclient);

        int howMany = headers->nitems(); // get the number of items
        int which = headers->value(); // get the index of current selected item
        std::cout << "You selected a new message header. Index: " << which << std::endl;

        // unselect all but the item just selected. Note, first item is item 1 not 0
        for(int i=1; i<howMany+1; i++){
            if(i != which ){
                headers->checked(i,0);
                std::cout << "calling checked("<< i << ")" << std::endl;
            }
        }
        std::cout << "You selected item " << which << " text " << headers->text(which)
            << "\nbrowser size is " << howMany
            << std::endl;

        // populate data to GUI
        Json::Value jsonMsg = (*messageLib).getMessage(headers->text(which), userId);
        Message msg(jsonMsg);
        toFI->value(msg.getMessageTo().c_str());
        fromFI->value(msg.getMessageFrom().c_str());
        dateFI->value(msg.getMessageDate().c_str());
        subjectFI->value(msg.getMessageSubject().c_str());
        messageFMI->value(msg.getMessageContent().c_str());
    }

    /*
    * Thie method put required message format to GUI
    * @param w
    * @param userdata
    */
    static void ClickedReply(Fl_Widget * w, void * userdata) {
        MessageClient* anInstance = (MessageClient*)userdata; // create a pointer to Client class

        // set up shorter name
        Fl_Check_Browser * headers = anInstance->headerFCB;
        Fl_Multiline_Input * messageFMI = anInstance->messageFMI;
        Fl_Input * dateFI = anInstance->dateFI;
        Fl_Input * subjectFI = anInstance->subjectFI;
        Fl_Input * toFI = anInstance->toFI;
        Fl_Input * fromFI = anInstance->fromFI;
        string userId = anInstance->userId;
        HttpClient httpclient(anInstance->urlServer);
        messagelibrarystub * messageLib = new messagelibrarystub(httpclient);

        int howMany = headers->nitems(); // get the number of items
        int which = headers->value(); // get the index of current selected item

        if (howMany == 0) { // if no item left, and reply button is clicked
            toFI->value("");
            fromFI->value("");
            dateFI->value("");
            subjectFI->value("");
            messageFMI->value("");
        } else { // if still some items left
            if (howMany > 0 && which == 0) { // when no selection, but still have items left, and reply button is clicked
                which = 1;
            }

            Json::Value jsonMsg = (*messageLib).getMessage(headers->text(which), userId);
            Message msg(jsonMsg);

            // get the time in a string
            auto aClk = std::chrono::system_clock::now();
            auto aTime = std::chrono::system_clock::to_time_t(aClk);
            std::string aDateStr(ctime(&aTime));
            // the conversion to string adds a new line at the end of the string. Trim it.
            std::string trimmed(trimMe(aDateStr));
            cout << "\ntrimmed: " << trimmed << "\naDateStr: " << aDateStr << endl;
            // load the date field with the current time in string form
            //dateFI->value(ctime(&aTime));
            dateFI->value(trimmed.c_str());
            std::cout << "date un-trimmed: "<<aDateStr<<". Date from the UI control: "
                << dateFI->value()<<std::endl;

            std::string replyStr = "Reply:\n\n\n" + msg.getMessageContent();
            toFI->value(msg.getMessageFrom().c_str());
            fromFI->value(msg.getMessageTo().c_str());
            //dateFI->value(msg.getMessageDate().c_str());
            subjectFI->value(msg.getMessageSubject().c_str());
            messageFMI->value(replyStr.c_str());
            std::cout << "You clicked Reply" << std::endl;
        }
    }

    /*
    * Thie method sends the JSON formatted message to server.
    * @param w
    * @param userdata
    */
    static void ClickedSend(Fl_Widget * w, void * userdata) {
        MessageClient* anInstance = (MessageClient*)userdata; // create a pointer to Client class
        // set up shorter name
        Fl_Check_Browser * headers = anInstance->headerFCB;
        Fl_Multiline_Input * messageFMI = anInstance->messageFMI;
        Fl_Input * dateFI = anInstance->dateFI;
        Fl_Input * subjectFI = anInstance->subjectFI;
        Fl_Input * toFI = anInstance->toFI;
        Fl_Input * fromFI = anInstance->fromFI;
        string userId = anInstance->userId;
        HttpClient httpclient(anInstance->urlServer);
        messagelibrarystub * messageLib = new messagelibrarystub(httpclient);

        Message msg(toFI->value(), fromFI->value(), subjectFI->value(), dateFI->value(), messageFMI->value());
        bool isSent = messageLib->sendClearText(msg.toJsonString(), toFI->value());
        if (isSent) {
            cout << "Successfully sent the message" << endl;
        }

        // Delete the header from GUI
        int howMany = headers->nitems(); // get the number of items
        int which = headers->value(); // get the index of current selected item

        if(howMany == 0) { // when no item left, and delete button is clicked
            toFI->value("");
            fromFI->value("");
            dateFI->value("");
            subjectFI->value("");
            messageFMI->value("");
        } else { 
            if (howMany > 0 && which == 0) { // when no selection, but still have items left, and delete button is clicked
                which = 1;
            }

            // delete the selected message from database
            bool isDeleted = messageLib->deleteMessage(headers->text(which), userId);
            if (isDeleted) {
                cout << "Message deleted from database. headers->text(which): " << headers->text(which) << endl;
            }

            // clear all existing entries from the list of message headers
            headers->clear();

            // reload all the messages to GUI
            Json::Value jsonHeaders = messageLib->getMessageFromHeaders(userId);
            vector<string> msgHeaders;
            for (int i = 0; i < jsonHeaders.size(); i++) {
                msgHeaders.push_back(jsonHeaders[i].asString());
            }
           
            if (msgHeaders.size() == 0) { // if last item just deleted
                toFI->value("");
                fromFI->value("");
                dateFI->value("");
                subjectFI->value("");
                messageFMI->value("");
            } else {
                for (int i = 0; i < msgHeaders.size(); i++) {
                    headers->add(trimMe(msgHeaders.at(i)).c_str());
                }
                Json::Value jsonMsg = (*messageLib).getMessage(msgHeaders.at(0), userId);
                Message msg(jsonMsg);
                headers->checked(1,1); // check the first item

                // populate data to GUI
                toFI->value(msg.getMessageTo().c_str());
                fromFI->value(msg.getMessageFrom().c_str());
                dateFI->value(msg.getMessageDate().c_str());
                subjectFI->value(msg.getMessageSubject().c_str());
                messageFMI->value(msg.getMessageContent().c_str());
            }
        }
        std::string fr(fromFI->value());
        std::cout << "You clicked Delete" << std::endl;
    }

    /*
    * This method trims out the new line character from a giving string
    * @param str string to trim
    */
    static std::string& trimMe (std::string& str) {
        // right trim
        while (str.length() > 0 && (str[str.length()-1] == ' ' || str[str.length()-1] == '\t'
                                    || str[str.length()-1] == '\n' ||  str[str.length()-1] == '\r')){
            str.erase(str.length ()-1, 1);
        }
        // left trim
        while (str.length () > 0 && (str[0] == ' ' || str[0] == '\t')){
            str.erase(0, 1);
        }
        return str;
    }

    public:
    string userId; // current user id
    string urlServer;
    //messagelibrarystub messageLib;
    MessageClient(const char * name = 0, string urlServer = "http://localhost:1099", string userId = "Tim.Jordan") : MessageGui(name) {
        callback(ClickedX);

        this->urlServer = urlServer;
        this->userId = userId;
        cout << "\n\nurlServer: " << urlServer << "--"<< endl;
        cout << "\n\nuserId: " << userId << "--" << endl;

        // assign the GUI buttons with corresponding function
        getButt->callback(ClickedGet, (void*)this);
        deleteButt->callback(ClickedDelete, (void*)this);
        replyButt->callback(ClickedReply, (void*)this);
        sendButt->callback(ClickedSend, (void*)this);
        headerFCB->callback(SelectedHeader, (void*)this);
        headerFCB->when(FL_WHEN_CHANGED);

    }
};

int main(int argc, char * argv[]) {
    string urlServer = "http://localhost:1099";
    string host = "localhost";
    string port = "1099";
    string userId = "Tim.Jordan";

    if (argc > 1) {
        host = string(argv[1]);
        port = string(argv[2]);
        userId = string(argv[3]);
    } 
    urlServer = "http://" + host + ":" + port;

    //string browserStr = userId + "Message Brower";
    MessageClient msgClient(userId.c_str(), urlServer, userId);
    //MessageClient.setMessageLib(messageLib);
    return (Fl::run());
}
