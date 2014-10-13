#include "network.h"

/**************************************************
 *  Server Section  
/**************************************************/

int Server::send_message(std::string msg, TCPsocket sock)
{
    char * buff = (char *)msg.c_str();
    return SDLNet_TCP_Send(sock, buff, MAXLEN);
}

void Server::send_client_message(std::string name, std::string buff)
{
    if (buff == "")
        return;
    int len = buff.size() + 1; // add one for the terminating NULL
    int result = send_message(buff, clients[name]);
    if (result < len)
    {
        std::cout << "Failed to send message to client: Disconnecting client"
                  << std::endl;
        handle_disconnect(name);
    }
}

void Server::send_message_to_all_clients(std::string buf)
{
    if(buf == "" || num_clients == 0)
        return;
    int len = buf.size() + 1; // add one for the terminating NULL
    typedef std::map< std::string, TCPsocket >::iterator it_type;
    for (it_type i = clients.begin(); i != clients.end(); i++)
    {
        int result = send_message(buf, i->second);
        // number of bytes sent is less than the length of bytes
        if (result < len)
        {
            std::cout << "Failed to send message to client: "
                      << i->first << " \n";
            printf( "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
            handle_disconnect(i->first);
        }
    }
}

std::string Server::receive_message(TCPsocket sock)
{
    char buff[MAXLEN] = {' '};
    SDLNet_TCP_Recv(sock, buff, MAXLEN);
    if (buff == NULL)
    {
        std::string s = "";
        return s;
    }

    std::string s(buff, strlen(buff));
    return s;
}

void Server::add_client(TCPsocket sock, std::string name)
{
    std::cout << "Trying to add a client." << std::endl;
    clients.insert(std::pair< std::string, TCPsocket >(name, sock));

    // Send an acknowledgement to the new client
    std::string player_number = "Your number is: ";
    player_number += to_string(num_clients);
    send_client_message(name, player_number);
    num_clients++;
}

int Server::find_client_name(std::string name)
{
    if(clients.count(name)> 0)
    {
        return 1;
    }
    
    return -1;
}

void Server::handle_login(TCPsocket sock, std::string name, int client_num)
{
    if(!name.length())
    {
        send_message("Invalid Nickname!", sock);
        SDLNet_TCP_Close(sock);
        return;
    }

    int cindex = find_client_name(name);
    
    if (cindex == -1)
    {
        add_client(sock, name);
        send_message("Account creation successful.", sock);
        return;
    }
    
    send_message("Duplicate Nickname!", sock);
    SDLNet_TCP_Close(sock);
    
    return;
}

void Server::handle_disconnect(std::string name)
{
    std::cout << "Disconnecting client: " << name << std::endl;
    SDLNet_TCP_Close(clients[name]);
    std::map < std::string, TCPsocket >::iterator it;
    it = clients.find(name);
    clients.erase(it);
}

// Creates a socket set that has the server socket and all the client sockets
SDLNet_SocketSet Server::create_sockset()
{
    static SDLNet_SocketSet set = NULL;
    if(set)
        SDLNet_FreeSocketSet(set);
    set = SDLNet_AllocSocketSet(num_clients + 1);
    if(!set)
    {
        std::cerr << "SDLNet_AllocSocketSet: " << SDLNet_GetError()
                  << std::endl;
        return 0;
    }

    SDLNet_TCP_AddSocket(set, server);
    typedef std::map< std::string, TCPsocket >::iterator it_type;
    for (it_type i = clients.begin(); i != clients.end(); i++)
        SDLNet_TCP_AddSocket(set, i->second);
    return set;
}


/**************************************************
 *  Client Section  
/**************************************************/

// Receive a string over TCP/IP
std::string Client::receive_message(TCPsocket sock)
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
void Client::send_message(std::string msg, TCPsocket sock)
{
    char * buff = (char *)msg.c_str();
    const int len = strlen(msg.c_str()) + 1;
    int result = SDLNet_TCP_Send(sock, buff, len);
    if (result < len)
        std::cerr << "SDLNet_TCP_Send: " << SDLNet_GetError()
                  << std::endl;
}

void Client::receive_player_number(std::string message)
{
    int i = 0;
    std::string temp_num = "";
    if(message[0] == 'N')
    {
        i++;
        while(message[i] != ';')
        {
            temp_num += message[i];
            i++;
        }
        player_num = atoi(temp_num.c_str());
    }
}
