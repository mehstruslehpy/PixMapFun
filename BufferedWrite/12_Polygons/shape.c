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
	if ((argc-1)%3!=0&&argc!=0)
	{
		printf("argc/argv\n");
		printf("argc=%d\n",argc);
		printf("argc-1=%d\n",argc);
		return EXIT_FAILURE;
	}

	printf("DEBUG: Set up polygon\n");
	Polygon poly;
	poly.count=(argc-1)/3;
	alloc_poly(&poly,poly.count);
	//args will be ./shape fov <xyz coords for polygon>
	for (int i=1,j=0;i<argc;i+=3,j++)
	{
		poly.pt[j]=(Vector){
			.x=atof(argv[i]),
			.y=atof(argv[i+1]),
			.z=atof(argv[i+2])
		};
	}
	//end points for the axes
	Vector origin = (Vector){0,0,0};
	Vector xaxis_end1= (Vector){X_BOUND,0,0};
	Vector xaxis_end2= (Vector){-X_BOUND,0,0};
	Vector yaxis_end1= (Vector){0,Y_BOUND,0};
	Vector yaxis_end2= (Vector){0,-Y_BOUND,0};
	Vector zaxis_end1= (Vector){0,0,Z_BOUND};
	Vector zaxis_end2= (Vector){0,0,-Z_BOUND};


	printf("DEBUG: Projecting cube to view\n");
	Vector eye_pos = (Vector){1200,1200,2048}; //position of eye
	Vector gaze_dir= (Vector){0,3,1}; //position of eye
	Vector view_up = (Vector){0,0,1}; //position of eye
	printf("\n");
	debug_print_vector("DEBUG:eye_pos:",&eye_pos);
	debug_print_vector("DEBUG:view_up:",&view_up);
	debug_print_vector("DEBUG:gaze_dir:",&gaze_dir);

	//Vector proj_vert1 = perspective_project_vector(vert1,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	for (int i=0;i<poly.count;i++) 
		poly.pt[i]=perspective_project_vector(poly.pt[i],eye_pos,gaze_dir,view_up,view_vol,nx,ny);

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
	debug_print_vector("DEBUG:Projected cube wmax:",&wmax);
	debug_print_vector("DEBUG:Projected cube wmin:",&wmin);
	debug_print_vector("DEBUG:Projected cube vmax:",&vmax);
	debug_print_vector("DEBUG:Projected cube vmin:",&vmin);
	printf("\n");

	for (int i=0;i<poly.count;i++) 
		poly.pt[i]=xform_window_to_view(poly.pt[i],wmax,wmin,vmax,vmin);

	Vector scale_origin=xform_window_to_view(proj_origin,wmax,wmin,vmax,vmin);
	Vector scale_xaxis1=xform_window_to_view(proj_xaxis1,wmax,wmin,vmax,vmin);
	Vector scale_xaxis2=xform_window_to_view(proj_xaxis2,wmax,wmin,vmax,vmin);
	Vector scale_yaxis1=xform_window_to_view(proj_yaxis1,wmax,wmin,vmax,vmin);
	Vector scale_yaxis2=xform_window_to_view(proj_yaxis2,wmax,wmin,vmax,vmin);
	Vector scale_zaxis1=xform_window_to_view(proj_zaxis1,wmax,wmin,vmax,vmin);
	Vector scale_zaxis2=xform_window_to_view(proj_zaxis2,wmax,wmin,vmax,vmin);

	printf("DEBUG: Draw cube.\n");
	//Pixel color = (Pixel){.red=rand()%255,.grn=rand()%255,.blu=rand()%255};
	//draw axes first to prevent overlapping wrong
	draw_line(white,scale_xaxis1,scale_xaxis2,frame_buffer,X_BOUND,Y_BOUND);
	draw_line(white,scale_yaxis1,scale_yaxis2,frame_buffer,X_BOUND,Y_BOUND);
	draw_line(white,scale_zaxis1,scale_zaxis2,frame_buffer,X_BOUND,Y_BOUND);

	draw_polygon(green,poly,frame_buffer,X_BOUND,Y_BOUND);

	printf("DEBUG: Write framebuffer to disk.\n");
	write_fbuf_to_disk("polygon.ppm",frame_buffer,X_BOUND,Y_BOUND);

	printf("DEBUG: Done.\n");
	dealloc_framebuf(frame_buffer,X_BOUND);
	dealloc_poly(&poly);

	//some coords for debugging
	printf("\n");
	debug_print_vector("DEBUG:Unprojected cube origin",&origin);
	debug_print_vector("DEBUG:Projected cube proj_origin:",&proj_origin);
	debug_print_vector("DEBUG:Scaled cube origin:",&scale_origin);
	printf("\n");

	debug_print_vector("DEBUG:Unprojected cube xaxis_end",&xaxis_end1);
	debug_print_vector("DEBUG:Projected cube proj_xaxis:",&proj_xaxis1);
	debug_print_vector("DEBUG:Scaled cube proj_xaxis:",&scale_xaxis1);
	printf("\n");

	debug_print_vector("DEBUG:Unprojected cube yaxis_end",&yaxis_end1);
	debug_print_vector("DEBUG:Projected cube proj_yaxis:",&proj_yaxis1);
	debug_print_vector("DEBUG:Scaled cube proj_yaxis:",&scale_yaxis1);
	printf("\n");

	debug_print_vector("DEBUG:Unprojected cube zaxis_end",&zaxis_end1);
	debug_print_vector("DEBUG:Projected cube proj_zaxis:",&proj_zaxis1);
	debug_print_vector("DEBUG:Scaled cube proj_zaxis:",&scale_zaxis1);
	printf("\n");
	
	return EXIT_SUCCESS;
}
