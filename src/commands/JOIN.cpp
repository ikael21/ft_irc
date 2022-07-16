#include "commands.hpp"


void send_channel_info(Command* command, Channel& channel)
{
  if (!channel.get_topic().empty())
    command->reply(RPL_TOPIC, channel.get_name(), channel.get_topic());

  std::vector<User*> users = channel.get_visible_users();
  std::stringstream nicks;
  for (size_t i = 0; i < users.size(); ++i) {
    nicks << (channel.user_is_oper(*users[i]) ? "@" : "+") << users[i]->get_nick() << " ";
  }
  command->reply(RPL_NAMREPLY, channel.get_name(), nicks.str());
  command->reply(RPL_ENDOFNAMES, channel.get_name());
}


bool user_not_invite(Channel& channel, User& user) {
  return channel.is_private() && !channel.is_invited(user);
}


bool bad_channel_prefix(std::string& channel) {
  return (channel[0] != '#' && channel[0] != '&');
}


void JOIN(Command *command) {

  User& user = command->get_user();
  irc::IrcServer& server = command->get_server();
  irc::IrcServer::t_channel_list channels = server.get_channels();

  std::vector<std::string> args = split(command->get_arguments()[0], ' ', 1);
  std::vector<std::string> channels_names = split(args[0], ',');
  std::vector<std::string> keys = std::vector<std::string>();

  if (args.size() > 1)
    keys = split(args[1], ',');

  for (size_t i = 0; i < channels_names.size(); ++i) {

    if (bad_channel_prefix(channels_names[i])) {
      command->reply(ERR_BADCHANMASK);
      continue;
    }

    irc::IrcServer::t_channel_list::iterator it = std::find(channels.begin(), channels.end(), channels_names[i]);

    if (it != channels.end()) {
      Channel& channel = *it;

      bool user_has_incorrect_key = channel.has_key() && (i <= keys.size() || !channel.is_correct_key(keys[i]));

      if (user_not_invite(channel, user)) {
        command->reply(ERR_INVITEONLYCHAN, channel.get_name());
      } else if (user_has_incorrect_key) {
        command->reply(ERR_BADCHANNELKEY, channel.get_name());
      } else if (channel.is_full()) {
        command->reply(ERR_CHANNELISFULL, channel.get_name());
      } else if (channel.is_banned(user)) {
        command->reply(ERR_BANNEDFROMCHAN, channel.get_name());
      } else {
        channel.add_user(user);
        send_channel_info(command, channel);
        std::string msg = user.get_prefix_msg() + command->get_command_name() + " :" + channel.get_name();
        channel.send_to_channel(user, msg);
      }
    } else {
      Channel channel = Channel();
      channel.set_name(channels_names[i]);

      if (keys.size() > i)
        channel.set_key(keys[i]);

      channel.add_user(user);
      channel.add_mode_to_user(user, U_OPERATOR);
      std::cout << channel.user_is_oper(user);
      server.add_channel(channel);
      send_channel_info(command, channel);
    }
  }
}
