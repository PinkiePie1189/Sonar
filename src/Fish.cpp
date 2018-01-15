#include "Fish.h"

Texture *fish_image,*torpedo_image,*friendly_torpedo_image;

const int RANGE=300;

void Load_images1()
{
 fish_image=Load_Transparent_Texture("img/fish.png");
 torpedo_image=Load_Transparent_Texture("img/torpedo.png");
 friendly_torpedo_image=Load_Transparent_Texture("img/friendly torpedo.png");
}

int Fish::GetX()
{
 return x;
}

int Fish::GetY()
{
 return y;
}

int Fish::Get_type()
{
 return type;
}

void Fish::Set(int _x,int _y,int _type,int _dirx,int _diry)
{
 x=_x;
 y=_y;
 type=_type;
 dirx=_dirx;
 diry=_diry;
}

void Fish::Update_Position()
{
 if(type==1)//Fish
    {
     if(rand()%101<=10)
        {
         int sq=rand()%2,semn=1;
         int _x=x,_y=y;
         for(int i=0;i<sq;i++)
             semn*=-1;
         _x+=semn*5;
         sq=rand()%2,semn=1;
         for(int i=0;i<sq;i++)
             semn*=-1;
         _y+=semn*5;
         if(!(_x>RANGE || _x<(-RANGE) || _y>RANGE || _y<(-RANGE)))
            x=_x,y=_y;
        }
    }
 else//Torpedo
    {
     x+=dirx;
     y+=diry;
    }
}

const int MIDX=1366/2,MIDY=768/2;

void Fish::Draw(Texture *_screen,bool detected)
{
 if(type==3)
    Apply_Texture(MIDX+x,MIDY+y,friendly_torpedo_image,_screen);
 else
    Apply_Texture(MIDX+x,MIDY+y,(type==1 || !detected)?fish_image:torpedo_image,_screen);
}
