
#include "Channel.hpp"

#define DEBUG_LOG(x) do { if (DEBUG) { std::cerr << x << std::endl; } } while (0)

t_channel_mode CHANNEL_MODES[] = { CH_PRIVATE, CH_SECRET, CH_INVITE_ONLY, CH_TOPIC_MODIFIERS };
t_user_mode    USER_MODES[] = { U_INVISIBLE, U_OPERATOR };


Channel :: Channel() :
    _limit_users(DEFAULT_USERS), _modes(std::vector<t_channel_mode>(DEFAULT_CHANNEL_MODES)) {}


Channel :: ~Channel() {}


std::string Channel ::_log_message(std::string message) {
  return "Channel " + _name + ": " + message;
}

void Channel :: _log(std::string message) {
  DEBUG_LOG(_log_message(message));
}


bool Channel :: userOnChannel(User& user) {
  return std::find(_users.begin(), _users.end(), &user) != _users.end();
}

void Channel :: addUser(User &user) {

  if (_users.size() == _limit_users) {
    return _log("Channel is full");
  }

  if (isPrivate() && !isInvited(user)) {
    return _log("User " + user.getUsername() + " not in the invited list");
  }

  if (userOnChannel(user)) {
    return _log("User " + user.getUsername() + " already in channel");
  }

  _users.push_back(&user);
  _log("User " +  user.getUsername() + " added to channel");
}

void Channel :: removeUser(User &user) {

  if (!userOnChannel(user)) {
    return _log("User " + user.getUsername() + " not in the channel");
  }

  _users.erase(std::find(_users.begin(), _users.end(), &user));
  _user_mode.erase(&user);
  _log("User " +  user.getUsername() + " remove from channel");
}

std::vector<User *> Channel :: getVisibleUsers() {

  std::vector<User *> users;

  for (size_t i = 0; i < _users.size(); ++i) {

    if (_userHaveMode(*_users[i], U_INVISIBLE))
      continue;

    users.push_back(_users[i]);
  }

  return users;
}

void Channel :: setName(std::string name) {

  if (name.empty() && name[0] != '#') {
    return _log("Channel name must starts with '#' symbol");
  }

  if (name.find_first_of(FORBIDDEN_CHANNEL_NAME_SYMBOLS) != std::string::npos) {
    return _log("Channel name constains forbidden symbols");
  }

  _name = name;
}


void Channel :: setKey(std::string key) { _key = key; }


void Channel :: setTopic(std::string topic) { _topic = topic; }


bool Channel :: isInvited(User& user) {
  return std::find(_invited.begin(), _invited.end(), &user) != _invited.end();
}


void Channel :: addToInviteList(User& user) {
  if (!isInvited(user)) {
    _invited.push_back(&user);
    _log("User " + user.getUsername() + " added to invite list");
  } else {
    _log("User " + user.getUsername() + " already invited");
  }
}


bool Channel :: isBanned(User& user) {
    return std::find(_banned.begin(), _banned.end(), &user) != _banned.end();
}


void Channel :: addToBanList(User& user) {
  if (!isBanned(user)) {
    _banned.push_back(&user);
    _log("User " + user.getUsername() + " added to ban list");
  } else {
    _log("User " + user.getUsername() + " already banned");
  }
}


void Channel :: addChannelMode(const std::string& mode) { _changeChannelMode(mode, false); }
void Channel :: removeChannelMode(const std::string& mode) { _changeChannelMode(mode, true); }

void Channel :: _changeChannelMode(const std::string& mode, bool remove) {

  std::vector<t_channel_mode> all_mode(CHANNEL_MODES, CHANNEL_MODES + sizeof(CHANNEL_MODES) / sizeof(CHANNEL_MODES[0]));

  for (size_t i = 0; i < mode.size(); ++i) {

    t_channel_mode cur_mode = static_cast<t_channel_mode>(mode[i]);

    if (std::find(all_mode.begin(), all_mode.end(), cur_mode) == all_mode.end()) {
      return _log("Invalid mode '" + mode + "' for channel");
    }

    bool alreadyHave = haveMode(cur_mode);

    if (remove && alreadyHave) {
      _modes.erase(std::find(_modes.begin(), _modes.end(), cur_mode));
    }
    else if (!remove && !alreadyHave) {
      _modes.push_back(cur_mode);
    }

  }
}


bool Channel :: isPrivate() {
  return haveMode(CH_PRIVATE);
}


bool Channel :: haveMode(t_channel_mode mode) {
  return std::find(_modes.begin(), _modes.end(), mode) != _modes.end();
}


bool Channel ::_userHaveMode(User& user, t_user_mode mode) {
  return std::find(_user_mode[&user].begin(), _user_mode[&user].end(), mode) != _user_mode[&user].end();
}


void Channel :: addModeToUser(User& user, const std::string& mode) {

  std::vector<t_user_mode> modes(USER_MODES, USER_MODES + sizeof(USER_MODES) / sizeof(USER_MODES[0]));

  if (!userOnChannel(user)) {
    return _log("User " + user.getUsername() + " is not on channel");
  }

  for (size_t i = 0; i < mode.size(); ++i) {

    t_user_mode cur_mode = static_cast<t_user_mode>(mode[i]);

    if (std::find(modes.begin(), modes.end(), cur_mode) == modes.end()) {
      _log("Invalid mode '" + mode + "' for user");
      continue;
    }
    addModeToUser(user, cur_mode);
  }
}


void Channel :: addModeToUser(User& user, t_user_mode mode) {
  if (!_userHaveMode(user, mode)) {
    _user_mode[&user].push_back(mode);
  }
}

void Channel :: removeUserMode(User& user, const std::string& mode) {

  if (!userOnChannel(user)) {
    return _log("User " + user.getUsername() + " is not on channel");
  }

  std::vector<t_user_mode> modes = _user_mode[&user];

  for (size_t i = 0; i < mode.size(); ++i) {
    removeUserMode(user, static_cast<t_user_mode>(mode[i]));
  }
}


void Channel :: removeUserMode(User& user, t_user_mode mode) {
  _user_mode[&user].erase(std::find(_user_mode[&user].begin(), _user_mode[&user].end(), mode));
}


bool Channel :: userIsOper(User& user) {

  if (!userOnChannel(user)) {
    _log("User " + user.getUsername() + " is not on channel");
    return false;
  }

  return _userHaveMode(user, U_OPERATOR);
}
