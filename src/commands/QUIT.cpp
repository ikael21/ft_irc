#include "commands.hpp"


void QUIT(Command* command) {

  User& user = command->get_user();

  std::string prefix_quit_msg = user.get_prefix_msg() + "PART ";
  std::string suffix_quit_msg;

  if (command->num_args())
    suffix_quit_msg = (command->get_arguments()[0][0] == ':' ? " " : " :") + command->get_arguments()[0];

  irc::IrcServer::t_channel_list& channels = command->get_server().get_channels();
  irc::IrcServer::t_channel_list::iterator ch = channels.begin();

  while (ch != channels.end()) {
    if (ch->user_on_channel(user)) {
      std::string full_msg = prefix_quit_msg + ch->get_name() + suffix_quit_msg;
      ch->send_to_channel(user, full_msg);
      ch->remove_user(user);

      ch = ch->is_empty() ? channels.erase(ch) : ++ch;
    }
  }

  // TODO refactor
  typedef irc::IrcServer::t_userlist::iterator user_iter;
  user_iter it = command->get_server().get_user_by_fd(user.get_fd());
  command->get_server().delete_client(it);
}
