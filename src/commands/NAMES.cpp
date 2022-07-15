#include "commands.hpp"

void NAMES(Command *command) {


//     Параметры: [<channel>{,<channel>}]
//   пользователь может получитт список всех
//   никнеймов, которые он видит на любом канале, на которых они находятся.
//   Имена каналов, которые они могут видеть это те, которые не приватные
//   (+p) или секретные (+s), или те, на которых сидит пользователь.
//   Параметр <channel> указывает, с какого канала(ов) собрать информацию.
//   Эта команда не возвращает кода ошибки из-за неправильных названий
//   каналов.
//   Если параметр <channel> не задан, выводится список всех каналов и имен
//   тех, кто на них находится. И к концу списка - список пользователей,
//   которые видимые, но не находятся ни на одной канале, или не на одном
//   видимом канале, которые начинаюся как 'channel' "*".

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