
#ifndef AROBOT_H
    #define AROBOT_H

#include "Robot.h"
#include <AR/ar.h>
#include <AR/gsub.h>
#include <AR/param.h>


class ARobot{
    private:
        double _pos[2];
        double _ang;
        int _id;
        int _direccion;
        double _fin[2];
        int _estado;
        Robot *_robot;
        ARMarkerInfo *_markerInfo;
        //float angulo;

    public:
        ARobot(int id);
        int getDir();
        int getId();
        double getAng();
        int getEst();
        void getFin(double f[2]);
        void setEst(int est);
        void setAng(double ang);
        void setFin(double f[2]);
        void setDir(int dir);

        void detener();
        void planifica(ARMarkerInfo *markerInfo,double inicio[2],double fin[2],int accion,double rotacion);

        ARobot &operator= (const ARobot &r);

};
#endif
