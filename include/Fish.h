#ifndef FISH_H
#define FISH_H

#include "texture.h"

extern const int RANGE;

void Load_images1();

class Fish
{
 private:
 int x,y;
 int dirx=0,diry=0;
 int type=0;

 public:
 int GetX();
 int GetY();
 int Get_type();
 void Set(int _x,int _y,int _type,int _dirx,int _diry);//Spawn
 void Update_Position();
 void Draw(Texture *_screen,bool detected);
};

#endif // FISH_H
