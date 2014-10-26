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
#include "GraphicText.h"

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

restart_game:
    Screen screen("Bomberman", 900, 636);
    SDL_Event event;
    if (TTF_Init() < 0)
    {
        std::cout << "Could not initialize True Type Font! " << SDL_GetError()
                  << std::endl;
        SDL_Quit();
        exit(0);
    }
    
    // main login screen
    Image logo("assets/pic/logo.png", screen);
    SDL_Rect logorect = {0, 0, 900, 313};
    SDL_Rect logopos = {10, 0, 900, 313};
    bool typing_user = true;
    Image textbox("assets/pic/loginBoxes.png", screen);
    SDL_Rect textboxrect = {13, 27, 295, 214};
    SDL_Rect textboxpos = {320, 340, 440, 214};
    SDL_Rect userpos = {320, 340, 440, 99};
    SDL_Rect passpos = {320, 462, 440, 99};
    Image login_register_buttons("assets/pic/LoginorRegister.png", screen);
    SDL_Rect buttonrect = {8, 9, 420, 60};
    SDL_Rect buttonpos = {320, 565, 420, 60};
    SDL_Rect log_butpos = {320, 565, 200, 60};
    SDL_Rect reg_butpos = {540, 565, 200, 60};

    SDL_Rect msgrect = {320, 320, 295, 25};
    Text msg(screen, msgrect, false, WHITE);
