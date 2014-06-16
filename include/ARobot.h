
#ifndef AROBOT_H
    #define AROBOT_H

#include "Robot.h"
#include <AR/ar.h>
#include <AR/gsub.h>
#include <AR/param.h>
#include <vector>
#include "Objeto.h"

using namespace std;

class ARobot{
    private:
        double _pos[2];
        double _ang;
        int _id;
        int _direccion;
        double _fin[2];
        int _estado;
        Robot *_robot;
        bool _path;
        vector<char> _lMov;
        ARMarkerInfo *_markerInfo;
        Objeto *_object;
        bool _ob;
        //float angulo;

    public:
        ARobot(int id);
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
        bool hasObj();
        void deleteObj();

        void detener();
        void planifica(ARMarkerInfo *markerInfo,double inicio[2],double fin[2],int accion,double rotacion);

        ARobot &operator= (const ARobot &r);

};
#endif
