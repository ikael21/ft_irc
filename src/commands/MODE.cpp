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
 *   m - модерируемый канал;
 *
 * WITH OPTIONS:
 *   o - брать/давать привилегии операторов канала
 *   l - установка ограничения на количество пользователей;
 *   b - установка маски бана;
 *   v - брать/давать возможность голоса при модерируемом режиме;
 *   k - установка на канал ключа (пароля).
 */
t_channel_mode __get_channel_mode(char mode) {

  static t_channel_mode channel_modes[] = {
    CH_UNKNOWN_MODE, CH_CHANGE_PRIVILEGE, CH_PRIVATE, CH_SECRET, CH_INVITE_ONLY,
    CH_TOPIC_MODIFIERS, CH_FORBID_OUT_MSG, CH_MODERATE, CH_CHANGE_LIMIT_USERS,
    CH_BAN_USER, CH_CHANGE_MODERATE, CH_CHANGE_KEY
  };

  for (size_t i = 0; i < sizeof(channel_modes) / sizeof(t_user_mode); ++i) {
    if (mode == channel_modes[i])
      return channel_modes[i];
  }

  return CH_UNKNOWN_MODE;
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
  } else if (!ch->is_oper(user))
    return command->reply(ERR_CHANOPRIVSNEEDED, ch->get_name());

  std::string modes = command->get_arguments()[1];
  std::vector<t_channel_mode> plus_modes = std::vector<t_channel_mode>();
  std::vector<t_channel_mode> minus_modes = std::vector<t_channel_mode>();
  bool plus = true;

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
        if (plus)
          ch->add_channel_mode(cur_mode);
        else
          ch->remove_channel_mode(cur_mode);

        if (plus && !ch->have_mode(cur_mode)) {
          ch->add_channel_mode(cur_mode);
          __switch_mode<t_channel_mode>(cur_mode, plus_modes, minus_modes);
        } else if (!plus && ch->have_mode(cur_mode)) {
          ch->remove_channel_mode(cur_mode);
          __switch_mode<t_channel_mode>(cur_mode, minus_modes, plus_modes);
        }
      }

      //TODO Write non switched modes
    }

    std::string apply_modes = __cast_modes_to_str<t_channel_mode>(plus_modes, minus_modes);

    if (apply_modes.length())
      command->reply(RPL_UMODEIS, apply_modes);

    std::string msg = user.get_prefix_msg() + command->get_command_name() + " " \
      + ch->get_name() + " " + apply_modes;

    ch->send_to_channel(user, msg);
    user.send_msg_to_user(user, msg);
  }
}

void MODE(Command* command) {
  if (is_channel(command->get_arguments()[0]))
    channel_mode(command);
  else
    user_mode(command);
}