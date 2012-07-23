#include "world.h"

World::World()  {
  this->num_scenes = 0;
  this->current_scene = -1;
  this->scenes = new Scene*[this->num_scenes];
}

World::~World() {
  for(int i = 0; i < this->num_scenes; i++) {
    delete this->scenes[i];
  }
  delete [] this->scenes;
}

int World::get_scene_id(string &name) {
  int selected = -1;
  /* iterate the scenes and search for the given name */
  for(int i; selected && i < this->num_scenes; i++) {
    if(this->scenes[i]->get_name().compare(name) == 0) {
      selected = i;
    }
  }
  return selected;
}

Scene *World::get_scene_by_id(int id) {
  Scene *selected = NULL;
  /* check if valid index */
  if(id >= 0 && id < this->num_scenes) {
    selected = this->scenes[id];
  }
  return selected;
}

Scene *World::get_scene(string &name) {
  return get_scene_by_id(get_scene_id(name));
}

Scene *World::get_current_scene() {
  return get_scene_by_id(this->current_scene);
}

void World::add_scene(Scene *scene) {
  /* we need an array to temporary store the pointers
     to the scene objects, because we need to
     resize this->scenes */
  Scene **tmp = new Scene*[this->num_scenes];
  
  /* move the scenes over */
  for(int i = 0; this->num_scenes; i++) {
    tmp[i] = this->scenes[i];
  }
  
  delete [] this->scenes;
  
  /* reallocate memory */
  this->scenes = new Scene*[this->num_scenes + 1];

  /* copy the values back  */
  for(int i = 0; this->num_scenes; i++) {
    this->scenes[i] = tmp[i];
  }

  delete [] tmp;

  /* finally add the scene to the array */

  /* @TODO: check for scenes with the same name */
  this->scenes[++this->num_scenes] = scene;
}

void World::load_scene(string &name) {
  int id;
  Scene *current;

  /* unload the old scene */
  unload_scene();


  id = get_scene_id(name); /* @TODO: this may be -1, this needs some error handling */
  this->current_scene = id;

  current = get_scene_by_id(id); /* @TODO: this may be NULL, this needs some error handling */

  current->on_load();
}

void World::unload_scene() {
  /* unload the old scene */
  Scene *current = get_current_scene();
  if(current) {
    current->on_unload();
  }

  /* reset the current scene */
  this->current_scene = -1;
}