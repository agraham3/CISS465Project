/******************************************************************************
 * This is a very simple example with SDL_net. There is no error checking
 * Run client.exe or telnet 127.0.0.1 1234 (local client) to test
 *****************************************************************************/


/******************************************************************************
 * BEGIN Includes.
 *****************************************************************************/

// Standard includes
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// SDL wrapper from Dr. Liow
#include "Includes.h"
#include "Event.h"
#include "compgeom.h"
#include "Constants.h"
#include "Surface.h"

// SDL net
#include "SDL_net.h"


/******************************************************************************
 * END Includes.
 *****************************************************************************/


/******************************************************************************
 * Class definitions.
 *****************************************************************************/
class Client
{
public:    
    Client(TCPsocket s=NULL, std::string n="", bool a=false)
        : sock(s), name(n), active(a), player_num(-1)
    {}
    
	TCPsocket sock;
	std::string name;
    bool active;
    int player_num
};


/******************************************************************************
 * Global Constants.
 *****************************************************************************/
const int MAXLEN = 1024;


/******************************************************************************
 * Global Variables.
 *****************************************************************************/
std::vector<Client> clients;
int num_clients=0;
TCPsocket server;


/******************************************************************************
 * Functions
 *****************************************************************************/
void send_client(int, std::string);
void send_all(std::string buf);
int find_client_name(std::string name);
void reconnect(std::string name);//, std::string password)
void add_client(TCPsocket sock, std::string name);


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
std::string recv_message(TCPsocket sock)
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
    return SDLNet_TCP_Send(sock, buff, MAXLEN);
}


/* find a client in our array of clients by it's socket. */
/* the socket is always unique */
int find_client(TCPsocket sock)
{
	for(int i = 0; i < num_clients; i++)
		if(clients[i].sock == sock)
			return(i);

    return -1;
}


/* find a client in our array of clients by it's name. */
/* the name is always unique */
int find_client_name(std::string name)
{
	for(int i=0; i < num_clients;i++)
		if (clients[i].name == name)
			return i;
		
	return -1;
}


// Handles logging in
void handle_login(TCPsocket sock, std::string name, int client_num)
{
    if(!name.length())
	{
		send_message("Invalid Nickname...bye bye!", sock);
		SDLNet_TCP_Close(sock);
		return;
	}

    int cindex = find_client_name(name);

    if (cindex == -1)
    {
        add_client(sock, name);
        return;
    }
    
    if (clients[cindex].active)
    {
        send_message("Duplicate Nickname... bye bye!", sock);
        SDLNet_TCP_Close(sock);
        return;
    }

    clients[cindex].sock = sock;
    clients[cindex].player_number = client_num;
    clients[cindex].active = true;
    return;
}


// Add a client to the list of clients
void add_client(TCPsocket sock, std::string name)
{	
	Client c;

	c.name = name;
	c.sock = sock;
    c.active = true;

	clients.push_back(c);

	num_clients++;

	// Send an acknowledgement
    std::string player_number = "N";
	player_number += itos(num_clients - 1);
	player_number += ";#";
	// send client their player number
	send_client(num_clients - 1, player_number);
}


/* closes the socket of a disconnected client */
void handle_disconnect(int i)
{
	std::string name=clients[i].name;

	if(i<0 || i>=num_clients)
		return;
	
	/* close the old socket, even if it's dead... */
	SDLNet_TCP_Close(clients[i].sock);
    clients[i].active = false;
    //std::cout << "Removed client # " << i << std::endl;
    //std::cin.ignore();
}


/* Reconnects a client */
void reconnect_client(std::string name)//, std::string password)
{
    clients[find_client_name(name)].active = true;
}


/* create a socket set that has the server socket and all the client sockets */
SDLNet_SocketSet create_sockset()
{
	static SDLNet_SocketSet set=NULL;

	if(set)
		SDLNet_FreeSocketSet(set);
	set = SDLNet_AllocSocketSet(num_clients + 1);
	if(!set)
    {
        std::cerr << "SDLNet_AllocSocketSet: " << SDLNet_GetError() << std::endl;
        return 0;
	}
	SDLNet_TCP_AddSocket(set, server);
	for(int i=0; i < num_clients; i++)
        if (clients[i].active)
            SDLNet_TCP_AddSocket(set, clients[i].sock);
	return(set);
}


