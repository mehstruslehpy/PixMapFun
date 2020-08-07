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
	

	//draw three triangles using bresenhams algorithm to window coords
	printf("DEBUG: Draw triangles.\n");
	Tuple pt1 = {200,200};
	Tuple pt2 = {200,1000};
	Tuple pt3 = {900,900};
	Tuple pt4 = {500,200};
	Tuple pt5 = {500,1000};
	Tuple pt6 = {1200,900};
	Tuple pt7 = {800,200};
	Tuple pt8 = {800,1000};
	Tuple pt9 = {1500,900};
	
	double coord_array1[8] = {pt1.x,pt1.y, pt2.x,pt2.y,pt3.x,pt3.y,pt1.x,pt1.y};
	double coord_array2[8] = {pt4.x,pt4.y,pt5.x,pt5.y,pt6.x,pt6.y,pt4.x,pt4.y};
	double coord_array3[8] = {pt7.x,pt7.y,pt8.x,pt8.y,pt9.x,pt9.y,pt7.x,pt7.y};
	bresenham_lines_array(red,coord_array1,8,window_model,W_X_BOUND,W_Y_BOUND);
	bresenham_lines_array(green,coord_array2,8,window_model,W_X_BOUND,W_Y_BOUND);
	bresenham_lines_array(blue,coord_array3,8,window_model,W_X_BOUND,W_Y_BOUND);


	//copy relevant portion of window model to frame buffer
	Tuple view_max = (Tuple) {.x=V_X_BOUND,.y=V_Y_BOUND};
	Tuple view_min = (Tuple) {.x=0,.y=0};
	Tuple window_min = (Tuple) {.x=atoi(argv[1]),.y=atoi(argv[2])};
	Tuple window_max = (Tuple) {.x=atoi(argv[3]),.y=atoi(argv[4])};
	//it's probably better not to copy full buffers into view
	//it will also result in less artifacts
	//it would be more efficient to just redraw the the triangle into the new buffer directly
	copy_into_view(frame_buffer,view_max,view_min,window_model,window_max,window_min);
					

	//write the frame buffer and deallocate memory used
	printf("DEBUG: Write framebuffer to disk.\n");
	write_fbuf_to_disk(argv[5],frame_buffer,X_BOUND,Y_BOUND);

	printf("DEBUG: Done.\n");
	dealloc_framebuf(frame_buffer,V_X_BOUND);
	dealloc_framebuf(window_model,W_X_BOUND);
	return EXIT_SUCCESS;
}
