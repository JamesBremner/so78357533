#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>

class cTime
{
public:
    int myTime;
    int myPriority;
    cTime(int t, int p)
        : myTime(t),
          myPriority(p)
    {
    }
    static int priority(int t);
};

class cPlayer
{
public:
    std::string myName;                     // name
    std::vector<int> myTimes;               // available times
    std::vector<std::string> myOpps;        // names of possible opponents
    int myMaxGames;                         // maximum games that can be played
    int myCountGames;                       // games scheduled

    cPlayer(const std::string name)
        : myName(name),
        myMaxGames( 0 )
    {
    }

    void addTime(int t)
    {
        myTimes.push_back(t);
    }
    
    void addOpp(const std::string &opp)
    {
        myOpps.push_back(opp);
    }

    bool operator==(std::string &name)
    {
        return myName == name;
    }
};

class cCourt
{
public:
    int myTime;
    std::string myName;

    cCourt(const std::string &name, int t)
        : myName(name),
          myTime(t)
    {
    }
};
/**
 * @brief A feasible match between two players on an available court at a particular time
 *
 */
class cGame
{
public:
    cPlayer *p1;
    cPlayer *p2;
    int myt;
    cCourt *myCourt;
    std::string myName;

    cGame();
    cGame(
        cPlayer &a,
        cPlayer &b,
        int t,
        cCourt &c);
    void display();
    static void displayAll();
    static bool find(
        const std::string &p1,
        const std::string &p2);
    static std::string nextName();
    static cGame &get(int index);
    static void sortTimePriority();
};

class cClub
{
public:
    std::vector<cGame> myFeasibleGames;
    std::vector<int> myGames;       /// indices of the scheduled games

    void generate1();
    void generate2();
    void check();
    void maxflow();
    void checkPlayerGames();
    void display();
};

extern std::vector<cPlayer> thePlayers;
extern std::vector<cCourt> theCourts;
extern std::vector<cTime> theTimes;
extern cClub theClub;
