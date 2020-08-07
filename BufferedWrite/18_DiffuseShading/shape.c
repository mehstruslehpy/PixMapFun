#include <time.h>
#include "bsptree.h"
#include "buffer.h"
#include "draw.h"
#include "file.h"
#include "matrix.h"
#include "polygon.h"
#include "vector.h"
#include "wireframe.h"
#include "xform.h"
#include "draw_zbuf.h"
#include "draw_shaded.h"
#include "zbuffer.h"
int main(int argc, char** argv)
{
	printf("DEBUG: Set up frame buffer and view volume.\n");
	Pixel white = {.red=128,.grn=128,.blu=128};
	Pixel black = {.red=0,.grn=0,.blu=0};
	Pixel red = {.red=255,.grn=0,.blu=0};
	Pixel blue= {.red=0,.grn=0,.blu=255};
	Pixel green= {.red=0,.grn=255,.blu=0};
	Pixel** frame_buffer=alloc_framebuf(X_BOUND,Y_BOUND);
	fill_fbuf_color(black,frame_buffer,X_BOUND,Y_BOUND);
	ViewVolume view_vol = (ViewVolume){.l=0,.r=X_BOUND,.t=0,.b=Y_BOUND,.n=-0.5*Z_BOUND,.f=0.5*Z_BOUND};
	double nx = 1;
	double ny = 1;

	//double check the argc values
	if (argc!=19)
	{
		printf("argc/argv\n");
		printf("argc=%d\n",argc);
		return EXIT_FAILURE;
	}

	printf("DEBUG: Set up wireframe\n");
	Wireframe wire;
	read_wireframe_from_disk(argv[16],argv[17],&wire);

	printf("DEBUG: Set up BSP tree\n");
	BSPNode* bsp_tree = build_bsp_tree_untriangulated(wire);

	printf("DEBUG: Set up view vectors\n");
	Vector eye_pos = (Vector){atof(argv[1]),atof(argv[2]),atof(argv[3])}; //position of eye
	Vector gaze_dir= (Vector){atof(argv[4]),atof(argv[5]),atof(argv[6])}; //gaze direction of eye
	Vector view_up = (Vector){atof(argv[7]),atof(argv[8]),atof(argv[9])}; //view up vector
	//pos of light source
	Vector light_source = (Vector){atof(argv[10]),atof(argv[11]),atof(argv[12])};
	double c_r=atof(argv[13]); //diffuse reactance 
	double c_l=atof(argv[14]); //light intensity
	double c_a=atof(argv[15]); //ambient light
	

	printf("DEBUG: Set up view to window vectors\n");
	Vector vmax = (Vector){X_BOUND,Y_BOUND,0};
	Vector vmin = (Vector){0,0,0};
	Vector wmax= (Vector){nx-0.5,ny-0.5,0};
	Vector wmin = (Vector){-0.5,-0.5,0};

	printf("DEBUG: Draw axes.\n");
	draw_axes_perspective(white,eye_pos,gaze_dir,view_up,view_vol,nx,ny,wmax,wmin,vmax,vmin,frame_buffer,X_BOUND,Y_BOUND,Z_BOUND);

	printf("DEBUG: Draw model.\n");
	//draw_bsp_tree_no_wire(bsp_tree,eye_pos,gaze_dir,view_up,view_vol,nx,ny,wmax,wmin,vmax,vmin,frame_buffer,X_BOUND,Y_BOUND);
	//draw_bsp_tree_wire(bsp_tree,black,eye_pos,gaze_dir,view_up,view_vol,nx,ny,wmax,wmin,vmax,vmin,frame_buffer,X_BOUND,Y_BOUND);
	draw_diffuse_shaded_bsp_tree_wire(bsp_tree,black,eye_pos,gaze_dir,view_up,light_source,c_r,c_l,c_a,view_vol,nx,ny,wmax,wmin,vmax,vmin,frame_buffer,X_BOUND,Y_BOUND);

	printf("DEBUG: Write framebuffer to disk.\n");
	write_fbuf_to_disk(argv[18],frame_buffer,X_BOUND,Y_BOUND);

	printf("DEBUG: Done.\n");
	dealloc_framebuf(frame_buffer,X_BOUND);
	dealloc_wire(&wire);
	delete_bsp_tree(bsp_tree);
	printf("\n");

	return EXIT_SUCCESS;
}
