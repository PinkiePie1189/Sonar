#ifndef TEXTURE_H
#define TEXTURE_H

#include "library.h"

class Texture
{
 public:
 int w,h;
 SDL_Texture *image=NULL;
};

Texture *Load_Texture(char *filename);
Texture *Load_Transparent_Texture(char *filename);
Texture *Create_TTF_Texture(TTF_Font *font,std::string text,SDL_Color color);
Texture *Create_Transparent_Texture(int w,int h);

void Apply_Texture(int x,int y,Texture *source,Texture *destination=NULL);
void Apply_Texture(int x,int y,int w,int h,Texture *source,Texture *destination=NULL);
void Apply_Texture(int xImage,int yImage,int xScreen,int yScreen,int w,int h,Texture *source,Texture *destination=NULL);

void Apply_Stretched_Texture(int x,int y,int w,int h,Texture *source=NULL,Texture *destination=NULL);

void Set_Texture_Blend_Mode(Texture *_texture,SDL_BlendMode blend_mode);
void Set_Texture_Alpha(Texture *_texture,Uint8 alpha);
void Destroy_Texture(Texture *_texture);

extern Texture *SCREEN,*last_frame;
void Flip_Buffers(Texture *screen);

void DrawRect(SDL_Rect _rect,SDL_Color _color,Texture *_screen);
void FillRect(SDL_Rect _rect,SDL_Color _color,Texture *_screen);
void DrawBorder(SDL_Rect _rect,SDL_Color _color,int thickness,Texture *_screen);
void DrawLine(int x1,int y1,int x2,int y2,SDL_Color _color,Texture* _screen);
#endif // TEXTURE_H
