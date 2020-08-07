#include "buffer.h"
#include <time.h>
//draw a line between an array of points
int main(int argc, char** argv)
{
	if (argc!=6)
	{
		printf("Usage: ./main <x-min> <y-min> <x-max> <y-max> <outfilename>\n");
	}

	srand(time(0));

	printf("DEBUG: Set up frame buffer.\n");
	int V_X_BOUND = X_BOUND;
	int V_Y_BOUND = Y_BOUND;
	Pixel red = {.red=255,.grn=0,.blu=0};
	Pixel green = {.red=0,.grn=255,.blu=0};
	Pixel blue = {.red=0,.grn=0,.blu=255};
	Pixel black = {.red=0,.grn=0,.blu=0};
	Pixel** frame_buffer=alloc_framebuf(V_X_BOUND,V_Y_BOUND);
	fill_fbuf_color(black,frame_buffer,V_X_BOUND,V_Y_BOUND);

	printf("DEBUG: Set up external model.\n");
	int W_X_BOUND = 2*X_BOUND;
	int W_Y_BOUND = 2*Y_BOUND;
	Pixel** window_model=alloc_framebuf(W_X_BOUND,W_Y_BOUND);
	fill_fbuf_color(black,window_model,W_X_BOUND,W_Y_BOUND);

	//tuples for view max/min and window max/min
	Tuple view_max = (Tuple) {.x=V_X_BOUND,.y=V_Y_BOUND};
	Tuple view_min = (Tuple) {.x=0,.y=0};
	Tuple window_min = (Tuple) {.x=atoi(argv[1]),.y=atoi(argv[2])};
	Tuple window_max = (Tuple) {.x=atoi(argv[3]),.y=atoi(argv[4])};

	printf("DEBUG: Compute window to viewport matrix.\n");
	double** wv_xform = alloc_window_to_view_matrix(window_max,window_min,view_max,view_min);

	//draw three triangles using bresenhams algorithm to window coords
	printf("DEBUG: Draw triangles.\n");
	Tuple wpt1 = {200,200};
	Tuple wpt2 = {200,1000};
	Tuple wpt3 = {900,900};
	Tuple wpt4 = {500,200};
	Tuple wpt5 = {500,1000};
	Tuple wpt6 = {1200,900};
	Tuple wpt7 = {800,200};
	Tuple wpt8 = {800,1000};
	Tuple wpt9 = {1500,900};

	double coord_array1[8] = {wpt1.x,wpt1.y, wpt2.x,wpt2.y,wpt3.x,wpt3.y,wpt1.x,wpt1.y};
	double coord_array2[8] = {wpt4.x,wpt4.y,wpt5.x,wpt5.y,wpt6.x,wpt6.y,wpt4.x,wpt4.y};
	double coord_array3[8] = {wpt7.x,wpt7.y,wpt8.x,wpt8.y,wpt9.x,wpt9.y,wpt7.x,wpt7.y};
	bresenham_lines_array(red,coord_array1,8,window_model,W_X_BOUND,W_Y_BOUND);
	bresenham_lines_array(green,coord_array2,8,window_model,W_X_BOUND,W_Y_BOUND);
	bresenham_lines_array(blue,coord_array3,8,window_model,W_X_BOUND,W_Y_BOUND);

	//copy relevant portion of window model to frame buffer
	Tuple vpt1 =xform_window_to_view(wpt1,window_max,window_min,view_max,view_min);
	Tuple vpt2 =xform_window_to_view(wpt2,window_max,window_min,view_max,view_min);
	Tuple vpt3 =xform_window_to_view(wpt3,window_max,window_min,view_max,view_min);
	Tuple vpt4 =xform_window_to_view(wpt4,window_max,window_min,view_max,view_min);
	Tuple vpt5 =xform_window_to_view(wpt5,window_max,window_min,view_max,view_min);
	Tuple vpt6 =xform_window_to_view(wpt6,window_max,window_min,view_max,view_min);
	Tuple vpt7 =xform_window_to_view(wpt7,window_max,window_min,view_max,view_min);
	Tuple vpt8 =xform_window_to_view(wpt8,window_max,window_min,view_max,view_min);
	Tuple vpt9 =xform_window_to_view(wpt9,window_max,window_min,view_max,view_min);
	double coord_array4[8] = {vpt1.x,vpt1.y, vpt2.x,vpt2.y,vpt3.x,vpt3.y,vpt1.x,vpt1.y};
	double coord_array5[8] = {vpt4.x,vpt4.y,vpt5.x,vpt5.y,vpt6.x,vpt6.y,vpt4.x,vpt4.y};
	double coord_array6[8] = {vpt7.x,vpt7.y,vpt8.x,vpt8.y,vpt9.x,vpt9.y,vpt7.x,vpt7.y};
	bresenham_lines_array(red,coord_array4,8,frame_buffer,V_X_BOUND,V_Y_BOUND);
	bresenham_lines_array(green,coord_array5,8,frame_buffer,V_X_BOUND,V_Y_BOUND);
	bresenham_lines_array(blue,coord_array6,8,frame_buffer,V_X_BOUND,V_Y_BOUND);

	//write the frame buffer and deallocate memory used
	printf("DEBUG: Write framebuffer to disk.\n");
	write_fbuf_to_disk(argv[5],frame_buffer,X_BOUND,Y_BOUND);

	printf("DEBUG: Done.\n");
	dealloc_window_to_view_matrix(wv_xform);
	dealloc_framebuf(frame_buffer,V_X_BOUND);
	dealloc_framebuf(window_model,W_X_BOUND);
	return EXIT_SUCCESS;
}
