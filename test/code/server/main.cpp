#include <iostream>
#include "SDL_net.h"

#include "Includes.h"
#include "Constants.h"
#include "compgeom.h"
#include "Surface.h"
#include "Event.h"

int main()
{
// create a listening TCP socket on port 8080 (server)
    IPaddress ip;
    TCPsocket server_tcpsock;
    if(SDLNet_ResolveHost(&ip,NULL,8080)==-1) {
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(1);
    }
    server_tcpsock=SDLNet_TCP_Open(&ip);
    if(!server_tcpsock) {
        printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(2);
    }

    // accept a connection coming in on server_tcpsock
    TCPsocket new_tcpsock;
    while (1)
    {
        new_tcpsock=SDLNet_TCP_Accept(server_tcpsock);
        if(!new_tcpsock) {
            printf("SDLNet_TCP_Accept: %s\n", SDLNet_GetError());
        }
        else {
            // communicate over new_tcpsock
            std::cout << "found" << std::endl;
            break;
        }
    }
    
// close the connection on sock
//TCPsocket sock;
    SDLNet_TCP_Close(server_tcpsock);

    return 0;
}
