#include "hl_types.h"

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
/*
GLES does not even have the ortho and identity and stuff!
That's why we have the XV_glXXXXX stuff below!
So this just emulates the old behavior in the shader!
*/
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
static GLfloat gIdentity[16]=
{
	1.0,0.0,0.0,0.0,
	0.0,1.0,0.0,0.0,
	0.0,0.0,1.0,0.0,
	0.0,0.0,0.0,1.0
};

void setIdentityMatrix(GLfloat m[16])
{
	memcpy(m,gIdentity,sizeof(gIdentity));
}

void buildRotation(GLfloat m[16],GLfloat angle,GLfloat x,GLfloat y,GLfloat z)
{
	memcpy(m,gIdentity,sizeof(gIdentity));

	GLfloat s=sin(angle*degtorad);
	GLfloat c=cos(angle*degtorad);
	GLboolean optimized=GL_FALSE;

#define M(row,col) m[col*4+row]

	if(x!=0.0)	/* rotate only around x-axis */
	if(y==0.0)
	if(z==0.0)
	{
		optimized=GL_TRUE;

		M(1,1)=c;
		M(2,2)=c;

		if(x<0.0)
		{
			M(1,2)= s;
			M(2,1)=-s;
		}
		else
		{
			M(1,2)=-s;
			M(2,1)= s;
		}
	}

	if(x==0.0)
	if(y!=0.0)	 /* rotate only around y-axis */
	if(z==0.0)
	{
		optimized=GL_TRUE;

		M(0,0)=c;
		M(2,2)=c;

		if(y<0.0)
		{
			M(0,2)=-s;
			M(2,0)= s;
		}
		else
		{
			M(0,2)= s;
			M(2,0)=-s;
		}
	}

	if(x==0.0)
	if(y==0.0)
	if(z!=0.0)	/* rotate only around z-axis */
	{
		optimized=GL_TRUE;

		M(0,0)=c;
		M(1,1)=c;

		if(z<0.0)
		{
			M(0,1)= s;
			M(1,0)=-s;
		}
		else
		{
			M(0,1)=-s;
			M(1,0)= s;
		}
	}

	if(!optimized)
	{
		const GLfloat mag=sqrt(x*x + y*y + z*z);
		if(mag<=1.0e-4)
			return;		/* no rotation, leave matrix as-is */

		x/=mag;
		y/=mag;
		z/=mag;

		GLfloat xx=x*x;
		GLfloat yy=y*y;
		GLfloat zz=z*z;
		GLfloat xy=x*y;
		GLfloat yz=y*z;
		GLfloat zx=z*x;
		GLfloat xs=x*s;
		GLfloat ys=y*s;
		GLfloat zs=z*s;
		GLfloat one_c=1.0-c;

		M(0,0)=(one_c*xx)+c ;
		M(0,1)=(one_c*xy)-zs;
		M(0,2)=(one_c*zx)+ys;
		M(1,0)=(one_c*xy)+zs;
		M(1,1)=(one_c*yy)+c ;
		M(1,2)=(one_c*yz)-xs;
		M(2,0)=(one_c*zx)-ys;
		M(2,1)=(one_c*yz)+xs;
		M(2,2)=(one_c*zz)+c ;
	}

#undef M
}

