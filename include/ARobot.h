
#ifndef AROBOT_H
    #define AROBOT_H

#include <AR/ar.h>
#include <AR/gsub.h>
#include <AR/param.h>
#include <vector>

class ARobot;

#include "Robot.h"
#include "Objeto.h"
#include "Scene.h"
#include "IA.h"
#include "busqueda.h"

using namespace std;

class ARobot{
    private:
        double _pos[2];
        double _ang;
        int _id;
        int _direccion;
        double _fin[2];
        int fin0,fin1;
        int _estado;
        int _guardia;
        Robot *_robot;
        bool _path;
        vector<char> _lMov;
        ARMarkerInfo *_markerInfo;
        Objeto *_object;
        bool _ob;
        void setPos(double p[2]);
        Scene* _scn;
        vector<IceUtil::ThreadControl> threads;
        //float angulo;

    public:
        ARobot(int id);
        void init();
        int getDir();
        int getId();
        double getAng();
        int getEst();
        void getFin(double f[2]);
        bool getPath();
        void addMov(char mov);
        void setPath(bool path);
        void setEst(int est);
        void setAng(double ang);
        void setFin(double f[2]);
        void setDir(int dir);
        void setObj(Objeto *obj);
        Objeto* getObj();
        void reset();
        void det();
        bool hasObj();
        void deleteObj();

        void detener();
        void planifica(Scene *scn,int event);

        bool casillaValida(Scene *scn);
        void orientar(Scene *scn);
        void nextMov();

        ARobot &operator= (const ARobot &r);

};
#endif
