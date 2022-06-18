# Internet Relay Chat
This project is about creating your own `IRC server`.

**Internet Relay Chat** or **IRC** is a text-based communication protocol on the Internet.
It offers real-time messaging that can be either public or private. Users can exchange
direct messages and join group channels

`IRC clients` connect to `IRC servers` in order to join channels. `IRC servers` are connected
together to form a network.

## Basic code style rules
[Google C++ code style guide](https://google.github.io/styleguide/cppguide.html#Formatting) (only **Formatting** part)

## References
- [RFC 1459 RUS](https://www.lissyara.su/doc/rfc/rfc1459/)
- [RFC 2813](https://datatracker.ietf.org/doc/html/rfc2813) (LAST ONE)

## Commands
- `PASS <password>`
- `NICK <nickname>`
- `USER <username> <hostname> <servername> <realname>`
- `PRIVMSG <receiver> {,<receiver>} <text to be sent>`
- `NOTICE <nickname> <text>`
- `JOIN <channel>{,<channel>} [<key>{,<key>}]`
- `LIST [<channel>{,<channel>} [<server>]]`
- `NAMES [<channel>{,<channel>}]`
- `INVITE <nickname> <channel>`
- `TOPIC <channel> [<topic>]`
- `KICK <channel> <user> [<comment>]`
- `PING <server>`
- `PONG <daemon>`
- `TIME <server>`
