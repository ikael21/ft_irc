#include "commands.hpp"


void sendChannelInfo(Command* command, Channel& channel)
{
  if (!channel.getTopic().empty())
    command->reply(RPL_TOPIC, channel.getName(), channel.getTopic());

  std::vector<User*> users = channel.getVisibleUsers();
  std::stringstream nicks;
  for (size_t i = 0; i < users.size(); ++i) {
    nicks << (channel.userIsOper(*users[i]) ? "@" : "+") << users[i]->getNick() << " ";
  }
  command->reply(RPL_NAMREPLY, channel.getName(), nicks.str());
  command->reply(RPL_ENDOFNAMES, channel.getName());
}


bool userNotInvite(Channel& channel, User& user) {
  return channel.isPrivate() && !channel.isInvited(user);
}


bool badChannelPrefix(std::string& channel) {
  return (channel[0] != '#' && channel[0] != '&');
}


void JOIN(Command *command) {

  User& user = command->getUser();
  // irc::IrcServer& server = command->getServer();

  std::vector<std::string> args = split(command->getArguments()[0], ' ', 1);
  std::vector<std::string> channels = split(args[0], ',');
  std::vector<std::string> keys = std::vector<std::string>();

  if (args.size() > 1)
    keys = split(args[1], ',');

  for (size_t i = 0; i < channels.size(); ++i) {

    if (badChannelPrefix(channels[i])) {
      command->reply(ERR_BADCHANMASK); // CHECK
      continue;
    }

    if (false /* server.channelOnServer(channels[i]) */) {
      Channel channel = Channel() /* server.getChannel(channels[i]) */;
      channel.setName(channels[i]);

      bool userHasIncorrectKey = channel.hasKey() && (i <= keys.size() || !channel.isCorrectKey(keys[i]));

      if (userNotInvite(channel, user)) {
        command->reply(ERR_INVITEONLYCHAN, channel.getName());
      } else if (userHasIncorrectKey) {
        command->reply(ERR_BADCHANNELKEY, channel.getName());
      } else if (channel.isFull()) {
        command->reply(ERR_CHANNELISFULL, channel.getName());
      } else if (channel.isBanned(user)) {
        command->reply(ERR_BANNEDFROMCHAN, channel.getName());
      } else {
        channel.addUser(user);
        sendChannelInfo(command, channel);
      }
    } else {
      Channel channel = Channel();
      channel.setName(channels[i]);

      if (keys.size() > i)
        channel.setKey(keys[i]);

      channel.addUser(user);
      channel.addModeToUser(user, U_OPERATOR);
      std::cout << channel.userIsOper(user);
      /* server.addChannel(channel) */
      sendChannelInfo(command, channel);
    }
  }
}
