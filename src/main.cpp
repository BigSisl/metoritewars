#include <irrlicht/irrlicht.h>
#include "engine/World.h"
#include "game/scene/GameMenu.h"
#include "game/scene/GameScene.h"
#include "game/scene/GameOver.h"
#include <ctime>

using namespace irr;

int main() {
  srand(time(NULL));
  World *world = new World();
  IrrlichtDevice* device = createDevice(video::EDT_OPENGL,
          core::dimension2d<u32 > (world->win.width, world->win.height), 16, world->win.fullscreen, true, world->win.fullscreen, world);
  if (device == 0) {
    return 1; // could not create selected driver.
  }

  world->setDevice(device);
  
  //create scene
  GameMenu* menu = new GameMenu(world);
  GameScene* game = new GameScene(world);
  GameOver* over = new GameOver(world);

  
  //add scenes
  world->addScene(menu);
  world->addScene(game);
  world->addScene(over);
  
  //load menu scene
  world->loadScene("menu");
  
  video::IVideoDriver* driver = device->getVideoDriver();
  scene::ISceneManager* smgr = device->getSceneManager();

  int lastFPS = -1;

	// In order to do framerate independent movement, we have to know
	// how long it was since the last frame
	u32 then = device->getTimer()->getTime();
  
  while (device->run()) {
    //Work out a frame delta time.
    const u32 now = device->getTimer()->getTime();
    world->setFrameDeltaTime((float)(now - then) / 1000.f); // Time in seconds
    world->setRunTime(now / 1000.f);
    then = now;

     
    driver->beginScene(true, true, video::SColor(255, 0, 0, 0));
    
    if(world->hasCurrentScene()) {
      world->getCurrentScene()->onTick();
    }
    
    smgr->drawAll(); // draw the 3d scene
    device->getGUIEnvironment()->drawAll(); // draw the gui environment (the logo)
    driver->endScene();

    
    int fps = driver->getFPS();
    if (lastFPS != fps)
    {
      core::stringw tmp(L"Metoritewars [Irrlicht][");
      tmp += driver->getName();
      tmp += L"] fps: ";
      tmp += fps;

      device->setWindowCaption(tmp.c_str());
      lastFPS = fps;
    }
  }

  delete menu;
  delete game;
  delete over;
  
  /*
  In the end, delete the Irrlicht device.
   */
  device->drop();

  return 0;
}

/*
That's it. Compile and play around with the program.
 **/
