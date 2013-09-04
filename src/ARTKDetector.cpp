#include "ARTKDetector.h"

ARTKDetector::ARTKDetector(int w, int h, int thres){
  _markerNum=0;   _markerInfo=NULL;  _thres = thres;
  _width = w;     _height = h;       _detected = false;
  readConfiguration();  
  readPatterns();
}
ARTKDetector::~ARTKDetector(){  argCleanup();  }
// ================================================================
// readPatterns: Carga la definicion de patrones 
int ARTKDetector::readPatterns(){
  if((_pattId=arLoadPatt("data/simple.patt")) < 0) return -1;
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
  int j, k;
  double p_width     = 120.0;        // Ancho de marca... FIJO!
  double p_center[2] = {0.0, 0.0};   // Centro de marca.. FIJO!

  _detected = false;
  if(frame->rows==0) return _detected;
  if(arDetectMarker(frame->data, _thres, 
                    &_markerInfo, &_markerNum) < 0){
    return _detected; 
  }
  for(j=0, k=-1; j < _markerNum; j++) {
    if(_pattId == _markerInfo[j].id) {
      if (k == -1) k = j;
      else if(_markerInfo[k].cf < _markerInfo[j].cf) k = j;
    }
  }
  if(k != -1) {   // Si ha detectado el patron en algun sitio...
    arGetTransMat(&_markerInfo[k], p_center, p_width, _pattTrans);
    _detected = true;
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
                   Ogre::Vector3 &look, Ogre::Vector3 &up){
  if (!_detected) return;

  double glAuxd[16]; Ogre::Matrix4 m;
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
