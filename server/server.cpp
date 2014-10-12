// created by: A-squared 
#include "includes.h"
#include "vec2d.h"
#include "functions.h"
#include "network.h"

int main(int argc, char **argv)
{
     // check our commandline
    if (argc < 2)
    {
        std::cout << "Must have port_number"
                  << std::endl;
        exit(0);
    }
    Uint16 port = (Uint16)strtol(argv[1],NULL,0);
    Server s(port);
    TCPsocket sock;
    int client_num = 0;

    while(1)
    {
        // check to see if any socket wants to do something
        int numready;
        s.set_socket_set(s.create_sockset());
        numready = SDLNet_CheckSockets(s.socket_set(), (Uint32)1000);
        if (numready == -1)
        {
            std::cout << "SDLNet_CheckSockets: ERROR" << std::endl;
            break;
        }
        if (numready == 0)
            continue;
        
        if(SDLNet_SocketReady(s.get_socket()))
        {
            numready--;
            sock = SDLNet_TCP_Accept(s.get_socket());
            if(sock)
            {
                std::string name;
                name = s.receive_message(sock);
                s.handle_login(sock, name, client_num);
                ++client_num;
            }
            else
                SDLNet_TCP_Close(sock);
        }

        // Loop through clients -- to write
        for (int i = 0; numready > 0 && i < s.get_num_clients(); i++)
        {
            std::string message = "";
            if (s.get_client(i).get_active())
            {
                message = s.receive_message(s.get_client(i).get_socket());
                if(message > "")
                    std::cout << "message from client: " << message << std::endl;
                numready--;
            }
        }
        
    }
    
    return 0;
}
