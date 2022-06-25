#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <string>
# include <vector>

class Command {

 public:
  Command(std::string command);

 private:

  std::string               _command;
  std::vector<std::string>  _arguments;

  std::string getCommand() { return _command; }
  std::vector<std::string> getArguments() { return _arguments; }

  size_t numArgs() { return _arguments.size(); }

};

# endif
