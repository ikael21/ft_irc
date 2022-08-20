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

Compile server

```bash
make
```

Compile server in debug mode

```bash
DEBUG=1 make re
```

Interact with `nc`:

```bash
nc -c <hostname> 6666
```

## Commands

- `AWAY [message]`
- `INVITE <nickname> <channel>`
- `JOIN <channel>{,<channel>} [<key>{,<key>}]`
- `KICK <channel> <user> [<comment>]`
- `LIST [<channel>{,<channel>} [<server>]]`
- `MODE <channel> {[+|-]|o|p|s|i|t|n|b|v} [<limit>] [<user>]`
- `MODE <nickname> {[+|-]|i|w|s|o}`
- `NAMES [<channel>{,<channel>}]`
- `NICK <nickname>`
- `NOTICE <nickname> <text>`
- `PART <channel>{,<channel>}`
- `PASS <password>`
- `PONG <daemon>`
- `PRIVMSG <receiver> {,<receiver>} <text to be sent>`
- `QUIT [<Quit message>]`
- `TIME <server>`
- `TOPIC <channel> [<topic>]`
- `USER <username> <hostname> <servername> <realname>`
- `USERS [<server>]`

For `Linux` please install [libkqueue](https://github.com/mheily/libkqueue).

```bash
sudo apt update
sudo apt install libkqueue-dev
```

## BUGS

### 1 Когда все пользователи выходят, замечено в LimeChat
```text
ircserv(4155,0x116cd9600) malloc: Heap corruption detected, free list is damaged at 0x6000020640a0
*** Incorrect guard value: 0
ircserv(4155,0x116cd9600) malloc: *** set a breakpoint in malloc_error_break to debug
[1]    4155 abort      ./ircserv 6667 1234
```