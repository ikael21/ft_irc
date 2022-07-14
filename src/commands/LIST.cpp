#include "commands.hpp"

void LIST(Command *command) {

    // Параметры: [<channel>{,<channel>} [<server>]]
    // используется для вывода списка канало и их топиков. Если
    // используется параметр <channel>, то выводится только статус этого
    // канала. Приватные каналы указаны (без их топиков) как каналы "Prv", без
    // указания количества клиентов, сидящих на этом канале. 
    // секретные каналы не приводятся в списке,
    // если, конечно, клиент не является членом подобного канала.

    std::vector<std::string> args = command->getArguments();
    std::string message;

    if (args.empty())
        return;

    message = args[0];

    // ?? channals = command->getChannals();

    //if (channals.empty())
     //   return reply(ERR_NOSUCHSERVER);

    return command->reply(RPL_LISTSTART);
    
    return command->reply(RPL_LIST);

    return command->reply(RPL_LISTEND);

}