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
        std::cout << "Failed to send message to client: "
                  << name
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

void Server::send_message_to_all_other_clients(std::string name, std::string buff)
{
    typedef std::map< std::string, TCPsocket >::iterator it_type;
    for(it_type i = clients.begin(); i != clients.end(); i++)
    {
        if (i->first != name)
        {
            send_client_message(i->first, buff);
        }
    }
}

std::string Server::receive_message(TCPsocket sock)
{
    char buff[MAXLEN] = {' '};
    int result = SDLNet_TCP_Recv(sock, buff, MAXLEN);
    if (result <= 0)
        return "";
    
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
    num_clients++;
    std::cout << "Added client: " << name
              << "\nport: " << sock
              << std::endl;
}

void Server::handle_login(TCPsocket sock, std::string name, std::string pass)
{
    if(!name.length())
    {
        send_message("Invalid Nickname!", sock);
        SDLNet_TCP_Close(sock);
        return;
    }

    try
    {
        User user(name, pass, true);
        
        // check if user is already logged in
        bool exists = false;
        typedef std::map< std::string, TCPsocket >::iterator it_type;
        for (it_type i = clients.begin(); i != clients.end(); i++)
        {
            if (i->first == name)
                exists = true;
        }
        if (exists)
        {
            send_message("This account is already logged in.", sock);
            SDLNet_TCP_Close(sock);
            return;
        }
        send_message("Login successful!", sock);
        add_client(sock, name);
        send_message(to_string(user), sock);
    }
    catch (USER_ERROR err)
    {
        send_message(err.message, sock);
        SDLNet_TCP_Close(sock);
    }
    
    return;
}

void Server::handle_register(TCPsocket sock, std::string name, std::string pass)
{
    try
    {
        User user(name, pass, false);
        send_message("Registration successful!", sock);
    }
    catch (USER_ERROR err)
    {
        send_message(err.message, sock);
        SDLNet_TCP_Close(sock);
    }
    return;
}

void Server::handle_disconnect(std::string name)
{
    int cnt = clients.count(name);
    if (cnt <= 0)
    {
        std::cout << "Client: " + name + " does not exits to disconnect." << std::endl;
        return;
    }
    std::cout << "Disconnecting client: " << name << std::endl;
    SDLNet_TCP_Close(clients[name]);
    std::map < std::string, TCPsocket >::iterator it;
    it = clients.find(name);
    clients.erase(it);
    send_message_to_all_other_clients(name, "msg:" + name + " has disconnected");
    send_message_to_all_other_clients(name, "rmv:" + name);

    User::update_file();
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
    {
        SDLNet_TCP_AddSocket(set, i->second);
    }
    return set;
}


/**************************************************
 *  Client Section  
/**************************************************/

// Receive a string over TCP/IP
std::string Client::receive_message(TCPsocket sock)
{
    char buff[MAXLEN] = {' '};
    int result = SDLNet_TCP_Recv(sock, buff, MAXLEN);
    if (result <= 0)
    {
        std::cout << "TCP connection broken because of error or closure"
                  << std::endl;
        SDLNet_TCP_Close(sock);
        exit(1);
    }

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

SDLNet_SocketSet Client::create_sockset()
{
    static SDLNet_SocketSet set = NULL;
    if(set)
        SDLNet_FreeSocketSet(set);
    set = SDLNet_AllocSocketSet(1);
    if(!set)
    {
        std::cerr << "SDLNet_AllocSocketSet: " << SDLNet_GetError()
                  << std::endl;
        return 0;
    }

    SDLNet_TCP_AddSocket(set, sock);
    return set;
}
