#ifndef IA_H
    #define IA_H

#include <IceUtil/Thread.h>
#include <boost/lexical_cast.hpp>
#include <boost/python/str.hpp>
#include <vector>

class IA;

#include "ARobot.h"
#include "busqueda.h"

//using namespace std;

class IA:public IceUtil::Thread{
    public:
    IA(Busqueda *b,ARobot *agente);
    virtual void run();
    Busqueda* _busqueda;
    ARobot* _agent;
};
#endif
