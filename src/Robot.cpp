#include "Robot.h"

Robot::Robot(int id){
    _pos[0] = 0.0;
    _pos[1] = 0.0;
    _rot = 0.0;
    puerto = 0;
    _id = id;
    fd = 0;
    direccion = 0;
    open_port();
    printf("open port");
}

void Robot::open_port()
{
   char ttyport[33];
   if (_id == 2){
       sprintf(ttyport, "/dev/ttyUSB2");
   }else if (_id == 1){
       sprintf(ttyport, "/dev/ttyUSB1");
   }else{
       sprintf(ttyport, "/dev/ttyUSB0");
   }
   fd = open(ttyport, O_RDWR | O_NDELAY);
   if (fd != -1)
   {
	  printf("\nPuerto %s ok!", ttyport );
   }

   if (fd == -1)
   {
       printf("\nttyUSB port error!\n\n");
   }
   else
   {
     struct termios options;

     // Get the current options for the port...
     tcgetattr(fd, &options);

     // Set the baud rates to B57600...
     cfsetispeed(&options, B57600);

     // Enable the receiver and set local mode...
     options.c_cflag |= (CLOCAL | CREAD);

     // Set the new options for the port...
     tcsetattr(fd, TCSANOW, &options);

     options.c_cflag &= ~CSIZE;   /* Mask the character size bits */
     options.c_cflag |= CS8;      /* Select 8 data bits */
     options.c_cflag &= ~PARENB;
     options.c_cflag &= ~CSTOPB;
     options.c_cflag &= ~CSIZE;
     options.c_cflag |= CS8;

     //~ fcntl(fd, F_SETFL, 0);
   }

}



void Robot::enviar_hacia_arduino(int puerto)
{
    write(puerto, codigo_hacia_arduino, strlen(codigo_hacia_arduino));
    //printf("\nHacia Arduino: %s", codigo_hacia_arduino);
}


int Robot::recibir_desde_arduino(int puerto)
{
   int n;
   n = 0;
   strcpy(codigo_desde_arduino, "");
   for(;;)
   {
       strcpy(dato, "");
       read(puerto,dato,1);
       if(dato[0] ==  10)
       {
          codigo_desde_arduino[n] = '\0';
          break;
       }
       else if(dato[0] == 13);
       else if(dato[0] > 0 && n < 98)
          codigo_desde_arduino[n++] = dato[0];
   }
   return strlen(codigo_desde_arduino);
}

void Robot::avanzar(){
    strcpy(codigo_hacia_arduino,"w");
    puerto=fd;
    enviar_hacia_arduino(puerto);
    std::cout<<"AVANZAR"<<std::endl;
}
void Robot::retroceder(){
    strcpy(codigo_hacia_arduino,"s");
    puerto=fd;
    enviar_hacia_arduino(puerto);
    std::cout<<"RETROCEDER"<<std::endl;
}
void Robot::izquierda(){
    strcpy(codigo_hacia_arduino,"a");
    puerto=fd;
    enviar_hacia_arduino(puerto);
    std::cout<<"IZQUIERDA"<<std::endl;
}
void Robot::derecha(){
    strcpy(codigo_hacia_arduino,"d");
    puerto=fd;
    enviar_hacia_arduino(puerto);
    std::cout<<"DERECHA"<<std::endl;
}
void Robot::parar(){
    strcpy(codigo_hacia_arduino,"x");
    puerto=fd;
    enviar_hacia_arduino(puerto);
    std::cout<<"PARAR"<<std::endl;
}

int Robot::getDir(){
    return direccion;
}

void Robot::setDir(int dir){
    direccion = dir;
}

void Robot::getFin(double f[2]){
    f[0] = _fin[0];
    f[1] = _fin[1];
}

double Robot::getRot(){
    return _rot;
}

int Robot::getEst(){
    return _estado;
}

void Robot::setEst(int est){
    _estado = est;
}

void Robot::setRot(double rot){
    _rot = rot;
}

void Robot::setFin(double f[2]){
    _fin[0] = f[0];
    _fin[1] = f[1];
}

Robot& Robot::operator= (const Robot &r){
    this->_fin[0] = r._fin[0];
    this->_fin[1] = r._fin[1];
    this->_pos[0] = r._pos[0];
    this->_pos[1] = r._pos[1];
    this->_rot = r._rot;
    this->puerto = r.puerto;
    this->_id = r._id;
    this->fd = r.fd;
    this->direccion = r.direccion;
    return *this;
}
