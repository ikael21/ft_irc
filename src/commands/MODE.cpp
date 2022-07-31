#include "commands.hpp"

/*
 * RPL_CHANNELMODEIS
 * ERR_CHANOPRIVSNEEDED
 * ERR_NOSUCHNICK
 * ERR_NOTONCHANNEL
 * ERR_KEYSET
 * RPL_BANLIST
 * RPL_ENDOFBANLIST
 * ERR_UNKNOWNMODE
 * ERR_NOSUCHCHANNEL
 * ERR_USERSDONTMATCH
 * RPL_UMODEIS
 * ERR_UMODEUNKNOWNFLAG
 */

template<typename T>
std::string __cast_modes_to_str(std::vector<T>& plus_modes, std::vector<T>& minus_modes) {

  std::string result;

  if (plus_modes.size()) {
    result += "+";
    for (size_t i = 0; i < plus_modes.size(); ++i)
      result += static_cast<char>(plus_modes[i]);
  }
  if (minus_modes.size()) {
    result += "-";
    for (size_t i = 0; i < minus_modes.size(); ++i)
      result += static_cast<char>(minus_modes[i]);
  }
  return result;
}


template<typename T>
void __switch_mode(T cur_mode, std::vector<T>& plus_modes, std::vector<T>& minus_modes) {
  if (std::find(minus_modes.begin(), minus_modes.end(), cur_mode) != minus_modes.end())
    minus_modes.erase(std::find(minus_modes.begin(), minus_modes.end(), cur_mode));
  else
    plus_modes.push_back(cur_mode);
}


/* Список доступных режимов пользователя:
 *   i - делает пользователя невидимым;
 *   s - marks a user for receipt of server notices;
 *   w - user receives wallops;
 *   o - флаг оператора.
 */
t_user_mode __get_user_mode(char mode) {

  static t_user_mode user_modes[] = {
    U_UNKNOWN_MODE, U_INVISIBLE, U_OPERATOR, U_WALLOPS, U_S_NOTICE
  };

  for (size_t i = 0; i < sizeof(user_modes) / sizeof(t_user_mode); ++i) {
    if (mode == user_modes[i])
      return user_modes[i];
  }

  return U_UNKNOWN_MODE;
}


void user_mode(Command* command) {

  User& user = command->get_user();
  std::string nick = command->get_arguments()[0];

  try {

    User& mod_user = command->get_server().get_user_by_nickname(nick);

    if (user != command->get_server().get_user_by_nickname(nick))
      return command->reply(ERR_USERSDONTMATCH);
    else if (command->num_args() == 1) {
      return command->reply(RPL_UMODEIS, user.get_modes_as_str());
    }

    std::string modes = command->get_arguments()[1];
    std::vector<t_user_mode> plus_modes = std::vector<t_user_mode>();
    std::vector<t_user_mode> minus_modes = std::vector<t_user_mode>();
    bool plus = true;

    for (size_t i = 0; i < modes.length(); ++i) {
      if (modes[i] == '-') {
        plus = false;
      } else if (modes[i] == '+') {
        plus = true;
      } else {
        t_user_mode cur_mode = __get_user_mode(modes[i]);

        if (cur_mode == U_UNKNOWN_MODE) {
          command->reply(ERR_UNKNOWNMODE, std::string(1, modes[i]));
          continue;
        }
        if (plus && !mod_user.has_mode(cur_mode)) {
          mod_user.add_mode(cur_mode);
          __switch_mode<t_user_mode>(cur_mode, plus_modes, minus_modes);
        } else if (!plus && mod_user.has_mode(cur_mode)) {
          mod_user.remove_mode(cur_mode);
          __switch_mode<t_user_mode>(cur_mode, minus_modes, plus_modes);
        }
      }
    }

    std::string apply_modes = __cast_modes_to_str<t_user_mode>(plus_modes, minus_modes);

    if (apply_modes.length())
      command->reply(RPL_UMODEIS, apply_modes);

  } catch (UserNotFound& e) {
    return command->reply(ERR_NOSUCHNICK, nick);
  }
}


bool is_switch_mode(t_channel_mode mode) {
  return mode == CH_PRIVATE || mode == CH_SECRET || mode == CH_INVITE_ONLY \
    || mode == CH_TOPIC_MODIFIERS || mode == CH_FORBID_OUT_MSG || mode == CH_MODERATE;
}


