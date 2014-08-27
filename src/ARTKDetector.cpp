#include "ARTKDetector.h"

ARTKDetector::ARTKDetector(int w, int h, int thres, Scene *scn){
  _markerNum=0;   _markerInfo=NULL;  _thres = thres;
  _width = w;     _height = h;       _detected = false;
  _nObjects = 5;  l=-1;
  readConfiguration();  _scene = scn;
  //readPatterns(/*nObjects,objects*/);
}
ARTKDetector::~ARTKDetector(){  argCleanup();  }
// ================================================================
// readPatterns: Carga la definicion de patrones
int ARTKDetector::readPatterns(/*int *nObjects, Marca objects[]*/){
//    double p_center[2] = {0.0, 0.0};
//    char p_patt[]="data/identic.patt";
//    _objects[_nObjects]=Marca(120.0,p_center,p_patt);
//    _objects[_nObjects].setVisible(false);
//    if((_objects[_nObjects].getId()) < 0) return -1;
//    _nObjects++;
//    strcpy(p_patt,"data/4x4_95.patt");
//    _objects[_nObjects]=Marca(120.0,p_center,p_patt);
//    _objects[_nObjects].setVisible(false);
//    if((_objects[_nObjects].getId()) < 0) return -1;
//    _nObjects++;
//    strcpy(p_patt,"data/4x4_3.patt");
//    _objects[_nObjects]=Marca(120.0,p_center,p_patt);
//    _objects[_nObjects].setVisible(false);
//    if((_objects[_nObjects].getId()) < 0) return -1;
//    _nObjects++;
//    strcpy(p_patt,"data/simple.patt");
//    _objects[_nObjects]=Marca(120.0,p_center,p_patt);
//    _objects[_nObjects].setVisible(false);
//    if((_objects[_nObjects].getId()) < 0) return -1;
//    _nObjects++;
  return 0;
}
// ================================================================
// readConfiguration: Carga archivos de configuracion de camara...
int ARTKDetector::readConfiguration(){
  ARParam  wparam, cparam;
  // Cargamos los parametros intrinsecos de la camara
  if(arParamLoad("data/camera_para.dat",1, &wparam) < 0) return -1;

  arParamChangeSize(&wparam, _width, _height, &cparam);
  arInitCparam(&cparam);   // Inicializamos la camara con "cparam"
  return 0;
}
// ================================================================
// detectMark (FIXME): Ojo solo soporta una marca de tamano fijo!
bool ARTKDetector::detectMark(cv::Mat* frame) {
  //double p_width     = 120.0;        // Ancho de marca... FIJO!
  //double p_center[2] = {0.0, 0.0};   // Centro de marca.. FIJO!
  int i, j, k;
  //printf("Nobjets,%d",_nObjects);
  double pattTrans[3][4];
  int actualizar = -1;
  int id = -1;
  _detected = false;
  vector<Marca*> m = _scene->getMarcas();
  Marca* marcas[5];
  for (int i=0; i<5; i++){
    marcas[i] = m[i];
  }
  if(frame->rows==0) return _detected;
  if(arDetectMarker(frame->data, _thres,
                    &_markerInfo, &_markerNum) < 0){
    return _detected;
  }

   for (i=0; i<_nObjects; i++) {
    actualizar = -1;
    //_marca = _scene->getMarca(i);
    _marca = marcas[i];
    for(j = 0, k = -1; j < _markerNum; j++) {
      if(_marca->getId() == _markerInfo[j].id) {
        if (k == -1) k = j;
        else if(_markerInfo[k].cf < _markerInfo[j].cf) k = j;
      }
    }

    if(k != -1) {   // Si ha detectado el patron en algun sitio...
      _marca->getPattTans(pattTrans);
      arGetTransMatCont(&_markerInfo[k], pattTrans, _marca->getCenter(), _marca->getWidth(), pattTrans);
      double posicion[2] = {_markerInfo[k].pos[0],_markerInfo[k].pos[1]};
      _marca->setPattTans(pattTrans);
      _marca->setMarkerInfo(_markerInfo[k]);
      int pm[2] = {100000,100000};
      int pM[2] = {-1,-1};
      for (int x=0;x<4;x++){
          if (_markerInfo[k].vertex[x][0]>pM[0])
            pM[0]=_markerInfo[k].vertex[x][0];
          if (_markerInfo[k].vertex[x][1]>pM[1])
            pM[1]=_markerInfo[k].vertex[x][1];
          if (_markerInfo[k].vertex[x][0]<pm[0])
            pm[0]=_markerInfo[k].vertex[x][0];
          if (_markerInfo[k].vertex[x][1]<pm[1])
            pm[1]=_markerInfo[k].vertex[x][1];
      }
//      int pm[2] = {_markerInfo[k].vertex[2][0],_markerInfo[k].vertex[3][1]};
//      int pM[2] = {_markerInfo[k].vertex[0][0],_markerInfo[k].vertex[1][1]};
      _marca->setMax(pM);
      _marca->setMin(pm);
      if(_marca->getVisible()){
//          double x = _marca->getPos()[0];
//          double y = _marca->getPos()[1];
//          int grid = _scene->getGrid();
//          if ((static_cast<int>(x/grid) != static_cast<int>(posicion[0]/grid))||(static_cast<int>(y/grid) != static_cast<int>(posicion[1]/grid))){
//              if (i>1)
//                _scene->modificarMarca(_marca);
//          }else if(abs(getRotation(_marca)-_marca->getRot()) > 10){
              if (i>1)
                _scene->modificarMarca(_marca);
//          }
          _marca->setPos(posicion);
          _marca->setRot(getRotation(_marca));
          actualizar = 2;
          id = i;

      }else {
          if ((i<2) || (i>1 && _scene->getFin()[0]!=0.0)){
              _marca->setVisible(true);
              _marca->setPos(posicion);
              _marca->setRot(getRotation(_marca));
              if(i>1)
                _scene->crearMarca(_marca);
              actualizar = 1;
              id = i;
          }
      }
      _detected = true;
    } else {
      if (_marca->getVisible()){
          if ((i<2) || (i>1 && _scene->getFin()[0]!=0.0)){
            _marca->setVisible(false);
            if (i>1)
                _scene->borrarMarca(_marca);
            actualizar = 3;
            id = i;
          }
      }
    }  // El objeto no es visible
//  } // Fin del for
  if (actualizar!=-1 && ((i==0 && _scene->getCenter()[0]==0.0) || (i==1 && _scene->getFin()[0]==0.0 && _scene->getCenter()[0]!=0.0) || (i>1 && _scene->getFin()[0]!=0.0))){
    _scene->Actualizar(actualizar,id);
  }
}
  return _detected;
}
// ================================================================
// Gl2Mat: Utilidad para convertir entre matriz OpenGL y Matrix4
void ARTKDetector::Gl2Mat(double *gl, Ogre::Matrix4 &mat){
  for(int i=0;i<4;i++) for(int j=0;j<4;j++) mat[i][j]=gl[i*4+j];
}
float ARTKDetector::getRotation(Marca *m){
    Ogre::Vector3 pos;  Ogre::Vector3 look;   Ogre::Vector3 up;
    getPosRot(pos, look, up, m);
    float mod = atan((-up[0])/up[2]) - PI;
    if (up[2]<0) mod = mod + PI;
    else if ((-up[0])<0) mod = mod + 2 * PI;
    if (mod<0) mod = mod + 2 * PI;
    return mod * 180 / PI;
}
// ================================================================
// getPosRot: Obtiene posicion y rotacion para la camara
void ARTKDetector::getPosRot(Ogre::Vector3 &pos,
                   Ogre::Vector3 &look, Ogre::Vector3 &up,Marca* marca){
   //std::cout<<"Marca PosRot "<<marca->getId()<<std::endl;
   if (!_detected) return;
  marca->getPattTans(_pattTrans);

  double glAuxd[16]; Ogre::Matrix4 m;
  argConvGlpara(_pattTrans,glAuxd);
  Gl2Mat(glAuxd, m);   // Convertir a Matrix4 de Ogre

  m[0][1]*=-1; m[1][1]*=-1;  m[2][1]*=-1; m[3][1]*=1;
  m = m.inverse();
  m = m.concatenate(Ogre::Matrix4(Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::UNIT_X)));

  pos  = Ogre::Vector3 (m[3][0],         m[3][1],         m[3][2]);
  look = Ogre::Vector3 (m[2][0]+m[3][0], m[2][1]+m[3][1], m[2][2]+m[3][2]);
  up   = Ogre::Vector3 (m[1][0],         m[1][1],         m[1][2]);
}
