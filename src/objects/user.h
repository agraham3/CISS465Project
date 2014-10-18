#ifndef USER_H
#define USER_H
#include "includes.h"
#include "functions.h"
#include "text.h"

const char file_name[] = "User.txt";
class USER_ERROR
{
public:
    std::string message;
};
class User
{
public:
    User(const std::string & user);
    User(const std::string & user, const std::string & pass, bool login);
    std::vector<std::string> vstr() const;
    int find();
    std::string user() const {return user_name;}
    std::string pass() const {return password;}
    int kills() const {return total_kills;}
    int deaths() const {return total_deaths;}
    int bombs_dropped() const {return num_bombs_dropped;}
    double KDR() const;
    static void update_file();
    void set_kills(int k) {total_kills = k;}
    void set_deaths(int d) {total_deaths = d;}
    void set_dropped(int d) {num_bombs_dropped = n;}
    void inc_kills(int a) {total_kills += a;}
    void inc_deaths(int a) {total_deaths += a;}
    void inc_dropped(int a) {num_bombs_dropped += a;}
private:
    std::string user_name;
    std::string password; 
    int total_kills; // keeps track of total number of times killed someone
    int total_deaths; // keeps track of total number of times died
    int num_bombs_dropped; //used for keeping track of total number of bombs dropped
    int row; //row in file (used for quick access)
    static std::vector<std::vector<std::string> > all_info;
    
};

std::string to_string(const User & u);
User from_string(std::string & s);

#endif
