
#include <iostream>
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

    channel->removeUser("User2");

    std::cout << channel->userOnChannel("User1") << std::endl;

    std::vector<std::string> users = channel->getUsers();

    for (size_t i = 0; i < users.size(); ++i) {
        std::cout << users[i] << ", " << std::endl;
    }

    std::cout << std::endl;

    return (0);
}