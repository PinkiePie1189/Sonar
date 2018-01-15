#ifndef SONAR_H
#define SONAR_H

#include "Fish.h"
#include "texture.h"
#include <vector>

struct Point
{
 int x,y;
};

class Sonar
{
 private:
 int scanning_index=0;
 std::vector<std::pair<Point,Point> > scanning_areas;
 std::vector<Texture*> scanning_areas_images;
 std::vector<int> scanning_areas_alpha;
 std::vector<bool> detected;
 std::vector<Fish> detected_objects;
 std::vector<int> spawn_time;
 Timer sqq,shoot_delay;

 public:
 void Load();
 void Start(Texture *_screen);
 void Update();
 void Print(Texture *_screen);
 void Shoot();
 void Check_collisions();
};

#endif // SONAR_H
