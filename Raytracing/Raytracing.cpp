//============================================================================
// Name        : Raytracing.cpp
// Author      : kllrnohj
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

// SDL_GL_Functions: Copied from gpwiki.org.
#include <cstdlib> // For some useful functions such as atexit :)
#include <cmath>
#include <cstdio>
#include <iostream>
#include "SDL.h" // main SDL header
using namespace std;

//OpenGL stuff :)
#include <GL/gl.h>
#include <GL/glu.h>

#include "Vector.h"
#include "Ray.h"
#include "Camera.h"
#include "Plane.h"
#include "Sphere.h"
#include "Primitive.h"

SDL_Surface *screen;

#define screen_width 640
#define screen_height 480
#define true 1
#define false 0 //You might have to declare True and False.

//Globals to simplify things :)
Camera cam;
int nrPrimitives = 1;
Primitive* scene[5];

//prototypes:
int InitVideo();
int setup_opengl( int width, int height );
void eventLoop();
void draw();

//might want to have some parms, options for user
int InitVideo() {
  // Load SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
    return false;
  }
  atexit(SDL_Quit); // Clean it up nicely :)

  // fullscreen can be toggled at run time :) any you might want to change the flags with params?
  //set the main screen to SCREEN_WIDTHxSCREEN_HEIGHT with a colour depth of 16:
  screen = SDL_SetVideoMode(screen_width, screen_height, 0, SDL_DOUBLEBUF | SDL_SWSURFACE);
  if (screen == NULL) {
    fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
    return false;
  }
  return true;
}

// ------------------- Drawing ---------------------


void draw()
{
	double inc_w = cam.getWindow().getWidth() / screen_width;
	double inc_h = cam.getWindow().getHeight() / screen_height;
	//start at top-left corner
	double loc_x = cam.getWindow()[0].X();
	double loc_y = cam.getWindow()[0].Y();
	Uint8   *bits, bpp;
	bpp = screen->format->BytesPerPixel;
	for (int w = 0; w < screen_width; w++)
	{
		for (int h = 0; h < screen_height; h++)
		{
			double px = w * inc_w + loc_x;
			double py = loc_y - h * inc_h;
			Vector dir = Vector(px, py, cam.getWindow().getZAt(px, py)) - cam.getOrigin();
			dir = dir.normalize();
			Ray ray(cam.getOrigin(), dir);

			Primitive* closest = 0;
			double cdist = -1;
			for (int pi = 0; pi < nrPrimitives; pi++)
			{
				Primitive* p = scene[pi];
				double dist = cdist;
				if (p->Intersect(ray, dist))
				{
					if (cdist == -1 || dist < cdist)
					{
						closest = p;
						cdist = dist;
					}
				}
			}
			Uint32 pixel;
			if (cdist > -1)
			{
				Color a_Acc(0,0,0);
				// determine color at point of intersection
				Vector pi = ray.Origin() + ray.Direction() * cdist;
				// trace lights
				for ( int l = 0; l < nrPrimitives; l++ )
				{
					Primitive* p = scene[l];
					if (p->IsLight()) 
					{
						Primitive* light = p;
						// calculate diffuse shading

						Vector L = ((Sphere*)light)->GetCenter() - pi;
						L = L.normalize(1);
						Vector N = closest->GetNormal( pi );
						if (closest->GetMaterial().GetDiffuse() > 0)
						{
							double dot = N.DotProduct(L);
							if (dot > 0)
							{
								double diff = dot * closest->GetMaterial().GetDiffuse();
								// add diffuse component to ray color

								Color pc = closest->GetMaterial().GetColor();
								Color lc = light->GetMaterial().GetColor();
								a_Acc.R += sqrt(pc.R * lc.R) * diff;
								a_Acc.G += sqrt(pc.G * lc.G) * diff;
								a_Acc.B += sqrt(pc.B * lc.B) * diff;
								a_Acc.A += sqrt(pc.A * lc.A) * diff;
								if (a_Acc.R > 255) a_Acc.R = 255;
								if (a_Acc.G > 255) a_Acc.G = 255;
								if (a_Acc.B > 255) a_Acc.B = 255;
								if (a_Acc.A > 255) a_Acc.A = 255;
							}
						}
					}
				}
				pixel = SDL_MapRGB(screen->format, a_Acc.R, a_Acc.G, a_Acc.B);
			} else {
				pixel = SDL_MapRGB(screen->format, 40, 40, 120);
			}
			bits = ((Uint8 *)screen->pixels)+h*screen->pitch+w*bpp;
			switch(bpp) {
			case 1:
				*((Uint8 *)(bits)) = (Uint8)pixel;
				break;
			case 2:
				*((Uint16 *)(bits)) = (Uint16)pixel;
				break;
			case 3: { /* Format/endian independent */
				Uint8 r, g, b;

				r = (pixel>>screen->format->Rshift)&0xFF;
				g = (pixel>>screen->format->Gshift)&0xFF;
				b = (pixel>>screen->format->Bshift)&0xFF;
				*((bits)+screen->format->Rshift/8) = r; 
				*((bits)+screen->format->Gshift/8) = g;
				*((bits)+screen->format->Bshift/8) = b;
			}
			break;
			case 4:
				*((Uint32 *)(bits)) = (Uint32)pixel;
				break;
			}
		}
	}
	SDL_Flip(screen);
}

// -------------------- The Event Loop -------------
void eventLoop()
{
	bool running = true;
	SDL_Event event;
	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				break;
			case SDL_QUIT:
				running = false;
				SDL_Quit();
				return;
			}
		}
		draw();
		SDL_Delay(500);
		//running = false;
		//SDL_Quit();
	}
}

// -------------------- The Game -------------------

int main(int argc, char **argv) {
	Vector v(1,2,3);
	Vector z(5,6,7);
	z += v;
	if (InitVideo() == false) return 1;
	
	// init scene
	Sphere* s = new Sphere(Vector(0, .5, 2), 1);
	Color scr(255,0,0);
	s->GetMaterial().SetColor(scr);
	s->GetMaterial().SetDiffuse(.5);
	scene[0] = (Primitive*) s;

	s = new Sphere(Vector(-4, -2, 1.2), 1.3);
	Color scg(0,255,0);
	s->GetMaterial().SetColor(scg);
	s->GetMaterial().SetDiffuse(.5);
	scene[2] = (Primitive*) s;

	s = new Sphere(Vector(3, 4, 6), 1);
	Color scb(0,0,255);
	s->GetMaterial().SetColor(scb);
	s->GetMaterial().SetDiffuse(.5);
	scene[3] = (Primitive*) s;
	
	Sphere* l = new Sphere(Vector(3, 7, -4), .3, 1);
	Color lc(255, 255, 255);
	l->GetMaterial().SetColor(lc);
	scene[1] = (Primitive*) l;
	
	nrPrimitives = 4;
	
	eventLoop();
	return 0;
}

