#include "VideoManager.h"
VideoManager::VideoManager(int device, int w, int h,
			   Ogre::SceneManager* sm){
  _sceneManager = sm;
  _capture = cvCreateCameraCapture(device);
  cvSetCaptureProperty(_capture, CV_CAP_PROP_FRAME_WIDTH, w);
  cvSetCaptureProperty(_capture, CV_CAP_PROP_FRAME_HEIGHT, h);
  createBackground (w, h);
  _scene = Scene::Instance();
  _frameIpl = NULL; _frameMat = NULL;
  std::cout<<"creado el video manager"<<std::endl;
}

VideoManager::~VideoManager(){
  cvReleaseCapture(&_capture); delete _frameIpl; delete _frameMat;
}
// ================================================================
// createBackground: Crea el plano sobre el que dibuja el video
void VideoManager::createBackground(int cols, int rows){
  Ogre::TexturePtr texture=Ogre::TextureManager::getSingleton().
    createManual("BackgroundTex", // Nombre de la textura
	Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	Ogre::TEX_TYPE_2D,   // Tipo de la textura
	cols, rows, 0,       // Filas, columas y Numero de Mipmaps
	Ogre::PF_BYTE_BGRA,
	Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE);

  Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().
    create("Background",
          Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
  mat->getTechnique(0)->getPass(0)->createTextureUnitState();
  mat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
  mat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
  mat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
  mat->getTechnique(0)->getPass(0)->getTextureUnitState(0)->
    setTextureName("BackgroundTex");

  // Creamos un rectangulo que cubra toda la pantalla
  Ogre::Rectangle2D* rect = new Ogre::Rectangle2D(true);
  rect->setCorners(-1.0, 1.0, 1.0, -1.0);
  rect->setMaterial("Background");

  // Dibujamos el background antes que nada
  rect->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);

  Ogre::SceneNode* node = _sceneManager->getRootSceneNode()->createChildSceneNode("BackgroundNode");
  node->attachObject(rect);
}
// ================================================================
//  UpdateFrame: Actualiza los punteros de frame Ipl y frame Mat
void VideoManager::UpdateFrame(){
  _frameIpl = cvQueryFrame(_capture);
  _frameMat = new cv::Mat(_frameIpl);
}
// = IplImage* getCurrentFrameIpl =================================
IplImage* VideoManager::getCurrentFrameIpl(){ return _frameIpl; }
// = IplImage* getCurrentFrameMat =================================
cv::Mat* VideoManager::getCurrentFrameMat(){  return _frameMat; }

bool VideoManager::rect_sup(double x, double y){
    double *sup=_scene->getP_sup();
    double *sup_der=_scene->getP_sup_der();
    return ((x-sup[0])*(sup_der[1]-sup[1])-(y-sup[1])*(sup_der[0]-sup[0])<0);
}
bool VideoManager::rect_inf(double x, double y){
    double *inf=_scene->getP_inf();
    double *inf_izq=_scene->getP_inf_izq();
    return ((x-inf[0])*(inf_izq[1]-inf[1])-(y-inf[1])*(inf_izq[0]-inf[0])<0);
}
bool VideoManager::rect_der(double x, double y){
    double *inf=_scene->getP_inf();
    double *inf_der=_scene->getP_inf_der();
    return ((x-inf_der[0])*(inf[1]-inf_der[1])-(y-inf_der[1])*(inf[0]-inf_der[0])<0);
}
bool VideoManager::rect_izq(double x, double y){
    double *sup=_scene->getP_sup();
    double *sup_izq=_scene->getP_sup_izq();
    return ((x-sup_izq[0])*(sup[1]-sup_izq[1])-(y-sup_izq[1])*(sup[0]-sup_izq[0])<0);
}

bool VideoManager::color(int x,int y){
    IplImage* img = 0;
    int anchura_fila,canales;
    uchar *data;
      img=getCurrentFrameIpl();
      anchura_fila = img->widthStep;
      canales = img->nChannels;
      data = (uchar *)img->imageData;

    if ((data[x*anchura_fila+y*canales + 1] > 80) &&
        !((data[x*anchura_fila+y*canales + 0] > data[x*anchura_fila+y*canales + 1]/2) ||
        (data[x*anchura_fila+y*canales + 2] > data[x*anchura_fila+y*canales + 1]/2))){
        return true;
      }
      else{
          return false;
      }
}

bool VideoManager::getColor(){
    return _col;
}
// ================================================================
//  DrawCurrentFrame: Despliega el ultimo frame actualizado
void VideoManager::DrawCurrentFrame(){
  if(_frameMat->rows==0) return;
  Ogre::TexturePtr tex = Ogre::TextureManager::getSingleton().
     getByName("BackgroundTex",
	 Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
  Ogre::HardwarePixelBufferSharedPtr pBuffer = tex->getBuffer();

  pBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD);
  const Ogre::PixelBox& pixelBox = pBuffer->getCurrentLock();
  bool colors = false;
  int x_v_cont = 0;
  int y_v_cont = 0;

  int x_v_total = 0;
  int y_v_total = 0;

  Ogre::uint8* pDest = static_cast<Ogre::uint8*>(pixelBox.data);
  _col=false;
  for(int j=0;j<_frameMat->rows;j++) {
    for(int i=0;i<_frameMat->cols;i++) {
      if(rect_sup(i,j)&&rect_der(i,j)&&rect_inf(i,j)&&rect_izq(i,j)){
          if (color(j,i)){
              int idx = ((j) * pixelBox.rowPitch + i )*4;
              pDest[idx] = 255;
              pDest[idx+1] = 0;
              pDest[idx+2] = 0;
              pDest[idx+3] = 255;
              colors=true;
              y_v_total = y_v_total + i;
              x_v_total = x_v_total + j;
              y_v_cont++;
              x_v_cont++;
              _col=true;
          }else{
              int idx = ((j) * pixelBox.rowPitch + i )*4;
              pDest[idx] = _frameMat->data[(j*_frameMat->cols+i)*3];
              pDest[idx+1] = _frameMat->data[(j*_frameMat->cols+i)*3+1];
              pDest[idx+2] = _frameMat->data[(j*_frameMat->cols+i)*3+2];
              pDest[idx+3] = 255;
          }
      }
      else{
          int idx = ((j) * pixelBox.rowPitch + i )*4;
          pDest[idx] = 255;
          pDest[idx+1] = 255;
          pDest[idx+2] = 255;
          pDest[idx+3] = 255;
      }
    }
  }
  if (colors){
    double fin[2];
    //_scene->getRobot(0)->setDir(0);
    fin[0] = x_v_total / x_v_cont;
    fin[1] = y_v_total / y_v_cont;
    _scene->getRobot(0)->setFin(fin);
    //_scene->getRobot(0)->setEst(7);
    //printf("Punto medio verde en %f, %f\n",fin[0], fin[1]);
  }

  pBuffer->unlock();
  Ogre::Rectangle2D* rect = static_cast<Ogre::Rectangle2D*>
    (_sceneManager->getSceneNode("BackgroundNode")->
        getAttachedObject(0));
}
