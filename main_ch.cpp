
#include <iostream>
#include <assert.h>
#include "Channel.hpp"

int main(int argc, char **argv) {

  if (argc != 1) {
    std::cout << "Usage: " << argv[0] << std::endl;
    return (1);
  }

  Channel *channel = new Channel();

  channel->setName("#ch1");

  channel->addUser("User1");
  channel->addUser("User2");
  channel->addUser("User3");

  assert(channel->getUsers().size() == 3L);

  channel->removeUser("User2");
  assert(channel->getUsers().size() == 2L);

  std::cout << "User1 on channel: " << channel->userOnChannel("User1") << std::endl;
  assert(channel->userOnChannel("User1"));

  std::cout << "Channel #ch1 is private: " << channel->isPrivate() << std::endl;
  assert(channel->isPrivate() == false);

  channel->addChannelMode("p");
  std::cout << "Channel #ch1 is private: " << channel->isPrivate() << std::endl;
  assert(channel->isPrivate() == true);

  channel->addUser("User4");
  assert(channel->getUsers().size() == 2L);

  channel->setName("#ch2");
  assert(channel->getName() == "#ch2");
  std::cout << "Users on channel " << channel->getUsers().size() << std::endl;

  channel->addModeToUser("User1", "io");
  assert(channel->userIsOper("User1") == true);
  std::cout << "User1 is oper: " << channel->userIsOper("User1") << std::endl;
  std::cout << "Users on channel " << channel->getUsers().size() << std::endl;


  channel->removeChannelMode("p");
  std::cout << "Channel #ch2 is private: " << channel->isPrivate() << std::endl;
  assert(channel->isPrivate() == false);

  channel->addUser("User4");

  assert(channel->getVisibleUsers().size() == 2L);
  channel->removeUserMode("User1", "i");
  assert(channel->getUsers().size() == 3L);

  return (0);
}
