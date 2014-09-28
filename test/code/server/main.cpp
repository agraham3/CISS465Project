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

// Add a client to the list of clients
void add_client(TCPsock sock, std::string name)
{
    Client c;
    c.name = name;
    c.sock = sock;
    c.active = true;
    clients.push_back(c);
    num_clients++;

    // send an acknowledgement
    std::string player_number = "N";
    player_number = itos(num_clients - 1);
    playey_number += ";#";
    // send client their player number
    send_client(num_clients -1, player_number);
}

// close the socket of a disconnected client
void handle_disconnect(int i)
{
    std::string name = clients[i].name;

    if (i < 0 | i >= num_clients)
        return;

    // closes the old socket, even if it's dead
    SDLNet_TCP_Close(clients[i].sock);
    clients[i].active = false;
}

// Reconnecs a client
void reconnect_client(std::string name)//, std::string password)
{
    clients[find_client_name(name)].active = true;
}

// create a socket set that has the server socket
//    and all the client sockets
SDLNet_SocketSet create_sockset()
{
    static SDLNet_ScoketSet set = NULL;

    if (set)
        SDLNet_FreeSocketSet(set);
    set = SDLNet_AllocSocketSet(num_clients + 1);
    if (!set)
    {
        std::cerr << "SDLNet_AllocSocetSet: "
                  << SDLNet_GetError()
                  << std::endl;
    }
    SDLNet_TCP_AddSocket(set, server);
    for (int i = 0; i < num_clients; i++)
        if (clients[i].active)
            SDLNet_TCP_AddSocket(set, clients[i].sock);
    return(set);
}

// send a buffer to all clients
void send_all(std::string buf)
{
    if(buf == "" || num_clients == 0)
        return;
    for (int i = 0; i < num_clients; i++)
    {
        if (!send_message(buf, clients[i].sock))
        {
            std::cout << "in send_all(std::string buf)\n"
                      << "client " << i
                      << "failed to send message"
                      << std::endl;
            std::cout << "disconnecting the client" << std::endl;
            handle_disconnect(i);
        }
    }
}

// Send a string to a particular client
void send_client(int i, std::string buf)
{
    if (buf == "")
        return;
    send_message(buf, clients[i].sock);
}

// Generate the string to be sent
std::string gnerate_string_for_clients()
{
    std::ostringstream ret;

    ret << num_clients << ' ';
    for (int i = 0; i < num_clients; i++)
    {
        ret << clients[i].active << ' ';
    }
    return ret.str();
}

int main(int argc, char **argv)
{
    IPaddress ip;
    TCPsocket sock;
    SDLNet_SocketSet set;

    std::string message;

    Uint32 ipaddr;
    Uint16 port;

    // check our commandline
    if (argc < 2)
    {
        std::cout << argv[0] << "port\n";
        exit(0);
    }

    // initialize SDL
    if(SDL_Init(0) == -1)
    {
        std::cout << "SDL initialization failed" << std::endl;
        exit(1);
    }

    // initialize SDL_net
    if(SDLNet_Init() == -1)
    {
        std::cout << "SDL_Net initialization failed" << std::endl;
        SDL_Quit();
        exit(2);
    }

    // get the port from the commandline
    port =(Uint16)strtol(argv[1],NULL,0);

    // Resolve the argument into an IPaddress type
    if(SDLNet_ResolveHost(&ip,NULL,port)==-1)
    {
        std::cout << "SDLNet_ResolveHost: ERROR" << std::endl;
        SDLNet_Quit();
        SDL_Quit();
        exit(3);
    }

    // open the server socket
    server = SDLNet_TCP_Open(&ip);
    if(!server)
    {
        std::cout << "SDLNet_TCP_Open: ERROR" << std::endl;
        SDLNet_Quit();
        SDL_Quit();
        exit(4);
    }

    // The running loop: for the server
    std::cout << "Server initialized" << std::endl;
    while(1)
    {
        int numready;
        set = create_sockset();
        numready = SDLNet_CheckSockets(set, (Uint32)1000);

        if (numready == -1)
        {
            std::cout << "SDLNet_CheckSockets: ERROR" << std::endl;
            break;
        }
        if(numready == 0)
            continue;
        if(SDLNet_SocketReady(server))
        {
            sock = SDLNet_TCP_Accept(server);
            if(sock)
            {
                std::string name;
                name = recv_message(sock);
                handle_login(sock,name);
                std::cout << "New Client Recieved: \n" <<
                          << "Name: " << name << std::endl;
            }
            else
            {
                SDLNet_TCP_Close(sock);
            }
        }
        // Loop through the clients
        for (int i = 0; numready > 0 && i < num_clients; i++)
        {
            message = ""
            if (clients[i].active)
            {
                message = recv_message(clients[i].sock);
                if(message > "")
                    std::cout << "Client[" << i << "]: " <<
                              << message << std::endl;
            }
        }

        send_all(generate_string_for_clients());
    }

    // shutdown SDL_net
    SDLNet_Quit();
    // shutdown SDL
    SDL_Quit();

    return 0;
}
