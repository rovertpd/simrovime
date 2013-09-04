#include <Ogre.h>
#include "VideoManager.h"
#include "ARTKDetector.h"
//using namespace std;
//using namespace Ogre;


class MyFrameListener : public Ogre::FrameListener {
private:
  //OIS::InputManager* _inputManager;
  //OIS::Keyboard* _keyboard;
  Ogre::Camera* _camera;
  Ogre::SceneNode* _node;
  Ogre::SceneManager* _sceneManager;
  VideoManager* _videoManager;
  ARTKDetector* _arDetector;

public:
  MyFrameListener(Ogre::RenderWindow* win,Ogre::Camera* cam, Ogre::SceneManager *sm);
  //MyFrameListener(Ogre::RenderWindow* win, Ogre::Camera* cam,Ogre::SceneNode* node, Ogre::SceneManager *sm);
  ~MyFrameListener();
  bool frameStarted(const Ogre::FrameEvent& evt);
};
