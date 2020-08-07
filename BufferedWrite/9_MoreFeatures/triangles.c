#include "buffer.h"
#include <time.h>
//draw a line between an array of points
int main(int argc, char** argv)
{
	if (argc!=3)
	{
		printf("argc/argv\n");
		return EXIT_FAILURE;
	}

	srand(time(0));

	printf("DEBUG: Set up frame buffer.\n");
	Pixel black = {.red=0,.grn=0,.blu=0};
	Pixel** frame_buffer=alloc_framebuf(X_BOUND,Y_BOUND);
	fill_fbuf_color(black,frame_buffer,X_BOUND,Y_BOUND);

	//draw triangles using bresenhams algorithm to window coords
	printf("DEBUG: Draw triangles.\n");
	int tri_count=atoi(argv[1]);
	for (int i =0;i<tri_count;i++)
	{
		Triangle tri = make_triangle(rand()%X_BOUND,rand()%Y_BOUND,
				rand()%X_BOUND,rand()%Y_BOUND,
				rand()%X_BOUND,rand()%Y_BOUND);
		Pixel color = (Pixel){.red=rand()%255,.grn=rand()%255,.blu=rand()%255};
		draw_triangle(color,tri,frame_buffer,X_BOUND,Y_BOUND);
	}


	//write the frame buffer and deallocate memory used
	printf("DEBUG: Write framebuffer to disk.\n");
	write_fbuf_to_disk(argv[2],frame_buffer,X_BOUND,Y_BOUND);

	printf("DEBUG: Done.\n");
	dealloc_framebuf(frame_buffer,X_BOUND);
	return EXIT_SUCCESS;
}
