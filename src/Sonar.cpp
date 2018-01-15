#include "Sonar.h"
#include <cmath>

Texture *background;
const int RESOLUTION_W=1366,RESOLUTION_H=768;

void Sonar::Load()
{
 background=Load_Transparent_Texture("img/bck.png");
 FILE *in=fopen("res/areas.txt","r");
 while(!feof(in))
       {
        std::pair<Point,Point> area;
        fscanf(in,"%d %d %d %d ",&area.first.x,&area.first.y,&area.second.x,&area.second.y);
        scanning_areas.push_back(area);
        Texture *image=Create_Transparent_Texture(RESOLUTION_W,RESOLUTION_H);
        scanning_areas_images.push_back(image);
        scanning_areas_alpha.push_back(0);
        detected.push_back(false);
       }
 fclose(in);
 in=fopen("res/objects.txt","r");
 int number_of_fish,number_of_torpedo;
 fscanf(in,"%d %d ",&number_of_fish,&number_of_torpedo);
 for(int i=0;i<number_of_fish;i++)
     {
      Fish aux;
      aux.Set(rand()%RANGE*((rand()%2)?(-1):1),rand()%RANGE*((rand()%2)?(-1):1),1,0,0);
      detected_objects.push_back(aux);
      spawn_time.push_back(0);
     }
 for(int i=0;i<number_of_torpedo;i++)
     {
      Fish aux;
      int x,y,dirx,diry,spawn;
      fscanf(in,"%d %d %d %d %d ",&x,&y,&dirx,&diry,&spawn);
      aux.Set(x,y,2,dirx,diry);
      detected_objects.push_back(aux);
      spawn_time.push_back(spawn);
     }
 fclose(in);
 sqq.start();
 shoot_delay.start();
}

void Sonar::Update()
{
 std::vector<Fish>::iterator it=detected_objects.begin();
 for(int i=0;i<detected_objects.size();i++,it++)
     {
      if(sqq.get_ticks()>spawn_time[i])
         {
          detected_objects[i].Update_Position();
          if((detected_objects[i].GetX()>RANGE || detected_objects[i].GetX()<(-RANGE) || detected_objects[i].GetY()>RANGE || detected_objects[i].GetY()<(-RANGE)))
             detected_objects.erase(it);
          if(detected_objects[i].Get_type()!=1 && sqq.get_ticks()-spawn_time[i]>=1000)
             detected[i]=true;
         }
     }
}

int Get_Area(Point A,Point B,Point C)
{
 int ret=std::abs((A.x*B.y+B.x*C.y+C.x*A.y-C.x*B.y-A.x*C.y-A.y*B.x)/2);
 return ret;
}

const int MIDX=1366/2,MIDY=768/2;

void Sonar::Print(Texture *_screen)
{
 Apply_Texture(0,0,background,_screen);
 Destroy_Texture(scanning_areas_images[scanning_index]);
 scanning_areas_images[scanning_index]=Create_Transparent_Texture(RESOLUTION_W,RESOLUTION_H);
 DrawLine(MIDX,MIDY,scanning_areas[scanning_index].first.x+MIDX,scanning_areas[scanning_index].first.y+MIDY,{0xFF,0xFF,0xFF},_screen);
 DrawLine(MIDX,MIDY,scanning_areas[scanning_index].second.x+MIDX,scanning_areas[scanning_index].second.y+MIDY,{0xFF,0xFF,0xFF},_screen);
 for(int i=0;i<detected_objects.size();i++)
     {
      if(sqq.get_ticks()>spawn_time[i])
         {
          Point A;
          A.x=detected_objects[i].GetX();
          A.y=detected_objects[i].GetY();
          if(Get_Area({0,0},scanning_areas[scanning_index].first,scanning_areas[scanning_index].second)==
             Get_Area({0,0},A,scanning_areas[scanning_index].first)+Get_Area({0,0},A,scanning_areas[scanning_index].second)+
             Get_Area(A,scanning_areas[scanning_index].first,scanning_areas[scanning_index].second))
             detected_objects[i].Draw(scanning_areas_images[scanning_index],detected[i]);
         }
     }
 scanning_areas_alpha[scanning_index]=256;
 for(int i=0;i<scanning_areas.size();i++)
     {
      if(i==scanning_index)
         continue;
      Set_Texture_Alpha(scanning_areas_images[i],scanning_areas_alpha[i]);
      Apply_Texture(0,0,scanning_areas_images[i],_screen);
      scanning_areas_alpha[i]-=256/16;
      scanning_areas_alpha[i]=std::max(0,scanning_areas_alpha[i]);
     }
 //Set_Texture_Alpha(scanning_areas_images[scanning_index],scanning_areas_alpha[scanning_index]);
 Apply_Texture(0,0,scanning_areas_images[scanning_index],_screen);
}

