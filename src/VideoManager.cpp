#include "VideoManager.h"

VideoManager::VideoManager(int device, int w, int h,
			   Ogre::SceneManager* sm){
  _filter = Filter::Instance();
  _sceneManager = sm;
  _capture = cvCreateCameraCapture(device);
  cvSetCaptureProperty(_capture, CV_CAP_PROP_FRAME_WIDTH, w);
  cvSetCaptureProperty(_capture, CV_CAP_PROP_FRAME_HEIGHT, h);
  createBackground (w, h);
  for (int i=0; i<3; i++){
      _colors[i] = false;
      _objs[i] = NULL;
  }
  //_scene = Scene::Instance();
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

void VideoManager::attach(Scene *s){
    _scene.push_back(s);
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

int VideoManager::color(int x,int y){
    IplImage* img = 0;
    int anchura_fila,canales;
    uchar *data;
    img=getCurrentFrameIpl();
    anchura_fila = img->widthStep;
    canales = img->nChannels;
    data = (uchar *)img->imageData;

    if ((data[x*anchura_fila+y*canales + 0] > 80) && !((data[x*anchura_fila+y*canales + 1] > data[x*anchura_fila+y*canales + 0]/2) || (data[x*anchura_fila+y*canales + 2] > data[x*anchura_fila+y*canales + 0]/2))){
        return 1;
    }else if ((data[x*anchura_fila+y*canales + 1] > 80) && !((data[x*anchura_fila+y*canales + 0] > data[x*anchura_fila+y*canales + 1]/2) || (data[x*anchura_fila+y*canales + 2] > data[x*anchura_fila+y*canales + 1]/2))){
            return 2;
    }else if ((data[x*anchura_fila+y*canales + 2] > 80) && !((data[x*anchura_fila+y*canales + 0] > data[x*anchura_fila+y*canales + 2]/2) || (data[x*anchura_fila+y*canales + 1] > data[x*anchura_fila+y*canales + 2]/2))){
            return 3;
    }
    return 0;
}

//bool VideoManager::getColor(){
//    return _col;
//}
// ================================================================
//  DrawCurrentFrame: Despliega el ultimo frame actualizado
void VideoManager::DrawCurrentFrame(int frame){
  if(_frameMat->rows==0) return;
  //_scene->clearObjs();
  Ogre::TexturePtr tex = Ogre::TextureManager::getSingleton().
     getByName("BackgroundTex",
	 Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
  Ogre::HardwarePixelBufferSharedPtr pBuffer = tex->getBuffer();

  pBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD);
  const Ogre::PixelBox& pixelBox = pBuffer->getCurrentLock();
  bool colors = false;
  vector<int*> cont;
  int c[8] = {0,0,0,0,99999999,99999999,-1,-1}; // x_cont, y_cont, x_total, y_total, x_min, y_min, x_max, y_max
  cont.push_back(c);    // Rojo
  cont.push_back(c);    // Verde
  cont.push_back(c);    // Azul
//  int x_r_cont = 0;
//  int y_r_cont = 0;
//  int x_r_total = 0;
//  int y_r_total = 0;
//  int x_v_cont = 0;
//  int y_v_cont = 0;
//  int x_v_total = 0;
//  int y_v_total = 0;
//  int x_a_cont = 0;
//  int y_a_cont = 0;
//  int x_a_total = 0;
//  int y_a_total = 0;
  Ogre::uint8* pDest = static_cast<Ogre::uint8*>(pixelBox.data);
////////////////////////////  if (_filter->getGrid()>0){
////////////////////////////      for (int j=0;j<_scene->getAncho()/_filter->getGrid();j++)
////////////////////////////        for (int i=0;i<_scene->getAlto()/_filter->getGrid();i++)
////////////////////////////          _scene->setMap(i,j,0);
////////////////////////////  }
  //_col=false;

  for(int j=0;j<_frameMat->rows;j++) {
    for(int i=0;i<_frameMat->cols;i++) {
      if(_filter->rect_sup(i,j)&&_filter->rect_der(i,j)&&_filter->rect_inf(i,j)&&_filter->rect_izq(i,j)){
          int col = color(j,i);
          if (col == 1){
              int idx = ((j) * pixelBox.rowPitch + i )*4;
              pDest[idx] = 0;
              pDest[idx+1] = 0;
              pDest[idx+2] = 255;
              pDest[idx+3] = 255;
              colors=true;
              cont[2][2] = cont[2][2] + j;
              cont[2][3] = cont[2][3] + i;
              cont[2][0] = cont[2][0] + 1;
              cont[2][1] = cont[2][1] + 1;
              if (j<cont[2][4]) cont[2][4] = j;
              else if (j>cont[2][6]) cont[2][6] = j;
              if (i<cont[2][5]) cont[2][5] = i;
              else if (i>cont[2][7]) cont[2][7] = i;
//              cout<<"VideoManager:: Detectado color azul"<<endl;
//              y_a_total = y_a_total + i;
//              x_a_total = x_a_total + j;
//              y_a_cont++;
//              x_a_cont++;
          }else if (col == 2){
              int idx = ((j) * pixelBox.rowPitch + i )*4;
              pDest[idx] = 255;
              pDest[idx+1] = 0;
              pDest[idx+2] = 0;
              pDest[idx+3] = 255;
              colors=true;
              cont[1][2] = cont[1][2] + j;
              cont[1][3] = cont[1][3] + i;
              cont[1][0] = cont[1][0] + 1;
              cont[1][1] = cont[1][1] + 1;
              if (j<cont[1][4]) cont[1][4] = j;
              else if(j>cont[1][6]) cont[1][6] = j;
              if (i<cont[1][5]) cont[1][5] = i;
              else if(i>cont[1][7]) cont[1][7] = i;
//              cout<<"VideoManager:: Detectado color verde"<<endl;
//              y_v_total = y_v_total + i;
//              x_v_total = x_v_total + j;
//              y_v_cont++;
//              x_v_cont++;
          }else if (col == 3){
              int idx = ((j) * pixelBox.rowPitch + i )*4;
              pDest[idx] = 0;
              pDest[idx+1] = 255;
              pDest[idx+2] = 0;
              pDest[idx+3] = 255;
              colors=true;
              cont[0][2] = cont[0][2] + j;
              cont[0][3] = cont[0][3] + i;
              cont[0][0] = cont[0][0] + 1;
              cont[0][1] = cont[0][1] + 1;
              if (j<cont[0][4]) cont[0][4] = j;
              else if(j>cont[0][6]) cont[0][6] = j;
              if (i<cont[0][5]) cont[0][5] = i;
              else if(i>cont[0][7]) cont[0][7] = i;
//              cout<<"VideoManager:: Detectado color rojo"<<endl;
//              y_r_total = y_r_total + i;
//              x_r_total = x_r_total + j;
//              y_r_cont++;
//              x_r_cont++;
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
      if ((_filter->getGrid()>0.0)){
        if (((i%_filter->getGrid()) == 0) && ((j%_filter->getGrid())==0)){
          int idx = ((j) * pixelBox.rowPitch + i )*4;
          pDest[idx] = 0;
          pDest[idx+1] = 0;
          pDest[idx+2] = 0;
          pDest[idx+3] = 255;
        }
      }
    }
  }
  if (colors){
      double fin[2];
      if (cont[0][0] > 0){
            fin[1] = cont[0][2] / cont[0][0]; //x_r_total / x_r_cont;
            fin[0] = cont[0][3] / cont[0][1]; //y_r_total / y_r_cont;
            if (!_colors[0]){
                _objs[0] = (new Objeto(1,5,fin));
                for (vector<Scene*>::iterator it = _scene.begin(); it != _scene.end(); ++it) {
                  double pM[2] = {cont[0][6], cont[0][7]}, pm[2] = {cont[0][4], cont[0][5]};
                  _objs[0]->setPos(fin, pM, pm);
                  (*it)->addObject(_objs[0]);
                  printf("VideoManager:: Punto medio rojo en %f, %f\n",fin[0], fin[1]);
                  printf("VideoManager:: Punto M: [%f,%f] Punto m: [%f,%f]\n",pM[0]/(*it)->getGrid(), pM[1]/(*it)->getGrid(), pm[0]/(*it)->getGrid(), pm[1]/(*it)->getGrid());
                  cout<<"VideoManager:: Add object "<<endl;
                }
            }else{
                double* pAnt = _objs[0]->getPos();
                if (sqrt(pow((fin[0]-pAnt[0]),2)+pow((fin[1]-pAnt[1]),2)) > 20){
                  for (vector<Scene*>::iterator it = _scene.begin(); it != _scene.end(); ++it) {
                    double pM[2] = {cont[0][6], cont[0][7]}, pm[2] = {cont[0][4], cont[0][5]};
                    _objs[0]->setPos(fin, pM, pm);
                    (*it)->changeObject(0, fin, pM, pm);
                    printf("VideoManager:: Punto medio rojo en %f, %f\n",fin[0], fin[1]);
                    cout<<"VideoManager:: Change object "<<endl;
                  }
                }
            }
            _colors[0] = true;
      }
//      if (x_v_cont > 0){
//            fin[1] = x_v_total / x_v_cont;
//            fin[0] = y_v_total / y_v_cont;
//            if (!_colors[1]){
//              _objs[1] = (new Objeto(2,3,fin));
//              for (vector<Scene*>::iterator it = _scene.begin(); it != _scene.end(); ++it) {
//                (*it)->addObject(*_objs[1]);
//              }
//            }else{
//              double pAnt[2];
//              _objs[1]->getPos(pAnt);
//              if (sqrt(pow((fin[0]-pAnt[0]),2)+pow((fin[1]-pAnt[1]),2)) > 20){
//                for (vector<Scene*>::iterator it = _scene.begin(); it != _scene.end(); ++it) {
//                  _objs[1]->setPos(fin);
//                  (*it)->changeObject(1,fin);
//                }
//              }
//            }
//            _colors[1] = true;
//            printf("Punto medio verde en %f, %f\n",fin[0], fin[1]);
//      }
//      if (x_a_cont > 0){
//            fin[1] = x_a_total / x_a_cont;
//            fin[0] = y_a_total / y_a_cont;
//            if (!_colors[2]){
//              _objs[2] = (new Objeto(3,1,fin));
//              for (vector<Scene*>::iterator it = _scene.begin(); it != _scene.end(); ++it) {
//                (*it)->addObject(*_objs[2]);
//              }
//            }else{
//              double pAnt[2];
//              _objs[2]->getPos(pAnt);
//              if (sqrt(pow((fin[0]-pAnt[0]),2)+pow((fin[1]-pAnt[1]),2)) > 20){
//                for (vector<Scene*>::iterator it = _scene.begin(); it != _scene.end(); ++it) {
//                  _objs[0]->setPos(fin);
//                  (*it)->changeObject(2,fin);
//                }
//              }
//            }
//            _colors[2] = true;
//            printf("Punto medio azul en %f, %f\n",fin[0], fin[1]);
//      }
  }else {
      for (int i=0; i<3; i++){
        if (_colors[i]){
          _colors[i] = false;
          _objs[i] = NULL;
          for (vector<Scene*>::iterator it = _scene.begin(); it != _scene.end(); ++it) {
            (*it)->deleteObject(i);
            cout<<"VideoManager:: Delete object "<<endl;
          }
        }
      }
  }

  pBuffer->unlock();
}
