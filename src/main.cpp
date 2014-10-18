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

typedef std::map< std::string, Bomber >::iterator it_type;
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

    Screen screen("Bomberman", 900, 636);
    SDL_Event event;
    
    // main login screen
    bool run_screen = true;
    Image logo("assets/pic/logo.png", screen);
    SDL_Rect logorect = {0, 0, 900, 313};
    SDL_Rect logopos = {10, 0, 900, 313};
    while(run_screen)
    {
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
                        case SPACE:
                            run_screen = false;
                            break; 
                    }
                    break;
            }
        }
        
        screen.clear();
        logo.draw(screen, &logorect, &logopos);
        screen.update();
    }
    
    std::string user_name = "";
    std::cout << "Enter user name: ";
    std::getline(std::cin, user_name);
    Uint16 port = (Uint16)strtol(argv[2],NULL,0);
    Client c(user_name, argv[1], port);
    int frame = 0;
    SDL_Rect a;
    a.x = 2;
    a.y = 0;
    a.w = 18;
    a.h = 32;
    std::string player_image = "assets/pic/bomber-ds.png";
    std::string bomb_image = "assets/pic/bombStages.png";
    std::string exp_image = "assets/pic/expStages.png";
    std::string green_arrow = "assets/pic/pointer-green.png";
    std::string red_arrow = "assets/pic/pointer-red.png";
    Bomber player(player_image, bomb_image, exp_image, green_arrow, screen);
    Stage stage(screen);
    std::map< std::string, Bomber > enemy;
    //move Bomber-Man
    int dir = -1;
    while (1)
    {
        int start = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    c.send_message("quit", c.get_socket());
                    SDL_Quit();
                    exit(0);
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                        case UP: player.set_animation(2);
                            dir = 2;
                            break;
                        case DOWN: player.set_animation(0);
                            dir = 0;
                            break;
                        case LEFT: player.set_animation(3);
                            dir = 3;
                            break;
                        case RIGHT: player.set_animation(1);
                            dir = 1;
                            break;
                        case ENTER:
                        {
                            std::string input;
                            std::cout << "Enter message: ";
                            std::getline(std::cin, input);
                            c.send_message("msg:" + c.get_name() + ": " +
                                           input, c.get_socket());
                        }
                            break;
                        case SPACE: player.drop_bomb();
                            break;
                            
                    }
                    break;
                case SDL_KEYUP:
                    switch(event.key.keysym.sym)
                    {
                        case UP: player.set_animation(2);
                            if (dir == 2)
                                dir = -1;
                            break;
                        case DOWN: player.set_animation(0);
                            if (dir == 0)
                                dir = -1;
                            break;
                        case LEFT: player.set_animation(3);
                            if (dir == 3)
                                dir = -1;
                            break;
                        case RIGHT: player.set_animation(1);
                            if (dir == 1)
                                dir = -1;
                            break;   
                    }
            }
        }
        switch(dir)
        {
            case 2:
                player.move_up();
                player.inc_frame();
                break;
            case 0:
                player.move_down();
                player.inc_frame();
                break;
            case 3:
                player.move_left();
                player.inc_frame();
                break;
            case 1:
                player.move_right();
                player.inc_frame();
                break;
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
                if (message.size() >= 3)
                {
                    // parse what the server has sent
                    try
                    {
                        std::string command = message.substr(0,3);
                        std::string data = message.substr(4);
                        if (command == "msg")
                            std::cout << data << std::endl;
                        else if (command == "bmr")
                        {
                            std::string name = get_name(data);
                            data = data.substr(name.size() + 1);
                            int cnt = enemy.count(name);
                            if (cnt > 0)
                            {
                                enemy[name].set(data);
                            }
                            else
                            {
                                enemy.insert(std::pair < std::string, Bomber>
                                             (name, Bomber(player_image, bomb_image,
                                                           exp_image, red_arrow, screen)));
                            }
                        }
                        else if (command == "new")
                        {
                            enemy.insert(std::pair < std::string, Bomber>
                                         (data, Bomber(player_image, bomb_image,
                                                       exp_image, red_arrow, screen)));
                        }
                        else if (command == "rmv")
                        {
                            std::map < std::string, Bomber >::iterator it;
                            it = enemy.find(data);
                            enemy.erase(it);
                        }
                    }
                    catch (const std::out_of_range &oor)
                    {
                        std::cerr << "Out of range error: "
                                  << oor.what()
                                  << '\n';
                        c.send_message("quit", c.get_socket());
                        continue;
                    }
                }
            }
        }
        int hits = player.any_collisions(player.get_actives());

        if (hits != -1)
        {
            player.take_damage(player.get_actives()[hits].get_power());
        }
        
        for (it_type i = enemy.begin(); i != enemy.end(); i++)
        {
            int hit = player.any_collisions((i->second).get_actives());
            if (hit != -1)
            {
                player.take_damage((i->second).get_actives()[hit].get_power());
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
        for (it_type i = enemy.begin(); i != enemy.end(); i++)
        {
            (i->second).fix_bombs();
            if ((i->second).draw(screen, 2) != 0)
            {
                (i->second).new_image(player_image, bomb_image,
                                      exp_image, red_arrow, screen);
                (i->second).draw(screen, 2);
            }
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
    c.send_message("quit", c.get_socket());
    
    SDL_Quit();
    return 0;
}
