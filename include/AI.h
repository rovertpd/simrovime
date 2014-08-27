#ifndef AI_H
    #define AI_H

#include <IceUtil/Thread.h>
#include <boost/lexical_cast.hpp>
#include <boost/python/str.hpp>
#include <vector>

class AI;

#include "ARobot.h"
#include "Search.h"

//using namespace std;

class AI:public IceUtil::Thread{
    public:
    AI(Search *b,ARobot *agente);
    virtual void run();
    Search* _Search;
    ARobot* _agent;
};
#endif
