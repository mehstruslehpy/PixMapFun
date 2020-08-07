#include "../buffer.h"
//#include <time.h>
int main(int argc, char** argv)
{
	if (argc!=4)
	{
		printf("Usage: ./main <x-min> <y-min> <x-max> <y-max> <outfilename>\n");
	}

	printf("Setup tuples.\n");
	double V_X_BOUND=60;
	double V_Y_BOUND=60;
	double W_X_BOUND=80;
	double W_Y_BOUND=80;
	Tuple in=(Tuple){.x=atof(argv[1]),.y=atof(argv[2]),.z=atof(argv[3])};
	Tuple view_max=(Tuple){.x=V_X_BOUND,.y=V_Y_BOUND};
	Tuple view_min=(Tuple){.x=30,.y=40};
	Tuple window_min=(Tuple){.x=20,.y=40};
	Tuple window_max=(Tuple){.x=W_X_BOUND,.y=W_Y_BOUND};

	Tuple out_new=xform_window_to_view(in,window_max,window_min,view_max,view_min);
	printf("view_max=(%f,%f,%f)\n",view_max.x,view_max.y,view_max.z);
	printf("view_min=(%f,%f,%f)\n",view_min.x,view_min.y,view_min.z);
	printf("window_max=(%f,%f,%f)\n",window_max.x,window_max.y,window_max.z);
	printf("window_min=(%f,%f,%f)\n",window_min.x,window_min.y,window_min.z);
	printf("in=(%f,%f,%f)\n",in.x,in.y,in.z);
	printf("out_new=(%f,%f,%f)\n",out_new.x,out_new.y,out_new.z);
	return EXIT_SUCCESS;
}
