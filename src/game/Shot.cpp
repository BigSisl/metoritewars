/* 
 * File:   Shot.cpp
 * Author: zurcherl
 * 
 * Created on 17. August 2012, 13:17
 */

#include "Shot.h"

Shot::Shot(const char*  filename, scene::ISceneManager* smgr, scene::IAnimatedMeshSceneNode* ship) {
  mainShot = smgr->addAnimatedMeshSceneNode(smgr->getMesh("objects/player/shot.x"),ship,32, core::vector3df(0.0,0.0,0.0), core::vector3df(0.0,-90.0,0.0), core::vector3df(0.2,0.2,0.2));

  mainShot->setVisible(false);
  
  this->smgr = smgr;
  
  //setup shot settings
  shotspeed = 5;
  shottimeout = 0.2;
  
  if(mainShot){
    mainShot->setMaterialFlag(video::EMF_LIGHTING, false);
  }
}

float Shot::getShotTimeout(){
  return shottimeout;
}
  
void Shot::createShot(core::vector3df shotpos, core::vector3df shiprot, core::vector3df shipscale){
  shots.push_back(mainShot->clone(smgr->getRootSceneNode(), smgr));
  
  //set visible again
  shots.back()->setVisible(true);
  
  //setup last shot
  shots.back()->setPosition(shotpos);
  shots.back()->setRotation(shiprot + core::vector3df(0.0,0.0,-90.0));
  shots.back()->setScale(shipscale * mainShot->getScale());
}

void Shot::move(float DeltaTime){
  ///scene::IAnimatedMeshSceneNode* shot;
  foreach(shot, shots) {
    //set position
    (*shot)->setPosition((*shot)->getRotation().rotationToDirection(core::vector3df(-1 * shotspeed * DeltaTime, 0.0,0.0)) + (*shot)->getPosition());
  }
}

void Shot::removeShot(std::vector<scene::ISceneNode*>::iterator &shot){
  smgr->addToDeletionQueue((*shot));
  shots.erase(shot);
}

void Shot::removeShot(scene::ISceneNode* removingshot){
  foreach(shot, shots){
    if((*shot) == removingshot){
      removeShot(shot);
    }
  }
}

std::vector<scene::ISceneNode*> &Shot::getShotNode(){
  return shots;
}

Shot::~Shot() {
}

