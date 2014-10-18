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
        numready = SDLNet_CheckSockets(s.socket_set(), (Uint32)80);
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
                std::string message;
                message = s.receive_message(sock);
                std::vector< std::string > temp= get_parts(message);
                std::string name = temp[0];
                std::string pass = temp[1];
                bool login = (temp[2] == "1");
                if (login)
                    s.handle_login(sock, name, pass);
                else
                    s.handle_register(sock, name, pass);
                s.send_message_to_all_other_clients(name,
                                                    "msg:New client connection: "
                                                    + name);
                s.send_message_to_all_other_clients(name, "new:" + name);
                ++client_num;
            }
            else
                SDLNet_TCP_Close(sock);
        }

        // Loop through clients -- to write
        typedef std::map< std::string, TCPsocket >::iterator it_type;
        std::map< std::string, TCPsocket > m = s.get_clients();
        for (it_type i = m.begin(); i != m.end(); i++)
        {
            if (!SDLNet_SocketReady(i->second))
                continue;
            std::string message = "";
            message = s.receive_message(i->second);
            if (message == "quit")
            {
                s.handle_disconnect(i->first);
            }
            std::string temp = message;
            temp.erase(remove_if(temp.begin(), temp.end(), isspace), temp.end());
            if (temp == "" || message =="")
            {
                s.handle_disconnect(i->first);
                continue;
            }
            s.send_message_to_all_other_clients(i->first, message);
            numready--;
        }
        
    }
    
    return 0;
}
