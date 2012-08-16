/* 
 * File:   GameScene.cpp
 * Author: zurcherl
 * 
 * Created on 14. August 2012, 17:40
 */

#include "GameScene.h"

GameScene::GameScene(World *world) : Scene("game", world){
  
};

void GameScene::onLoad(){
  ship = new Ship("objects/player/ship.3ds", this->smgr);
}

bool GameScene::OnEvent(const SEvent& event){
  return false;
}

void GameScene::onTick(){
  
}

void GameScene::onUnload(){
  delete ship;
}
