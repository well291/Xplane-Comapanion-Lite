tman_class tman;

GLubyte planet_array[planet_di][planet_dj][3]={0};

//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
xint tman_class::tex_loaded(const enum_tex t)
{
	return(	tex_dx_vis[t]>0 ||
			tex_dy_vis[t]>0 ||
			tex_dx_tot[t]>0 ||
			tex_dy_tot[t]>0 );
}

void tman_class::tex_unload(const enum_tex t)
{
	tex_dx_vis[t]=0;
	tex_dy_vis[t]=0;
	tex_dx_tot[t]=0;
	tex_dy_tot[t]=0;

	GLuint tex=t;
	glDeleteTextures(1,&tex);
}

//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
void find_spacing(const enum_tex tex,GLubyte* rgb_data)
{
	xint c			=0;
	xflt buffer_lo	=0.0;
	xflt buffer_hi	=1.0;
	xint jdim		=(tex==t_font_4)?1:4;

	for(xint j=jdim-1;j>=0;j--)	// the 4 rows of text
	{
		xint x			=-1;
		xint looking_lo	= 1;
		xint looking_hi	= 0;
		while(x++ < tman.tex_dx_vis[tex])
		{
			const xint dx=	  tman.tex_dx_tot[tex]				;
			const xint y2=(j+1)*tman.tex_dy_vis[tex]/(xflt)jdim	;
			const xint y1=(j  )*tman.tex_dy_vis[tex]/(xflt)jdim	;

			xint found_ink=0;
			for(xint y=y1;y<y2;y++)
			if(rgb_data[(y*dx+x)*4+0]>16)
			if(rgb_data[(y*dx+x)*4+1]>16)
			if(rgb_data[(y*dx+x)*4+2]>16)
			if(rgb_data[(y*dx+x)*4+3]>16)
				found_ink=1;

			if(looking_lo &&  found_ink){
				tman.font_s1[tex][c]=(((xflt)x-buffer_lo)								)/tman.tex_dx_tot[tex];
				tman.font_t1[tex][c]=(((xflt)(j  )/(xflt)jdim)*tman.tex_dy_vis[tex]		)/tman.tex_dy_tot[tex];	looking_lo=0;	looking_hi=1;		}

			if(looking_hi && !found_ink){
				tman.font_s2[tex][c]=(((xflt)x+buffer_hi)								)/tman.tex_dx_tot[tex];
				tman.font_t2[tex][c]=(((xflt)(j+1)/(xflt)jdim)*tman.tex_dy_vis[tex]-1	)/tman.tex_dy_tot[tex];	looking_lo=1;	looking_hi=0;	c++;}

			if(c>=TEXT_DIM)break;
		}
	}

	tman.font_s1[tex][37]-=1.0/tman.tex_dx_tot[tex];	// the 'l' needs more space
	tman.font_s2[tex][37]+=1.0/tman.tex_dx_tot[tex];	// the 'l' needs more space
}

static xint vram_for_texture_any(const GLenum tex_format,xint w,xint h,const xint with_mipmap)
{
	xint max=0;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE,&max);	// note max tex size

	if(w>max)APP_alert("Texture size exceeds OS limit! REMEMBER WE CAN LOAD ON MAIN THREAD ONLY!","w/max=",int_to_str(w),int_to_str(max));	// IF MAX==0 WE ARE ON A THREAD: CANNOT LOAD!
	if(h>max)APP_alert("Texture size exceeds OS limit! REMEMBER WE CAN LOAD ON MAIN THREAD ONLY!","h/max=",int_to_str(h),int_to_str(max));	// IF MAX==0 WE ARE ON A THREAD: CANNOT LOAD!

	xint total=0;

	do
	{
		switch(tex_format)
		{
			case GL_RGB:								total+=3*w*h;				break	;
			case GL_RGBA:								total+=4*w*h;				break	;
			case GL_ALPHA:								total+=1*w*h;				break	;
			case GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG:	total+=intmax2(w*h/2,32);	break	;
			case GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG:	total+=intmax2(w*h/4,32);	break	;
			case GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG:	total+=intmax2(w*h/2,32);	break	;
			case GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG:	total+=intmax2(w*h/4,32);	break	;
			default:																return 0;	// WTF case
		}
		if(w == 1 && h == 1	)break;
		if(!with_mipmap		)break;

		if (w > 1) w /= 2;
		if (h > 1) h /= 2;
	}
	while(1);

	return total;
}

