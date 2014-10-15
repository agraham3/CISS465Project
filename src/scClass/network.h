#ifndef NETWORK_H
#define NETWORK_H

#include "includes.h"

const int MAXLEN = 1024;

// note: maybe change the username string to the user class?
class Client
{
public:
    Client(std::string n="", const char * host = "",
           Uint16 port = 0)
        : sock(NULL), name(n), active(false), player_num(-1), set(NULL),
          net_thread(NULL), local_thread(NULL)
    {
        // initilaize SDL
        if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
        {
            std::cout << "SDL_Init: Errror" << std::endl;
            exit(0);
        }

        // initialize SDL_net
        if(SDLNet_Init() == -1)
        {
            std::cout << "SDLNet_Init: ERROR" << std::endl;
            SDL_Quit();
            exit(0);
        }

        set = SDLNet_AllocSocketSet(1);
        if(!set)
        {
            std::cout << "SDLNet_AllocSocketSet: ERROR" << std::endl;
            SDLNet_Quit();
            SDL_Quit();
            exit(0);
        }

        // Resolve the argument into an IPAddress type
        std::cout << "Connecting to " << host
                  << " port " << port
                  << std::endl;
        
        if(SDLNet_ResolveHost(&ip,host,port) == -1)
        {
            std::cout << "SDLNet_ResolveHost: ERROR" << std::endl;
            SDLNet_Quit();
            SDL_Quit();
            exit(0);
        }

        // open the server socket
        sock = SDLNet_TCP_Open(&ip);
        if (!sock)
        {
            std::cout << "SDLNet_TCP_Open: ERROR" << std::endl;
            SDLNet_Quit();
            SDL_Quit();
            exit(0);
        }

        if(SDLNet_TCP_AddSocket(set, sock) == -1)
        {
            std::cout << "SDLNet_TCP_AddSocket: ERROR" << std::endl;
            SDLNet_Quit();
            SDL_Quit();
            exit(0);
        }
        send_message(name, sock);
        std::cout << "Logged in as: " << name << std::endl;
        std::cout << receive_message(sock) << std::endl;
    }

    TCPsocket get_socket() const { return sock; }
    std::string get_name() const { return name; }
    bool get_active() const { return active; }
    int get_player_num() const { return player_num; }

    void set_socket(const TCPsocket s) { sock = s; }
    void set_name(const std::string n) { name = n; }
    void set_active(const bool a) { active = a; }
    void set_player_num(const int x) { player_num = x; }

    std::string receive_message(TCPsocket sock);
    void send_message(std::string message, TCPsocket sock);
    void receive_player_number(std::string message);

    SDLNet_SocketSet socket_set() const { return set; }
    void set_socket_set(SDLNet_SocketSet s) { set = s; }
    SDLNet_SocketSet create_sockset();
    
private:
    IPaddress ip;
    Uint16 port;
    TCPsocket sock;
    std::string name;
    bool active;
    int player_num;
    SDL_Thread *net_thread, *local_thread;
    SDLNet_SocketSet set;
};

class Server
{
public:
    Server(Uint16 _port)
        : port(_port), num_clients(0)
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
            std::cout << "SDLNet_ResolveHoast: ERROR" << std::endl;
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
    void send_client_message(std::string name, std::string message);
    void send_message_to_all_clients(std::string buf);
    void send_message_to_all_other_clients(std::string name, std::string buff);
    std::string receive_message(TCPsocket sock);
    
    void add_client(TCPsocket sock, std::string name);
    int find_client_name(std::string name);
    //void reconnect(std::string name);//, std::string password)

    void handle_login(TCPsocket sock, std::string name,
                      int client_num);
    void handle_disconnect(std::string name);
    
    SDLNet_SocketSet create_sockset();

    void set_socket_set(SDLNet_SocketSet s) { set = s; }

    SDLNet_SocketSet socket_set() const { return set; }
    IPaddress get_ip() const { return ip; }
    TCPsocket get_socket() const { return server; }
    int get_num_clients() const { return num_clients; }

    std::map< std::string, TCPsocket > get_clients() const { return clients; }
   
private:
    std::map< std::string, TCPsocket > clients;
    int num_clients;
    SDLNet_SocketSet set;
    IPaddress ip;
    TCPsocket server;
    Uint16 port;
};

#endif

