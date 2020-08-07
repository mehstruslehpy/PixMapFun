#include "draw.h"
void draw_axes_perspective(Pixel color,Vector eye_pos,Vector gaze_dir,Vector view_up,
		ViewVolume view_vol,int nx,int ny,Vector wmax,
		Vector wmin,Vector vmax,Vector vmin,
		Pixel** frame_buffer,int x_bound ,int y_bound,int z_bound)
{
	Vector origin = (Vector){0,0,0};
	Vector xaxis_end1= (Vector){x_bound,0,0};
	Vector xaxis_end2= (Vector){-x_bound,0,0};
	Vector yaxis_end1= (Vector){0,y_bound,0};
	Vector yaxis_end2= (Vector){0,-y_bound,0};
	Vector zaxis_end1= (Vector){0,0,z_bound};
	Vector zaxis_end2= (Vector){0,0,-z_bound};
	//perspective project
	//Vector proj_origin= perspective_project_vector(origin,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	Vector proj_xaxis1= perspective_project_vector(xaxis_end1,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	Vector proj_xaxis2= perspective_project_vector(xaxis_end2,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	Vector proj_yaxis1= perspective_project_vector(yaxis_end1,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	Vector proj_yaxis2= perspective_project_vector(yaxis_end2,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	Vector proj_zaxis1= perspective_project_vector(zaxis_end1,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	Vector proj_zaxis2= perspective_project_vector(zaxis_end2,eye_pos,gaze_dir,view_up,view_vol,nx,ny);
	//xform to view
	//Vector scale_origin=xform_window_to_view(proj_origin,wmax,wmin,vmax,vmin);
	Vector scale_xaxis1=xform_window_to_view(proj_xaxis1,wmax,wmin,vmax,vmin);
	Vector scale_xaxis2=xform_window_to_view(proj_xaxis2,wmax,wmin,vmax,vmin);
	Vector scale_yaxis1=xform_window_to_view(proj_yaxis1,wmax,wmin,vmax,vmin);
	Vector scale_yaxis2=xform_window_to_view(proj_yaxis2,wmax,wmin,vmax,vmin);
	Vector scale_zaxis1=xform_window_to_view(proj_zaxis1,wmax,wmin,vmax,vmin);
	Vector scale_zaxis2=xform_window_to_view(proj_zaxis2,wmax,wmin,vmax,vmin);
	//draw
	draw_line(color,scale_xaxis1,scale_xaxis2,frame_buffer,x_bound,y_bound);
	draw_line(color,scale_yaxis1,scale_yaxis2,frame_buffer,x_bound,y_bound);
	draw_line(color,scale_zaxis1,scale_zaxis2,frame_buffer,x_bound,y_bound);
}
void fillcolor_triangle(Vector vert1,Vector vert2,Vector vert3,Pixel color1,Pixel color2 ,Pixel color3, Pixel** frame_buffer,int x_bound,int y_bound)
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
					plot_fbuf(x,y,new_color,frame_buffer,x_bound,y_bound);
				}
			}
		}
}

void fillcolor_convex_polygon(Polygon poly,Pixel** frame_buffer,int x_bound,int y_bound)
{
	//polygon is a line or point there is nothing to fill
	if (poly.count<3) return;
	//fill polygon as a collection of triangles
	fillcolor_triangle(poly.pt[0],poly.pt[1],poly.pt[2],
			poly.color[0],poly.color[1],poly.color[2],
			frame_buffer,x_bound,y_bound);
	for (int i = 2;i<poly.count-1;i++)
	{
		fillcolor_triangle(poly.pt[0],poly.pt[i],poly.pt[i+1],
				poly.color[0],poly.color[i],poly.color[i+1],
				frame_buffer,x_bound,y_bound);

	}
}