//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
void tman_class::load_scratch(	const enum_tex t		,
								const string path		,
								const GLenum tex_type	,
								const xint dx_vis		,
								const xint dy_vis		,
									  xint dx_tot		,
									  xint dy_tot		,
								GLubyte*   rgb_data		,
								const xint is_pvr		)
{
	if(!intrange(tex_type	,GL_RGB	,GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG))APP_alert("Bad minify arg.","","","");
	if(!intrange(dx_vis		,1		,TEX_DIM							))APP_alert("Trying to create a texture either less than 1 or greater than 1024!",path,"","");
	if(!intrange(dy_vis		,1		,TEX_DIM							))APP_alert("Trying to create a texture either less than 1 or greater than 1024!",path,"","");
	if(!intrange(dx_tot		,1		,TEX_DIM							))APP_alert("Trying to create a texture either less than 1 or greater than 1024!",path,"","");
	if(!intrange(dy_tot		,1		,TEX_DIM							))APP_alert("Trying to create a texture either less than 1 or greater than 1024!",path,"","");

//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
// SET THE PARAMETERS
//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
	xint reduc	=0;	// ALL 2 AND 4 BIT PVRS HAVE MINIFICATION BUILT IN. SO THIS IS AVAIL FOR ALL PVRS. PROBLEM IS: IT LOOKS LIKE DONKEY-ASS. SO, LEAVE IT OFF!
	xint mini	=0;	// WE STILL GET A 15% RAM SAVINGS AND 10% FR BOOST FROM SWITCHING TO PVR!

	tex_dx_vis[t]=dx_vis;
	tex_dy_vis[t]=dy_vis;
	tex_dx_tot[t]=dx_tot;
	tex_dy_tot[t]=dy_tot;

	glBindTexture	(GL_TEXTURE_2D,t);
	glTexParameteri	(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S	,GL_REPEAT	);	// wrapping is faster.
	glTexParameteri	(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T	,GL_REPEAT	);	// wrapping is faster.
	glTexParameteri	(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR	);
	glTexParameteri	(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR	);
	glTexParameterf	(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,1.0);	// THIS IS FASTEST, AND WE ONLY NEED ANISO IF MIP-MAPPED, SINCE MIP-MAPPING IS THE ONLY THING THAT TAKES US DOWN TO A LOW-RES TEX THAT NEEDS ANISO!

	if(mini)
	{
		// THIS IS WHAT MAKES MIPMAPS SUCK LESS!
		// THIS LETS THEM STILL HAVE DECENTLY HI RES IN THE DIRECTION YOU WANT IT,
		// AND MAY ALLOW THE APP TO BE FASTER SINCE THERE WILL BE LESS MEMORY THROUGHPUT ON THE CARD
		// SETTING TO MAX ISO MIGHT MAKE PVR-MINIFACTION LOOK A TINY BIT BETTER, AND HAS NO MEASURABLE REDUCTION IN FRAME-RATE THAT I CAN SEE,
		// AND OF COURSE HAS NO IMPACT ON RAM
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,1.0				);	// aniso makes mini suck a little less, with no change in ram, and no change in fr that i can measure in the ipad mini
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR	);	// THEORETICAL BEST MIPMAP QUALITY COMES FROM: GL_LINEAR_MIPMAP_LINEAR
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,              GL_LINEAR	);	// MAYBE YOU GET FASTER RESULTS WITH: GL_LINEAR_MIPMAP_NEAREST
	}

