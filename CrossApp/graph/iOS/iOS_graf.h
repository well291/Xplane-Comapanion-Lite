#ifndef hl_graphics_h
#define hl_graphics_h

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// WE USE OPENGL 2.0 WHICH USES SHADERS, SO WE HAVE A LAYER OF ABSTRACTION HERE TO USE ALL THE GL CALLS THAT XAVION USES
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
extern GLint u_tex0,u_mvp_matrix,attr_position,attr_tex_coord,attr_color;

void XV_glMatrixMode	(GLenum mode);
#define glMatrixMode	XV_glMatrixMode

void XV_glLoadIdentity	(void);
#define glLoadIdentity	XV_glLoadIdentity

void XV_glOrtho			(GLdouble left,GLdouble right,GLdouble bottom,GLdouble top,GLdouble zNear,GLdouble zFar);
#define glOrtho			XV_glOrtho

void XV_glFrustum		(GLdouble left,GLdouble right,GLdouble bottom,GLdouble top,GLdouble zNear,GLdouble zFar);
#define glFrustum		XV_glFrustum

void XV_glPushMatrix	(void);
#define glPushMatrix	XV_glPushMatrix

void XV_glPopMatrix		(void);
#define glPopMatrix		XV_glPopMatrix

void XV_glTranslatef	(GLfloat x,GLfloat y,GLfloat z);
#define glTranslatef	XV_glTranslatef

void XV_glRotatef		(GLfloat angle,GLfloat x,GLfloat y,GLfloat z);
#define glRotatef		XV_glRotatef

void XV_glDrawArrays	(GLenum mode,GLint first,GLsizei count);
#define glDrawArrays	XV_glDrawArrays

void XV_glDrawElements	(GLenum mode,GLsizei count,GLenum type,const GLvoid *indices);
#define glDrawElements	XV_glDrawElements

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
enum
{
	path_2d_init=0,
	path_2d_add	=1,
	path_2d_plot=2
};

const xflt path_dw=10;

const xflt s_white_tex=0.5;
const xflt t_white_tex=0.5;

const xint dep_read_true=100;
const xint dep_read_fals=101;
const xint dep_writ_true=102;
const xint dep_writ_fals=103;
const xint col_vert_true=104;
const xint col_vert_fals=105;

inline void OGL_set_state(const xint col_vert,const xint dep_read,const xint dep_writ)
{
	static xint old_col_vert=col_vert_fals;
	static xint old_dep_read=dep_read_fals;
	static xint old_dep_writ=dep_writ_fals;

	#if DEV
		if(!intrange(	 col_vert , col_vert_true,col_vert_fals))APP_alert("Bad setstate argument!","","","");
		if(!intrange(	 dep_read , dep_read_true,dep_read_fals))APP_alert("Bad setstate argument!","","","");
		if(!intrange(	 dep_writ , dep_writ_true,dep_writ_fals))APP_alert("Bad setstate argument!","","","");
		if(!intrange(old_col_vert , col_vert_true,col_vert_fals))APP_alert("Bad setstate argument!","","","");
		if(!intrange(old_dep_read , dep_read_true,dep_read_fals))APP_alert("Bad setstate argument!","","","");
		if(!intrange(old_dep_writ , dep_writ_true,dep_writ_fals))APP_alert("Bad setstate argument!","","","");
	#endif

	dev_alert_OGL("OGL_set_state 0");

	if((old_col_vert!=col_vert_true) && (col_vert==col_vert_true)){glEnableVertexAttribArray	(attr_color		);	old_col_vert=col_vert;}
	if((old_col_vert!=col_vert_fals) && (col_vert==col_vert_fals)){glDisableVertexAttribArray	(attr_color		);	old_col_vert=col_vert;}
	if((old_dep_read!=dep_read_true) && (dep_read==dep_read_true)){glEnable						(GL_DEPTH_TEST	);	old_dep_read=dep_read;}
	if((old_dep_read!=dep_read_fals) && (dep_read==dep_read_fals)){glDisable					(GL_DEPTH_TEST	);	old_dep_read=dep_read;}
	if((old_dep_writ!=dep_writ_true) && (dep_writ==dep_writ_true)){glDepthMask					(GL_TRUE		);	old_dep_writ=dep_writ;}
	if((old_dep_writ!=dep_writ_fals) && (dep_writ==dep_writ_fals)){glDepthMask					(GL_FALSE		);	old_dep_writ=dep_writ;}

	dev_alert_OGL("OGL_set_state 1");
}

