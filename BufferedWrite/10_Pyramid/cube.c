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
	ViewVolume view_vol = (ViewVolume){0,X_BOUND,0,Y_BOUND,0,Z_BOUND};
	double nx = 1;
	double ny = 1;

	printf("DEBUG: Set up cube\n");
	//vert1 to vert4 form the base
	Tuple vert1 = (Tuple){0,0,0};
	Tuple vert2 = (Tuple){500,0,0};
	Tuple vert3 = (Tuple){0,500,0};
	Tuple vert4 = (Tuple){500,500,0};
	//vert5 to vert 8 form the top
	Tuple vert5 = (Tuple){0,0,500};
	Tuple vert6 = (Tuple){500,0,500};
	Tuple vert7 = (Tuple){0,500,500};
	Tuple vert8 = (Tuple){500,500,500};
	Tuple origin = (Tuple){0,0,0};
	Tuple xaxis_end= (Tuple){X_BOUND,0,0};
	Tuple yaxis_end= (Tuple){0,Y_BOUND,0};
	Tuple zaxis_end= (Tuple){0,0,Z_BOUND};


	printf("DEBUG: Projecting pyramid to view\n");
	//Tuple eye_pos = (Tuple){X_BOUND,Y_BOUND,Z_BOUND}; //position of eye
	Tuple eye_pos = (Tuple){atof(argv[1]),atof(argv[2]),atof(argv[3])}; //position of eye
	Tuple gaze_dir= (Tuple){atof(argv[4]),atof(argv[5]),atof(argv[6])}; //position of eye
	Tuple view_up = (Tuple){atof(argv[7]),atof(argv[8]),atof(argv[9])}; //position of eye
	debug_print_tuple("DEBUG:eye_pos:",&eye_pos);
	debug_print_tuple("DEBUG:view_up:",&view_up);
	debug_print_tuple("DEBUG:gaze_dir:",&gaze_dir);

	Tuple proj_vert1 = project_tuple(vert1,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	Tuple proj_vert2 = project_tuple(vert2,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	Tuple proj_vert3 = project_tuple(vert3,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	Tuple proj_vert4 = project_tuple(vert4,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	Tuple proj_vert5 = project_tuple(vert5,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	Tuple proj_vert6 = project_tuple(vert6,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	Tuple proj_vert7 = project_tuple(vert7,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	Tuple proj_vert8 = project_tuple(vert8,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	Tuple proj_origin= project_tuple(origin,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	Tuple proj_xaxis= project_tuple(xaxis_end,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	Tuple proj_yaxis= project_tuple(yaxis_end,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	Tuple proj_zaxis= project_tuple(zaxis_end,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	debug_print_view_volume("DEBUG:View volume is:",&view_vol);
	printf("\n");
	debug_print_tuple("DEBUG:Unprojected cube vert1:",&vert1);
	debug_print_tuple("DEBUG:Projected cube vert1:",&proj_vert1);
	printf("\n");
	debug_print_tuple("DEBUG:Unprojected cube vert2:",&vert2);
	debug_print_tuple("DEBUG:Projected cube vert2:",&proj_vert2);
	printf("\n");
	debug_print_tuple("DEBUG:Unprojected cube vert3:",&vert3);
	debug_print_tuple("DEBUG:Projected cube vert3:",&proj_vert3);
	printf("\n");
	debug_print_tuple("DEBUG:Unprojected cube vert4:",&vert4);
	debug_print_tuple("DEBUG:Projected cube vert4:",&proj_vert4);
	printf("\n");
	debug_print_tuple("DEBUG:Unprojected cube vert5:",&vert5);
	debug_print_tuple("DEBUG:Projected cube vert5:",&proj_vert5);
	printf("\n");
	debug_print_tuple("DEBUG:Unprojected cube vert6:",&vert6);
	debug_print_tuple("DEBUG:Projected cube vert6:",&proj_vert6);
	printf("\n");
	debug_print_tuple("DEBUG:Unprojected cube vert7:",&vert7);
	debug_print_tuple("DEBUG:Projected cube vert7:",&proj_vert7);
	printf("\n");
	debug_print_tuple("DEBUG:Unprojected cube vert8:",&vert8);
	debug_print_tuple("DEBUG:Projected cube vert8:",&proj_vert8);
	printf("\n");

	//does this even work?
	printf("DEBUG: Scaling to frame buffer coords\n");
	Tuple vmax = (Tuple){X_BOUND,Y_BOUND,0};
	Tuple vmin = (Tuple){0,0,0};
	Tuple wmax= (Tuple){nx-0.5,ny-0.5,0};
	Tuple wmin = (Tuple){-0.5,-0.5,0};
	proj_vert1=xform_window_to_view(proj_vert1,wmax,wmin,vmax,vmin);
	proj_vert2=xform_window_to_view(proj_vert2,wmax,wmin,vmax,vmin);
	proj_vert3=xform_window_to_view(proj_vert3,wmax,wmin,vmax,vmin);
	proj_vert4=xform_window_to_view(proj_vert4,wmax,wmin,vmax,vmin);
	proj_vert5=xform_window_to_view(proj_vert5,wmax,wmin,vmax,vmin);
	proj_vert6=xform_window_to_view(proj_vert6,wmax,wmin,vmax,vmin);
	proj_vert7=xform_window_to_view(proj_vert7,wmax,wmin,vmax,vmin);
	proj_vert8=xform_window_to_view(proj_vert8,wmax,wmin,vmax,vmin);
	proj_origin=xform_window_to_view(proj_origin,wmax,wmin,vmax,vmin);
	proj_xaxis=xform_window_to_view(proj_xaxis,wmax,wmin,vmax,vmin);
	proj_yaxis=xform_window_to_view(proj_yaxis,wmax,wmin,vmax,vmin);
	proj_zaxis=xform_window_to_view(proj_zaxis,wmax,wmin,vmax,vmin);
	debug_print_tuple("DEBUG:Scaled cube vert1:",&proj_vert1);
	printf("\n");
	debug_print_tuple("DEBUG:Scaled cube vert2:",&proj_vert2);
	printf("\n");
	debug_print_tuple("DEBUG:Scaled cube vert3:",&proj_vert3);
	printf("\n");
	debug_print_tuple("DEBUG:Scaled cube vert4:",&proj_vert4);
	printf("\n");
	debug_print_tuple("DEBUG:Scaled cube vert5:",&proj_vert5);
	printf("\n");
	debug_print_tuple("DEBUG:Scaled cube vert6:",&proj_vert6);
	printf("\n");
	debug_print_tuple("DEBUG:Scaled cube vert7:",&proj_vert7);
	printf("\n");
	debug_print_tuple("DEBUG:Scaled cube vert8:",&proj_vert8);
	printf("\n");




	printf("DEBUG: Draw pyramid.\n");
	//Pixel color = (Pixel){.red=rand()%255,.grn=rand()%255,.blu=rand()%255};
	draw_segment(blue,proj_vert1,proj_vert2,frame_buffer,X_BOUND,Y_BOUND);
	draw_segment(blue,proj_vert2,proj_vert4,frame_buffer,X_BOUND,Y_BOUND);
	draw_segment(blue,proj_vert4,proj_vert3,frame_buffer,X_BOUND,Y_BOUND);
	draw_segment(blue,proj_vert3,proj_vert1,frame_buffer,X_BOUND,Y_BOUND);

	draw_segment(red,proj_vert5,proj_vert6,frame_buffer,X_BOUND,Y_BOUND);
	draw_segment(red,proj_vert6,proj_vert8,frame_buffer,X_BOUND,Y_BOUND);
	draw_segment(red,proj_vert8,proj_vert7,frame_buffer,X_BOUND,Y_BOUND);
	draw_segment(red,proj_vert7,proj_vert5,frame_buffer,X_BOUND,Y_BOUND);

	draw_segment(green,proj_vert1,proj_vert5,frame_buffer,X_BOUND,Y_BOUND);
	draw_segment(green,proj_vert2,proj_vert6,frame_buffer,X_BOUND,Y_BOUND);
	draw_segment(green,proj_vert3,proj_vert7,frame_buffer,X_BOUND,Y_BOUND);
	draw_segment(green,proj_vert4,proj_vert8,frame_buffer,X_BOUND,Y_BOUND);

	draw_segment(white,proj_origin,proj_xaxis,frame_buffer,X_BOUND,Y_BOUND);
	draw_segment(white,proj_origin,proj_yaxis,frame_buffer,X_BOUND,Y_BOUND);
	draw_segment(white,proj_origin,proj_zaxis,frame_buffer,X_BOUND,Y_BOUND);
	printf("DEBUG: Write framebuffer to disk.\n");
	write_fbuf_to_disk(argv[10],frame_buffer,X_BOUND,Y_BOUND);

	printf("DEBUG: Done.\n");
	dealloc_framebuf(frame_buffer,X_BOUND);
	return EXIT_SUCCESS;
}
