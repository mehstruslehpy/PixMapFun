#include "buffer.h"
#include <time.h>
int main(int argc, char** argv)
{
	printf("DEBUG: Set up frame buffer and view volume.\n");
	Pixel white= {.red=128,.grn=128,.blu=128};
	Pixel black = {.red=0,.grn=0,.blu=0};
	Pixel red = {.red=255,.grn=0,.blu=0};
	Pixel blue= {.red=0,.grn=0,.blu=255};
	Pixel** frame_buffer=alloc_framebuf(X_BOUND,Y_BOUND);
	fill_fbuf_color(black,frame_buffer,X_BOUND,Y_BOUND);
	ViewVolume view_vol = (ViewVolume){0,X_BOUND,0,Y_BOUND,0,Z_BOUND};
	double nx = 1;
	double ny = 1;

	printf("DEBUG: Set up pyramid\n");
	Triangle base = (Triangle) {
		(Tuple){0,0,0},	
			(Tuple){0,500,0},	
			(Tuple){500,0,0},	
	};
	Tuple tip = (Tuple){100,200,500};

	Tuple origin = (Tuple){0,0,0};
	Tuple xaxis_end= (Tuple){X_BOUND,0,0};
	Tuple yaxis_end= (Tuple){0,Y_BOUND,0};
	Tuple zaxis_end= (Tuple){0,0,Z_BOUND};

	printf("DEBUG: Projecting pyramid to view\n");
	//Tuple eye_pos = (Tuple){X_BOUND,Y_BOUND,Z_BOUND}; //position of eye
	Tuple eye_pos = (Tuple){atof(argv[1]),atof(argv[2]),atof(argv[3])}; //position of eye
	Tuple gaze_dir = (Tuple){atof(argv[4]),atof(argv[5]),atof(argv[6])}; //position of eye
	//Tuple gaze_dir = tip; //direction of gaze
	//Tuple view_up = (Tuple){0,0,1}; //up is in the z direction
	Tuple view_up = (Tuple){atof(argv[7]),atof(argv[8]),atof(argv[9])}; //position of eye
	debug_print_tuple("DEBUG:eye_pos:",&eye_pos);
	debug_print_tuple("DEBUG:view_up:",&view_up);
	debug_print_tuple("DEBUG:gaze_dir:",&gaze_dir);

	//Triangle proj_base = ortho_project_triangle(base,eye_pos,gaze_dir,view_up,view_vol,X_BOUND,Y_BOUND);
	Triangle proj_base = project_triangle(base,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	//Tuple proj_tip = ortho_project_tuple(tip,eye_pos,gaze_dir,view_up,view_vol,X_BOUND,Y_BOUND);
	Tuple proj_origin = project_tuple(origin,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	Tuple proj_xaxis = project_tuple(xaxis_end,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	Tuple proj_yaxis = project_tuple(yaxis_end,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	Tuple proj_zaxis= project_tuple(zaxis_end,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	Tuple proj_tip = project_tuple(tip,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	debug_print_view_volume("DEBUG:View volume is:",&view_vol);
	printf("\n");
	debug_print_tuple("DEBUG:Unprojected triangle vert1:",&base.vert1);
	debug_print_tuple("DEBUG:Projected triangle vert1:",&proj_base.vert1);
	printf("\n");
	debug_print_tuple("DEBUG:Unprojected triangle vert2:",&base.vert2);
	debug_print_tuple("DEBUG:Projected triangle vert2:",&proj_base.vert2);
	printf("\n");
	debug_print_tuple("DEBUG:Unprojected triangle vert2:",&base.vert3);
	debug_print_tuple("DEBUG:Projected triangle vert3:",&proj_base.vert3);
	printf("\n");
	debug_print_tuple("DEBUG:Unprojected triangle tip:",&tip);
	debug_print_tuple("DEBUG:Projected triangle tip:",&proj_tip);
	printf("\n");

	//does this even work?
	printf("DEBUG: Scaling to frame buffer coords\n");
	Tuple vmax = (Tuple){X_BOUND,Y_BOUND,0};
	Tuple vmin = (Tuple){0,0,0};
	Tuple wmax= (Tuple){nx-0.5,ny-0.5,0};
	Tuple wmin = (Tuple){-0.5,-0.5,0};
	proj_origin=xform_window_to_view(proj_origin,wmax,wmin,vmax,vmin);
	proj_xaxis=xform_window_to_view(proj_xaxis,wmax,wmin,vmax,vmin);
	proj_yaxis=xform_window_to_view(proj_yaxis,wmax,wmin,vmax,vmin);
	proj_zaxis=xform_window_to_view(proj_zaxis,wmax,wmin,vmax,vmin);
	proj_base.vert1=xform_window_to_view(proj_base.vert1,wmax,wmin,vmax,vmin);
	proj_base.vert2=xform_window_to_view(proj_base.vert2,wmax,wmin,vmax,vmin);
	proj_base.vert3=xform_window_to_view(proj_base.vert3,wmax,wmin,vmax,vmin);
	proj_tip=xform_window_to_view(proj_tip,wmax,wmin,vmax,vmin);
	debug_print_tuple("DEBUG:Scaled triangle vert1:",&proj_base.vert1);
	printf("\n");
	debug_print_tuple("DEBUG:Scaled triangle vert2:",&proj_base.vert2);
	printf("\n");
	debug_print_tuple("DEBUG:Scaled triangle vert3:",&proj_base.vert3);
	printf("\n");
	debug_print_tuple("DEBUG:Scaled triangle tip:",&proj_tip);
	printf("\n");


	printf("DEBUG: Draw pyramid.\n");
	//Pixel color = (Pixel){.red=rand()%255,.grn=rand()%255,.blu=rand()%255};
	draw_triangle(red,proj_base,frame_buffer,X_BOUND,Y_BOUND);
	draw_segment(blue,proj_tip,proj_base.vert1,frame_buffer,X_BOUND,Y_BOUND);
	draw_segment(blue,proj_tip,proj_base.vert2,frame_buffer,X_BOUND,Y_BOUND);
	draw_segment(blue,proj_tip,proj_base.vert3,frame_buffer,X_BOUND,Y_BOUND);
	draw_segment(white,proj_origin,proj_xaxis,frame_buffer,X_BOUND,Y_BOUND);
	draw_segment(white,proj_origin,proj_yaxis,frame_buffer,X_BOUND,Y_BOUND);
	draw_segment(white,proj_origin,proj_zaxis,frame_buffer,X_BOUND,Y_BOUND);

	printf("DEBUG: Write framebuffer to disk.\n");
	write_fbuf_to_disk(argv[10],frame_buffer,X_BOUND,Y_BOUND);

	printf("DEBUG: Done.\n");
	dealloc_framebuf(frame_buffer,X_BOUND);
	return EXIT_SUCCESS;
}
