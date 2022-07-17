
#include <iostream>
#include <assert.h>
#include "Channel.hpp"
#include "utils.hpp"

void print_vector(std::vector<std::string> v) {
  for (size_t i = 0; i < v.size(); ++i)
    std::cout << "|" + v[i] + "|" << std::endl;
}

int main(int argc, char **argv) {

  if (argc != 1) {
    std::cout << "Usage: " << argv[0] << std::endl;
    return (1);
  }

  Channel channel = Channel();
  User user1 = User();
  User user2 = User();
  User user3 = User();
  user1.set_username("User1");
  user2.set_username("User2");
  user3.set_username("User3");

  channel.set_name("#ch1");

  channel.add_user(user1);
  channel.add_user(user2);
  channel.add_user(user3);

  assert(channel.get_users().size() == 3L);

  channel.remove_user(user2);
  assert(channel.get_users().size() == 2L);

  std::cout << "User1 on channel: " << channel.user_on_channel(user1) << std::endl;
  assert(channel.user_on_channel(user1));

  std::cout << "Channel #ch1 is private: " << channel.is_private() << std::endl;
  assert(channel.is_private() == false);

  channel.add_channel_mode("p");
  std::cout << "Channel #ch1 is private: " << channel.is_private() << std::endl;
  assert(channel.is_private() == true);

  User user4 = User();
  user4.set_username("User4");

  channel.add_user(user4);
  assert(channel.get_users().size() == 2L);

  channel.set_name("#ch2");
  assert(channel.get_name() == "#ch2");
  std::cout << "Users on channel " << channel.get_users().size() << std::endl;

  channel.add_mode_to_user(user1, "io");
  assert(channel.user_is_oper(user1) == true);
  std::cout << "User1 is oper: " << channel.user_is_oper(user1) << std::endl;
  std::cout << "Users on channel " << channel.get_users().size() << std::endl;


  channel.remove_channel_mode("p");
  std::cout << "Channel #ch2 is private: " << channel.is_private() << std::endl;
  assert(channel.is_private() == false);

  channel.add_user(user4);

  assert(channel.get_visible_users().size() == 2L);
  channel.remove_user_mode(user1, "i");
  assert(channel.get_users().size() == 3L);

  channel.remove_user(user1);
  channel.remove_user(user3);
  channel.remove_user(user4);

  assert(channel.get_users().size() == 0L);
  return 0;
}
