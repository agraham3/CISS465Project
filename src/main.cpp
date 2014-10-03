// created by: A-squared 
#include "includes.h"
#include "vec2d.h"
#include "text.h"
#include "functions.h"
#include "user.h"
#include "network.h"

int main(int argc, char **argv)
{
    Uint16 port;
    // check our commandline
    if (argc < 2)
    {
        std::cout << "Please insert a port number" << std::endl;
        exit(0);
    }
    // get port form the commandline
    port = (Uint16)strtol(argv[1],NULL,0);
    return 0;
}
