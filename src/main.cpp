// created by: A-squared 
#include "includes.h"
#include "vec2d.h"
#include "text.h"
#include "functions.h"
#include "user.h"
#include "network.h"

#include "Bomber.h"

//SDL Classes packaged for SDL
#include "Screen.h"
#include "Image.h"

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
    // Server s(port);
    // Client c(NULL, argv[3], argv[1], port);
    Screen test("Testing", 640, 480);
    test.clear();
    SDL_Rect a;
    a.x = 0;
    a.y = 0;
    a.w = 20;
    a.h = 32;
    Bomber player("/home/student/Documents/classes/ciss465/CISS465Project/assets/pic/bomber-ds.png", test, &a);
    player.draw(test);
    test.update();
    SDL_Delay(3000);
    
    return 0;
}
