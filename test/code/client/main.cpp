#include <iostream>
#include "SDL_net.h"

#include "Includes.h"
#include "Constants.h"
#include "compgeom.h"
#include "Surface.h"
#include "Event.h"

int main()
{
// create a listening TCP socket on port 9999 (client)
    IPaddress ip;
    TCPsocket client_tcpsock;
    if(SDLNet_ResolveHost(&ip,"local_host",9999)==-1) {
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(1);
    }
    client_tcpsock=SDLNet_TCP_Open(&ip);
    if(!client_tcpsock) {
        printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(2);
    }
    
// close the connection on sock
//TCPsocket sock;
    SDLNet_TCP_Close(client_tcpsock);

    return 0;
}
