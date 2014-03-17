#include "ARTKDetector.h"

ARTKDetector::ARTKDetector(int w, int h, int thres/*, int *nObjects, Marca objects[]*/){
  _markerNum=0;   _markerInfo=NULL;  _thres = thres;
  _width = w;     _height = h;       _detected = false;
  _nObjects = 5;  l=-1;
  readConfiguration();
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
bool ARTKDetector::detectMark(cv::Mat* frame,/*, int nObjects*/ Marca objects[5]) {
  //double p_width     = 120.0;        // Ancho de marca... FIJO!
  //double p_center[2] = {0.0, 0.0};   // Centro de marca.. FIJO!
  int i, j, k;
  //printf("Nobjets,%d",_nObjects);
  double center[2];
  double pattTrans[3][4];
  _detected = false;
  if(frame->rows==0) return _detected;
  if(arDetectMarker(frame->data, _thres,
                    &_markerInfo, &_markerNum) < 0){
    return _detected;
  }

   for (i=0; i<_nObjects; i++) {
    //_marca = _scene->getMarca(i);
    _marca = &objects[i];
    for(j = 0, k = -1; j < _markerNum; j++) {
      if(_marca->getId() == _markerInfo[j].id) {
        if (k == -1) k = j;
        else if(_markerInfo[k].cf < _markerInfo[j].cf) k = j;
      }
    }

    if(k != -1) {   // Si ha detectado el patron en algun sitio...
      _marca->setVisible(true);
      _marca->getPattTans(pattTrans);
      _marca->getCenter(center);
      arGetTransMatCont(&_markerInfo[k], pattTrans, center, _marca->getWidth(), pattTrans);
      _marca->setPattTans(pattTrans);
      _marca->setMarkerInfo(_markerInfo[k]);
      _detected = true;
    } else {  _marca->setVisible(false); }  // El objeto no es visible
  }

  return _detected;
}
// ================================================================
// Gl2Mat: Utilidad para convertir entre matriz OpenGL y Matrix4
void ARTKDetector::Gl2Mat(double *gl, Ogre::Matrix4 &mat){
  for(int i=0;i<4;i++) for(int j=0;j<4;j++) mat[i][j]=gl[i*4+j];
}
// ================================================================
// getPosRot: Obtiene posicion y rotacion para la camara
void ARTKDetector::getPosRot(Ogre::Vector3 &pos,
                   Ogre::Vector3 &look, Ogre::Vector3 &up,Marca* marca){
   //std::cout<<"Marca PosRot "<<marca->getId()<<std::endl;
  if (!_detected) return;

  double glAuxd[16]; Ogre::Matrix4 m;
  marca->getPattTans(_pattTrans);
  argConvGlpara(_pattTrans,glAuxd);
  Gl2Mat(glAuxd, m);   // Convertir a Matrix4 de Ogre

  m[0][1]*=-1; m[1][1]*=-1;  m[2][1]*=-1; m[3][1]*=-1;
  m = m.inverse();
  m = m.concatenate(Ogre::Matrix4(
      Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_X)));
  pos  = Ogre::Vector3 (m[3][0],         m[3][1],        m[3][2]);
  look = Ogre::Vector3 (m[2][0]+m[3][0], m[2][1]+m[3][1],
                                                 m[2][2]+m[3][2]);
  up   = Ogre::Vector3 (m[1][0],         m[1][1],        m[1][2]);

}

/*void ARTKDetector::getObjects(Marca ob[5]){
    for (int i=0;i<_nObjects; i++){
        ob[i]=_objects[i];
    }
}*/
