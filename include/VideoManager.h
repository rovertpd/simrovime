#ifndef VMGR_H
 #define VMGR_H
#include <Ogre.h>
#include <iostream>
#include "cv.h"
#include "highgui.h"
#include <vector>

class VideoManager;

#include "Scene.h"
#include "Filter.h"

class VideoManager {
 private:
  void createBackground(int cols, int rows);
  void ReleaseCapture();
  CvCapture* _capture;
  IplImage* _frameIpl;
  cv::Mat* _frameMat;
  cv::Mat _hsv;
  Ogre::SceneManager* _sceneManager;
  vector<Scene*> _scene;
  bool _colors[3];  //0->rojo,1->verde,2->azul
  Objeto* _objs[3];
  Filter* _filter;
  cv::Vec3b hsv;

 public:
  void attach(Scene *s);
  VideoManager(int device, int w, int h,
               Ogre::SceneManager* sm);
  ~VideoManager();
  void UpdateFrame();
  IplImage* getCurrentFrameIpl();
  cv::Mat getCurrentFrameIplHSV();
  void setHSV();
  cv::Mat* getCurrentFrameMat();
  void DrawCurrentFrame(int frame);
//  bool rect_sup(double x, double y);
//  bool rect_inf(double x, double y);
//  bool rect_der(double x, double y);
//  bool rect_izq(double x, double y);
  int color(int x,int y);
//  bool getColor();
};
#endif
