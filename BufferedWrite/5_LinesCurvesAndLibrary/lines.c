#include "buffer.h"
//draw a line between an array of points
int main(int argc, char** argv)
{
	if ((argc-1)%2!=0||argc<2)
	{
		printf("Usage: ./main <sequence of x y values>\n");
		printf("\tExample: ./main 0 50 100 50 50 100\n");
		printf("\tNote: Each x y value must be within screen bounds or they will be ignored the y_bound=%d and x_bound=%d.\n",Y_BOUND,X_BOUND);
		printf("\tNote: Floating point values are fine for each tuple but will be truncated to integer coordinates.\n");
		printf("\targc=%d.\n",argc);
	}

	srand(time(0));

	printf("DEBUG: Set up frame buffer.\n");
	Pixel color = {.red=rand()%255,.grn=rand()%255,.blu=rand()%255};
	Pixel black = {.red=0,.grn=0,.blu=0};
	Pixel frame_buffer[X_BOUND][Y_BOUND];
	fill_fbuf_color(frame_buffer,X_BOUND,Y_BOUND,black);
	
	//allocate an array of doubles for the input coordinates
	printf("DEBUG: Allocate points array.\n");
	double* coord_array = malloc(sizeof(double)*(argc-1));
	printf("DEBUG:");
	for (int i=0;i<argc-1;i++) 
	{
		printf("%f,",atof(argv[i+1]));
		coord_array[i]=atof(argv[i+1]);
	}
	printf("\n");

	//draw lines using bresenhams algorithm
	printf("DEBUG: Draw line.\n");
	bresenham_lines_array(frame_buffer,color,coord_array,argc-1);

	//write the frame buffer and deallocate memory used
	printf("DEBUG: Write framebuffer to disk.\n");
	free(coord_array);
	write_fbuf_to_disk("lines.ppm",frame_buffer,X_BOUND,Y_BOUND);

	printf("DEBUG: Done.\n");
	return EXIT_SUCCESS;
}
