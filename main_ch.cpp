
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

    channel->removeUser("User2");

    return (0);
}