inline void multMatrices44(GLfloat dst[16],const GLfloat a[16],const GLfloat b[16])
{
	dst[0 ]=b[0 ]*a[0] + b[1 ]*a[4] + b[2 ]*a[8 ] + b[3 ]*a[12];
	dst[1 ]=b[0 ]*a[1] + b[1 ]*a[5] + b[2 ]*a[9 ] + b[3 ]*a[13];
	dst[2 ]=b[0 ]*a[2] + b[1 ]*a[6] + b[2 ]*a[10] + b[3 ]*a[14];
	dst[3 ]=b[0 ]*a[3] + b[1 ]*a[7] + b[2 ]*a[11] + b[3 ]*a[15];
	dst[4 ]=b[4 ]*a[0] + b[5 ]*a[4] + b[6 ]*a[8 ] + b[7 ]*a[12];
	dst[5 ]=b[4 ]*a[1] + b[5 ]*a[5] + b[6 ]*a[9 ] + b[7 ]*a[13];
	dst[6 ]=b[4 ]*a[2] + b[5 ]*a[6] + b[6 ]*a[10] + b[7 ]*a[14];
	dst[7 ]=b[4 ]*a[3] + b[5 ]*a[7] + b[6 ]*a[11] + b[7 ]*a[15];
	dst[8 ]=b[8 ]*a[0] + b[9 ]*a[4] + b[10]*a[8 ] + b[11]*a[12];
	dst[9 ]=b[8 ]*a[1] + b[9 ]*a[5] + b[10]*a[9 ] + b[11]*a[13];
	dst[10]=b[8 ]*a[2] + b[9 ]*a[6] + b[10]*a[10] + b[11]*a[14];
	dst[11]=b[8 ]*a[3] + b[9 ]*a[7] + b[10]*a[11] + b[11]*a[15];
	dst[12]=b[12]*a[0] + b[13]*a[4] + b[14]*a[8 ] + b[15]*a[12];
	dst[13]=b[12]*a[1] + b[13]*a[5] + b[14]*a[9 ] + b[15]*a[13];
	dst[14]=b[12]*a[2] + b[13]*a[6] + b[14]*a[10] + b[15]*a[14];
	dst[15]=b[12]*a[3] + b[13]*a[7] + b[14]*a[11] + b[15]*a[15];
}

inline void copyMatrix(GLfloat dst[16],const GLfloat src[16])
{
	memcpy(dst,src,sizeof(GLfloat)*16);
}

inline void applyTranslation4( GLfloat m[16],GLfloat x,GLfloat y,GLfloat z)
{
	m[12]=m[0]*x + m[4]*y + m[8] *z + m[12];
	m[13]=m[1]*x + m[5]*y + m[9] *z + m[13];
	m[14]=m[2]*x + m[6]*y + m[10]*z + m[14];
	m[15]=m[3]*x + m[7]*y + m[11]*z + m[15];
}

inline void applyRotation4( GLfloat m[16],GLfloat angle,GLfloat x,GLfloat y,GLfloat z)
{
	GLfloat	temp[16],rot[16];
	buildRotation(rot,angle,x,y,z);
	multMatrices44(temp,m,rot);
	copyMatrix(m,temp);
}

void buildFrustumMatrix(GLfloat m[16],
						GLfloat l,GLfloat r,
						GLfloat b,GLfloat t,
						GLfloat n,GLfloat f)
{
	m[ 0]= (2.0*n)/(r-l);
	m[ 1]=  0.0;
	m[ 2]=  0.0;
	m[ 3]=  0.0;
	m[ 4]=  0.0;
	m[ 5]= (2.0*n)/(t-b);
	m[ 6]=  0.0;
	m[ 7]=  0.0;
	m[ 8]=  (r+l)/(r-l);
	m[ 9]=  (t+b)/(t-b);
	m[10]= -(f+n)/(f-n);
	m[11]= -1.0;
	m[12]=  0.0;
	m[13]=  0.0;
	m[14]=-(2.0*f*n)/(f-n);
	m[15]=  0.0;
}

void buildOrthoMatrix(	GLfloat m[16],
						GLfloat l,GLfloat r,
						GLfloat b,GLfloat t,
						GLfloat n,GLfloat f)
{
	GLfloat x = 2.0	/(r-l);
	GLfloat y = 2.0	/(t-b);
	GLfloat z =-2.0		/(f-n);
	GLfloat tx=-(r+l)	/(r-l);
	GLfloat ty=-(t+b)	/(t-b);
	GLfloat tz=-(f+n)	/(f-n);

#define M(row,col)  m[col*4+row]

	M(0,0)=x  ;	M(0,1)=0.0;	M(0,2)=0.0;	M(0,3)=tx ;
	M(1,0)=0.0;	M(1,1)=y  ;	M(1,2)=0.0;	M(1,3)=ty ;
	M(2,0)=0.0;	M(2,1)=0.0;	M(2,2)=z  ;	M(2,3)=tz ;
	M(3,0)=0.0;	M(3,1)=0.0;	M(3,2)=0.0;	M(3,3)=1.0;

#undef M
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// WE USE OPENGL 2.0 WHICH USES SHADERS, SO WE HAVE A LAYER OF ABSTRACTION HERE TO USE ALL THE GL CALLS THAT XAVION USES
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
GLint u_tex0,u_mvp_matrix,attr_position,attr_tex_coord,attr_color=0;

GLuint load_shader(const char *shader_source,GLenum type)
{
	const char* shader_prefix="// prefix.\n";

	const char* srcs[2]={ shader_prefix,shader_source };
	GLuint			shader=glCreateShader(type);
	glShaderSource (shader,2,srcs,NULL);
	glCompileShader(shader);

	GLint compile_status;
	glGetShaderiv(shader,GL_COMPILE_STATUS,&compile_status);
	if(!compile_status)
	{
		vector<char> buf;
		GLint log_len;
		glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&log_len);
		buf.resize(log_len+1);
		buf.back()=0;
		glGetShaderInfoLog(shader,(GLsizei)buf.size()-1,&log_len,&*buf.begin());
		APP_alert("could not load shader!",&*buf.begin(),"","");
	}

	return shader;
}

