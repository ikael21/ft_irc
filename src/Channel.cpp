#include "Channel.hpp"
#include "algorithm"
#include "iostream"


Channel :: Channel(std::string name, std::string user) :
    _name(name), _operator(user) {}

Channel :: ~Channel() {}


void Channel :: _log(std::string message) {
    std::cout << "Channel " << _name << ": " << message << std::endl;
}


bool Channel :: userInChannel(std::string user) {
    return std::find(_users.begin(), _users.end(), user) != _users.end();
}


bool Channel :: isInvited(std::string user) {
    return std::find(_invited.begin(), _invited.end(), user) != _invited.end();
}


void  Channel :: addUser(std::string user) {

    if (isPrivate() && !isInvited(user)) {
        _log("User " + user + " not in the invited list");
        return;
    }

    if (userInChannel(user)) {
        _log("User " + user + " already in channel");
        return;
    }

    _users.push_back(user);
    _log("User " + user + " added to channel");
}

void Channel :: removeUser(std::string user) {
    
    if (!userInChannel(user)) {
        _log("User " + user + " not in the channel");
        return;
    }

    _users.erase(std::find(_users.begin(), _users.end(), user));
    _log("User " + user + " remove from channel");

}

void Channel :: setName(std::string name) { 
    _name = name;
}

void Channel :: setTopic(std::string topic) { _topic = topic; }

/** need to resolve */
void Channel :: setMode(std::string mode) { _mode = mode; }