void copy_into_view(Pixel** frame_buffer,Vector view_max,Vector view_min,Pixel** window_model,Vector window_max,Vector window_min)
{
	for (int i=window_min.x;i<window_max.x;i++)
	{
		for (int j=window_min.y;j<window_max.y;j++)
		{
			//printf("DEBUG:i=%d,j=%d\n",i,j);
			Vector oldcoord=(Vector){.x=i,.y=j};
			Pixel color=read_fbuf(i,j,window_model,window_max.x,window_max.y);
			Vector newcoord=xform_window_to_view(oldcoord,window_max,window_min,view_max,view_min);
			plot_fbuf(newcoord.x,newcoord.y,color,frame_buffer,view_max.x,view_max.y);
		}
	}
}
void fast_copy_into_view(double** xform,Pixel** frame_buffer,Vector view_max,Vector view_min,Pixel** window_model,Vector window_max,Vector window_min)
{
	for (int i=window_min.x;i<window_max.x;i++)
	{
		for (int j=window_min.y;j<window_max.y;j++)
		{
			//printf("DEBUG:i=%d,j=%d\n",i,j);
			Vector oldcoord=(Vector){.x=i,.y=j};
			Pixel color=read_fbuf(i,j,window_model,window_max.x,window_max.y);
			Vector newcoord=fast_window_view_xform(xform,oldcoord);
			plot_fbuf(newcoord.x,newcoord.y,color,frame_buffer,view_max.x,view_max.y);
		}
	}
}

