#include "commands.hpp"


void QUIT(Command* command) {

    User& user = command->get_user();
    irc::IrcServer& server = command->get_server();

    if (command->num_args()) {
      std::string quit_msg = user.get_prefix_msg() + command->get_command_name() + " :" + command->get_arguments()[0];
      irc::IrcServer::t_channel_list channels = server.get_channels();

      for (irc::IrcServer::t_channel_list::iterator ch = channels.begin(); ch != channels.end(); ++ch) {
      if ((*ch).user_on_channel(user))
          (*ch).send_to_channel(user, quit_msg);
      }
    }

    //TODO close User's connection and delete User
}