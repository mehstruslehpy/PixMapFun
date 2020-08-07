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
	if (argc!=12)
	{
		printf("argc/argv\n");
		printf("argc=%d\n",argc);
		return EXIT_FAILURE;
	}

	printf("DEBUG: Set up wireframe\n");
	Wireframe wire;
	read_wireframe_from_disk(argv[10],&wire);
	//debug_print_wireframe("DEBUG: wireframe read:",&wire);

	//end points for the axes
	Vector origin = (Vector){0,0,0};
	Vector xaxis_end1= (Vector){X_BOUND,0,0};
	Vector xaxis_end2= (Vector){-X_BOUND,0,0};
	Vector yaxis_end1= (Vector){0,Y_BOUND,0};
	Vector yaxis_end2= (Vector){0,-Y_BOUND,0};
	Vector zaxis_end1= (Vector){0,0,Z_BOUND};
	Vector zaxis_end2= (Vector){0,0,-Z_BOUND};


	printf("DEBUG: Projecting model to view\n");
	Vector eye_pos = (Vector){atof(argv[1]),atof(argv[2]),atof(argv[3])}; //position of eye
	Vector gaze_dir= (Vector){atof(argv[4]),atof(argv[5]),atof(argv[6])}; //position of eye
	Vector view_up = (Vector){atof(argv[7]),atof(argv[8]),atof(argv[9])}; //position of eye
	printf("\n");
	debug_print_vector("DEBUG:eye_pos:",&eye_pos);
	debug_print_vector("DEBUG:view_up:",&view_up);
	debug_print_vector("DEBUG:gaze_dir:",&gaze_dir);

	for (int i=0;i<wire.count;i++) 
		for (int j=0;j<wire.poly[i].count;j++) 
			wire.poly[i].pt[j]=perspective_project_vector(wire.poly[i].pt[j],eye_pos,gaze_dir,view_up,view_vol,nx,ny);

	Vector proj_origin= perspective_project_vector(origin,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	Vector proj_xaxis1= perspective_project_vector(xaxis_end1,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	Vector proj_xaxis2= perspective_project_vector(xaxis_end2,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	Vector proj_yaxis1= perspective_project_vector(yaxis_end1,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	Vector proj_yaxis2= perspective_project_vector(yaxis_end2,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	Vector proj_zaxis1= perspective_project_vector(zaxis_end1,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	Vector proj_zaxis2= perspective_project_vector(zaxis_end2,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	printf("\n");
	debug_print_view_volume1("DEBUG:View volume is:",&view_vol);
	debug_print_view_volume2("DEBUG:View volume is:",&view_vol);

	//does this even work?
	printf("DEBUG: Scaling to frame buffer coords\n");
	Vector vmax = (Vector){X_BOUND,Y_BOUND,0};
	Vector vmin = (Vector){0,0,0};
	Vector wmax= (Vector){nx-0.5,ny-0.5,0};
	Vector wmin = (Vector){-0.5,-0.5,0};
	
	printf("\n");
	debug_print_vector("DEBUG:Projected model wmax:",&wmax);
	debug_print_vector("DEBUG:Projected model wmin:",&wmin);
	debug_print_vector("DEBUG:Projected model vmax:",&vmax);
	debug_print_vector("DEBUG:Projected model vmin:",&vmin);
	printf("\n");

	for (int i=0;i<wire.count;i++) 
		for (int j=0;j<wire.poly[i].count;j++) 
			wire.poly[i].pt[j]=xform_window_to_view(wire.poly[i].pt[j],wmax,wmin,vmax,vmin);

	Vector scale_origin=xform_window_to_view(proj_origin,wmax,wmin,vmax,vmin);
	Vector scale_xaxis1=xform_window_to_view(proj_xaxis1,wmax,wmin,vmax,vmin);
	Vector scale_xaxis2=xform_window_to_view(proj_xaxis2,wmax,wmin,vmax,vmin);
	Vector scale_yaxis1=xform_window_to_view(proj_yaxis1,wmax,wmin,vmax,vmin);
	Vector scale_yaxis2=xform_window_to_view(proj_yaxis2,wmax,wmin,vmax,vmin);
	Vector scale_zaxis1=xform_window_to_view(proj_zaxis1,wmax,wmin,vmax,vmin);
	Vector scale_zaxis2=xform_window_to_view(proj_zaxis2,wmax,wmin,vmax,vmin);

	printf("DEBUG: Draw model.\n");
	//draw axes first to prevent overlapping wrong
	draw_line(white,scale_xaxis1,scale_xaxis2,frame_buffer,X_BOUND,Y_BOUND);
	draw_line(white,scale_yaxis1,scale_yaxis2,frame_buffer,X_BOUND,Y_BOUND);
	draw_line(white,scale_zaxis1,scale_zaxis2,frame_buffer,X_BOUND,Y_BOUND);

	//draw_polygon(green,poly,frame_buffer,X_BOUND,Y_BOUND);
	//draw_wireframe(green,wire,frame_buffer,X_BOUND,Y_BOUND);
	//color polygons in wireframe
	for (int i=0;i<wire.count;i++)
	{
		Pixel color = {rand()%255,rand()%255,rand()%255};
		draw_polygon(color,wire.poly[i],frame_buffer,X_BOUND,Y_BOUND);
		fillcolor_convex_polygon(color,wire.poly[i],frame_buffer,X_BOUND,Y_BOUND);
	}


	printf("DEBUG: Write framebuffer to disk.\n");
	write_fbuf_to_disk(argv[11],frame_buffer,X_BOUND,Y_BOUND);

	printf("DEBUG: Done.\n");
	dealloc_framebuf(frame_buffer,X_BOUND);
	dealloc_wire(&wire);

	//some coords for debugging
	printf("\n");
	debug_print_vector("DEBUG:Unprojected model origin",&origin);
	debug_print_vector("DEBUG:Projected model proj_origin:",&proj_origin);
	debug_print_vector("DEBUG:Scaled model origin:",&scale_origin);
	printf("\n");

	debug_print_vector("DEBUG:Unprojected model xaxis_end",&xaxis_end1);
	debug_print_vector("DEBUG:Projected model proj_xaxis:",&proj_xaxis1);
	debug_print_vector("DEBUG:Scaled model proj_xaxis:",&scale_xaxis1);
	printf("\n");

	debug_print_vector("DEBUG:Unprojected model yaxis_end",&yaxis_end1);
	debug_print_vector("DEBUG:Projected model proj_yaxis:",&proj_yaxis1);
	debug_print_vector("DEBUG:Scaled model proj_yaxis:",&scale_yaxis1);
	printf("\n");

	debug_print_vector("DEBUG:Unprojected model zaxis_end",&zaxis_end1);
	debug_print_vector("DEBUG:Projected model proj_zaxis:",&proj_zaxis1);
	debug_print_vector("DEBUG:Scaled model proj_zaxis:",&scale_zaxis1);
	printf("\n");
	
	return EXIT_SUCCESS;
}
