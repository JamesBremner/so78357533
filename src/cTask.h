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
    std::string myName;
    std::vector<int> myTimes;
    std::vector<std::string> myOpps;

    cPlayer(const std::string name)
        : myName(name)
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
 * @brief A feasible match between two players on an available cort at a particular time
 *
 */
class cTask
{
public:
    cPlayer *p1;
    cPlayer *p2;
    int myt;
    cCourt *myCourt;
    std::string myName;
    cTask()
    {
    }
    cTask(
        cPlayer &a,
        cPlayer &b,
        int t,
        cCourt &c)
        : p1(&a),
          p2(&b),
          myt(t),
          myCourt(&c)
    {
        myName = nextName();
    }
    void display();
    static void displayAll();
    static bool find(
        const std::string &p1,
        const std::string &p2);
    static std::string nextName();
    static cTask &get(int index);
    static void sortTimePriority();
};

extern std::vector<cPlayer> thePlayers;
extern std::vector<cCourt> theCourts;
extern std::vector<cTask> theTasks;
extern std::vector<cTime> theTimes;

void generate2();
void check();
void maxflow();
