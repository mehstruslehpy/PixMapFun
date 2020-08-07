#include "draw_zbuf.h"
void draw_wire_zbuffer(Wireframe wire,double** zbuffer,Vector eye_pos,Vector gaze_dir,Vector view_up,
		ViewVolume vol,int nx,int ny,Vector wmax,
		Vector wmin,Vector vmax,Vector vmin,
		Pixel** frame_buffer,int x_bound ,int y_bound)
{
	for (int i=0;i<wire.count;i++)
	{
		//The reason we need to allocate and copy a new polygon is because the z buffer needs
		//to be in terms of the z values directly after the perspective transform but before
		//the window to view transform, hence copying in this way saves the correct z values
		//for when they are needed. Ideally we would reorder things so that this extra copy is
		//not needed or is done at a better time without allocating more memory per every triangle
		//draw but this code works and this whole library is mainly just a proof of concept lots
		//of other things could be done more efficiently as well.
		Polygon no_view;
		alloc_poly(&no_view,wire.poly[i].count);
		perspective_project_polygon(&(wire.poly[i]),eye_pos,gaze_dir,view_up,vol,nx,ny);
		copy_poly(&(wire.poly[i]),&no_view);
		xform_polygon_window_to_view(&(wire.poly[i]),wmax,wmin,vmax,vmin);
		fillcolor_convex_polygon_zbuf(wire.poly[i],no_view,frame_buffer,zbuffer,x_bound,y_bound);
		dealloc_poly(&no_view);

	}
}
void fillcolor_triangle_zbuf(Vector vert1,Vector vert2,Vector vert3,
		Vector no_view1,Vector no_view2,Vector no_view3,
		Pixel color1,Pixel color2 ,Pixel color3, 
		Pixel** frame_buffer,double** zbuffer,
		int x_bound,int y_bound)
{
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
					//interpolate the new z value linearly
					/*
					   Vector temp_az=scalar_mul(alpha,vert1);
					   Vector temp_bz=scalar_mul(beta,vert2);
					   Vector temp_cz=scalar_mul(gamma,vert3);
					   Vector new_zvalue=add(temp_az,add(temp_bz,temp_cz));
					   plot_zbuf_fbuf(x,y,new_zvalue.z_buf,new_color,frame_buffer,zbuffer,x_bound,y_bound);
					//plot_zbuf_fbuf(x,y,1/new_zvalue.z_buf,new_color,frame_buffer,zbuffer,x_bound,y_bound);
					*/
					//double inverse_zvalue = alpha/vert1.z_buf+beta/vert2.z_buf+gamma/vert3.z_buf;
					double zval = alpha/no_view1.z+beta/no_view2.z+gamma/no_view3.z;
					plot_zbuf_fbuf(x,y,1.0/zval,new_color,frame_buffer,zbuffer,x_bound,y_bound);
				}
			}
		}
}

void fillcolor_convex_polygon_zbuf(Polygon poly,Polygon no_view,Pixel** frame_buffer,double** zbuffer,int x_bound,int y_bound)
{
	//polygon is a line or point there is nothing to fill
	if (poly.count<3) return;
	if (no_view.count<3) return;
	//fill polygon as a collection of triangles
	fillcolor_triangle_zbuf(poly.pt[0],poly.pt[1],poly.pt[2],
			no_view.pt[0],no_view.pt[1],no_view.pt[2],
			poly.color[0],poly.color[1],poly.color[2],
			frame_buffer,zbuffer,x_bound,y_bound);
	for (int i = 2;i<poly.count-1;i++)
	{
		fillcolor_triangle_zbuf(poly.pt[0],poly.pt[i],poly.pt[i+1],
				no_view.pt[0],no_view.pt[i],no_view.pt[i+1],
				poly.color[0],poly.color[i],poly.color[i+1],
				frame_buffer,zbuffer,x_bound,y_bound);

	}
}