/* Список доступных режимов канала:
 * SWITCHED:
 *   p - флаг приватности канала;
 *   s - флаг секретности канала;
 *   i - флаг канала invite-only;
 *   t - при установке этого флага, менять топик могут только операторы;
 *   n - запрещает сообщения на канал от посторонних клиентов;
 * ? m - модерируемый канал;
 *
 * WITH OPTIONS:
 *   o - брать/давать привилегии операторов канала
 *   l - установка ограничения на количество пользователей;
 *   b - установка маски бана;
 * ? v - брать/давать возможность голоса при модерируемом режиме;
 *   k - установка на канал ключа (пароля).
 */
t_channel_mode __get_channel_mode(char mode) {

  static t_channel_mode channel_modes[] = {
    CH_UNKNOWN_MODE, CH_CHANGE_PRIVILEGE, CH_PRIVATE, CH_SECRET, CH_INVITE_ONLY,
    CH_TOPIC_MODIFIERS, CH_FORBID_OUT_MSG, CH_MODERATE, CH_CHANGE_LIMIT_USERS,
    CH_BAN_USER, CH_CHANGE_VOICE, CH_CHANGE_KEY
  };

  for (size_t i = 0; i < sizeof(channel_modes) / sizeof(t_user_mode); ++i) {
    if (mode == channel_modes[i])
      return channel_modes[i];
  }

  return CH_UNKNOWN_MODE;
}

void __send_info(User& user,
                 irc::IrcServer::t_channel_list::iterator ch,
                 std::string comm_name,
                 std::string& msg) {

  std::string full_msg = user.get_prefix_msg() + comm_name + " " + ch->get_name() + " " + msg;
  ch->send_to_channel(user, full_msg);
  user.send_msg_to_user(user, full_msg);
}

void __change_oper_in_channel(Command* command,
                              User& user,
                              irc::IrcServer::t_channel_list::iterator ch,
                              std::string& nick,
                              bool plus) {

  try {
    User& new_op_user = command->get_server().get_user_by_nickname(nick);
    std::string msg = (plus ? "+o " : "-o ") + nick;

    if (!ch->user_on_channel(new_op_user)) {
      command->reply(ERR_USERNOTINCHANNEL, nick, ch->get_name());
    } else if (plus && !ch->is_oper(new_op_user)) {
      ch->add_oper(new_op_user);
      __send_info(user, ch, command->get_command_name(), msg);
    } else if (!plus && ch->is_oper(new_op_user)) {
      ch->remove_oper(new_op_user);
      __send_info(user, ch, command->get_command_name(), msg);
    }
  } catch (UserNotFound &e) {
    command->reply(ERR_NOSUCHNICK, nick);
  }
}

void __change_ban_in_channel(Command* command,
                             User& user,
                             irc::IrcServer::t_channel_list::iterator ch,
                             std::string& nick,
                             bool plus) {

  try {
    User& banned_user = command->get_server().get_user_by_nickname(nick);
    std::string msg = (plus ? "+b " : "-b ") + nick + "!*@*";

    if (plus && !ch->is_banned(banned_user)) {
      ch->add_to_ban_list(banned_user, user);
      __send_info(user, ch, command->get_command_name(), msg);
    } else if (!plus && ch->is_banned(banned_user)) {
      ch->remove_from_ban_list(banned_user);
      __send_info(user, ch, command->get_command_name(), msg);
    }
  } catch (UserNotFound &e) {
    if (nick.empty()) {
      std::vector<std::string> ban_list = ch->get_ban_list();
      for (size_t i = 0; i < ban_list.size(); ++i)
        command->reply(RPL_BANLIST, ch->get_name(), ban_list[i]);
      command->reply(RPL_ENDOFBANLIST, ch->get_name());
    } else {
      command->reply(ERR_NOSUCHNICK, nick);
    }
  }
}

void __change_key_in_channel(Command* command,
                             User& user,
                             irc::IrcServer::t_channel_list::iterator ch,
                             std::string& new_key,
                             bool plus) {

    std::string msg = (plus ? ("+k " + new_key) : ("-k " + ch->get_key()));

    if (plus && !ch->has_key()) {
      ch->set_key(new_key);
      __send_info(user, ch, command->get_command_name(), msg);
    } else if (plus && ch->get_key() == new_key) {
      command->reply(ERR_KEYSET, ch->get_name());
    } else if (!plus) {
      ch->set_key("");
      __send_info(user, ch, command->get_command_name(), msg);
    }
}

