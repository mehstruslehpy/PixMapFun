#ifndef DRAW_H 
#define DRAW_H
#include "xform.h"
#define NDEBUG
//draw various things
void draw_triangle(Pixel,Triangle,Pixel**,int,int);
void draw_segment(Pixel,Vector,Vector,Pixel**,int,int);
void draw_line(Pixel,Vector,Vector,Pixel**,int,int);
void draw_polygon(Pixel,Polygon,Pixel**,int,int);
//draw a bsp tree using a perspective projection
void draw_bsp_tree_no_wire(BSPNode* root,Vector eye_pos,Vector gaze_dir,Vector view_up,
		ViewVolume vol,int nx,int ny,Vector wmax,
		Vector wmin,Vector vmax,Vector vmin,
		Pixel** frame_buffer,int x_bound ,int y_bound);
//draw a bsp tree with wireframes using a perspective projection
void draw_bsp_tree_wire(BSPNode* root,Pixel wire_color,Vector eye_pos,Vector gaze_dir,Vector view_up,
		ViewVolume vol,int nx,int ny,Vector wmax,
		Vector wmin,Vector vmax,Vector vmin,
		Pixel** frame_buffer,int x_bound ,int y_bound);
//draw x,y,z axes using perspective projection
//TODO: for some reason the axes don't always draw
void draw_axes_perspective(Pixel color,Vector eye_pos,Vector gaze_dir,Vector view_up,
		ViewVolume vol,int nx,int ny,Vector wmax,
		Vector wmin,Vector vmax,Vector vmin,
		Pixel** frame_buffer,int x_bound ,int y_bound,int z_bound);
void draw_wireframe_bsp_tree(Pixel,BSPNode*,Vector,Pixel**,int,int);
//read in an array of coords x0 y0 x1 y1 ... and plot line segments connecting them to the frame buffer
void bresenham_lines_array(Pixel,double *,int,Pixel**,int,int);
//plot a line segment between two points
void bresenham_plotline(Pixel,double,double,double,double,Pixel**,int,int);
//fill regions these do not have separate options for outlines
//we will follow the triangle rasterizing algorithm from the Shirley book
void fillcolor_triangle(Vector,Vector,Vector,Pixel,Pixel,Pixel,Pixel**,int,int);
//any concave polygon can be decomposed into triangles hence we can use the above function to
//implement arbitrary convex polygons
void fillcolor_convex_polygon(Polygon,Pixel**,int,int);
//later we can try to break convex polygons into concave polygons and apply fill in terms of these
#endif
