#ifndef DRAW_ZBUF_H 
#define DRAW_ZBUF_H
#include "xform.h"
#include "zbuffer.h"
#include "draw.h"
#define NDEBUG
void draw_wire_zbuffer(Wireframe wire,double** zbuffer,Vector eye_pos,Vector gaze_dir,Vector view_up,
		ViewVolume vol,int nx,int ny,Vector wmax,
		Vector wmin,Vector vmax,Vector vmin,
		Pixel** frame_buffer,int x_bound ,int y_bound);
void fillcolor_triangle_zbuf(Vector,Vector,Vector,Vector,Vector,Vector,Pixel,Pixel,Pixel,Pixel**,double**,int,int);
void fillcolor_convex_polygon_zbuf(Polygon,Polygon,Pixel**,double**,int,int);
#endif
