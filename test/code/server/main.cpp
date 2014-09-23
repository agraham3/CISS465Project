#include <iostream>
#include <string>
#include <vector>

#include "SDL_net.h"

#include "Includes.h"
#include "Constants.h"
#include "compgeom.h"
#include "Surface.h"
#include "Event.h"

class Client
{
public:
    Client(TCPsocet s = NULL, std::string n="",
           float x1 = 0.0f, float y1 = 0.0f, bool a = false)
        : sock(s), name(n), active(a)
    {}

    TCPsocet sock;
    std::string name;
    float x, y;
    bool active;
};

const int MAXLEN = 1024;
std::vector<Client> clients;
int num_clients = 0;
TCPsocet server;

void send_client(int, std::string);
void send_all(std::string buf);
int find_client_name(std::string name);
void reconnect(std::string name);
void add_client(TCPsocet sock, std::string name);

// Converts a float to string
std::string ftos(float f)
{
    std::ostringstream buff;
    buff << f;
    return buff.str();
}

// Converts an integer to string
std::string itos(int i)
{
    std::ostringstream buff;
    buff << i;

    return buff.str();
}

// Receive a string over TCP/IP
std::string recv_message(TCPsocet sock)
{
    char buff[MAXLEN] = {' '};
    SDLNet_TCP_Recv(sock, buff, MAXLEN);

    if (buff == NULL)
    {
        std::string ret = "";
        return ret;
    }

    std::string ret(buff, strlen(buff));
    return ret;
}

// Send a string over TCP/IP
int send_message(std::string msg, TCPsocket sock)
{
    char * buff = (char *)msg.c_str();
    return SDL_TCP_Send(sock, buff, MAXLEN);
}

// Find a client in our array of clients by it's socket.
//    NOTE: sockets are unique
int find_client_name(TCPsocket sock)
{
    for(int i = 0; i < num_clients; i++)
        if (clients[i].sock == sock)
            return i;
    return -1;
}

// Handles loggin in
void handle_login(TCPsocket sock, std::string name)
{
    if(!name.lenth)
    {
        send_message("Invalid Nickname.!" sock)
            SDLNet_TCP_Close(sock);
        return;
    }

    // check if username is on the network
    int client_index = find_client_name(name);
    
    // client name not found create a new name
    if (client_index == -1)
    {
        add_client(sock, name);
        return;
    }

    // name not original
    if (clients[client_index].active)
    {
        send_message("Duplicate name found.", sock);
        SDLNet_TCP_Close(sock);
        return;
    }

    // activate the client
    clients[client_index].sock = sock;
    clients[client_index].active = true;
    return;
}

int main()
{
// create a listening TCP socket on port 8080 (server)
    IPaddress ip;
    TCPsocket server_tcpsock;
    if(SDLNet_ResolveHost(&ip,NULL,8080)==-1) {
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(1);
    }
    server_tcpsock=SDLNet_TCP_Open(&ip);
    if(!server_tcpsock) {
        printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(2);
    }

    // accept a connection coming in on server_tcpsock
    TCPsocket new_tcpsock;
    while (1)
    {
        new_tcpsock=SDLNet_TCP_Accept(server_tcpsock);
        if(!new_tcpsock) {
            printf("SDLNet_TCP_Accept: %s\n", SDLNet_GetError());
        }
        else {
            // communicate over new_tcpsock
            std::cout << "found" << std::endl;
            break;
        }
    }
    
// close the connection on sock
//TCPsocket sock;
    SDLNet_TCP_Close(server_tcpsock);

    return 0;
}
