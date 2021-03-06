#define _METEOR_CPP
#include "Meteor.h"

Meteor::Meteor(scene::ISceneManager* smgr) {
  this->smgr = smgr;
  colliding_with = 0;
  velocity = 1;
  /* load the mesh only once from the disk */
  if(static_mesh == 0)  {
    static_mesh = smgr->getMesh("objects/player/meteor.x");
  }
  
  /* generate random size */
  size =  sqrt(sqrt((rand() / (float)RAND_MAX) * 100)) / 250;
  
  /* create the animated mesh */
  mesh = smgr->addAnimatedMeshSceneNode(static_mesh, smgr->getRootSceneNode(), 15, core::vector3df(0.0, 0.0, 0.0), core::vector3df(0.0, 0.0, 0.0), core::vector3df(size, size, size));
  mesh->setPosition(spawn_offset);
  
  /* attach lightning */
  if(mesh) {
    mesh->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    mesh->setMaterialFlag(video::EMF_LIGHTING, true); 
    attachRotateAnimator();
  }
  angle = genRandomAngle();
  update(1);
  angle += (rand() / (float)RAND_MAX) * (M_PI * 1.6f) + (M_PI * 0.2f);
  float force = 0.005;
  velocity = force / size;
  
}

void Meteor::attachRotateAnimator() {
  /* generate random coordinates for rotatino*/
  float rotatex = (rand() / (float)RAND_MAX) * 0.8f - 0.4f;
  float rotatey = (rand() / (float)RAND_MAX) * 0.8f - 0.4f;
  float rotatez = (rand() / (float)RAND_MAX) * 0.8f - 0.4f;
  
  /* create animation and attach it to the mesh */
  scene::ISceneNodeAnimator* anim_rotate = smgr->createRotationAnimator(core::vector3df(rotatex, rotatey, rotatez));
  mesh->addAnimator(anim_rotate);
  anim_rotate->drop();
}


float Meteor::calcTransitionTime(float distance) {
  return (distance / velocity) * 25000000;
}

Angle Meteor::genRandomAngle() {
  /* generate random angle in RAD */
  return Angle(((rand() / (float)RAND_MAX) * 360) / 180 * M_PI);
}

Angle Meteor::genRandomAngle(Angle before) {
  /* generate random angle in RAD but it should not be near `before` */
  Angle angle(0.0f), upper(0.0f), lower(0.0f);
  do {
    angle = genRandomAngle();
    upper = angle + M_PI_2;
    lower = angle - M_PI_2;
  } while(before < upper && before > lower);
  return angle;
}

scene::IAnimatedMeshSceneNode* Meteor::getMesh() {
  return mesh;
}

scene::IParticleSystemSceneNode *Meteor::createExplosion() {
  scene::IParticleSystemSceneNode *explosion = smgr->addParticleSystemSceneNode(false);
  scene::IParticleEmitter* em = explosion->createBoxEmitter(
    core::aabbox3d<f32>(-0.7,0,-0.7,0.7,0.1,0.7), // emitter size
    core::vector3df(0.0f,0.0f,0.0f),   // initial direction
    80,100,                             // emit rate
    video::SColor(0,255,255,0),       // darkest color
    video::SColor(0,255,255,255),       // brightest color
    800,2000,0,                         // min and max age, angle
    core::dimension2df(0.1f,0.0f),         // min size
    core::dimension2df(0.2f,0.2f));        // max size

  explosion->setEmitter(em); // this grabs the emitter

  scene::IParticleAffector* paf = explosion->createFadeOutParticleAffector();

  explosion->addAffector(paf); // same goes for the affector
  paf->drop();

  scene::ISceneNodeAnimator *del = smgr->createDeleteAnimator(200);
  explosion->addAnimator(del);
  del->drop();


  explosion->setRotation(core::vector3df(0.0,0.0,0.0));
  explosion->setScale(core::vector3df(1.0,1.0,0.1));
  explosion->setPosition(mesh->getPosition());
  explosion->setMaterialFlag(video::EMF_LIGHTING, true);
  explosion->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
  explosion->setMaterialTexture(0, smgr->getVideoDriver()->getTexture("objects/player/ship-effect.png"));
  explosion->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);

  return explosion;
}

bool Meteor::collidesWith(scene::ISceneNode* node, float NodeScale) {
  /* check if the bounding boxes collide */
  return node->getPosition()
          .getDistanceFrom(this->mesh->getPosition()) 
          < this->mesh->getScale()
          .X * 6 + node->getScale().X * NodeScale;
}

bool Meteor::collidesWith(Meteor* meteor) {
  bool back = collidesWith(meteor->getMesh(), 6);
  bool ret = back && colliding_with == 0;
  if(!back && meteor == colliding_with) {
    colliding_with = 0;
  } else if(back && meteor == 0) {
    colliding_with = meteor;
  }
  return ret;
}

void Meteor::bounceOf(Meteor* meteor) {
  core::vector3df pos1 = this->mesh->getPosition();
  core::vector3df pos2 = meteor->getMesh()->getPosition();
  bool vertical = false; 
  
  vertical = abs(pos1.X - pos2.X) < abs(pos1.Y - pos2.Y);
  if(!vertical) {
    angle -= M_PI_2;
  }
  angle = angle > M_PI ? M_PI - (angle - M_PI) + M_PI : M_PI - angle;
  
  if(!vertical) {
    angle += M_PI_2;
  }
}

void Meteor::update(float delta) {
  float radius = 2.2f;
  core::vector3df pos = this->mesh->getPosition();
  pos.X += sin(angle.getRAD()) * radius * velocity * delta;
  pos.Y += cos(angle.getRAD()) * radius * velocity * delta;
  pos.Z = 0.0f;
  this->mesh->setPosition(pos);
}

Meteor::~Meteor() {
  mesh->remove();
}

void Meteor::setSpawnOffset(core::vector3df spawn_offset) {
  Meteor::spawn_offset = spawn_offset;
}
