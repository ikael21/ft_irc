
#include "Channel.hpp"

Channel :: Channel(std::string name, std::string user) :
    _name(name), _operator(user) {}

Channel :: ~Channel() {}

void Channel :: _log(std::string message) {
    std::cout << "Channel " << _name << ": " << message << std::endl;
}


bool Channel :: userOnChannel(std::string user) {
    return std::find(_users.begin(), _users.end(), user) != _users.end();
}

void  Channel :: addUser(std::string user) {

    if (isPrivate() && !isInvited(user)) {
        _log("User " + user + " not in the invited list");
        return;
    }

    if (userOnChannel(user)) {
        _log("User " + user + " already in channel");
        return;
    }

    _users.push_back(user);
    _log("User " + user + " added to channel");
}

void Channel :: removeUser(std::string user) {
    
    if (!userOnChannel(user)) {
        _log("User " + user + " not in the channel");
        return;
    }

    _users.erase(std::find(_users.begin(), _users.end(), user));
    _log("User " + user + " remove from channel");

}

void Channel :: setName(std::string name) { 
    
    if (name.empty() && name[0] != '#')
        throw std::invalid_argument("Channel name must starts with '#' symbol");

    if (name.find_first_of(FORBIDDEN_CHANNEL_NAME_SYMBOLS) != std::string::npos) {
        throw std::invalid_argument("Channel name constains forbidden symbols");
    }

    _name = name;
}

void Channel :: setTopic(std::string topic) { _topic = topic; }


bool Channel :: isInvited(std::string user) {
    return std::find(_invited.begin(), _invited.end(), user) != _invited.end();
}


void Channel :: addToInviteList(std::string user) {
    if (!isInvited(user)) {
        _invited.push_back(user);
        _log("User " + user + " added to invite list");
    }
}

/* Need to read more about channel MODEs and whitch we need to implement */
//TODO
void Channel :: setMode(std::string mode) { _mode = mode; }