void Sonar::Start(Texture *_screen)
{
 Load();
 bool quit=false;
 SDL_Event event;
 Timer sqqq;
 sqqq.start();
 while(!quit)
       {
        SDL_PollEvent(&event);
        if(event.type==SDL_QUIT || (event.type==SDL_KEYDOWN && event.key.keysym.scancode==SDL_SCANCODE_ESCAPE))
           quit=true;
        if(event.type==SDL_KEYDOWN && event.key.keysym.scancode==SDL_SCANCODE_SPACE)
           Shoot();
        Print(_screen);
        Flip_Buffers(_screen);
        SDL_Delay(100);
        Update();
        Check_collisions();
        if(sqqq.get_ticks()>=200)
           {
            scanning_index=(scanning_index+1)%scanning_areas.size();
            sqqq.start();
           }
       }
}

int Get_distance(int x1,int y1,int x2,int y2)
{
 int dx=std::abs(x1-x2),dy=std::abs(y1-y2);
 return std::sqrt(dx*dx+dy*dy);
}

int Gcd(int a,int b)
{
 a=std::abs(a);
 b=std::abs(b);
 int r=a%b;
 while(r!=0)
       {
        a=b;
        b=r;
        r=a%b;
       }
 return b;
}

void Sonar::Shoot()
{
 if(shoot_delay.get_ticks()<1000)
    return;
 int dmin=10000,index=-1;
 for(int i=0;i<detected_objects.size();i++)
     {
      if(detected_objects[i].Get_type()==2)
         {
          int dist=Get_distance(0,0,detected_objects[i].GetX(),detected_objects[i].GetY());
          if(dist<dmin)
             {
              dmin=dist;
              index=i;
             }
         }
     }
 if(dmin<=200 && index!=-1)
    {
     Fish aux;
     int dirx,diry;
     dirx=detected_objects[index].GetX();
     diry=detected_objects[index].GetY();
     int gcd=Gcd(dirx,diry);
     dirx/=gcd;
     diry/=gcd;
     aux.Set(dirx,diry,3,dirx,diry);
     detected_objects.push_back(aux);
     spawn_time.push_back(sqq.get_ticks());
     shoot_delay.start();
    }
}

void Sonar::Check_collisions()
{
 for(int i=0;i<detected_objects.size()-1;i++)
     {
      if(detected_objects[i].Get_type()==1)
         continue;
      for(int j=i+1;j<detected_objects.size();j++)
          {
           if(detected_objects[i].Get_type()!=1 || detected_objects[j].Get_type()!=1)
              {
               if(std::abs(detected_objects[i].GetX()-detected_objects[j].GetX())<=4 && std::abs(detected_objects[i].GetY()-detected_objects[j].GetY())<=4)
                  {
                   if(detected_objects[i].Get_type()!=1 && detected_objects[i].Get_type()!=1)
                      {
                       detected.erase(detected.begin()+j);
                       detected_objects.erase(detected_objects.begin()+j);
                       spawn_time.erase(spawn_time.begin()+j);

                       detected.erase(detected.begin()+i);
                       detected_objects.erase(detected_objects.begin()+i);
                       spawn_time.erase(spawn_time.begin()+i);
                       break;
                      }
                   int winner,loser;
                   if(detected_objects[i].Get_type()!=1)
                      winner=i,loser=j;
                   else
                      winner=j,loser=i;
                   detected.erase(detected.begin()+loser);
                   detected_objects.erase(detected_objects.begin()+loser);
                   spawn_time.erase(spawn_time.begin()+loser);
                   if(loser==i)
                      break;
                  }
              }
          }
     }
}