//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
// USE RAW DATA IF DESRIED
//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
	if(!is_pvr)	// two loads on these tex: once as PNG to load up the syn-vis colors from the huge array,
	{			// once as PVR to save RAM! (and we save a LOT!)
		if(t==t_earth_full_4k1)
		for(xint i=0;i<dx_vis;i++)
		for(xint j=0;j<dy_vis;j++){
			planet_array[i			][j][0]=rgb_data[(j*dx_tot+i)*4+0];
			planet_array[i			][j][1]=rgb_data[(j*dx_tot+i)*4+1];
			planet_array[i			][j][2]=rgb_data[(j*dx_tot+i)*4+2];}

		if(t==t_earth_full_4k2)
		for(xint i=0;i<dx_vis;i++)
		for(xint j=0;j<dy_vis;j++){
			planet_array[dx_tot+i	][j][0]=rgb_data[(j*dx_tot+i)*4+0];
			planet_array[dx_tot+i	][j][1]=rgb_data[(j*dx_tot+i)*4+1];
			planet_array[dx_tot+i	][j][2]=rgb_data[(j*dx_tot+i)*4+2];}
	}

//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
// LOAD THE TEXTURE
//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
	for(;;)
	{
		dev_alert_OGL("glTexImage2D-1");

		if(is_pvr)
		{
			const xint vram=vram_for_texture_any(tex_type,dx_tot,dy_tot,xfals);	// note that we do not call with ram for minifcation, since we load the outer level only, since that is all we need!

			glCompressedTexImage2D(	GL_TEXTURE_2D,		// target
									0,					// reduction... 0 for non-mip-mapped, and first level of mipmap, and we simply do not load the mip-mapped portion of the files here because they do not seem to look any better at all
									tex_type,			// gives # color components
									dx_tot,				// width and height... this needs power of 2 size only!!!!!
									dy_tot,				// width and height... this needs power of 2 size only!!!!!
									0,					// width of border... 0 or 1 only
									vram,
									rgb_data);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D,					// target
						 reduc,							// reduction... 0 for non-mip-mapped, and first level of mipmap
						 tex_type,
						 dx_tot,						// width and height... this needs power of 2 size only!!!!!
						 dy_tot,						// width and height... this needs power of 2 size only!!!!!
						 0,								// width of border... 0 or 1 only
						 tex_type,						// GL_RGB, GL_RGBA
						 GL_UNSIGNED_BYTE,				// format type
						 rgb_data);						// actual texture buffer
		}

		dev_alert_OGL("glTexImage2D-2");

		if(!mini	)break;		reduc++		;
		if(dx_tot<=1)break;		dx_tot/=2	;
		if(dy_tot<=1)break;		dy_tot/=2	;

		xint step=(tex_type==GL_RGB)?3:4;

		for(xint j=0;j<dy_tot;j++)	// go in right order so we do not step on our data while we mod it
		for(xint i=0;i<dx_tot;i++)	// go in right order so we do not step on our data while we mod it
		{
			const xint nim1=intlim(i*2-1,0,dx_tot*2-1);
			const xint nip1=intlim(i*2+1,0,dx_tot*2-1);
			const xint njm1=intlim(j*2-1,0,dy_tot*2-1);
			const xint njp1=intlim(j*2+1,0,dy_tot*2-1);

			const xint addy_lo =( j  * dx_tot   + i  )*step;
			const xint addy_hi1=(njm1*(dx_tot*2)+nim1)*step;
			const xint addy_hi2=(njm1*(dx_tot*2)+nip1)*step;
			const xint addy_hi3=(njp1*(dx_tot*2)+nim1)*step;
			const xint addy_hi4=(njp1*(dx_tot*2)+nip1)*step;

			for(xint c=0;c<step;c++)
			rgb_data[addy_lo+c]=(	rgb_data[addy_hi1+c]+
									rgb_data[addy_hi2+c]+
									rgb_data[addy_hi3+c]+
									rgb_data[addy_hi4+c])/4;
		}
	}
}

//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
#include <CoreGraphics/CGImage.h>
#include <CoreGraphics/CGContext.h>
#include <CoreGraphics/CGBitmapContext.h>