static int mvp_dirty=1;

static GLfloat mv[16]		 ={ 1,0,0,0,
								0,1,0,0,
								0,0,1,0,
								0,0,0,1 };
static GLfloat projection[16]={ 1,0,0,0,
								0,1,0,0,
								0,0,1,0,
								0,0,0,1 };

static vector<GLfloat>	mv_stack;
static vector<GLfloat>	proj_stack;
static GLfloat *		the_matrix		= mv		;
static vector<GLfloat> *the_matrix_stack=&mv_stack	;

void setup_shader(void)
{
	if(mvp_dirty)
	{
		GLfloat mvp[16];
		multMatrices44(mvp,projection,mv);
		glUniformMatrix4fv(u_mvp_matrix,1,GL_FALSE,mvp);
		mvp_dirty=0;
	}
}

void XV_glMatrixMode(GLenum mode)
{
	switch(mode)
	{
		case GL_MODELVIEW:
			the_matrix=mv;
			the_matrix_stack=&mv_stack;
			break;
		case GL_PROJECTION:
			the_matrix=projection;
			the_matrix_stack=&proj_stack;
			break;
		default:
			APP_alert("illegal matrix mode","","","");
	}
}

void XV_glLoadIdentity(void)
{
	setIdentityMatrix(the_matrix);
	mvp_dirty=1;
}

void XV_glOrtho(GLdouble left,GLdouble right,GLdouble bottom,GLdouble top,GLdouble zNear,GLdouble zFar)
{
	GLfloat o[16],m[16];
	buildOrthoMatrix(o,left,right,bottom,top,zNear,zFar);
	multMatrices44(m,the_matrix,o);
	copyMatrix(the_matrix,m);
	mvp_dirty=1;
}

void XV_glFrustum(GLdouble left,GLdouble right,GLdouble bottom,GLdouble top,GLdouble zNear,GLdouble zFar)
{
	GLfloat f[16],m[16];
	buildFrustumMatrix(f,left,right,bottom,top,zNear,zFar);
	multMatrices44(m,the_matrix,f);
	copyMatrix(the_matrix,m);
	mvp_dirty=1;
}

void XV_glPushMatrix(void)
{
	the_matrix_stack->insert(the_matrix_stack->end(),the_matrix,the_matrix+16);
}

void XV_glPopMatrix(void)
{
	if(the_matrix_stack->empty())APP_alert("can't pop matrix with nothing on it!","","","");
	memcpy(the_matrix,&*the_matrix_stack->end()-16,sizeof(GLfloat)*16);
	the_matrix_stack->resize(the_matrix_stack->size()-16);
	mvp_dirty=1;
}

void XV_glTranslatef(GLfloat x,GLfloat y,GLfloat z)
{
	applyTranslation4(the_matrix,x,y,z);
	mvp_dirty=1;
}

void XV_glRotatef(GLfloat angle,GLfloat x,GLfloat y,GLfloat z)
{
	applyRotation4(the_matrix,angle,x,y,z);
	mvp_dirty=1;
}

#undef glDrawArrays
#undef glDrawElements

void XV_glDrawArrays(GLenum mode,GLint first,GLsizei count)
{
	setup_shader();
	glDrawArrays(mode,first,count);
}

