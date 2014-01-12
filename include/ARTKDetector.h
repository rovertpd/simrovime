#include <AR/ar.h>
#include <AR/gsub.h>
#include <AR/param.h>
#include <Ogre.h>
#include <iostream>
#include "cv.h"
#include "Marca.h"
#include "Scene.h"

class ARTKDetector {
 private:
  ARMarkerInfo *_markerInfo;
  int _markerNum;
  int _thres;
  int _width; int _height;
  double _pattTrans[3][4];
  bool _detected;
  int l;
  int _nObjects;
  Marca* _marca;
  Scene* _scene;

  int readConfiguration();
  int readPatterns(/*int *nObjects, Marca objects[]*/);
  void Gl2Mat(double *gl, Ogre::Matrix4 &mat);

 public:
  ARTKDetector(int w, int h, int thres/*, int *nObjects, Marca objects[]*/);
  ~ARTKDetector();
  bool detectMark(cv::Mat* frame/*, int nObjects, Marca objects[]*/);
  void getPosRot(Ogre::Vector3 &pos, Ogre::Vector3 &look,
		 Ogre::Vector3 &up,int indice);
  //void getObjects(Marca ob[5]);
};
