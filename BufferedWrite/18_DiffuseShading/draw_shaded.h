#ifndef DRAW_SHADED_H 
#define DRAW_SHADED_H
#include "draw.h"
#define NDEBUG
//draw various things

void draw_diffuse_shaded_bsp_tree_wire(BSPNode* root,Pixel wire_color,Vector eye_pos,Vector gaze_dir,Vector view_up,
		Vector l, double c_r,double c_l,double c_a,
		ViewVolume vol,int nx,int ny,Vector wmax,
		Vector wmin,Vector vmax,Vector vmin,
		Pixel** frame_buffer,int x_bound ,int y_bound);

void fillcolor_diffuse_shaded_triangle(Vector,Vector,Vector,Vector,Vector,Vector,Pixel,Pixel,Pixel,Vector,double,double,double,Pixel**,int,int);
//any concave polygon can be decomposed into triangles hence we can use the above function to
//implement arbitrary convex polygons
void fillcolor_diffuse_shaded_convex_polygon(Polygon,Polygon,Vector,double,double,double,Pixel**,int,int);
//later we can try to break convex polygons into concave polygons and apply fill in terms of these
#endif