void XV_glDrawElements(GLenum mode,GLsizei count,GLenum type,const GLvoid *indices)
{
	setup_shader();
	glDrawElements(mode,count,type,indices);
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// INIT
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void OGL_init(void)
{
	dev_alert_OGL("OGL_init");

	glBlendFunc		(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc		(GL_LEQUAL							);
	glFrontFace		(GL_CW								);
	glDepthMask		(GL_FALSE							);
	glCullFace		(GL_BACK							);
	glDisable		(GL_DEPTH_TEST						);
	glEnable		(GL_CULL_FACE						);
	glEnable		(GL_BLEND							);
	glPixelStorei	(GL_UNPACK_ALIGNMENT,1				);	// the default is to align the bytes in the bitmap to the nearest 4.
	glPixelStorei	(GL_PACK_ALIGNMENT  ,1				);	// this requires a row width of (3*Width+3)&~3 for access, which is annoying... we therefore align to nearest byte... this is faster anyway

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
/*
October 2019 from Sidney:
There is no longer a non-shader OpenGL because no hardware works that way!
So we ALWAYS have shaders, and here this is!


OpenGL ES-2 REQUIRES shaders.
This is what we have here.

This is elsewhere, and gives a opretty decent hint as to what OGL we are using!
 context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

NOTE:
	GL is for fesktop
	GL ES is for mobile!

GLES does not even have the ortho and identity and stuff!
thats why we have the XV_glXXXXX stuff above!
*/
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	const char vertex_src[] =
	"													\
		uniform	highp mat4			u_mvp_matrix;		\
														\
		attribute highp	vec4		position;			\
		attribute highp vec2		tex_coord;			\
		attribute lowp  vec4		color;				\
														\
		varying highp	vec2		v_tex_coord;		\
		varying lowp	vec4		v_color;			\
														\
		void main()										\
		{												\
			gl_Position=u_mvp_matrix * position;		\
			v_tex_coord = tex_coord;					\
			v_color = color;							\
		}												\
	";

	const char fragment_src[] =
	"													\
		uniform lowp	sampler2D	u_tex0;				\
		varying highp	vec2		v_tex_coord;		\
		varying lowp	vec4		v_color;			\
														\
		void main()										\
		{												\
			gl_FragColor=v_color *						\
				texture2D(u_tex0,v_tex_coord);			\
		}												\
	";

	GLuint vertexShader		=load_shader(vertex_src		,GL_VERTEX_SHADER	);	// the openGL part: load vertex shader
	GLuint fragmentShader	=load_shader(fragment_src	,GL_FRAGMENT_SHADER	);	// the openGL part: load fragment shader

	GLuint shaderProgram=glCreateProgram();			// create program object
	dev_alert_OGL("glCreateProgram");

	glAttachShader	(shaderProgram,vertexShader);	// and attach both...
	dev_alert_OGL("vertexShader");

	glAttachShader	(shaderProgram,fragmentShader);	// ...shaders to it
	dev_alert_OGL("fragmentShader");

	glLinkProgram	(shaderProgram);				// link the program
	dev_alert_OGL("glLinkProgram");

	glUseProgram	(shaderProgram);				// and select it for usage: THIS IS WHERE WE BIND THE SAHDER! SO IT IS BOUND HERE FOR EVERYTHING!
	dev_alert_OGL("glUseProgram");

	u_tex0			=glGetUniformLocation(shaderProgram,"u_tex0"		);
	u_mvp_matrix	=glGetUniformLocation(shaderProgram,"u_mvp_matrix"	);
	attr_position	=glGetAttribLocation (shaderProgram,"position"		);
	attr_tex_coord	=glGetAttribLocation (shaderProgram,"tex_coord"		);
	attr_color		=glGetAttribLocation (shaderProgram,"color"			);

	if(u_tex0<0||u_mvp_matrix<0							){cerr<<"Unable to get uniform location"<<endl; return;}
	if(attr_position<0||attr_tex_coord<0||attr_color<0	){cerr<<"Unable to get attrib location"	<<endl; return;}

	OGL_set_viewport_2d(0,0,graf.dev_dx_use(),graf.dev_dy_use());

	glEnableVertexAttribArray(attr_position	);
	glEnableVertexAttribArray(attr_tex_coord);
	glDisableVertexAttribArray(attr_color	);
}
