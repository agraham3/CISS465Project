#include "network.h"

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

void Server::handle_disconnect(int i)
{
    std::string name = clients[i].get_name();
    if (i < 0 || i >= num_clients)
        return;

    // close the old socket, even if its dead
    SDLNet_TCP_Close(clients[i].get_socket());
    clients[i].set_active(false);
}