void __change_limit_in_channel(Command* command,
                             User& user,
                             irc::IrcServer::t_channel_list::iterator ch,
                             std::string& limit,
                             bool plus) {

    int new_limit = atoi(limit.c_str());
    if (new_limit < 0)
      new_limit = 1;
    else if (new_limit > 1000000)
      new_limit = 1000000;
    else if (!plus)
      new_limit = DEFAULT_USERS;

    std::string msg = (plus ? ("+l " + std::to_string(new_limit)) : "-l ");

    ch->set_limit_users(new_limit);
    if (new_limit)
      __send_info(user, ch, command->get_command_name(), msg);
}


void channel_mode(Command* command) {

  User& user = command->get_user();
  std::string channel_name = command->get_arguments()[0];
  irc::IrcServer::t_channel_list& channels = command->get_server().get_channels();

  irc::IrcServer::t_channel_list::iterator ch = std::find(channels.begin(), channels.end(), channel_name);

  if (ch == channels.end()) {
    return command->reply(ERR_NOSUCHCHANNEL, channel_name);
  } else if (command->num_args() == 1) {
    return command->reply(RPL_CHANNELMODEIS, ch->get_name(), ch->get_modes_as_str());
  } else if (!ch->is_oper(user)) {
    return command->reply(ERR_CHANOPRIVSNEEDED, ch->get_name());
  }

  std::string modes = command->get_arguments()[1];
  std::vector<t_channel_mode> plus_modes = std::vector<t_channel_mode>();
  std::vector<t_channel_mode> minus_modes = std::vector<t_channel_mode>();
  bool plus = true;

  std::vector<std::string> mode_args = std::vector<std::string>();
  if (command->num_args() == 3)
    mode_args = split(command->get_arguments()[2], ' ');
  std::vector<std::string>::iterator m_arg = mode_args.begin();

  for (size_t i = 0; i < modes.length(); ++i) {
    if (modes[i] == '-') {
      plus = false;
    } else if (modes[i] == '+') {
      plus = true;
    } else {

      t_channel_mode cur_mode = __get_channel_mode(modes[i]);

      if (cur_mode == CH_UNKNOWN_MODE) {
        command->reply(ERR_UNKNOWNMODE, std::string(1, modes[i]));
        continue;
      }

      if (is_switch_mode(cur_mode)) {
        if (plus && !ch->have_mode(cur_mode)) {
          ch->add_channel_mode(cur_mode);
          __switch_mode<t_channel_mode>(cur_mode, plus_modes, minus_modes);
        } else if (!plus && ch->have_mode(cur_mode)) {
          ch->remove_channel_mode(cur_mode);
          __switch_mode<t_channel_mode>(cur_mode, minus_modes, plus_modes);
        }
      } else if (cur_mode == CH_BAN_USER) {
        std::string arg = (m_arg != mode_args.end() ? *m_arg : "");
        __change_ban_in_channel(command, user, ch, arg, plus);
        ++m_arg;
      } else if (m_arg != mode_args.end()) {
        if (cur_mode == CH_CHANGE_PRIVILEGE)
          __change_oper_in_channel(command, user, ch, *m_arg, plus);
        else if (cur_mode == CH_CHANGE_KEY)
          __change_key_in_channel(command, user, ch, *m_arg, plus);
        else if (cur_mode == CH_CHANGE_LIMIT_USERS)
          __change_limit_in_channel(command, user, ch, *m_arg, plus);
        ++m_arg;
      }
    }
  }

  std::string apply_modes = __cast_modes_to_str<t_channel_mode>(plus_modes, minus_modes);

  if (apply_modes.length()) {
    command->reply(RPL_UMODEIS, apply_modes);
    std::string msg = user.get_prefix_msg() + command->get_command_name() + " " \
      + ch->get_name() + " " + apply_modes;
    ch->send_to_channel(user, msg);
  }
}

void MODE(Command* command) {
  if (is_channel(command->get_arguments()[0]))
    channel_mode(command);
  else
    user_mode(command);
}