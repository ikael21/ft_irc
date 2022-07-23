#include "commands.hpp"

void LIST(Command *command) {

    // Параметры: [<channel>{,<channel>} [<server>]]
    // используется для вывода списка канало и их топиков. Если
    // используется параметр <channel>, то выводится только статус этого
    // канала. Приватные каналы указаны (без их топиков) как каналы "Prv", без
    // указания количества клиентов, сидящих на этом канале. 
    // секретные каналы не приводятся в списке,
    // если, конечно, клиент не является членом подобного канала.
    
    std::vector<std::string> args = command->get_arguments();
    irc::IrcServer::t_channel_list& channels = command->get_server().get_channels();    
    
    if (channels.empty())
       return command->reply(ERR_NOSUCHSERVER);

    command->reply(RPL_LISTSTART);
    
    if (args.empty()){
        for (int i = 0; ;++i)
        {
            irc::IrcServer::t_channel_list::iterator channel = std::find(channels.begin(), channels.end(), args[i]);
            command->reply(RPL_LIST, channel->get_name());
            if (channel == channels.end())
                 break;
        }  
    }
    else{
        irc::IrcServer::t_channel_list::iterator channel = std::find(channels.begin(), channels.end(), args[0]);
        command->reply(RPL_LIST, channel->get_name(), channel->get_topic());
    }
    command->reply(RPL_LISTEND);
    return;

}