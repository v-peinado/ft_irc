# ft_irc

A lightweight IRC server implementation for 42 School, supporting standard IRC commands and client connections.

## Overview

This project implements a basic Internet Relay Chat (IRC) server that follows RFC standards. It handles multiple client connections concurrently using socket programming with non-blocking I/O multiplexing via the `poll()` function. The server supports essential IRC commands necessary for text communication, channel management, and user administration.

## Features

- User authentication with password protection
- Channel creation and management
- Private messaging between users
- Channel operator privileges
- Multiple simultaneous client connections
- Standard IRC command implementation

## Compilation and Usage

### Building the project

```bash
make
```

### Running the server

```bash
./ircserv <port> <password>
```

Example:
```bash
./ircserv 8080 12345678
```

This starts the server listening on port 8080 with the connection password "12345678".

## Connecting to the Server

### Using irssi (recommended)

```bash
irssi -c localhost -p <port> -w <password>
```

Example:
```bash
irssi -c localhost -p 8080 -w 12345678
```

### Using netcat (for testing)

```bash
nc localhost <port>
```

Then manually enter the commands:
```
PASS <password>
NICK <nickname>
USER <username> 0 * :<realname>
```

## Supported Commands

The server implements the following IRC commands. Commands can be used with either the slash prefix (client-side format) or directly in uppercase (protocol format):

| Command   | Description                                   | Client Usage                | Protocol Usage                |
|-----------|-----------------------------------------------|-----------------------------|-----------------------------|
| PASS      | Set the connection password                   | `/pass password`            | `PASS password`             |
| NICK      | Set or change your nickname                   | `/nick nickname`            | `NICK nickname`             |
| USER      | Set your username and real name               | `/user username 0 * :Real Name` | `USER username 0 * :Real Name` |
| QUIT      | Disconnect from the server                    | `/quit [message]`           | `QUIT [message]`            |
| JOIN      | Join a channel                                | `/join #channelname [key]`  | `JOIN #channelname [key]`   |
| KICK      | Remove a user from a channel                  | `/kick #channel nickname [reason]` | `KICK #channel nickname [reason]` |
| TOPIC     | View or change a channel's topic              | `/topic #channel [new topic]` | `TOPIC #channel [new topic]` |
| MODE      | Change channel or user modes                  | `/mode #channel +/-flag [parameters]` | `MODE #channel +/-flag [parameters]` |
| PRIVMSG   | Send a message to a user or channel           | `/privmsg target :message`  | `PRIVMSG target :message`   |
| INVITE    | Invite a user to a channel                    | `/invite nickname #channel` | `INVITE nickname #channel`  |
| INFO      | Display server information                    | `/info`                     | `INFO`                      |
| PING      | Check server connection                       | `/ping [server]`            | `PING [server]`             |

Note: When using IRC clients like irssi, you typically use the slash commands. When using netcat or implementing your own client, you use the protocol format (uppercase without slash).

## Technical Implementation

### Socket Programming

The server uses Berkeley sockets API for network communication:
- Socket creation with `socket()`
- Address binding with `bind()`
- Connection listening with `listen()`
- Client acceptance with `accept()`

### I/O Multiplexing with poll()

Rather than using one thread per client, the server uses I/O multiplexing with `poll()` to handle multiple client connections efficiently:

1. The server creates a listening socket
2. Each new connection is added to a `pollfd` array
3. `poll()` monitors all file descriptors for activity
4. When activity is detected, the server reads/writes data accordingly

This approach allows a single thread to handle many connections concurrently without blocking.

### Command Processing

The server implements a command pattern:
1. Receives raw data from clients
2. Parses the data into command structures
3. Validates commands and arguments
4. Executes the appropriate handler for each command
5. Sends responses back to clients

## Channel Operations

- Channels are created with the `/JOIN` command
- Channel names must begin with '#'
- Channel operators can set modes with `/MODE`
- Supported channel modes include:
  - `+k`: Set/remove channel key (password)
  - `+i`: Set/remove invite-only flag
  - `+t`: Restrict topic changes to operators
  - `+l`: Set/remove user limit

## Error Handling

The server implements standard IRC numeric replies and error codes as defined in the RFC. Some common errors:

- 401: No such nick/channel
- 403: No such channel
- 461: Not enough parameters
- 464: Password incorrect
- 471: Cannot join channel (+l)
- 473: Cannot join channel (+i)
- 475: Cannot join channel (+k)
- 482: You're not channel operator

## Notes

- This server is designed for educational purposes and may not implement all features of a full IRC server
- IPv4 connections only
- Messages are delimited by "\r\n" as per IRC protocol