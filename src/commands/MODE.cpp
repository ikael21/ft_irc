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

/* Список доступных режимов пользователя:
 *   i - делает пользователя невидимым;
 *   s - marks a user for receipt of server notices;
 *   w - user receives wallops;
 *   o - флаг оператора.
 */
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
    std::string apply_modes;
    bool plus = true;

    for (size_t i = 0; i < modes.length(); ++i) {
      if (modes[i] == '-') {
        apply_modes += (apply_modes.back() == '-' ? "" : "-");
        plus = false;
      } else if (modes[i] == '+') {
        apply_modes += (apply_modes.back() == '+' ? "" : "+");
        plus = true;
      } else {
        t_user_mode cur_mode = __get_user_mode(modes[i]);

        if (cur_mode == U_UNKNOWN_MODE) {
          command->reply(ERR_UNKNOWNMODE, std::string(1, modes[i]));
          continue;
        }
        if (plus)
          mod_user.add_mode(cur_mode);
        else
          mod_user.remove_mode(cur_mode);
        apply_modes += modes[i];
      }
    }
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
  std::string apply_modes;
  std::string apply_values;
  bool plus = true;

  for (size_t i = 0; i < modes.length(); ++i) {
    if (modes[i] == '-') {
      apply_modes += (apply_modes.back() == '-' ? "" : "-");
      plus = false;
    } else if (modes[i] == '+') {
      apply_modes += (apply_modes.back() == '+' ? "" : "+");
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


      } else if (cur_mode == CH_CHANGE_KEY) {

      }

      //TODO Write non switched modes
    }

    std::string msg = user.get_prefix_msg() + command->get_command_name() + " " \
      + ch->get_name() + " " + apply_modes + apply_values;

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