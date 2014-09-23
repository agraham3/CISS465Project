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
        : sock(s), name(n), x(x1), y(y1), active(a)
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
