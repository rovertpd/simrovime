#ifndef ROBOT_H
    #define ROBOT_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <signal.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>


class Robot{
    private:
        double _pos[2];
        double _rot;
        char codigo_hacia_arduino[99];
        char codigo_desde_arduino[99];
        char dato[2];
        int fd;   /* File descriptor for the port */
        int _id;
        int puerto;
        int direccion;
        double _fin[2];
        int _estado;
        //float angulo;
        void open_port();
        void enviar_hacia_arduino(int puerto);
        int recibir_desde_arduino(int puerto);

    public:
        Robot(int id);
        void avanzar();
        void retroceder();
        void izquierda();
        void derecha();
        void parar();
        int getDir();
        void getFin(double f[2]);
        double getRot();
        int getEst();
        void setEst(int est);
        void setRot(double rot);
        void setFin(double f[2]);
        void setDir(int dir);
        Robot &operator= (const Robot &r);

};
#endif
