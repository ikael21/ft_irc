#include "commands.hpp"

void LIST(Command *command) {

    // TODO
    // 
    // секретные каналы не приводятся в списке,
    // если, конечно, клиент не является членом подобного канала.
        //  402     ERR_NOSUCHSERVER
        //                "<server name> :No such server"

        //        - Используется для сообщения, что сервер, указанный в
        //          строке параметров, не найден.
        //if (!server)
    //   return command->reply(ERR_NOSUCHSERVER);

    std::vector<std::string> args = command->get_arguments();
    irc::IrcServer& server = command->get_server();
  
    irc::IrcServer::t_channel_list& channels = server.get_channels();    
    command->reply(RPL_LISTSTART);
    
    if (!args.size()){
        for( irc::IrcServer::t_channel_list::iterator channel = channels.begin(); channel != channels.end(); ++channel)
        {
            if(!channel->is_private()){
                std::cout <<  channel->get_name() << std::endl;
                command->reply(RPL_LIST, channel->get_name(), channel->get_topic());
            }
        }  
    }
    else{
        irc::IrcServer::t_channel_list::iterator channel = std::find(channels.begin(), channels.end(), args[0]);
        command->reply(RPL_LIST, channel->get_topic());
    }
    command->reply(RPL_LISTEND);
    return;
}