void tman_class::load_pngX(const enum_tex t,const string path)
{
	if(!check_ext(path,".png"))return;

	xmap_class in_file("RESOURCE/"+path);
	if(!in_file.exists	()	)APP_alert("Could not load png!",path,"","");
	if( in_file.size	()<8)APP_alert("Could not load png!",path,"","");	// NO image file is valid at less than 8 bytes, all have some header.  Avoid seg faults.

	CGDataProviderRef		provider=CGDataProviderCreateWithData(NULL,in_file.begin(),in_file.size(),NULL)			;
	CGImageRef				image=CGImageCreateWithPNGDataProvider(provider,NULL,FALSE,kCGRenderingIntentDefault)	;
	CGDataProviderRelease(	provider)																				;

	void*			data	=nil;
	CGContextRef	context	=nil;

	CGImageAlphaInfo info=CGImageGetAlphaInfo(image);
	xint hasAlpha=(	(info==kCGImageAlphaPremultipliedLast	) ||
					(info==kCGImageAlphaPremultipliedFirst	) ||
					(info==kCGImageAlphaLast				) ||
					(info==kCGImageAlphaFirst				) );

	CGSize	imageSize	=CGSizeMake(CGImageGetWidth(image), CGImageGetHeight(image));
	xint	dx_vis	=imageSize.width	;
	xint	dy_vis	=imageSize.height	;

	if(info==kCGImageAlphaOnly)
	{
		data	=malloc(dy_vis * dx_vis);
		context	=CGBitmapContextCreate(data, dx_vis, dy_vis, 8, dx_vis, NULL, kCGImageAlphaOnly | kCGBitmapByteOrder32Big);
	}
	else if(hasAlpha)
	{
		CGColorSpaceRef	colorSpace = CGColorSpaceCreateDeviceRGB();
		data	=malloc(dy_vis * dx_vis * 4);
		context	=CGBitmapContextCreate(data, dx_vis, dy_vis, 8, 4 * dx_vis, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
		CGColorSpaceRelease(colorSpace);
	}
	else
	{
		CGColorSpaceRef	colorSpace = CGColorSpaceCreateDeviceRGB();
		data	=malloc(dy_vis * dx_vis * 4);
		context	=CGBitmapContextCreate(data, dx_vis, dy_vis, 8, 4 * dx_vis, colorSpace, kCGImageAlphaNoneSkipLast | kCGBitmapByteOrder32Big);
		CGColorSpaceRelease(colorSpace);
	}

	CGContextClearRect(context,CGRectMake(0,0,dx_vis,dy_vis));
	CGContextTranslateCTM(context,0,dy_vis);
	CGContextScaleCTM(context,1,-1);
	CGContextDrawImage(context,CGRectMake(0,0,CGImageGetWidth(image),CGImageGetHeight(image)),image);
	CGContextRelease(context);

	const xint		dy_tot	=get_pow2(dy_vis);
	const xint		dx_tot	=get_pow2(dx_vis );
	const GLenum	tex_format	=GL_RGBA;
	const xint		tex_bp		=4;
	const xint		src_bp		=4;	// this must be 4, even for non-alpha pngs, for some reason that i do not get at all!

	if(!intrange(dy_tot	,1,TEX_DIM))APP_alert("Trying to create a texture either less than 1 or greater than 1024!",path,"","");
	if(!intrange(dx_tot	,1,TEX_DIM))APP_alert("Trying to create a texture either less than 1 or greater than 1024!",path,"","");

	GLubyte* png_buffer=(xbyt*)malloc(dx_tot*dy_tot*tex_bp);	// this is freed in the calling proc! we load this with data in this proc and return it full of data!

	for(xint y=0;y<dy_vis;y++)
	{
		GLubyte* srcb=(GLubyte*)data	+dx_vis*y*src_bp;
		GLubyte* dstb=png_buffer		+dx_tot*y*tex_bp;
		memcpy(dstb,srcb,dx_vis*src_bp);
	}

	tman.load_scratch(t,path,tex_format,dx_vis,dy_vis,dx_tot,dy_tot,png_buffer,xfals);
	CGImageRelease(image);
	free(data);

	if(t==t_font_1		)find_spacing(t,png_buffer);
	if(t==t_font_2		)find_spacing(t,png_buffer);
	if(t==t_font_3		)find_spacing(t,png_buffer);
	if(t==t_font_4		)find_spacing(t,png_buffer);
	if(t==t_font_1_shad	)find_spacing(t,png_buffer);
	if(t==t_font_3_shad	)find_spacing(t,png_buffer);

	free(png_buffer);
	shar.shar_per_init("load_pngX",path);
}

//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
// LOAD PVR
//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
enum
{
   OGL_RGBA_4444=0x10,
   OGL_RGBA_5551,
   OGL_RGBA_8888,
   OGL_RGB_565	,
   OGL_RGB_555	,
   OGL_RGB_888	,
   OGL_I_8		,
   OGL_AI_88	,
   OGL_PVRTC2	,
   OGL_PVRTC4	,
   OGL_PVRTC2_2	,
   OGL_PVRTC2_4
};

//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
// LOAD THE PVR DATA FROM THE FILE
// ALL OF THE 2-BIT AND 4-BIT PVR'S ALWAYS HAVE MINIFICTION... THERE IS NO WAY AROUND IT!
//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
void tman_class::load_pvrX(const enum_tex t,const string path)
{
	xmap_class m("RESOURCE/"+path);

	if(!check_ext	(path,".pvr")	){APP_alert("Calling load_pvrX with a non-pvr file type!"	,path,"","");	return;}
	if(!m.exists	(			)	){APP_alert("load_pvrX failed because file not found!"		,path,"","");	return;}
	if( m.size		(			)<8	){APP_alert("load_pvrX failed because file too small!"		,path,"","");	return;}

	unsigned int* dat=(unsigned int*)m.begin();

	GLubyte* 	pvr_buffer		=m.begin()+	dat[0]			;
	xint		height			=			dat[1]			;
	xint		width			=			dat[2]			;
//	xint		pvr_EXP_minify	=			dat[3] > 1		;
	xint		mode			=		   (dat[4] & 0xFF)	;
//	xint		pvr_act_ram		=			dat[5]			;
	GLenum		tex_format		=			0				;

	switch(mode)
	{
		case OGL_PVRTC4		:	tex_format=GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;	break	;
		case OGL_PVRTC2		:	tex_format=GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;	break	;
		case OGL_RGBA_4444	:	tex_format=GL_RGBA;								break	;
		case OGL_RGBA_5551	:	tex_format=GL_RGBA;								break	;
		case OGL_RGBA_8888	:	tex_format=GL_RGBA;								break	;
		case OGL_RGB_565	:	tex_format=GL_RGB;								break	;
		case OGL_RGB_555	:													return	;	// WE SHOULD NEVER USE THIS! WASTING A BIT IS STOOOOPID.
		case OGL_RGB_888	:	tex_format=GL_RGB;								break	;
		case OGL_I_8		:	tex_format=GL_ALPHA;							break	;
		case OGL_AI_88		:													return	;	// sim does not support 2-channel formats!
		default:																return	;
	}

//	if(	pvr_EXP_minify!=mini_me(t))
//		APP_alert(path,"PVR minification not as desired by TMAN!","","");
//
//	xint pvr_EXP_ram =vram_for_texture_any(tex_format,width,height,pvr_EXP_minify);			// get the ram for all minification levels in the file! this is all minification levels added up to the total ram!
//	if	(pvr_EXP_ram!=pvr_act_ram)
//		APP_alert(path,"PVR RAM not as expected!","","");

//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
// LOAD IT INTO OPENGL
//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
	load_scratch(t,path,tex_format,width,height,width,height,pvr_buffer,xtrue);
	shar.shar_per_init("load_pvrX",path);
}
