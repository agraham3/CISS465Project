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
    if (argc < 3)
    {
        std::cout << "Must have localhost or ip, port_number"
                  << std::endl;
        exit(0);
    }
    std::string user_name = "";
    std::cout << "Enter user name: ";
    std::getline(std::cin, user_name);
    SDL_Event event;
    Uint16 port = (Uint16)strtol(argv[2],NULL,0);
    Client c(user_name, argv[1], port);
    int frame = 0;
    SDL_Rect a;
    a.x = 2;
    a.y = 0;
    a.w = 18;
    a.h = 32;
    Screen screen("Testing", 900, 636);
    Bomber player("assets/pic/bomber-ds.png", screen);
    Stage stage(screen);
    std::map< std::string, Bomber > enemy;
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
                            c.send_message("msg:" + c.get_name() + ": " +
                                           input, c.get_socket());
                            break;    
                    }
                    break;
            }
        }

        // send data to server
        c.send_message(player.send_info(c.get_name()), c.get_socket());

        // receive data from server
        c.set_socket_set(c.create_sockset());
        int numready = SDLNet_CheckSockets(c.socket_set(), (Uint32)42);
        if (numready)
        {
            if (SDLNet_SocketReady(c.get_socket()))
            {
                std::string message = c.receive_message(c.get_socket());
                // parse what the server has sent
                std::string command = message.substr(0,3);
                std::string data = message.substr(4);
                if (command == "msg")
                    std::cout << message << std::endl;
                else if (command == "bmr")
                {
                    std::string name = get_name(data);
                    data = data.substr(name.size());
                    enemy[name].set(data);
                }
                else if (command == "new")
                {
                    enemy.insert(std::pair < std::string, Bomber>
                                 (data, Bomber("assets/pic/bomber-ds.png", screen)));
                }
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
        typedef std::map< std::string, TCPsocket >::iterator it_type;
        for (it_type i = clients.begin(); i != clients.end(); i++)
        {
            (i->second).draw(screen);
        }
        screen.update();
        int end = SDL_GetTicks();
        int frame_length = 1000 / FRAMES_PER_SEC;
        int dt = frame_length - (end - start);
        if (dt > 0)
        {
            SDL_Delay(dt);
        }
    }
    /*
    c.send_message("quit", c.get_socket());
    */
    SDL_Quit();
    return 0;
}
