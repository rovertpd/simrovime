#include <Ogre.h>
#include <iostream>
#include "cv.h"
#include "highgui.h"
#include "Scene.h"

class VideoManager {
 private:
  void createBackground(int cols, int rows);
  void ReleaseCapture();
  CvCapture* _capture;
  IplImage* _frameIpl;
  cv::Mat* _frameMat;
  Ogre::SceneManager* _sceneManager;
  Scene* _scene;

 public:
  VideoManager(int device, int w, int h,
               Ogre::SceneManager* sm);
  ~VideoManager();
  void UpdateFrame();
  IplImage* getCurrentFrameIpl();
  cv::Mat* getCurrentFrameMat();
  void DrawCurrentFrame();
  bool rect_sup(double x, double y);
  bool rect_inf(double x, double y);
  bool rect_der(double x, double y);
  bool rect_izq(double x, double y);
};
