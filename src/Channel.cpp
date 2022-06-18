
#include "Channel.hpp"

t_channel_mode CHANNEL_MODES[] = { CH_PRIVATE, CH_SECRET, CH_INVITE_ONLY, CH_TOPIC_MODIFIERS };
t_user_mode    USER_MODES[] = { U_INVISIBLE, U_OPERATOR };

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


bool Channel :: userOnChannel(std::string username) {
  return std::find(_users.begin(), _users.end(), username) != _users.end();
}

void Channel :: addUser(std::string username) {

    if (_users.size() == _limit_users) {
        _log("Channel is full");
        return;
    }

    if (isPrivate() && !isInvited(username)) {
        _log("User " + username + " not in the invited list");
        return;
    }

    if (userOnChannel(username)) {
        _log("User " + username + " already in channel");
        return;
    }

    _users.push_back(username);
    _user_mode[username] = std::vector<t_user_mode>();

    _log("User " + username + " added to channel");
}

void Channel :: removeUser(std::string username) {

  if (!userOnChannel(username)) {
    _log("User " + username + " not in the channel");
    return;
  }

  _users.erase(std::find(_users.begin(), _users.end(), username));
  _user_mode.erase(username);
  _log("User " + username + " remove from channel");
}

std::vector<std::string> Channel :: getVisibleUsers() {

  std::vector<std::string> users;

  for (size_t i = 0; i < _users.size(); ++i) {

    if (_userHaveMode(_users[i], U_INVISIBLE))
      continue;

    if (_userHaveMode(_users[i], U_OPERATOR)) {
      users.push_back("@" + _users[i]);
    } else {
      users.push_back(_users[i]);
    }
  }

  return users;
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


bool Channel :: isInvited(std::string username) {
  return std::find(_invited.begin(), _invited.end(), username) != _invited.end();
}


void Channel :: addToInviteList(std::string username) {
  if (!isInvited(username)) {
    _invited.push_back(username);
    _log("User " + username + " added to invite list");
  } else {
    _log("User " + username + " already invited");
  }
}

void Channel :: addChannelMode(std::string mode) { _changeChannelMode(mode, false); }
void Channel :: removeChannelMode(std::string mode) { _changeChannelMode(mode, true); }

void Channel :: _changeChannelMode(std::string mode, bool remove) {

  std::vector<t_channel_mode> all_mode(CHANNEL_MODES, CHANNEL_MODES + sizeof(CHANNEL_MODES) / sizeof(CHANNEL_MODES[0]));

  for (size_t i = 0; i < mode.size(); ++i) {

    if (std::find(all_mode.begin(), all_mode.end(), (t_channel_mode)mode[i]) == all_mode.end()) {
      throw std::invalid_argument(_log_message("Invalid mode '" + mode + "' for channel"));
    }

    bool alreadyHave = haveMode((t_channel_mode)mode[i]);

    if (remove && alreadyHave) {
      _modes.erase(std::find(_modes.begin(), _modes.end(), (t_channel_mode)mode[i]));
    }
    else if (!remove && !alreadyHave) {
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


bool Channel ::_userHaveMode(std::string username, t_user_mode mode) {
  return std::find(_user_mode[username].begin(), _user_mode[username].end(), mode) != _user_mode[username].end();
}

void Channel :: addModeToUser(std::string username, std::string mode) {

  std::vector<t_user_mode> modes(USER_MODES, USER_MODES + sizeof(USER_MODES) / sizeof(USER_MODES[0]));

  if (!userOnChannel(username)) {
    _log("User " + username + " is not on channel");
    return;
  }

  for (size_t i = 0; i < mode.size(); ++i) {

    if (std::find(modes.begin(), modes.end(), (t_user_mode)mode[i]) == modes.end()) {
      throw std::invalid_argument(_log_message("Invalid mode '" + mode + "' for user"));
    }

    if (!_userHaveMode(username, (t_user_mode)mode[i])) {
      _user_mode[username].push_back((t_user_mode)mode[i]);
    }
  }
}

void Channel :: removeUserMode(std::string username, std::string mode) {

  if (!userOnChannel(username)) {
    _log("User " + username + " is not on channel");
    return;
  }

  std::vector<t_user_mode> modes = _user_mode[username];

  for (size_t i = 0; i < mode.size(); ++i) {
    _user_mode[username].erase(std::find(_user_mode[username].begin(), _user_mode[username].end(), (t_user_mode)mode[i]));
  }
}

bool Channel :: userIsOper(std::string username) {

  if (!userOnChannel(username)) {
    _log("User " + username + " is not on channel");
    return false;
  }

  return _userHaveMode(username, U_OPERATOR);
}
