
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
  user1.setUsername("User1");
  user2.setUsername("User2");
  user3.setUsername("User3");

  channel.setName("#ch1");

  channel.addUser(user1);
  channel.addUser(user2);
  channel.addUser(user3);

  assert(channel.getUsers().size() == 3L);

  channel.removeUser(user2);
  assert(channel.getUsers().size() == 2L);

  std::cout << "User1 on channel: " << channel.userOnChannel(user1) << std::endl;
  assert(channel.userOnChannel(user1));

  std::cout << "Channel #ch1 is private: " << channel.isPrivate() << std::endl;
  assert(channel.isPrivate() == false);

  channel.addChannelMode("p");
  std::cout << "Channel #ch1 is private: " << channel.isPrivate() << std::endl;
  assert(channel.isPrivate() == true);

  User user4 = User();
  user4.setUsername("User4");

  channel.addUser(user4);
  assert(channel.getUsers().size() == 2L);

  channel.setName("#ch2");
  assert(channel.getName() == "#ch2");
  std::cout << "Users on channel " << channel.getUsers().size() << std::endl;

  channel.addModeToUser(user1, "io");
  assert(channel.userIsOper(user1) == true);
  std::cout << "User1 is oper: " << channel.userIsOper(user1) << std::endl;
  std::cout << "Users on channel " << channel.getUsers().size() << std::endl;


  channel.removeChannelMode("p");
  std::cout << "Channel #ch2 is private: " << channel.isPrivate() << std::endl;
  assert(channel.isPrivate() == false);

  channel.addUser(user4);

  assert(channel.getVisibleUsers().size() == 2L);
  channel.removeUserMode(user1, "i");
  assert(channel.getUsers().size() == 3L);

  channel.removeUser(user1);
  channel.removeUser(user3);
  channel.removeUser(user4);

  assert(channel.getUsers().size() == 0L);
  return 0;
}
