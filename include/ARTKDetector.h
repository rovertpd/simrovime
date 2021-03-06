#ifndef ARTK_H
  #define ARTK_H

#include <AR/ar.h>
#include <AR/gsub.h>
#include <AR/param.h>
#include <OGRE/Ogre.h>
#include <iostream>
#include "cv.h"

class ARTKDetector;

#include "Scene.h"
#include "Marca.h"

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
  ARTKDetector(int w, int h, int thres, Scene *scn);
  ~ARTKDetector();
  bool detectMark(cv::Mat* frame);
  void getPosRot(Ogre::Vector3 &pos, Ogre::Vector3 &look,
         Ogre::Vector3 &up,Marca* marca);
  float getRotation(Marca *m);
};
#endif
