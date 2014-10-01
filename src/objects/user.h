#ifndef USER_H
#define USER_H
#include "includes.h"
#include "functions.h"
#include "text.h"

const char file_name[] = "User.txt";
class User
{
public:
    User(const std::string & user);
    User(const std::string & user, const std::string & pass);
    std::vector<std::string> vstr() const;
    int find();
    std::string user() const {return user_name;}
    std::string pass() const {return password;}
    int kills() const {return total_kills;}
    int deaths() const {return total_deaths;}
    int bombs_dropped() const {return num_bombs_dropped;}
    double KDR() const;
private:
    std::string user_name;
    std::string password; 
    int total_kills; // keeps track of total number of times killed someone
    int total_deaths; // keeps track of total number of times died
    int num_bombs_dropped; //used for keeping track of total number of bombs dropped
    int row; //row in file (used for quick access)
    
};

#endif
