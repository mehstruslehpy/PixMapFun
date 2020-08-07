#include "buffer.h"
#include <time.h>

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
	if (argc!=13)
	{
		printf("argc/argv\n");
		printf("argc=%d\n",argc);
		return EXIT_FAILURE;
	}

	printf("DEBUG: Set up wireframe\n");
	Wireframe wire;
	read_wireframe_from_disk(argv[10],argv[11],&wire);
	BSPNode* bsp_tree = build_bsp_tree_untriangulated(wire);

	//end points for the axes
	Vector origin = (Vector){0,0,0};
	Vector xaxis_end1= (Vector){X_BOUND,0,0};
	Vector xaxis_end2= (Vector){-X_BOUND,0,0};
	Vector yaxis_end1= (Vector){0,Y_BOUND,0};
	Vector yaxis_end2= (Vector){0,-Y_BOUND,0};
	Vector zaxis_end1= (Vector){0,0,Z_BOUND};
	Vector zaxis_end2= (Vector){0,0,-Z_BOUND};

	printf("DEBUG: Projecting axes to view\n");
	Vector eye_pos = (Vector){atof(argv[1]),atof(argv[2]),atof(argv[3])}; //position of eye
	Vector gaze_dir= (Vector){atof(argv[4]),atof(argv[5]),atof(argv[6])}; //position of eye
	Vector view_up = (Vector){atof(argv[7]),atof(argv[8]),atof(argv[9])}; //position of eye
	printf("\n");

	Vector proj_origin= perspective_project_vector(origin,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	Vector proj_xaxis1= perspective_project_vector(xaxis_end1,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	Vector proj_xaxis2= perspective_project_vector(xaxis_end2,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	Vector proj_yaxis1= perspective_project_vector(yaxis_end1,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	Vector proj_yaxis2= perspective_project_vector(yaxis_end2,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	Vector proj_zaxis1= perspective_project_vector(zaxis_end1,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	Vector proj_zaxis2= perspective_project_vector(zaxis_end2,eye_pos,gaze_dir,view_up,view_vol,nx,ny);

	printf("DEBUG: Scaling to frame buffer coords\n");
	Vector vmax = (Vector){X_BOUND,Y_BOUND,0};
	Vector vmin = (Vector){0,0,0};
	Vector wmax= (Vector){nx-0.5,ny-0.5,0};
	Vector wmin = (Vector){-0.5,-0.5,0};

	Vector scale_origin=xform_window_to_view(proj_origin,wmax,wmin,vmax,vmin);
	Vector scale_xaxis1=xform_window_to_view(proj_xaxis1,wmax,wmin,vmax,vmin);
	Vector scale_xaxis2=xform_window_to_view(proj_xaxis2,wmax,wmin,vmax,vmin);
	Vector scale_yaxis1=xform_window_to_view(proj_yaxis1,wmax,wmin,vmax,vmin);
	Vector scale_yaxis2=xform_window_to_view(proj_yaxis2,wmax,wmin,vmax,vmin);
	Vector scale_zaxis1=xform_window_to_view(proj_zaxis1,wmax,wmin,vmax,vmin);
	Vector scale_zaxis2=xform_window_to_view(proj_zaxis2,wmax,wmin,vmax,vmin);

	printf("DEBUG: Draw axes.\n");
	//draw axes first to prevent overlapping above the real things we want to draw
	draw_line(white,scale_xaxis1,scale_xaxis2,frame_buffer,X_BOUND,Y_BOUND);
	draw_line(white,scale_yaxis1,scale_yaxis2,frame_buffer,X_BOUND,Y_BOUND);
	draw_line(white,scale_zaxis1,scale_zaxis2,frame_buffer,X_BOUND,Y_BOUND);

	printf("DEBUG: Draw model.\n");
	draw_bsp_tree(bsp_tree,eye_pos,gaze_dir,view_up,view_vol,nx,ny,wmax,wmin,vmax,vmin,frame_buffer,X_BOUND,Y_BOUND);

	printf("DEBUG: Write framebuffer to disk.\n");
	write_fbuf_to_disk(argv[12],frame_buffer,X_BOUND,Y_BOUND);

	printf("DEBUG: Done.\n");
	dealloc_framebuf(frame_buffer,X_BOUND);
	dealloc_wire(&wire);
	delete_bsp_tree(bsp_tree);
	printf("\n");

	return EXIT_SUCCESS;
}
