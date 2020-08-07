#include "draw_shaded.h"

void fillcolor_diffuse_shaded_triangle(Vector vert1,Vector vert2,Vector vert3,
		Vector world1,Vector world2,Vector world3,
		Pixel color1,Pixel color2,Pixel color3,
		Vector l,double c_r, double c_l,double c_a,
		Pixel** frame_buffer,int x_bound,int y_bound)
{
	Vector normal=normalize(normal_vector(world1,world2,world3));
	//equation 9.3 in shirley
	//light source l and normal vector are both normalized we need c to be in [0,1]
	//if we don't normalize at some point then the values will not be in the correct range
	double c = c_r*(c_a+c_l*fmax(0,dot_product(normal,normalize(l))));
	//double c = c_r*(c_a+c_l*fabs(dot_product(normal,normalize(l))));
	//assert(c<=1&&c>=0);
	//debug_print_vector("DEBUG: normal",&normal);
	//debug_print_vector("DEBUG: light source",&l);
	//printf("DEBUG: c=%f\n\n",c);
	color1=scalar_mul_pixel(c,color1);
	color2=scalar_mul_pixel(c,color2);
	color3=scalar_mul_pixel(c,color3);

	double xmin=floor(fmin(vert1.x,fmin(vert2.x,vert3.x)));
	double xmax=ceil(fmax(vert1.x,fmax(vert2.x,vert3.x)));
	double ymin=floor(fmin(vert1.y,fmin(vert2.y,vert3.y)));
	double ymax=ceil(fmax(vert1.y,fmax(vert2.y,vert3.y)));
	double falpha = fij(vert2,vert3,vert1.x,vert1.y);
	double fbeta = fij(vert3,vert1,vert2.x,vert2.y);
	double fgamma = fij(vert1,vert2,vert3.x,vert3.y);
	for (int y=ymin;y<=ymax;y++)
		for (int x=xmin;x<=xmax;x++)
		{
			double alpha = fij(vert2,vert3,x,y)/falpha;
			double beta = fij(vert3,vert1,x,y)/fbeta;
			double gamma = fij(vert1,vert2,x,y)/fgamma;
			if (alpha>=0&&beta>=0&&gamma>=0) 
			{
				if ((alpha>0||falpha*fij(vert2,vert3,-1,-1)>0)&&
						(beta>0||fbeta*fij(vert3,vert1,-1,-1)>0)&&
						(gamma>0||fgamma*fij(vert1,vert2,-1,-1)>0))
				{
					Pixel temp_a=scalar_mul_pixel(alpha,color1);
					Pixel temp_b=scalar_mul_pixel(beta,color2);
					Pixel temp_c=scalar_mul_pixel(gamma,color3);
					Pixel new_color=add_pixel(temp_a,add_pixel(temp_b,temp_c));
					plot_fbuf(x,y,new_color,frame_buffer,x_bound,y_bound);
				}
			}
		}
}

void fillcolor_diffuse_shaded_convex_polygon(Polygon poly,Polygon world,Vector l,double c_r,double c_l,double c_a,Pixel** frame_buffer,int x_bound,int y_bound)
{
	//polygon is a line or point there is nothing to fill
	if (poly.count<3) return;
	if (world.count<3) return;
	//fill polygon as a collection of triangles
	fillcolor_diffuse_shaded_triangle(poly.pt[0],poly.pt[1],poly.pt[2],
			world.pt[0],world.pt[1],world.pt[2],
			poly.color[0],poly.color[1],poly.color[2],
			l,c_r,c_l,c_a,
			frame_buffer,x_bound,y_bound);
	for (int i = 2;i<poly.count-1;i++)
	{
		fillcolor_diffuse_shaded_triangle(poly.pt[0],poly.pt[i],poly.pt[i+1],
				world.pt[0],world.pt[i],world.pt[i+1],
				poly.color[0],poly.color[i],poly.color[i+1],
				l,c_r,c_l,c_a,
				frame_buffer,x_bound,y_bound);

	}
}
void draw_diffuse_shaded_bsp_tree_wire(BSPNode* root,Pixel wire_color,Vector eye_pos,Vector gaze_dir,Vector view_up,
		Vector l, double c_r, double c_l,double c_a,
		ViewVolume vol,int nx,int ny,Vector wmax,
		Vector wmin,Vector vmax,Vector vmin,
		Pixel** frame_buffer,int x_bound ,int y_bound)
{
	if (root==NULL) return;
	//color calculations are done in world coordinates not in perspective
	//xformed coords
	Polygon world;
	alloc_poly(&world,root->poly->count);
	copy_poly(root->poly,&world);
	if (f(*(root->poly),eye_pos)<0)
	{
		draw_diffuse_shaded_bsp_tree_wire(root->pos,wire_color,eye_pos,gaze_dir,view_up,l,c_r,c_l,c_a,vol,nx,ny,wmax,wmin,vmax,vmin,frame_buffer,x_bound,y_bound);
		perspective_project_polygon(root->poly,eye_pos,gaze_dir,view_up,vol,nx,ny);
		xform_polygon_window_to_view(root->poly,wmax,wmin,vmax,vmin);
		fillcolor_diffuse_shaded_convex_polygon(*(root->poly),world,l,c_r,c_l,c_a,frame_buffer,x_bound,y_bound);
		draw_polygon(wire_color,*(root->poly),frame_buffer,x_bound,y_bound);
		draw_diffuse_shaded_bsp_tree_wire(root->neg,wire_color,eye_pos,gaze_dir,view_up,l,c_r,c_l,c_a,vol,nx,ny,wmax,wmin,vmax,vmin,frame_buffer,x_bound,y_bound);
	}
	else
	{
		draw_diffuse_shaded_bsp_tree_wire(root->neg,wire_color,eye_pos,gaze_dir,view_up,l,c_r,c_l,c_a,vol,nx,ny,wmax,wmin,vmax,vmin,frame_buffer,x_bound,y_bound);
		perspective_project_polygon(root->poly,eye_pos,gaze_dir,view_up,vol,nx,ny);
		xform_polygon_window_to_view(root->poly,wmax,wmin,vmax,vmin);
		fillcolor_diffuse_shaded_convex_polygon(*(root->poly),world,l,c_r,c_l,c_a,frame_buffer,x_bound,y_bound);
		draw_polygon(wire_color,*(root->poly),frame_buffer,x_bound,y_bound);
		draw_diffuse_shaded_bsp_tree_wire(root->pos,wire_color,eye_pos,gaze_dir,view_up,l,c_r,c_l,c_a,vol,nx,ny,wmax,wmin,vmax,vmin,frame_buffer,x_bound,y_bound);
	}
	dealloc_poly(&world);
}

