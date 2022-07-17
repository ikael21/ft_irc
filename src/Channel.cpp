
#include "Channel.hpp"

#define DEBUG_LOG(x) do { if (DEBUG) { std::cerr << x << std::endl; } } while (0)

t_channel_mode CHANNEL_MODES[] = { CH_PRIVATE, CH_SECRET, CH_INVITE_ONLY, CH_TOPIC_MODIFIERS };
t_user_mode    USER_MODES[] = { U_INVISIBLE, U_OPERATOR };


Channel::Channel() :
    _limit_users(DEFAULT_USERS), _modes(std::vector<t_channel_mode>(DEFAULT_CHANNEL_MODES)) {}


Channel::~Channel() {}


std::string Channel ::_log_message(std::string message) {
  return "Channel " + _name + ": " + message;
}

void Channel::_log(std::string message) {
  DEBUG_LOG(_log_message(message));
}


bool Channel::user_on_channel(User& user) {
  return std::find(_users.begin(), _users.end(), &user) != _users.end();
}

void Channel::add_user(User &user) {

  if (_users.size() == _limit_users) {
    return _log("Channel is full");
  }

  if (is_private() && !is_invited(user)) {
    return _log("User " + user.get_nick() + " not in the invited list");
  }

  if (user_on_channel(user)) {
    return _log("User " + user.get_nick() + " already in channel");
  }

  _users.push_back(&user);
  _log("User " +  user.get_nick() + " added to channel");
}

void Channel::remove_user(User &user) {

  if (!user_on_channel(user)) {
    return _log("User " + user.get_nick() + " not in the channel");
  }

  _users.erase(std::find(_users.begin(), _users.end(), &user));
  _user_mode.erase(&user);
  _log("User " +  user.get_nick() + " remove from channel");
}

std::vector<User *> Channel::get_visible_users() {

  std::vector<User *> users;

  for (size_t i = 0; i < _users.size(); ++i) {

    if (_user_have_mode(*_users[i], U_INVISIBLE))
      continue;

    users.push_back(_users[i]);
  }

  return users;
}

void Channel::set_name(std::string name) {

  if (name.empty() && name[0] != '#') {
    return _log("Channel name must starts with '#' symbol");
  }

  if (name.find_first_of(FORBIDDEN_CHANNEL_NAME_SYMBOLS) != std::string::npos) {
    return _log("Channel name constains forbidden symbols");
  }

  _name = name;
}


void Channel::set_key(std::string key) { _key = key; }


void Channel::set_topic(std::string topic) { _topic = topic; }


bool Channel::is_invited(User& user) {
  return std::find(_invited.begin(), _invited.end(), &user) != _invited.end();
}


void Channel::add_to_invite_list(User& user) {
  if (!is_invited(user)) {
    _invited.push_back(&user);
    _log("User " + user.get_nick() + " added to invite list");
  } else {
    _log("User " + user.get_nick() + " already invited");
  }
}


bool Channel::is_banned(User& user) {
    return std::find(_banned.begin(), _banned.end(), &user) != _banned.end();
}


void Channel::add_to_ban_list(User& user) {
  if (!is_banned(user)) {
    _banned.push_back(&user);
    _log("User " + user.get_nick() + " added to ban list");
  } else {
    _log("User " + user.get_nick() + " already banned");
  }
}


void Channel::add_channel_mode(const std::string& mode) { _change_channel_mode(mode, false); }
void Channel::remove_channel_mode(const std::string& mode) { _change_channel_mode(mode, true); }

void Channel::_change_channel_mode(const std::string& mode, bool remove) {

  std::vector<t_channel_mode> all_mode(CHANNEL_MODES, CHANNEL_MODES + sizeof(CHANNEL_MODES) / sizeof(CHANNEL_MODES[0]));

  for (size_t i = 0; i < mode.size(); ++i) {

    t_channel_mode cur_mode = static_cast<t_channel_mode>(mode[i]);

    if (std::find(all_mode.begin(), all_mode.end(), cur_mode) == all_mode.end()) {
      return _log("Invalid mode '" + mode + "' for channel");
    }

    bool alreadyHave = have_mode(cur_mode);

    if (remove && alreadyHave) {
      _modes.erase(std::find(_modes.begin(), _modes.end(), cur_mode));
    }
    else if (!remove && !alreadyHave) {
      _modes.push_back(cur_mode);
    }

  }
}


bool Channel::is_private() {
  return have_mode(CH_PRIVATE);
}


bool Channel::have_mode(t_channel_mode mode) {
  return std::find(_modes.begin(), _modes.end(), mode) != _modes.end();
}


bool Channel ::_user_have_mode(User& user, t_user_mode mode) {
  return std::find(_user_mode[&user].begin(), _user_mode[&user].end(), mode) != _user_mode[&user].end();
}


void Channel::add_mode_to_user(User& user, const std::string& mode) {

  std::vector<t_user_mode> modes(USER_MODES, USER_MODES + sizeof(USER_MODES) / sizeof(USER_MODES[0]));

  if (!user_on_channel(user)) {
    return _log("User " + user.get_nick() + " is not on channel");
  }

  for (size_t i = 0; i < mode.size(); ++i) {

    t_user_mode cur_mode = static_cast<t_user_mode>(mode[i]);

    if (std::find(modes.begin(), modes.end(), cur_mode) == modes.end()) {
      _log("Invalid mode '" + mode + "' for user");
      continue;
    }
    add_mode_to_user(user, cur_mode);
  }
}


void Channel::add_mode_to_user(User& user, t_user_mode mode) {
  if (!_user_have_mode(user, mode)) {
    _user_mode[&user].push_back(mode);
  }
}

void Channel::remove_user_mode(User& user, const std::string& mode) {

  if (!user_on_channel(user)) {
    return _log("User " + user.get_nick() + " is not on channel");
  }

  std::vector<t_user_mode> modes = _user_mode[&user];

  for (size_t i = 0; i < mode.size(); ++i) {
    remove_user_mode(user, static_cast<t_user_mode>(mode[i]));
  }
}


void Channel::remove_user_mode(User& user, t_user_mode mode) {
  _user_mode[&user].erase(std::find(_user_mode[&user].begin(), _user_mode[&user].end(), mode));
}


bool Channel::user_is_oper(User& user) {

  if (!user_on_channel(user)) {
    _log("User " + user.get_nick() + " is not on channel");
    return false;
  }

  return _user_have_mode(user, U_OPERATOR);
}


void Channel::send_to_channel(User& user, std::string& msg) {
  for (size_t i = 0; i < _users.size(); ++i) {
    if (user == *_users[i])
      continue;
    user.send_msg_to_user(*_users[i], msg);
  }
}

bool operator==(const Channel& left, const Channel& right) {
  return left._name == right._name;
}

bool operator==(const Channel& left, const std::string& channel_name) {
  return left._name == channel_name;
}