void draw_triangle(Pixel color,Triangle tri,Pixel** frame_buffer, int x_bound ,int y_bound)
{
	double* arr=(double[]){tri.vert1.x,tri.vert1.y,
		tri.vert2.x,tri.vert2.y,
		tri.vert3.x,tri.vert3.y,
		tri.vert1.x,tri.vert1.y};
	bresenham_lines_array(color,arr,8,frame_buffer,x_bound,y_bound);
}
void draw_bsp_tree_no_wire(BSPNode* root,Vector eye_pos,Vector gaze_dir,Vector view_up,
		ViewVolume vol,int nx,int ny,Vector wmax,
		Vector wmin,Vector vmax,Vector vmin,
		Pixel** frame_buffer,int x_bound ,int y_bound)
{
	if (root==NULL) return;
	if (f(*(root->poly),eye_pos)<0)
	{
		draw_bsp_tree_no_wire(root->pos,eye_pos,gaze_dir,view_up,vol,nx,ny,wmax,wmin,vmax,vmin,frame_buffer,x_bound,y_bound);
		perspective_project_polygon(root->poly,eye_pos,gaze_dir,view_up,vol,nx,ny);
		xform_polygon_window_to_view(root->poly,wmax,wmin,vmax,vmin);
		fillcolor_convex_polygon(*(root->poly),frame_buffer,x_bound,y_bound);
		//draw_polygon(wire_color,*(root->poly),frame_buffer,x_bound,y_bound);
		draw_bsp_tree_no_wire(root->neg,eye_pos,gaze_dir,view_up,vol,nx,ny,wmax,wmin,vmax,vmin,frame_buffer,x_bound,y_bound);
	}
	else
	{
		draw_bsp_tree_no_wire(root->neg,eye_pos,gaze_dir,view_up,vol,nx,ny,wmax,wmin,vmax,vmin,frame_buffer,x_bound,y_bound);
		perspective_project_polygon(root->poly,eye_pos,gaze_dir,view_up,vol,nx,ny);
		xform_polygon_window_to_view(root->poly,wmax,wmin,vmax,vmin);
		fillcolor_convex_polygon(*(root->poly),frame_buffer,x_bound,y_bound);
		//draw_polygon(wire_color,*(root->poly),frame_buffer,x_bound,y_bound);
		draw_bsp_tree_no_wire(root->pos,eye_pos,gaze_dir,view_up,vol,nx,ny,wmax,wmin,vmax,vmin,frame_buffer,x_bound,y_bound);
	}
}
void draw_bsp_tree_wire(BSPNode* root,Pixel wire_color,Vector eye_pos,Vector gaze_dir,Vector view_up,
		ViewVolume vol,int nx,int ny,Vector wmax,
		Vector wmin,Vector vmax,Vector vmin,
		Pixel** frame_buffer,int x_bound ,int y_bound)
{
	if (root==NULL) return;
	if (f(*(root->poly),eye_pos)<0)
	{
		draw_bsp_tree_wire(root->pos,wire_color,eye_pos,gaze_dir,view_up,vol,nx,ny,wmax,wmin,vmax,vmin,frame_buffer,x_bound,y_bound);
		perspective_project_polygon(root->poly,eye_pos,gaze_dir,view_up,vol,nx,ny);
		xform_polygon_window_to_view(root->poly,wmax,wmin,vmax,vmin);
		fillcolor_convex_polygon(*(root->poly),frame_buffer,x_bound,y_bound);
		draw_polygon(wire_color,*(root->poly),frame_buffer,x_bound,y_bound);
		draw_bsp_tree_wire(root->neg,wire_color,eye_pos,gaze_dir,view_up,vol,nx,ny,wmax,wmin,vmax,vmin,frame_buffer,x_bound,y_bound);
	}
	else
	{
		draw_bsp_tree_wire(root->neg,wire_color,eye_pos,gaze_dir,view_up,vol,nx,ny,wmax,wmin,vmax,vmin,frame_buffer,x_bound,y_bound);
		perspective_project_polygon(root->poly,eye_pos,gaze_dir,view_up,vol,nx,ny);
		xform_polygon_window_to_view(root->poly,wmax,wmin,vmax,vmin);
		fillcolor_convex_polygon(*(root->poly),frame_buffer,x_bound,y_bound);
		draw_polygon(wire_color,*(root->poly),frame_buffer,x_bound,y_bound);
		draw_bsp_tree_wire(root->pos,wire_color,eye_pos,gaze_dir,view_up,vol,nx,ny,wmax,wmin,vmax,vmin,frame_buffer,x_bound,y_bound);
	}
}
void draw_polygon(Pixel color,Polygon p,Pixel** frame_buffer, int x_bound ,int y_bound)
{
	for (int i=0;i<p.count-1;i++)
	{
		draw_segment(color,p.pt[i],p.pt[i+1],frame_buffer,x_bound,y_bound);
	}
	draw_segment(color,p.pt[p.count-1],p.pt[0],frame_buffer,x_bound,y_bound);
}
void draw_segment(Pixel color,Vector pt1,Vector pt2,Pixel** frame_buffer, int x_bound ,int y_bound)
{
	double* arr=(double[]){pt1.x,pt1.y,pt2.x,pt2.y};
	bresenham_lines_array(color,arr,4,frame_buffer,x_bound,y_bound);
}
void draw_line(Pixel color,Vector P,Vector Q,Pixel** frame_buffer, int x_bound ,int y_bound)
{
	Vector start = (Vector){.x=0,.y=0};
	Vector end = (Vector){.x=x_bound,.y=0};
	double a = Q.y-P.y; 
	double b = P.x-Q.x; 
	double c = a*(P.x) + b*(P.y); 
	//the line is ax+by=c in other words by=c-ax or y=c/b-(a/b)x
	if (b==0.0) 
	{
		start.x=P.x;	
		end.x=P.x;	
		start.y=0;
		end.y=y_bound;

	}
	else
	{
		start.y=(c/b)-(a/b)*start.x;
		end.y=(c/b)-(a/b)*end.x;
	}
	draw_segment(color,start,end,frame_buffer,x_bound,y_bound);
}
void bresenham_lines_array(Pixel color,double* pts,int count,Pixel** frame_buffer, int x_bound ,int y_bound)
{
	assert(pts!=NULL);
	assert(count>=4); //at least two points
	assert(frame_buffer!=NULL);

	for (int i=0;i<count-2;i+=2) 
	{
		bresenham_plotline(color,pts[i],pts[i+1],pts[i+2],pts[i+3],frame_buffer,x_bound,y_bound);
	}
}
void bresenham_plotline(Pixel color,double x_0,double y_0,double x_1,double y_1,Pixel** frame_buffer,int x_bound,int y_bound)
{
	assert(frame_buffer!=NULL);

	//need integer coordinates
	int x0 = x_0;
	int y0 = y_0;
	int x1 = x_1;
	int y1 = y_1;

	//this is essentially the bresenham algo from rosetta code for the
	//c programming language
	int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
	int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
	int err = (dx>dy ? dx : -dy)/2, e2;
	for(;;)
	{
		plot_fbuf(x0,y0,color,frame_buffer,x_bound,y_bound);
		if (x0==x1 && y0==y1) break;
		e2 = err;
		if (e2 >-dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
}
