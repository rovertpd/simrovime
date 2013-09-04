#include "MyFrameListener.h"

MyFrameListener::MyFrameListener(Ogre::RenderWindow* win,Ogre::Camera* cam,Ogre::SceneManager *sm) {
  // Omitido el resto del codigo...
  _videoManager = new VideoManager(1, 640, 480, _sceneManager);
  _arDetector = new ARTKDetector(640, 480, 100);
}

MyFrameListener::~MyFrameListener() {

}

bool MyFrameListener::frameStarted(const Ogre::FrameEvent& evt) {
  // Omitido el resto del codigo...
  Ogre::Vector3 pos;  Ogre::Vector3 look;   Ogre::Vector3 up;

  _videoManager->UpdateFrame();
  _videoManager->DrawCurrentFrame();
  if (_arDetector->detectMark(_videoManager->getCurrentFrameMat())){
    _arDetector->getPosRot(pos, look, up);
    _camera->setPosition(pos);
    _camera->lookAt(look);
    _camera->setFixedYawAxis(true, up);
  }
  return true;
}
