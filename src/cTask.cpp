#include <GraphTheory.h>

#include "cTask.h"

std::vector<cPlayer> thePlayers;
std::vector<cCourt> theCourts;
std::vector<cTime> theTimes;
cClub theClub;

int cTime::priority(int t)
{
    for (auto &tc : theTimes)
    {
        if (tc.myTime == t)
            return tc.myPriority;
    }
    return 0;
}

     cGame::cGame()
    : myt( -1 )
    {
    }
     cGame::cGame(
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

void cGame::displayAll()
{
    for (auto &t : theClub.myFeasibleGames)
    {
        t.display();
    }
}

bool cGame::find(const std::string &p1n, const std::string &p2n)
{
    for (auto &t : theClub.myFeasibleGames)
        if (t.p1->myName == p1n && t.p2->myName == p2n)
            return true;
    return false;
}
std::string cGame::nextName()
{
    return "task" + std::to_string(theClub.myFeasibleGames.size());
}
void cGame::display()
{
    std::cout << p1->myName
              << " v " << p2->myName
              << " at " << myt
              << " on " << myCourt->myName
              << "\n";
}
void cGame::sortTimePriority()
{
    std::sort(
        theClub.myFeasibleGames.begin(), theClub.myFeasibleGames.end(),
        [](const cGame &a, cGame &b)
        {
            int ap = cTime::priority(a.myt);
            int bp = cTime::priority(b.myt);
            return ap > bp;
        });
}

void cClub::generate1()
{
    // generate players
    thePlayers.emplace_back("pA");
    thePlayers.emplace_back("pB");

    // add possible opponents
    thePlayers[0].addOpp("pB");
    thePlayers[1].addOpp("pA");

    // add available times
    thePlayers[0].addTime(1);
    thePlayers[0].addTime(3);
    thePlayers[0].addTime(2);
    thePlayers[1].addTime(2);
    thePlayers[1].addTime(3);

    // generate court times
    theCourts.emplace_back("cA", 1);
    theCourts.emplace_back("cA", 2);
    theCourts.emplace_back("cA", 3);
}
void cClub::generate2()
{
    // generate players
    thePlayers.emplace_back("pA");
    thePlayers.emplace_back("pB");

    // add possible opponents
    thePlayers[0].addOpp("pB");
    thePlayers[1].addOpp("pA");

    // add available times
    thePlayers[0].addTime(1);
    thePlayers[0].addTime(2);
    thePlayers[1].addTime(1);
    thePlayers[1].addTime(2);

    // generate court times
    theCourts.emplace_back("cA", 1);
    theCourts.emplace_back("cA", 2);

    // set time priority
    theTimes.emplace_back(2, 1);
}
int findPlayer(const std::string &name)
{
    for (int ip = 0; ip < thePlayers.size(); ip++)
        if (thePlayers[ip].myName == name)
            return ip;
    return -1;
}

/**
 * @brief Check for feasibility
 *
 * i.e. every player has at least one opponent with a matching available time and possible court time
 */
void cClub::check()
{
    // loop over players
    int pi = -1;
    for (auto &p : thePlayers)
    {
        pi++;

        // assume that player has no possible opponent with matching time
        bool ok = false;

        // loop over player available times
        for (int t : p.myTimes)
        {
            // loop over player possible opponents
            for (auto &os : p.myOpps)
            {
                // loop over opponents available times
                int oi = findPlayer(os);
                for (int ot : thePlayers[oi].myTimes)
                {
                    if (t == ot)
                    {
                        // loop over court times
                        for (auto &c : theCourts)
                        {
                            if (t == c.myTime)
                            {
                                // found a match with opponent and court

                                ok = true;

                                // create a task for each feasible pair
                                // only record task when player 1 index less than player 2
                                if (pi < oi)
                                {
                                    myFeasibleGames.emplace_back(p, thePlayers[oi], t, c);
                                }
                            }
                        }
                    }
                }
            }
        }
        if (!ok)
        {
            std::cout << "Cannot find opponent for " << p.myName << "\n";
            exit(1);
        }
    }
    std::cout << "found feasible opponents for all players\n";
    cGame::displayAll();
}
void cClub::maxflow()
{
    myGames.clear();

    cGame::sortTimePriority();

    // setup the flow graph
    raven::graph::sGraphData gd;
    gd.g.directed();
    for (auto &c : theCourts)
        for (auto &t : myFeasibleGames)
            if (t.myt == c.myTime)
            {
                // Game can be played on this court
                gd.g.add(c.myName, t.myName);
            }

    // apply the maxflow algorithm, allocating games to courts
    auto ga = raven::graph::alloc(gd);

    for (int ei = 0; ei < ga.edgeCount(); ei++)
        myGames.push_back(atoi(gd.g.userName(gd.g.dest(ei))
                                   .substr(4)
                                   .c_str()));
}

void cClub::checkPlayerGames()
{
    for (auto &p : thePlayers)
        p.myCountGames = 0;
    for (int gi : myGames)
    {
        auto &g = myFeasibleGames[gi];
        if (g.p1->myCountGames == g.p1->myMaxGames ||
            g.p2->myCountGames == g.p2->myMaxGames)
            // one or both players are at their maximum games per week
            // unschedule this game
            g.myt = -1;
        else
        {
            g.p1->myCountGames++;
            g.p2->myCountGames++;
        }
    }
}

void cClub::display()
{
    std::cout << "\nGame Schedule\n";
    for (int gi : myGames)
        if( myFeasibleGames[gi].myt>0 )
            myFeasibleGames[gi].display();
}