/* send a buffer to all clients */
void send_all(std::string buf)
{
	if(buf == "" || num_clients == 0)
		return;
    
    for (int i = 0; i < num_clients; i++)
	{
		if(!send_message(buf, clients[i].sock))
        {
            std::cout << "Failed to send message to client["
                      << i << "]\n";
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
std::string generate_string_for_clients()
{
    std::ostringstream ret;

    ret << "number of clients: " << num_clients << '\n';
    for (int i = 0; i < num_clients; i++)
    {
        ret << "Client number " << i << ": ";
        if (clients[i].active)
            ret << " active\n";
        else
            ret << " disconnected\n";
	}

    return ret.str();
}


// Point of entry
int main(int argc, char **argv)
{
	IPaddress ip;
	TCPsocket sock;
	SDLNet_SocketSet set;
	
	std::string message;
	
	Uint32 ipaddr;
	Uint16 port;
	
	/* check our commandline */
	if(argc < 2)
	{
		std::cout << argv[0] << "port\n";
		exit(0);
	}
	
	/* initialize SDL */
	if(SDL_Init(0)==-1)
	{
		//printf("SDL_Init: %s\n",SDL_GetError());
		std::cout << "Failed to initialize SDL: ERROR" << std::endl;
		exit(1);
	}

	/* initialize SDL_net */
	if(SDLNet_Init()==-1)
	{
		//printf("SDLNet_Init: %s\n",SDLNet_GetError());
		std::cout << "Failed to initilize SDL_net: ERROR" << std::endl;
		SDL_Quit();
		exit(2);
	}

	/* get the port from the commandline */
	port=(Uint16)strtol(argv[1],NULL,0);

	/* Resolve the argument into an IPaddress type */
	if(SDLNet_ResolveHost(&ip,NULL,port)==-1)
	{
		std::cout << "SDLNet_ResolveHost: ERROR" << std::endl;
		SDLNet_Quit();
		SDL_Quit();
		exit(3);
	}

	/* open the server socket */
	server=SDLNet_TCP_Open(&ip);
	if(!server)
	{
		std::cout << "SDLNet_TCP_Open: ERROR" << std::endl;
		SDLNet_Quit();
		SDL_Quit();
		exit(4);
	}

    int prev_count = 0;
    int client_num = 0;
	while(1)
	{
		int numready;
		set = create_sockset();
		numready = SDLNet_CheckSockets(set, (Uint32)1000);
        if (numready > prev_count)
        {
            prev_count = numready;
            std::cout << "Numready " << numready << std::endl;
        }
		if(numready == -1)
		{
			std::cout << "SDLNet_CheckSockets: ERROR" << std::endl;
			break;
		}
		if(numready == 0)
			continue;
        
		if(SDLNet_SocketReady(server))
		{
			sock=SDLNet_TCP_Accept(server);
			if(sock)
			{
				std::string name;       
				name = recv_message(sock);
                handle_login(sock,name,client_num);
                ++client_num;
            }
            else
                SDLNet_TCP_Close(sock);
		}


		//---------------------------------------------------------------------
		// LOOP THROUGH CLIENTS
		//---------------------------------------------------------------------
		for(int i = 0; numready > 0 && i < num_clients; i++)
		{
			message = "";
            if (clients[i].active)
            {
                if(SDLNet_SocketReady(clients[i].sock))
                {
                    //---------------------------------------------------------
                    // GET DATA FROM CLIENT
                    //---------------------------------------------------------
                    message = recv_message(clients[i].sock);
                    if(message == "I want to stop")
                    {
                        handle_disconnect(i);
                    }
                    
                    if(message > "")
                        std::cout << message << std::endl;
                }
            }
        }
        send_all(generate_string_for_clients());
	}
    
	/* shutdown SDL_net */
	SDLNet_Quit();

	/* shutdown SDL */
	SDL_Quit();

	return(0);
}
