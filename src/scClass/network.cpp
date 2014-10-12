#include "network.h"

/**************************************************
 *  Server Section  
/**************************************************/

int Server::send_message(std::string msg, TCPsocket sock)
{
    char * buff = (char *)msg.c_str();
    return SDLNet_TCP_Send(sock, buff, MAXLEN);
}

void Server::send_client_message(int i, std::string buff)
{
    if (buff == "")
        return;
    send_message(buff, get_client(i).get_socket());
}

void Server::send_message_to_all_clients(std::string buf)
{
    if(buf == "" || num_clients == 0)
        return;
    int len = buf.size() + 1; // add one for the terminating NULL
    for(int i = 0; i < num_clients; i++)
    {
        int result = send_message(buf, clients[i].get_socket());
        // number of bytes sent is less than the length of bytes
        if (result < len)
        {
            std::cout << "Failed to send message to client["
                      << i << "]\n";
            printf( "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
            handle_disconnect(i);
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
    Client c;
    c.set_name(name);
    c.set_socket(sock);
    c.set_active(true);
    clients.push_back(c);
    num_clients++;

    // Send an acknowledgement to the new client
    std::string player_number = "Your number is: ";
    player_number += to_string(num_clients - 1);
    player_number += "\n";
    send_client_message(num_clients - 1, player_number);
}

int Server::find_client_name(std::string name)
{
    for(int i = 0; i < num_clients; i++)
    {
        if (clients[i].get_name() == name)
            return i;
    }
    return -1;
}

int Server::find_client_index(TCPsocket sock)
{
    for (int i = 0; i < num_clients; i++)
    {
        if(clients[i].get_socket() == sock)
            return i;
    }
    return -1;
}

void Server::reconnect(std::string name) //, std::string password)
{
    clients[find_client_name(name)].set_active(true);
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
        clients[cindex].set_socket(sock);
        clients[cindex].set_player_num(client_num);
        clients[cindex].set_active(true);
        send_message("Account creation successful.", sock);
        return;
    }

    if (clients[cindex].get_active())
    {
        send_message("Duplicate Nickname!", sock);
        SDLNet_TCP_Close(sock);
        return;
    }
    return;
}

void Server::handle_disconnect(int i)
{
    std::string name = clients[i].get_name();
    if (i < 0 || i >= num_clients)
        return;

    // close the old socket, even if its dead
    SDLNet_TCP_Close(clients[i].get_socket());
    clients[i].set_active(false);
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
    for(int i = 0; i < num_clients; i++)
        SDLNet_TCP_AddSocket(set, clients[i].get_socket());
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
