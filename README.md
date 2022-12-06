# tcp/ip-server

This is a test program for tcp/ip. You can exchange messages with another computer.

<b>client</b>

<img width="365" alt="スクリーンショット 2022-12-06 15 11 16" src="https://user-images.githubusercontent.com/79553411/205835212-86c0a7d3-28bf-4606-abaa-19553b0065c6.png">

<b>server</b>

<img width="261" alt="スクリーンショット 2022-12-06 15 11 28" src="https://user-images.githubusercontent.com/79553411/205835219-76eb67fa-f80a-49de-9984-314de8845439.png">

## functions

- quit command
- message exchange

## Usage

After clone this repo,

<b>client</b>

Change ip address and port number you want to connect.

```c
11 char *ip = "127.0.0.1";  //ip address

20 server.sin_port = htons(4000); //port number
```

If you want to connect another computer, you have to find local-ip-address.

<b>example</b>

```sh
# Type : on Linux
$ ifconfig 

# Type : on windows
$ ipconfig
```

<b>server</b>

Change port number.

```c
32 addr.sin_port = htons(4000); //port number
```

After this, start the server program, then start client program.

```sh
#client
./tcp-cli

#server
./tcp-ser

# shell

This is a simple shell that works like bash and zsh.

## functions

- background processing
- inside command
- processing switch to bg or fg

## Usage

After clone this repo,

```sh
cd c_study/shell

./code
```
