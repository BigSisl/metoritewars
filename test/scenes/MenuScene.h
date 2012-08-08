#ifndef MENUSCENE_H
#define	MENUSCENE_H

#include "../../src/Scene.h"

class MenuScene : public Scene {
  public: 
    MenuScene(World *world);
    virtual void onLoad();
    virtual void onDraw();
    virtual void onUnload();

};

#endif	/* MENUSCENE_H */
