
#define _WORLD_CPP 15

#include "World.h"
#include "KeyBuffer.h"
#include <iostream>
#include <cstdio>

using namespace std;

World::World() : IEventReceiver() {
  this->device = 0;
  this->scenes = new vector<Scene*>(0);
  this->current_scene = this->no_current_scene = this->scenes->end();
  this->keys = new KeyBuffer();
}

World::~World() {
  /* free the pointer array */
  this->scenes->clear();
  delete this->scenes;
  delete this->keys;
}

/*  get the iterator of the scene which
    has the given name */
vector<Scene*>::iterator World::getSceneIterator(const char *name) {
  vector<Scene*>::iterator search;
  /* iterator through all Scene objects */
  for(search = this->scenes->begin(); search != this->scenes->end(); search++) {
    /* compare names */
    if((*search)->getName().compare(string(name)) == 0) {
      break;
    }
  }

  /* throw an exception if the scene doesn't exist */
  if(search == this->scenes->end()) {
    throw invalid_argument("no scene with name '" + string(name) + "'");
  }
  return search;
}

/* get the pointer to the scene object
   which has the given name*/
Scene *World::getScene(const char *name) {
  return *getSceneIterator(name);
}


bool World::hasCurrentScene() {
  return this->current_scene != this->no_current_scene;
}

/* get the currently loaded scene */
Scene *World::getCurrentScene() {
  if(hasCurrentScene()) {
    throw std::out_of_range("no scene selected");
  }
  return *this->current_scene;
}

/* add a scene object to the array of
   scenes in this world */
void World::addScene(Scene *scene) {
  /* check if there already is a scene with the same name  */
  bool exists = false;
  try {
    getSceneIterator(scene->getName().c_str());
    exists = true;
  } catch(invalid_argument &e) {
    /* nope there is none, so add the scene to the array */
    this->scenes->push_back(scene);
  }

  if(exists) {
    /* there is already a scene with the same name, so throw an exception */
    throw invalid_argument("scene '" + scene->getName() + "' has been added before");
  }
}

/* remove a scene by pointer  */
void World::removeScene(Scene *scene) {
  Scene *current = NULL;
  if(hasCurrentScene()) {
    current = getCurrentScene();
  }
  
  foreach(iterator, (*this->scenes)) {
    if(*iterator == scene) {
      if(current == *iterator) {
        unloadScene();
      }
      this->scenes->erase(iterator);
      break;
    }
  }
}

/* remove a scene by name */
void World::removeScene(const char *name) {
  this->scenes->erase(getSceneIterator(name));
}
/* load the scene which has the given name

   the onLoad() method in the Scene object gets called
   if an other scene was loaded before, the
   onUnload()-method gets called too */
void World::loadScene(const char *name) {
  /* unload the old scene (if there is one) */
  unloadScene();

  /* set the new current scene */
  this->current_scene = getSceneIterator(name);

  /* execute the onLoad event in the scene */
  (*this->current_scene)->onLoad();
}

/* this unloads the current loaded scene
   (if there is one)
   
   this also calls the UnloadScene()-method
   on the scene object */
void World::unloadScene() {
  /* unload the old scene */
  if(hasCurrentScene()) {
    getCurrentScene()->onUnload();
  }

  /* reset the current scene */
  this->current_scene = this->no_current_scene;
}

KeyBuffer *World::getKeys() {
  return keys;
}

bool World::OnEvent(const SEvent& event) {
  bool result = false;
  
  if(device != 0) {
  
    if(event.EventType == EET_KEY_INPUT_EVENT) {
      keys->set(event.KeyInput.Key, event.KeyInput.PressedDown);
      keys->control(event.KeyInput.Control);
      keys->shift(event.KeyInput.Shift);
    }
    
    if(hasCurrentScene()) {
      result = getCurrentScene()->OnEvent(event);
    }
          
  }
  return result;
}

void World::setDevice(IrrlichtDevice *device) {
  this->device = device;
}

IrrlichtDevice *World::getDevice() {
  return device;
}