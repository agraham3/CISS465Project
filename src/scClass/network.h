#ifndef NETWORK_H
#define NETWORK_H

#include "includes.h"

const int MAXLEN = 1024;

// note: maybe change the username string to the user class?
class Client
{
public:
    Client(TCPsocket s=NULL, std::string n="", bool a=false)
        : sock(s), name(n), active(a), player_num(-1)
    {}

    TCPsocket get_socket() const { return sock; }
    std::string get_name() const { return name; }
    bool get_active() const { return active; }
    int get_player_num() const { return player_num; }

    void set_socket(const TCPsocket s) { sock = s; }
    void set_name(const std::string n) { name = n; }
    void set_active(const bool a) { active = a; }
    void set_player_num(const int x) { player_num = x; }
    
private:
    TCPsocket sock;
    std::string name;
    bool active;
    int player_num;
};

class Server
{
public:
    Server(Uint16 _port)
        : port(_port)
    {
        // Initialize SDL
        if(SDL_Init(0) == -1)
        {
            std::cout << "Failed to initialize SDL: ERROR" << std::endl;
            exit(1);
        }

        // Initialize SDL_net
        if(SDLNet_Init() == -1)
        {
            std::cout << "Failed to initilize SDL_net: ERROR" << std::endl;
            SDL_Quit();
            exit(2);
        }

        // Resolve the argument into an IPaddress type
        if (SDLNet_ResolveHost(&ip,NULL,port) == -1)
        {
            std::cout << "SDLNet_ResolveHost: ERROR" << std::endl;
            SDLNet_Quit();
            SDL_Quit();
            exit(3);
        }

        // Open the server socket
        server = SDLNet_TCP_Open(&ip);
        if(!server)
        {
            std::cout << "SDLNet_TCP_Open: ERROR" << std::endl;
            SDLNet_Quit();
            SDL_Quit();
            exit(4);
        }
        
    }

    int send_message(std::string message, TCPsocket sock);
    void send_client_message(int client_num, std::string message);
    void send_message_to_all_clients(std::string buf);
    std::string receive_message(TCPsocket sock);
    
    void add_client(TCPsocket sock, std::string name);
    int find_client_name(std::string name);
    int find_client_index(TCPsocket sock);
    void reconnect(std::string name);//, std::string password)

    void handle_login(TCPsocket sock, std::string name,
                      int client_num);
    void handle_disconnect(int client_num);
    
    SDLNet_SocketSet create_sockset();

    Client get_client(int i) { return clients[i]; }
    
private:
    std::vector<Client> clients;
    int num_clients;
    SDLNet_SocketSet set;
    IPaddress ip;
    TCPsocket server;
    Uint16 port;
};

#endif