retrylog:
    bool run_screen = true;
    bool login = true;
    Text user(screen, userpos);
    Text pass(screen, passpos, true);
    while(run_screen)
    {
        if (typing_user)
        {
            int k = get_typed_char(user.get_message());
            if (k == -1)
            {
                SDL_Quit();
                exit(0);
            }
        }
        else
        {
            int k = get_typed_char(pass.get_message());
            if (k == -1)
            {
                SDL_Quit();
                exit(0);
            }
        }
        
        int x, y;
        if (SDL_GetMouseState(&x, &y) &&
               SDL_BUTTON(SDL_BUTTON_LEFT))
        {
            if(clicked(x,y, userpos))
            {
                typing_user = true;
            }
            else if(clicked(x,y,passpos))
            {
                typing_user = false;
            }
            else if(clicked(x,y,log_butpos))
            {
                login = true;
                run_screen = false;
            }
            else if(clicked(x,y,reg_butpos))
            {
                login = false;
                run_screen = false;
            }
        }
        screen.clear();
        logo.draw(screen, &logorect, &logopos);
        textbox.draw(screen, &textboxrect, &textboxpos);
        user.draw(screen);
        msg.draw(screen);
        pass.draw(screen);
        login_register_buttons.draw(screen, &buttonrect, &buttonpos);
        //std::cout << user.get_message() << std::endl;
        screen.update();
    }
    
    Uint16 port = (Uint16)strtol(argv[2],NULL,0);
    Client c(user.get_message(), pass.get_message(), login, argv[1], port);
    std::string Loginmessage = c.receive_message(c.get_socket());
    std::cout << Loginmessage << std::endl;
    SDL_Delay(1000);
    if (Loginmessage != "Login successful!")
    {
        msg.set_message(Loginmessage);
        goto retrylog;
    }
    
    std::string usertxt = c.receive_message(c.get_socket());
    User user_player = from_string(usertxt);
    
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
    std::vector<int> destroyed;
    Text end_game_msg(screen, END_GAME_RECT);
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
                            if (player.dir() != 2)
                                c.send_message("dir:" + c.get_name() + '|' + to_string(2),
                                               c.get_socket());
                            player.dir(2);
                            break;
                        case DOWN: player.set_animation(0);
                            if (player.dir() != 0)
                                c.send_message("dir:" + c.get_name() + '|' + to_string(0),
                                               c.get_socket());
                            player.dir(0);
                            break;
                        case LEFT: player.set_animation(3);
                            if (player.dir() != 3)
                                c.send_message("dir:" + c.get_name() + '|' + to_string(3),
                                               c.get_socket());
                            player.dir(3);
                            break;
                        case RIGHT: player.set_animation(1);
                            if (player.dir() != 1)
                                c.send_message("dir:" + c.get_name() + '|' + to_string(1),
                                               c.get_socket());
                            player.dir(1);
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
                            if (player.dir() == 2)
                                player.dir(-1);
                            break;
                        case DOWN: player.set_animation(0);
                            if (player.dir() == 0)
                                player.dir(-1);
                            break;
                        case LEFT: player.set_animation(3);
                            if (player.dir() == 3)
                                player.dir(-1);
                            break;
                        case RIGHT: player.set_animation(1);
                            if (player.dir() == 1)
                                player.dir(-1);
                            break;   
                    }
                    c.send_message(player.send_info(c.get_name()), c.get_socket());
            }
        }
        switch(player.dir())
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
        
        for (it_type i = enemy.begin(); i != enemy.end(); i++)
        {
            switch((i->second).dir())
            {
                case 2:
                    (i->second).move_up();
                    (i->second).inc_frame();
                    break;
                case 0:
                    (i->second).move_down();
                    (i->second).inc_frame();
                    break;
                case 3:
                    (i->second).move_left();
                    (i->second).inc_frame();
                    break;
                case 1:
                    (i->second).move_right();
                    (i->second).inc_frame();
                    break;
        }
        }
        
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
                        else if (command == "dir")
                        {
                            std::string name = get_name(data);
                            data = data.substr(name.size() + 1);
                            enemy[name].dir(atoi(data.c_str()));
                        }
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
                        else if(command == "kil")
                        {
                            if (user_player.user() == data)
                                player.inc_kills();
                        }
                        else if(command == "blk")
                        {
                            stage.set_destructibles(data);
                        }
                        else if(command == "dst")
                        {
                            std::vector<int> others = get_ints(data);
                            for (int i = 0; i < others.size(); ++i)
                            {
                                if (!in_array(others[i], destroyed))
                                {
                                    destroyed.push_back(others[i]);
                                }
                            }
                            stage.destroy_destructibles(data);
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
                        else if(command == "win")
                        {
                            if (c.get_name() == data)
                            {
                                end_game_msg.set_message("You Win!");
                                end_game_msg.set_color(GREEN);
                            }
                            else
                            {
                                end_game_msg.set_message("You Lose!");
                                end_game_msg.set_color(RED);
                            }
                            goto status;
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
        std::vector < SDL_Rect > player_bombs = player.get_explosion_rects();
        for (int i = 0; i < player_bombs.size(); i++)
        {
            stage.hit_destructibles(player_bombs[i], destroyed);
        }
        if (destroyed.size() > 0)
        {
            c.send_message("dst:" + to_string(destroyed), c.get_socket());
            destroyed.clear();
        }
        for (it_type i = enemy.begin(); i != enemy.end(); i++)
        {
            int hit = player.any_collisions((i->second).get_actives());
            if (hit != -1)
            {
                player.take_damage((i->second).get_actives()[hit].get_power());
                c.send_message("kil:"+ i->first, c.get_socket());
            }
        }
        
        int coll = stage.collision(player.get_rect());
        if (coll != -1)
        {
            player.reposition(stage.get_blocks(), coll);
        }
        coll = stage.hit_destructible(player.get_rect(), true);
        if (coll != -1)
        {
            player.reposition(stage.get_destructibles(), coll, true);
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

        // check for winner
        bool all_dead = (!enemy.empty());
        for (it_type i = enemy.begin(); i != enemy.end(); i++)
        {
            if((i->second).get_lives() != 0)
            {
                all_dead = false;
                end_game_msg.set_message("You Win!");
                end_game_msg.set_color(GREEN);
                break;
            }
        }
        
        if (all_dead)
        {
            std::cout << "it's a new game!\n" << std::endl;
            c.send_message("new game", c.get_socket());
            goto status;
        }
        
        int end = SDL_GetTicks();
        int frame_length = 1000 / FRAMES_PER_SEC;
        int dt = frame_length - (end - start);
    }

status:
    c.send_message("quit", c.get_socket());
    Text kills_msg(screen, KILLS_RECT);
    Text deaths_msg(screen, DEATHS_RECT);
    Text drops_msg(screen, DROPS_RECT);
    Text kda_msg(screen, KDA_RECT);
    kills_msg.set_message("Kills: " + to_string(player.get_kills()));
    deaths_msg.set_message("Deaths: " + to_string(player.get_deaths()));
    drops_msg.set_message("Drops: " + to_string(player.get_drops()));
    kda_msg.set_message("KD: " + to_string(player.get_kda()));
    kills_msg.set_color(WHITE);
    deaths_msg.set_color(WHITE);
    drops_msg.set_color(WHITE);
    kda_msg.set_color(WHITE);
    Text prompt(screen, PROMPT_RECT);
    prompt.set_message("Hit Enter Key to continue...");
    prompt.set_color(WHITE);
    while(1)
    {
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
                        case ENTER:
                        {
                            SDL_Quit();
                            goto restart_game;
                        }   
                    }
                    break;
            }
        }
        screen.clear();
        end_game_msg.draw(screen);
        kills_msg.draw(screen);
        deaths_msg.draw(screen);
        drops_msg.draw(screen);
        kda_msg.draw(screen);
        prompt.draw(screen);
        screen.update();
    }
    
    SDL_Quit();
    goto restart_game;
    return 0;
}
