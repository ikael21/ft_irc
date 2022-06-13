#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>

class Channel
{
private:
    
    std::string _name;
    std::string _operator;
    std::string _topic;
    bool        _isPrivate;
    std::string _mode;

    std::vector<std::string> _users;
    std::vector<std::string> _invited;

    void _log(std::string message);

public:
    Channel() {};
    Channel(std::string name, std::string user);

    ~Channel();

    bool isPrivate() { return this->_isPrivate; }

    void addUser(std::string user);
    void removeUser(std::string user);

    bool userInChannel(std::string user);
    bool isInvited(std::string user);

    void setName(std::string name);
    std::string getName() { return _name; }

    void setTopic(std::string topic);
    std::string getTopic() { return _topic; }

    void setMode(std::string mode);
    std::string getMode() { return _mode; }

};

#endif