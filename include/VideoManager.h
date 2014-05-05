#ifndef VMGR_H
 #define VMGR_H
#include <Ogre.h>
#include <iostream>
#include "cv.h"
#include "highgui.h"
#include "Filter.h"
#include <vector>

class VideoManager;

#include "Scene.h"

class VideoManager {
 private:
  void createBackground(int cols, int rows);
  void ReleaseCapture();
  CvCapture* _capture;
  IplImage* _frameIpl;
  cv::Mat* _frameMat;
  Ogre::SceneManager* _sceneManager;
  vector<Scene*> _scene;
  bool _col;
  Filter* _filter;

 public:
  void attach(Scene *s);
  VideoManager(int device, int w, int h,
               Ogre::SceneManager* sm);
  ~VideoManager();
  void UpdateFrame();
  IplImage* getCurrentFrameIpl();
  cv::Mat* getCurrentFrameMat();
  void DrawCurrentFrame(int frame);
//  bool rect_sup(double x, double y);
//  bool rect_inf(double x, double y);
//  bool rect_der(double x, double y);
//  bool rect_izq(double x, double y);
  int color(int x,int y);
  bool getColor();
};
#endif
