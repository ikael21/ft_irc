#include "Channel.hpp"

#define DEBUG_LOG(x) do { if (DEBUG) { std::cerr << x << std::endl; } } while (0)

irc::t_channel_mode DEFAULT_CHANNEL_MODES[] = {
  irc::CH_TOPIC_MODIFIERS, irc::CH_FORBID_OUT_MSG
};


irc::Channel::Channel() :
    _limit_users(DEFAULT_USERS) {

  _modes = std::vector<t_channel_mode>(DEFAULT_CHANNEL_MODES, DEFAULT_CHANNEL_MODES \
    + sizeof(DEFAULT_CHANNEL_MODES) / sizeof(DEFAULT_CHANNEL_MODES[0]));
}


irc::Channel::~Channel() {}


std::string irc::Channel::_log_message(std::string message) {
  return "Channel " + _name + ": " + message;
}

void irc::Channel::_log(std::string message) {
  DEBUG_LOG(_log_message(message));
}


bool irc::Channel::user_on_channel(irc::User& user) {
  for (std::vector<irc::User*>::iterator u = _users.begin(); u != _users.end(); ++u) {
    if ((*u)->get_fd() == user.get_fd())
      return true;
  }
  return false;
}

void irc::Channel::add_user(irc::User& user) {

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

void irc::Channel::remove_user(irc::User& user) {

  if (!user_on_channel(user)) {
    return _log("User " + user.get_nick() + " not in the channel");
  }

  _users.erase(std::find(_users.begin(), _users.end(), &user));

  if (is_oper(user))
    _operators.erase(std::find(_operators.begin(), _operators.end(), &user));

  _log("User " +  user.get_nick() + " remove from channel");
}

std::vector<irc::User*> irc::Channel::get_visible_users() {

  std::vector<User *> users;

  for (size_t i = 0; i < _users.size(); ++i) {

    if (_users[i]->is_invisible())
      continue;

    users.push_back(_users[i]);
  }

  return users;
}

void irc::Channel::set_name(std::string name) {

  if (name.empty() && name[0] != '#') {
    return _log("Channel name must starts with '#' symbol");
  }

  if (name.find_first_of(FORBIDDEN_CHANNEL_NAME_SYMBOLS) != std::string::npos) {
    return _log("Channel name constains forbidden symbols");
  }

  _name = name;
}


void irc::Channel::set_key(std::string key) { _key = key; }


void irc::Channel::set_limit_users(int new_limit) { _limit_users = new_limit; }


void irc::Channel::set_topic(std::string topic) { _topic = topic; }


bool irc::Channel::is_invited(irc::User& user) {
  return std::find(_invited.begin(), _invited.end(), &user) != _invited.end();
}


void irc::Channel::add_to_invite_list(irc::User& user) {
  if (!is_invited(user)) {
    _invited.push_back(&user);
    _log("User " + user.get_nick() + " added to invite list");
  } else {
    _log("User " + user.get_nick() + " already invited");
  }
}


void irc::Channel::remove_from_invite_list(irc::User& user) {
  if (is_invited(user))
    _invited.erase(std::find(_invited.begin(), _invited.end(), &user));
}


bool irc::Channel::is_banned(irc::User& user) {
  for (std::vector<BannedUser>::iterator it = _banned.begin(); it != _banned.end(); ++it) {
    if (it->user == &user)
      return true;
  }
  return false;
}


void irc::Channel::add_to_ban_list(irc::User& user, irc::User& who) {
  if (!is_banned(user)) {
    _banned.push_back(BannedUser(&user, &who));
    _log("User " + user.get_nick() + " added to ban list");
  } else {
    _log("User " + user.get_nick() + " already banned");
  }
}


void irc::Channel::remove_from_ban_list(irc::User& user) {
  for (std::vector<BannedUser>::iterator it = _banned.begin(); it != _banned.end(); ++it) {
    if (it->user == &user) {
      _banned.erase(it);
      break;
    }
  }
}

std::vector<std::string> irc::Channel::get_ban_list() {

  std::vector<std::string> ban_list = std::vector<std::string>();

  for (size_t i = 0; i < _banned.size(); ++i) {
    std::stringstream ban_id;
    ban_id << _banned[i].user->get_nick() << "!*@* " \
      << _banned[i].who->get_nick() << " " << _banned[i].when;
    ban_list.push_back(ban_id.str());
  }
  return ban_list;
}


void irc::Channel::add_channel_mode(t_channel_mode mode) {
  if (!have_mode(mode))
    _modes.push_back(mode);
}


void irc::Channel::remove_channel_mode(t_channel_mode mode) {
  if (have_mode(mode))
    _modes.erase(std::find(_modes.begin(), _modes.end(), mode));
}


bool irc::Channel::is_private() {
  return have_mode(CH_PRIVATE);
}

bool irc::Channel::is_secret() {
  return have_mode(CH_SECRET);
}


bool irc::Channel::have_mode(t_channel_mode mode) {
  return std::find(_modes.begin(), _modes.end(), mode) != _modes.end();
}


void irc::Channel::add_oper(irc::User& user) {
  if (!is_oper(user))
    _operators.push_back(&user);
}


void irc::Channel::remove_oper(irc::User& user) {
  _operators.erase(std::find(_operators.begin(), _operators.end(), &user));
}


bool irc::Channel::is_oper(irc::User& user) {
  return std::find(_operators.begin(), _operators.end(), &user) != _operators.end();
}


void irc::Channel::send_to_channel(irc::User&user, std::string& msg, bool include_myself) {
  for (size_t i = 0; i < _users.size(); ++i) {
    if (!include_myself && user == *_users[i])
      continue;
    user.send_msg_to_user(*_users[i], msg);
  }
}


bool irc::operator==(const irc::Channel& left, const irc::Channel& right) {
  return left._name == right._name;
}


bool irc::operator==(const irc::Channel& left, const std::string& channel_name) {
  return left._name == channel_name;
}


std::string irc::Channel::get_modes_as_str() {

  std::stringstream modes;
  std::stringstream values;

  modes << "+";
  for (size_t i = 0; i < _modes.size(); ++i) {

    modes << static_cast<char>(_modes[i]);

    if (_modes[i] == CH_CHANGE_KEY)
      values << " " << _key;
    else if (_modes[i] == CH_CHANGE_LIMIT_USERS)
      values << " " << _limit_users;
  }

  return modes.str() + values.str();
}
