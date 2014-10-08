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
     // check our commandline
    if (argc < 4)
    {
        std::cout << "Must have localhost port_number user_name"
                  << std::endl;
        exit(0);
    }
    Uint16 port = (Uint16)strtol(argv[2],NULL,0);
    // Server(port);
    // Client(NULL, argv[3], argv[1], port);
    Surface test("Testing", 640, 480);
    test.clear();
    test.update();
    SDL_Delay(3000);
    
    return 0;
}
