
#include "Channel.hpp"

Channel :: Channel() :
    _limit_users(DEFAULT_USERS), _modes(std::vector<t_channel_mode>(DEFAULT_CHANNEL_MODES)) {}

Channel :: Channel(std::string name, std::string user) :
    _name(name), _operator(user), _limit_users(DEFAULT_USERS), _modes(std::vector<t_channel_mode>(DEFAULT_CHANNEL_MODES)) {}

Channel :: ~Channel() {}

std::string Channel ::_log_message(std::string message) {
    return "Channel " + _name + ": " + message;
}

void Channel :: _log(std::string message) {
    std::cout << _log_message(message) << std::endl;
}


bool Channel :: userOnChannel(std::string user) {
    return std::find(_users.begin(), _users.end(), user) != _users.end();
}

void Channel :: addUser(std::string user) {

    if (_users.size() == _limit_users) {
        _log("Channel is full");
        return;
    }

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
        throw std::invalid_argument(_log_message("Channel name must starts with '#' symbol"));

    if (name.find_first_of(FORBIDDEN_CHANNEL_NAME_SYMBOLS) != std::string::npos) {
        throw std::invalid_argument(_log_message("Channel name constains forbidden symbols"));
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
    } else {
        _log("User " + user + " is not in invite list");
    }
}

void Channel :: setChannelMode(std::string mode) {

    std::vector<t_channel_mode> modes(CHANNEL_MODES);

    for (size_t i = 0; i < mode.size(); ++i) {
        if (std::find(modes.begin(), modes.end(), (t_channel_mode)mode[i]) == modes.end()) {
            throw std::invalid_argument(_log_message("Invalid mode '" + std::string(mode[i], 1) + "' for channel"));
        }

        if (!haveMode((t_channel_mode)mode[i])) {
            _modes.push_back((t_channel_mode)mode[i]);
        }
    }
}

bool Channel :: isPrivate() {
    return haveMode(CH_PRIVATE);
}

bool Channel :: haveMode(t_channel_mode mode) {
    return std::find(_modes.begin(), _modes.end(), mode) != _modes.end();
}
