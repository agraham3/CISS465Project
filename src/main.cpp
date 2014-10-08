// created by: A-squared 
#include "includes.h"
#include "vec2d.h"
#include "text.h"
#include "functions.h"
#include "user.h"
#include "network.h"

//SDL Classes packaged for SDL
#include "Surface.h"

int main(int argc, char **argv)
{
    Uint16 port;
    // check our commandline
    if (argc < 4)
    {
        std::cerr << "Must have localhost port_number user_name" << std::endl;
        exit(0);
    }
    // get port form the commandline
    port = (Uint16)strtol(argv[1],NULL,0);

    Surface test(640, 480, "Testing");
    return 0;
}
