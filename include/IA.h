#ifndef IA_H
    #define IA_H

#include <IceUtil/Thread.h>
#include <boost/lexical_cast.hpp>
#include <boost/python/str.hpp>
#include <vector>

class IA;

#include "ARobot.h"
#include "estado.h"

//using namespace std;

class IA:public IceUtil::Thread{
    public:
    IA(Estado *e,ARobot *agente);
    virtual void run();
    Estado* _state;
    ARobot* _agent;
};
#endif