inline void OGL_bind_tex(const enum_tex in_tex)
{
	dev_alert_OGL("OGL bind tex 1");

	static enum_tex old_tex=texture_dim;
	if(	old_tex!=in_tex){
		old_tex =in_tex;
		glBindTexture(GL_TEXTURE_2D,in_tex);}

	dev_alert_OGL("OGL bind tex 2");
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
const	xint		 vrt_dim=36000	;	// big enough for a PFD-MAP hi-res terrain strip: search arr_i_DIM
extern	xflt vrt_geo[vrt_dim]		;	// used for hoops in 3-D, 2-D plan, and 2-D profile!
extern	xint		 vrt_ind		;	// counter to run thru this buffer

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline void plot_geo(	enum_tex	in_tex0					,
						enum_col	in_color				,
						xflt		in_alpha				,
						xflt*		in_geo					,	// fastest for the opengl driver to take the geo, normal, st, and color all in 1 big array, not a handfull of little ones.
						xint		in_dims					,
						xint*		in_array_index			,
						GLenum		in_gl_type				,
						xint		in_col_vert				,
						xint		in_dep_read				,
						xint		in_dep_writ				,
						enum_tex	in_tex1 = texture_dim	)
{
	dev_alert_OGL("plot geo");

	#if DEV
		if(in_gl_type!=GL_LINES			)
		if(in_gl_type!=GL_LINE_STRIP	)
		if(in_gl_type!=GL_LINE_LOOP		)
		if(in_gl_type!=GL_TRIANGLES		)
		if(in_gl_type!=GL_TRIANGLE_STRIP)
			APP_alert("in_gl_type bad","","","");

		if(*in_array_index>=vrt_dim)
			APP_alert("in_array_index bad","","","");

		if(in_dims==2){
			if(in_dep_read==dep_read_true)APP_alert("in_dep_read bad","","","");
			if(in_dep_writ==dep_writ_true)APP_alert("in_dep_writ bad","","","");}

		if(in_dims==3){
			if(in_dep_read==dep_read_fals)APP_alert("in_dep_read bad","","","");}
	#endif

	xint							in_stride =in_dims	;	// num floats
	if(in_col_vert==col_vert_true)	in_stride+=1		;	// rgba, 4 BYTES GO INTO 1 FLOAT! SO ONLY STRIDE INCREASES BY 1 FLOAT HERE!
									in_stride+=2		;	// st

	if(in_gl_type==GL_LINES			 && *in_array_index/in_stride<2){*in_array_index=0;	return;}	// reset counter for next time if we get a partial set somehow, like just 1 bit of tri strip or something
	if(in_gl_type==GL_LINE_STRIP	 && *in_array_index/in_stride<2){*in_array_index=0;	return;}	// reset counter for next time if we get a partial set somehow, like just 1 bit of tri strip or something
	if(in_gl_type==GL_LINE_LOOP		 && *in_array_index/in_stride<2){*in_array_index=0;	return;}	// reset counter for next time if we get a partial set somehow, like just 1 bit of tri strip or something
	if(in_gl_type==GL_TRIANGLES		 && *in_array_index/in_stride<3){*in_array_index=0;	return;}	// reset counter for next time if we get a partial set somehow, like just 1 bit of tri strip or something
	if(in_gl_type==GL_TRIANGLE_STRIP && *in_array_index/in_stride<4){*in_array_index=0;	return;}	// reset counter for next time if we get a partial set somehow, like just 1 bit of tri strip or something

	dev_alert_OGL("plot geo");

	OGL_set_state(in_col_vert,in_dep_read,in_dep_writ);
	OGL_bind_tex(in_tex0);

	glUniform1i				(u_tex0			,0														);	// this must be every call, no matter what.
	glVertexAttribPointer	(attr_position	,in_dims,GL_FLOAT,false,in_stride*sizeof(GLfloat),in_geo);	in_geo+=in_dims	;

	if(in_col_vert==col_vert_true)
	{
		glVertexAttribPointer(attr_color,4,GL_UNSIGNED_BYTE,true,in_stride*sizeof(GLfloat),in_geo	);	in_geo+=1;
	}
	else
	{
		const xflt rgba_rats[4]=
		{
			colr.private_col_rgb[in_color][0],
			colr.private_col_rgb[in_color][1],
			colr.private_col_rgb[in_color][2],
								 in_alpha
		};
		glVertexAttrib4fv(attr_color,rgba_rats);
	}

	glVertexAttribPointer(attr_tex_coord,2,GL_FLOAT,false,in_stride*sizeof(GLfloat),in_geo);

	glDrawArrays(in_gl_type,0,	*in_array_index/in_stride);	// geo offset, number of verts. this taks 3x the ram to do as the indexed tris, but is fine for UI i guess.
								*in_array_index=0;			// ok we can zero out the index now that we have plotted, so we build from 0 next time on whatever array we passed in!

	dev_alert_OGL("plot geo");
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// ANY GL ROTATE	WILL LEAVE THIS HOSED!
// ANY GL TRANSLATE	WILL LEAVE THIS HOSED!
// ANY SCISSORS		WILL LEAVE THIS HOSED!
// ANY VIEWPORT		WILL LEAVE THIS HOSED!
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
class batch_class
{
public:
   ~batch_class(){}
	batch_class(){memset(this,0,sizeof(*this));}

enum_tex	vert_tex		;
GLenum		vert_type		;
xint		vert_dims		;
xint		vert_col_vert	;
xint		vert_dep_read	;
xint		vert_dep_writ	;

public:
inline void init_batch(	const enum_tex	tex			,
						const xint		in_col_vert	,
						const xint		in_dep_read	,
						const xint		in_dep_writ	)
{
	#if DEV
		if(!intrange(in_col_vert , col_vert_true,col_vert_fals))APP_alert("Bad init_batch argument!","","","");
		if(!intrange(in_dep_read , dep_read_true,dep_read_fals))APP_alert("Bad init_batch argument!","","","");
		if(!intrange(in_dep_writ , dep_writ_true,dep_writ_fals))APP_alert("Bad init_batch argument!","","","");
	#endif

	if(vrt_ind != 0)
		APP_alert("vrt_ind!=0 at start of init_batch!","","","");

	vert_tex		=tex		;
	vert_type		=0			;
	vert_dims		=0			;
	vert_col_vert	=in_col_vert;
	vert_dep_read	=in_dep_read;
	vert_dep_writ	=in_dep_writ;
}

inline void plot_batch()
{
	if(vrt_ind>0)	// we need to check this cause we often close a batch out even if nothing was drawn cause nothing in range!
		plot_geo(vert_tex,col_white,1.0,vrt_geo,vert_dims,&vrt_ind,vert_type , vert_col_vert,vert_dep_read,vert_dep_writ);
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// SHAPES TO ADD
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline void add_line_2d_tri(xflt x,xflt y,xflt col_FLT)
{
	vrt_geo[vrt_ind++]=x-5;	vrt_geo[vrt_ind++]=y-5;		vrt_geo[vrt_ind++]=col_FLT;	vrt_geo[vrt_ind++]=s_white_tex;	vrt_geo[vrt_ind++]=t_white_tex;
	vrt_geo[vrt_ind++]=x  ;	vrt_geo[vrt_ind++]=y+5;		vrt_geo[vrt_ind++]=col_FLT;	vrt_geo[vrt_ind++]=s_white_tex;	vrt_geo[vrt_ind++]=t_white_tex;
	vrt_geo[vrt_ind++]=x+5;	vrt_geo[vrt_ind++]=y-5;		vrt_geo[vrt_ind++]=col_FLT;	vrt_geo[vrt_ind++]=s_white_tex;	vrt_geo[vrt_ind++]=t_white_tex;

		vert_type=GL_TRIANGLES;
		vert_dims=2;
	if(	vrt_ind>=vrt_dim-15)
		plot_batch();
}

inline void add_line_2d_vertex(xflt x1,xflt y1,xflt col_FLT1)
{
	vrt_geo[vrt_ind++]=x1;	vrt_geo[vrt_ind++]=y1;	vrt_geo[vrt_ind++]=col_FLT1;	vrt_geo[vrt_ind++]=s_white_tex;	vrt_geo[vrt_ind++]=t_white_tex;

		vert_type=GL_LINE_STRIP;
		vert_dims=2;
	if(	vrt_ind>=vrt_dim-5)
		plot_batch();
}

inline void add_line_3d_vertex(xflt x1,xflt y1,xflt z1,xflt col_FLT1)
{
	vrt_geo[vrt_ind++]=x1;	vrt_geo[vrt_ind++]=y1;	vrt_geo[vrt_ind++]=z1;	vrt_geo[vrt_ind++]=col_FLT1;	vrt_geo[vrt_ind++]=s_white_tex;	vrt_geo[vrt_ind++]=t_white_tex;

		vert_type=GL_LINE_STRIP;
		vert_dims=3;
	if(	vrt_ind>=vrt_dim-6)
		plot_batch();
}

inline void add_line_2d(xflt x1,xflt y1,xflt col_FLT1,
						xflt x2,xflt y2,xflt col_FLT2)
{
	vrt_geo[vrt_ind++]=x1;	vrt_geo[vrt_ind++]=y1;	vrt_geo[vrt_ind++]=col_FLT1;	vrt_geo[vrt_ind++]=s_white_tex;	vrt_geo[vrt_ind++]=t_white_tex;
	vrt_geo[vrt_ind++]=x2;	vrt_geo[vrt_ind++]=y2;	vrt_geo[vrt_ind++]=col_FLT2;	vrt_geo[vrt_ind++]=s_white_tex;	vrt_geo[vrt_ind++]=t_white_tex;

		vert_type=GL_LINES;
		vert_dims=2;
	if(	vrt_ind>=vrt_dim-12)
		plot_batch();
}

inline void add_line_3d(xflt x1,xflt y1,xflt z1,xflt col_FLT1,
						xflt x2,xflt y2,xflt z2,xflt col_FLT2)
{
	vrt_geo[vrt_ind++]=x1;	vrt_geo[vrt_ind++]=y1;	vrt_geo[vrt_ind++]=z1;		vrt_geo[vrt_ind++]=col_FLT1;	vrt_geo[vrt_ind++]=s_white_tex;	vrt_geo[vrt_ind++]=t_white_tex;
	vrt_geo[vrt_ind++]=x2;	vrt_geo[vrt_ind++]=y2;	vrt_geo[vrt_ind++]=z2;		vrt_geo[vrt_ind++]=col_FLT2;	vrt_geo[vrt_ind++]=s_white_tex;	vrt_geo[vrt_ind++]=t_white_tex;

		vert_type=GL_LINES;
		vert_dims=3;
	if(	vrt_ind>=vrt_dim-12)
		plot_batch();
}

inline void add_tristrip_2d(xflt x1,xflt y1,xflt col_FLT1,xflt s1,xflt t1,
							xflt x2,xflt y2,xflt col_FLT2,xflt s2,xflt t2)
{
	vrt_geo[vrt_ind++]=x1;
	vrt_geo[vrt_ind++]=y1;
	vrt_geo[vrt_ind++]=col_FLT1;
	vrt_geo[vrt_ind++]=s1;
	vrt_geo[vrt_ind++]=t1;

	vrt_geo[vrt_ind++]=x2;
	vrt_geo[vrt_ind++]=y2;
	vrt_geo[vrt_ind++]=col_FLT2;
	vrt_geo[vrt_ind++]=s2;
	vrt_geo[vrt_ind++]=t2;

		vert_type=GL_TRIANGLE_STRIP;
		vert_dims=2;
	if(	vrt_ind>=vrt_dim-10)
		plot_batch();
}

inline void add_tristrip_3d(xflt x1,xflt y1,xflt z1,xflt col_FLT1,xflt s1,xflt t1,
							xflt x2,xflt y2,xflt z2,xflt col_FLT2,xflt s2,xflt t2)
{
	vrt_geo[vrt_ind++]=x1;
	vrt_geo[vrt_ind++]=y1;
	vrt_geo[vrt_ind++]=z1;
	vrt_geo[vrt_ind++]=col_FLT1;
	vrt_geo[vrt_ind++]=s1;
	vrt_geo[vrt_ind++]=t1;

	vrt_geo[vrt_ind++]=x2;
	vrt_geo[vrt_ind++]=y2;
	vrt_geo[vrt_ind++]=z2;
	vrt_geo[vrt_ind++]=col_FLT2;
	vrt_geo[vrt_ind++]=s2;
	vrt_geo[vrt_ind++]=t2;

		vert_type=GL_TRIANGLE_STRIP;
		vert_dims=3;
	if(	vrt_ind>=vrt_dim-12)
		plot_batch();
}

inline void add_quad_tex_2d_color_vert(	xflt x1,xflt y1 , xflt s1,xflt t1,xflt col_FLT1,
										xflt x2,xflt y2 , xflt s2,xflt t2,xflt col_FLT2,
										xflt x3,xflt y3 , xflt s3,xflt t3,xflt col_FLT3,
										xflt x4,xflt y4 , xflt s4,xflt t4,xflt col_FLT4)
{
	vrt_geo[vrt_ind++]=x1;	vrt_geo[vrt_ind++]=y1;		vrt_geo[vrt_ind++]=col_FLT1;	vrt_geo[vrt_ind++]=s1;	vrt_geo[vrt_ind++]=t1;
	vrt_geo[vrt_ind++]=x2;	vrt_geo[vrt_ind++]=y2;		vrt_geo[vrt_ind++]=col_FLT2;	vrt_geo[vrt_ind++]=s2;	vrt_geo[vrt_ind++]=t2;
	vrt_geo[vrt_ind++]=x3;	vrt_geo[vrt_ind++]=y3;		vrt_geo[vrt_ind++]=col_FLT3;	vrt_geo[vrt_ind++]=s3;	vrt_geo[vrt_ind++]=t3;
	vrt_geo[vrt_ind++]=x3;	vrt_geo[vrt_ind++]=y3;		vrt_geo[vrt_ind++]=col_FLT3;	vrt_geo[vrt_ind++]=s3;	vrt_geo[vrt_ind++]=t3;
	vrt_geo[vrt_ind++]=x4;	vrt_geo[vrt_ind++]=y4;		vrt_geo[vrt_ind++]=col_FLT4;	vrt_geo[vrt_ind++]=s4;	vrt_geo[vrt_ind++]=t4;
	vrt_geo[vrt_ind++]=x1;	vrt_geo[vrt_ind++]=y1;		vrt_geo[vrt_ind++]=col_FLT1;	vrt_geo[vrt_ind++]=s1;	vrt_geo[vrt_ind++]=t1;

		vert_type=GL_TRIANGLES;
		vert_dims=2;
	if(	vrt_ind>=vrt_dim-30)
		plot_batch();
}

inline void add_quad_tex_2d(xflt x1,xflt y1 , xflt s1,xflt t1,
							xflt x2,xflt y2 , xflt s2,xflt t2,
							xflt x3,xflt y3 , xflt s3,xflt t3,
							xflt x4,xflt y4 , xflt s4,xflt t4,xflt col_FLT)
{
	vrt_geo[vrt_ind++]=x1;	vrt_geo[vrt_ind++]=y1;		vrt_geo[vrt_ind++]=col_FLT;		vrt_geo[vrt_ind++]=s1;	vrt_geo[vrt_ind++]=t1;
	vrt_geo[vrt_ind++]=x2;	vrt_geo[vrt_ind++]=y2;		vrt_geo[vrt_ind++]=col_FLT;		vrt_geo[vrt_ind++]=s2;	vrt_geo[vrt_ind++]=t2;
	vrt_geo[vrt_ind++]=x3;	vrt_geo[vrt_ind++]=y3;		vrt_geo[vrt_ind++]=col_FLT;		vrt_geo[vrt_ind++]=s3;	vrt_geo[vrt_ind++]=t3;
	vrt_geo[vrt_ind++]=x3;	vrt_geo[vrt_ind++]=y3;		vrt_geo[vrt_ind++]=col_FLT;		vrt_geo[vrt_ind++]=s3;	vrt_geo[vrt_ind++]=t3;
	vrt_geo[vrt_ind++]=x4;	vrt_geo[vrt_ind++]=y4;		vrt_geo[vrt_ind++]=col_FLT;		vrt_geo[vrt_ind++]=s4;	vrt_geo[vrt_ind++]=t4;
	vrt_geo[vrt_ind++]=x1;	vrt_geo[vrt_ind++]=y1;		vrt_geo[vrt_ind++]=col_FLT;		vrt_geo[vrt_ind++]=s1;	vrt_geo[vrt_ind++]=t1;

		vert_type=GL_TRIANGLES;
		vert_dims=2;
	if(	vrt_ind>=vrt_dim-30)
		plot_batch();
}

inline void add_quad_tex_3d(xflt x1,xflt y1,xflt z1 , xflt s1,xflt t1,
							xflt x2,xflt y2,xflt z2 , xflt s2,xflt t2,
							xflt x3,xflt y3,xflt z3 , xflt s3,xflt t3,
							xflt x4,xflt y4,xflt z4 , xflt s4,xflt t4,xflt col_FLT)
{
	vrt_geo[vrt_ind++]=x1;	vrt_geo[vrt_ind++]=y1;	vrt_geo[vrt_ind++]=z1;		vrt_geo[vrt_ind++]=col_FLT;		vrt_geo[vrt_ind++]=s1;	vrt_geo[vrt_ind++]=t1;
	vrt_geo[vrt_ind++]=x2;	vrt_geo[vrt_ind++]=y2;	vrt_geo[vrt_ind++]=z2;		vrt_geo[vrt_ind++]=col_FLT;		vrt_geo[vrt_ind++]=s2;	vrt_geo[vrt_ind++]=t2;
	vrt_geo[vrt_ind++]=x3;	vrt_geo[vrt_ind++]=y3;	vrt_geo[vrt_ind++]=z3;		vrt_geo[vrt_ind++]=col_FLT;		vrt_geo[vrt_ind++]=s3;	vrt_geo[vrt_ind++]=t3;
	vrt_geo[vrt_ind++]=x3;	vrt_geo[vrt_ind++]=y3;	vrt_geo[vrt_ind++]=z3;		vrt_geo[vrt_ind++]=col_FLT;		vrt_geo[vrt_ind++]=s3;	vrt_geo[vrt_ind++]=t3;
	vrt_geo[vrt_ind++]=x4;	vrt_geo[vrt_ind++]=y4;	vrt_geo[vrt_ind++]=z4;		vrt_geo[vrt_ind++]=col_FLT;		vrt_geo[vrt_ind++]=s4;	vrt_geo[vrt_ind++]=t4;
	vrt_geo[vrt_ind++]=x1;	vrt_geo[vrt_ind++]=y1;	vrt_geo[vrt_ind++]=z1;		vrt_geo[vrt_ind++]=col_FLT;		vrt_geo[vrt_ind++]=s1;	vrt_geo[vrt_ind++]=t1;

		vert_type=GL_TRIANGLES;
		vert_dims=3;
	if(	vrt_ind>=vrt_dim-36)
		plot_batch();
}

inline void add_quad_tex_3d_col(xflt x1,xflt y1,xflt z1 , xflt s1,xflt t1,xflt col_FLT1,
								xflt x2,xflt y2,xflt z2 , xflt s2,xflt t2,xflt col_FLT2,
								xflt x3,xflt y3,xflt z3 , xflt s3,xflt t3,xflt col_FLT3,
								xflt x4,xflt y4,xflt z4 , xflt s4,xflt t4,xflt col_FLT4)
{
	vrt_geo[vrt_ind++]=x1;	vrt_geo[vrt_ind++]=y1;	vrt_geo[vrt_ind++]=z1;		vrt_geo[vrt_ind++]=col_FLT1;	vrt_geo[vrt_ind++]=s1;	vrt_geo[vrt_ind++]=t1;
	vrt_geo[vrt_ind++]=x2;	vrt_geo[vrt_ind++]=y2;	vrt_geo[vrt_ind++]=z2;		vrt_geo[vrt_ind++]=col_FLT2;	vrt_geo[vrt_ind++]=s2;	vrt_geo[vrt_ind++]=t2;
	vrt_geo[vrt_ind++]=x3;	vrt_geo[vrt_ind++]=y3;	vrt_geo[vrt_ind++]=z3;		vrt_geo[vrt_ind++]=col_FLT3;	vrt_geo[vrt_ind++]=s3;	vrt_geo[vrt_ind++]=t3;
	vrt_geo[vrt_ind++]=x3;	vrt_geo[vrt_ind++]=y3;	vrt_geo[vrt_ind++]=z3;		vrt_geo[vrt_ind++]=col_FLT3;	vrt_geo[vrt_ind++]=s3;	vrt_geo[vrt_ind++]=t3;
	vrt_geo[vrt_ind++]=x4;	vrt_geo[vrt_ind++]=y4;	vrt_geo[vrt_ind++]=z4;		vrt_geo[vrt_ind++]=col_FLT4;	vrt_geo[vrt_ind++]=s4;	vrt_geo[vrt_ind++]=t4;
	vrt_geo[vrt_ind++]=x1;	vrt_geo[vrt_ind++]=y1;	vrt_geo[vrt_ind++]=z1;		vrt_geo[vrt_ind++]=col_FLT1;	vrt_geo[vrt_ind++]=s1;	vrt_geo[vrt_ind++]=t1;

		vert_type=GL_TRIANGLES;
		vert_dims=3;
	if(	vrt_ind>=vrt_dim-36)
		plot_batch();
}

inline void add_icon(xflt x,xflt y , xflt val,xflt num_vals , xflt col_FLT , xflt psi_rad=0.0 , xflt size_rat=1.0)
{
	const xflt ds=tman.tex_dx_vis[vert_tex]/tman.tex_dx_tot[vert_tex]	/num_vals	;
	const xflt dt=tman.tex_dy_vis[vert_tex]/tman.tex_dy_tot[vert_tex]				;
	const xint dx=tman.tex_dx_vis[vert_tex]								/num_vals	;	const xint sdx=dx*0.5*size_rat;
	const xint dy=tman.tex_dy_vis[vert_tex]											;	const xint sdy=dy*0.5*size_rat;
	const xflt s1= val		*ds;
	const xflt s2=(val+1.0)	*ds;

	if(psi_rad==0)
	{
		vrt_geo[vrt_ind++]=x-sdx;	vrt_geo[vrt_ind++]=y-sdy;		vrt_geo[vrt_ind++]=col_FLT;		vrt_geo[vrt_ind++]=s1;	vrt_geo[vrt_ind++]= 0;
		vrt_geo[vrt_ind++]=x-sdx;	vrt_geo[vrt_ind++]=y+sdy;		vrt_geo[vrt_ind++]=col_FLT;		vrt_geo[vrt_ind++]=s1;	vrt_geo[vrt_ind++]=dt;
		vrt_geo[vrt_ind++]=x+sdx;	vrt_geo[vrt_ind++]=y+sdy;		vrt_geo[vrt_ind++]=col_FLT;		vrt_geo[vrt_ind++]=s2;	vrt_geo[vrt_ind++]=dt;
		vrt_geo[vrt_ind++]=x+sdx;	vrt_geo[vrt_ind++]=y+sdy;		vrt_geo[vrt_ind++]=col_FLT;		vrt_geo[vrt_ind++]=s2;	vrt_geo[vrt_ind++]=dt;
		vrt_geo[vrt_ind++]=x+sdx;	vrt_geo[vrt_ind++]=y-sdy;		vrt_geo[vrt_ind++]=col_FLT;		vrt_geo[vrt_ind++]=s2;	vrt_geo[vrt_ind++]= 0;
		vrt_geo[vrt_ind++]=x-sdx;	vrt_geo[vrt_ind++]=y-sdy;		vrt_geo[vrt_ind++]=col_FLT;		vrt_geo[vrt_ind++]=s1;	vrt_geo[vrt_ind++]= 0;
	}
	else
	{
		xflt sin_psi=sin(psi_rad);
		xflt cos_psi=cos(psi_rad);

		vrt_geo[vrt_ind++]=x-sdx*cos_psi - sdy*sin_psi;	vrt_geo[vrt_ind++]=y-sdy*cos_psi + sdx*sin_psi;		vrt_geo[vrt_ind++]=col_FLT;		vrt_geo[vrt_ind++]=s1;	vrt_geo[vrt_ind++]= 0;
		vrt_geo[vrt_ind++]=x-sdx*cos_psi + sdy*sin_psi;	vrt_geo[vrt_ind++]=y+sdy*cos_psi + sdx*sin_psi;		vrt_geo[vrt_ind++]=col_FLT;		vrt_geo[vrt_ind++]=s1;	vrt_geo[vrt_ind++]=dt;
		vrt_geo[vrt_ind++]=x+sdx*cos_psi + sdy*sin_psi;	vrt_geo[vrt_ind++]=y+sdy*cos_psi - sdx*sin_psi;		vrt_geo[vrt_ind++]=col_FLT;		vrt_geo[vrt_ind++]=s2;	vrt_geo[vrt_ind++]=dt;
		vrt_geo[vrt_ind++]=x+sdx*cos_psi + sdy*sin_psi;	vrt_geo[vrt_ind++]=y+sdy*cos_psi - sdx*sin_psi;		vrt_geo[vrt_ind++]=col_FLT;		vrt_geo[vrt_ind++]=s2;	vrt_geo[vrt_ind++]=dt;
		vrt_geo[vrt_ind++]=x+sdx*cos_psi - sdy*sin_psi;	vrt_geo[vrt_ind++]=y-sdy*cos_psi - sdx*sin_psi;		vrt_geo[vrt_ind++]=col_FLT;		vrt_geo[vrt_ind++]=s2;	vrt_geo[vrt_ind++]= 0;
		vrt_geo[vrt_ind++]=x-sdx*cos_psi - sdy*sin_psi;	vrt_geo[vrt_ind++]=y-sdy*cos_psi + sdx*sin_psi;		vrt_geo[vrt_ind++]=col_FLT;		vrt_geo[vrt_ind++]=s1;	vrt_geo[vrt_ind++]= 0;
	}

		vert_type=GL_TRIANGLES;
		vert_dims=2;
	if(	vrt_ind>=vrt_dim-30)
		plot_batch();
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// PATH PLOT
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
#if XAVION	//only xavion has the t_map_path tex
inline void path_2d(const xint path_2d_en,const xflt i_xpix=0.0,const xflt i_ypix=0.0,const xflt i_psir=0.0,const xflt i_fcol=0.0)
{
	const xflt t_mid=0.5;
	const xflt t_end=1.0;

	static xflt run_xpix=0.0;
	static xflt run_ypix=0.0;
	static xflt run_fcol=0.0;

	static xflt osx=0.0;
	static xflt osy=0.0;
	static xflt sdx=0.0;
	static xflt sdy=0.0;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// INIT
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(path_2d_en==path_2d_init)
	{
		init_batch(t_map_path , col_vert_true,dep_read_fals,dep_writ_fals);

		vert_type=GL_TRIANGLE_STRIP;
		vert_dims=2;
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// GEO
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(path_2d_en==path_2d_add)
	{
		run_xpix=i_xpix;
		run_ypix=i_ypix;
		run_fcol=i_fcol;

		osx= path_dw*sin(i_psir);
		osy= path_dw*cos(i_psir);
		sdx=-path_dw*cos(i_psir);
		sdy= path_dw*sin(i_psir);

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// STARTING ENDCAP
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		if(vrt_ind==0)
		{
			vrt_geo[vrt_ind++]=run_xpix-osx-sdx;
			vrt_geo[vrt_ind++]=run_ypix-osy-sdy;
			vrt_geo[vrt_ind++]=run_fcol;
			vrt_geo[vrt_ind++]=0.0;
			vrt_geo[vrt_ind++]=t_end;

			vrt_geo[vrt_ind++]=run_xpix-osx+sdx;
			vrt_geo[vrt_ind++]=run_ypix-osy+sdy;
			vrt_geo[vrt_ind++]=run_fcol;
			vrt_geo[vrt_ind++]=1.0;
			vrt_geo[vrt_ind++]=t_end;
		}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// GEO
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		vrt_geo[vrt_ind++]=run_xpix-sdx;
		vrt_geo[vrt_ind++]=run_ypix-sdy;
		vrt_geo[vrt_ind++]=run_fcol;
		vrt_geo[vrt_ind++]=0.0;
		vrt_geo[vrt_ind++]=t_mid;

		vrt_geo[vrt_ind++]=run_xpix+sdx;
		vrt_geo[vrt_ind++]=run_ypix+sdy;
		vrt_geo[vrt_ind++]=run_fcol;
		vrt_geo[vrt_ind++]=1.0;
		vrt_geo[vrt_ind++]=t_mid;

		if(	vrt_ind>=vrt_dim-10)
			plot_batch();
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// ENDING ENDCAP
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(path_2d_en==path_2d_plot)
	{
		vrt_geo[vrt_ind++]=run_xpix+osx-sdx;
		vrt_geo[vrt_ind++]=run_ypix+osy-sdy;
		vrt_geo[vrt_ind++]=run_fcol;
		vrt_geo[vrt_ind++]=0.0;
		vrt_geo[vrt_ind++]=t_end;

		vrt_geo[vrt_ind++]=run_xpix+osx+sdx;
		vrt_geo[vrt_ind++]=run_ypix+osy+sdy;
		vrt_geo[vrt_ind++]=run_fcol;
		vrt_geo[vrt_ind++]=1.0;
		vrt_geo[vrt_ind++]=t_end;

		plot_batch();
	}
}
#endif
};
extern batch_class batch;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// GRAF
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
enum
{
	orient_portrait=0	,
	orient_invert		,
	orient_land_lft		,	// IPAD rotated to the left  90 deg
	orient_land_rgt			// IPAD rotated to the right 90 deg
};

class graf_class
{
public:
   ~graf_class(){}
	graf_class()
	{
		memset(this,0,sizeof(*this));
		dev_orient		= orient_portrait;
		pix_scale		= 1.0;
		key_entry_code	=-1;	// -1 for no button to keyboard right now of course
	}

	xint is_phone,dev_orient;
	xflt dev_dx_raw,dev_dy_raw,pix_scale;
	xint key_entry_code;		// button code for the current keyboard entry: it is the button that is orange and takes input

	xint sci_on;				// current scissors, which apply to mouse-clicks as well so we do not touch airports out of the map, scrolling items our of range, etc
	xflt sci_x1;				// current scissors, which apply to mouse-clicks as well so we do not touch airports out of the map, scrolling items our of range, etc
	xflt sci_y1;				// current scissors, which apply to mouse-clicks as well so we do not touch airports out of the map, scrolling items our of range, etc
	xflt sci_x2;				// current scissors, which apply to mouse-clicks as well so we do not touch airports out of the map, scrolling items our of range, etc
	xflt sci_y2;				// current scissors, which apply to mouse-clicks as well so we do not touch airports out of the map, scrolling items our of range, etc

	inline xint dev_orient_now	(){return  dev_orient;}
	inline xint in_landscape	(){return (dev_orient_now()==orient_land_lft || dev_orient_now()==orient_land_rgt	);}
	inline xint in_portrait		(){return (dev_orient_now()==orient_portrait || dev_orient_now()==orient_invert		);}

	inline xflt dev_cx_use		(){return (dev_orient_now()==orient_portrait || dev_orient_now()==orient_invert)?dev_dx_raw*0.5	:dev_dy_raw*0.5	;}	// con pad uses this
	inline xflt dev_cy_use		(){return (dev_orient_now()==orient_portrait || dev_orient_now()==orient_invert)?dev_dy_raw*0.5	:dev_dx_raw*0.5	;}	// con pad uses this

	inline xflt dev_dx_use		(){return (dev_orient_now()==orient_portrait || dev_orient_now()==orient_invert)?dev_dx_raw		:dev_dy_raw		;}
	inline xflt dev_dy_use		(){return (dev_orient_now()==orient_portrait || dev_orient_now()==orient_invert)?dev_dy_raw		:dev_dx_raw		;}

	inline xflt buff_y2			(){if(!is_phone)return 0;	if(dev_orient_now()==orient_portrait)return 100;	if(dev_orient_now()==orient_invert)return  26;	return 0;}	// space for swipe-up or camera
	inline xflt buff_y1			(){if(!is_phone)return 0;	if(dev_orient_now()==orient_portrait)return  26;	if(dev_orient_now()==orient_invert)return 100;	return 0;}	// space for swipe-up or camera

	inline xint key_entry_code_for_xy(const xflt x,const xflt y){return intround(y*dev_dx_use()+x);}
};
extern class graf_class graf;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// EXTERN
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void OGL_arc(const enum_col col,const xflt alpha,const xflt x,const xflt y,const xflt r,const xflt a1,const xflt da);
void OGL_line(const enum_col col,const xflt alpha,const xflt x1,const xflt y1,const xflt x2,const xflt y2);
void OGL_framerect(const enum_col col,const xflt alpha,const xflt x,const xflt y,const xflt dx,const xflt dy);
void OGL_fillrect(const enum_col col,const xflt alpha,const xflt x,const xflt y,const xflt dx,const xflt dy);
void OGL_filltri(const enum_col col,const xflt alpha,const xflt x1,const xflt y1,const xflt x2,const xflt y2,const xflt x3,const xflt y3);
void OGL_quad(const enum_tex tex,const enum_col col,const xflt alpha,const xflt x1,const xflt y1,const xflt s1,const xflt t1,const xflt x2,const xflt y2,const xflt s2,const xflt t2,const xflt x3,const xflt y3,const xflt s3,const xflt t3,const xflt x4,const xflt y4,const xflt s4,const xflt t4);
void OGL_quad_lines(const enum_tex tex,const enum_col col,const xflt alpha,		// this is CW order
					const xflt x1,const xflt y1 , const xflt s1,const xflt t1,
					const xflt x2,const xflt y2 , const xflt s2,const xflt t2,
					const xflt x3,const xflt y3 , const xflt s3,const xflt t3,
					const xflt x4,const xflt y4 , const xflt s4,const xflt t4);
void OGL_quad_alpha(const enum_tex tex,const enum_col col,		// this is CW order
					const xflt x1,const xflt y1 , const xflt s1,const xflt t1,const xflt a1,
					const xflt x2,const xflt y2 , const xflt s2,const xflt t2,const xflt a2,
					const xflt x3,const xflt y3 , const xflt s3,const xflt t3,const xflt a3,
					const xflt x4,const xflt y4 , const xflt s4,const xflt t4,const xflt a4);
void OGL_tri_alpha(	const enum_tex tex,const enum_col col,		// this is CW order
					const xflt x1,const xflt y1,const xflt z1 , const xflt s1,const xflt t1,const xflt a1,
					const xflt x2,const xflt y2,const xflt z2 , const xflt s2,const xflt t2,const xflt a2,
					const xflt x3,const xflt y3,const xflt z3 , const xflt s3,const xflt t3,const xflt a3);
void OGL_line_fade(const enum_col col,const xflt a1,const xflt a2,const xflt x1,const xflt y1,const xflt x2,const xflt y2);
void OGL_rect_fade(xflt x1,xflt y1,xflt col_FLT1,xflt x2,xflt y2,xflt col_FLT2,xflt x3,xflt y3,xflt col_FLT3,xflt x4,xflt y4,xflt col_FLT4);
void OGL_quad3d(const enum_tex tex,const enum_col col,const xflt alpha,const xflt x1,const xflt y1,const xflt z1,const xflt s1,const xflt t1,const xflt x2,const xflt y2,const xflt z2,const xflt s2,const xflt t2,const xflt x3,const xflt y3,const xflt z3,const xflt s3,const xflt t3,const xflt x4,const xflt y4,const xflt z4,const xflt s4,const xflt t4);
void app_rect_to_device_rect(xflt x1_app_space,xflt* x1_ipad,xflt y1_app_space,xflt* y1_ipad,xflt x2_app_space,xflt* x2_ipad,xflt y2_app_space,xflt* y2_ipad);
void OGL_poly_offset(const xint amount);
void sci_stop();
void sci_start(xflt x1_app_space,xflt y1_app_space,xflt dx_app_space,xflt dy_app_space);
void OGL_set_viewport_2d(xflt x1_app_space,xflt y1_app_space,xflt x2_app_space,xflt y2_app_space);
void OGL_set_viewport_3d(xflt fov_l,xflt fov_r,xflt fov_d,xflt fov_u,xflt vis_m);

inline xflt top_y(const xflt y1_window,const xflt y2_window,const xflt dy_ui)
{
	const xflt ret = y2_window-graf.buff_y2() - (y2_window-graf.buff_y2()-y1_window-dy_ui) * 0.5;

//	OGL_line(col_yellow,1.0,0,y2_window-1,9999,y2_window-1);
//	OGL_line(col_yellow,1.0,0,y1_window+1,9999,y1_window+1);
//	OGL_fillrect(col_green,0.2 , 0,ret-dy_ui , 9999,dy_ui);

	return ret;
}
#endif
