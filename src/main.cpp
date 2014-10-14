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
#include "constants.h"
#include "Stage.h"

int main(int argc, char **argv)
{
    srand((unsigned int) time(NULL));
    // check our commandline
    if (argc < 4)
    {
        std::cout << "Must have localhost port_number user_name"
                  << std::endl;
        exit(0);
    }
    SDL_Event event;
    Uint16 port = (Uint16)strtol(argv[2],NULL,0);
    Client c(NULL, argv[3], argv[1], port);
    int frame = 0;
    SDL_Rect a;
    a.x = 2;
    a.y = 0;
    a.w = 18;
    a.h = 32;
    Screen screen("Testing", 900, 636);
    Bomber player("assets/pic/bomber-ds.png", screen);
    Stage stage(screen);
    //move Bomber-Man
    while (1)
    {
        int start = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    SDL_Quit();
                    exit(0);
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                        case UP: player.set_animation(2);
                            player.move_up();
                            player.inc_frame();
                            break;
                        case DOWN: player.set_animation(0);
                            player.move_down();
                            player.inc_frame();
                            break;
                        case LEFT: player.set_animation(3);
                            player.move_left();
                            player.inc_frame();
                            break;
                        case RIGHT: player.set_animation(1);
                            player.move_right();
                            player.inc_frame();
                            break;
                        case ENTER: std::string input;
                            std::cout << "Enter message: ";
                            std::getline(std::cin, input);
                            c.send_message(input, c.get_socket());
                            break;
                    }
                    //case SDL_KEYUP:
                    //std::cout << "Hello" << std::endl;
                    //player.reset_frame();
            }
        }
        int coll = stage.collision(player.get_rect());
        if (coll != -1)
        {
            player.reposition(stage, coll);
        }
        player.update();
        screen.clear();
        stage.draw(screen);
        player.draw(screen);
        screen.update();

        int end = SDL_GetTicks();
        int frame_length = 1000 / FRAMES_PER_SEC;
        int dt = frame_length - (end - start);
        if (dt > 0)
        {
            SDL_Delay(dt);
        }
    }
    SDL_Quit();
    return 0;
}
