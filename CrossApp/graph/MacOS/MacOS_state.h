#ifndef OGL_state_h
#define OGL_state_h

enum ogl_fog_mode	{fog_true,fog_fals};
enum ogl_lit_mode	{lit_true,lit_fals};
enum ogl_al_mode	{al_none ,al_blend,al_test,al_both};
enum ogl_deR_mode	{deR_true,deR_fals};
enum ogl_deW_mode	{deW_true,deW_fals};

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// SETSTATE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline void OGL_setstate(const ogl_fog_mode fog,xint tex,const ogl_lit_mode lit,const ogl_al_mode al,const ogl_deR_mode deR,const ogl_deW_mode deW)
{
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// ALPHA
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	static xint o_alB=(glIsEnabled(GL_BLEND		))?xtrue:xfals;
	static xint o_alT=(glIsEnabled(GL_ALPHA_TEST))?xtrue:xfals;

	#if DEV
		dev_OGL_error("setstate");
		if(!intrange(al,al_none,al_both))APP_alert("Bad OGLsetstate argument!","","","");

		if(o_alB!=(glIsEnabled(GL_BLEND		)?xtrue:xfals))APP_alert("Out of sync OGL state!","Somewhere I changed GL_BLEND"		,(o_alB?"got turned OFF somehow!":"got turned ON somehow!"),"");
		if(o_alT!=(glIsEnabled(GL_ALPHA_TEST)?xtrue:xfals))APP_alert("Out of sync OGL state!","Somewhere I changed GL_ALPHA_TEST"	,(o_alT?"got turned OFF somehow!":"got turned ON somehow!"),"");
	#endif

		 if(al==al_none ){if( o_alB)glDisable(GL_BLEND		);
						  if( o_alT)glDisable(GL_ALPHA_TEST	);}
	else if(al==al_blend){if(!o_alB)glEnable (GL_BLEND		);
						  if( o_alT)glDisable(GL_ALPHA_TEST	);}
	else if(al==al_test ){if( o_alB)glDisable(GL_BLEND		);
						  if(!o_alT)glEnable (GL_ALPHA_TEST	);}
	else if(al==al_both ){if(!o_alB)glEnable (GL_BLEND		);
						  if(!o_alT)glEnable (GL_ALPHA_TEST	);}

	o_alB=(al==al_blend||al==al_both);
	o_alT=(al==al_test ||al==al_both);

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// DEPTH
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	static xint o_deR=(glIsEnabled(GL_DEPTH_TEST))?deR_true	:deR_fals;
	static xint o_deW=deW_fals;

	#if DEV
		dev_OGL_error("setstate");
		if(!intrange(deR,deR_true,deR_fals))APP_alert("Bad OGLsetstate argument!","","","");
		if(!intrange(deW,deW_true,deW_fals))APP_alert("Bad OGLsetstate argument!","","","");

		if(o_deR!=(glIsEnabled(GL_DEPTH_TEST)?deR_true:deR_fals))APP_alert("Out of sync OGL state!","Somewhere I changed GL_DEPTH_TEST",(o_deR?"got turned OFF somehow!":"got turned ON somehow!"),"");
	#endif

	if(o_deR!=deR){if(deR==deR_true)glEnable   (GL_DEPTH_TEST	); else glDisable  (GL_DEPTH_TEST	);}
	if(o_deW!=deW){if(deW==deW_true)glDepthMask(GL_TRUE			); else glDepthMask(GL_FALSE		);}

	o_deR=deR;
	o_deW=deW;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// OTHER
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	static xint o_fog=(glIsEnabled(GL_FOG		))?fog_true	:fog_fals;
	static xint o_tex=(glIsEnabled(GL_TEXTURE_2D))?num_units:0       ;
	static xint o_lit=(glIsEnabled(GL_LIGHTING	))?lit_true	:lit_fals;

	#if DEV
		dev_OGL_error("setstate");
		if(!intrange(fog,fog_true	,fog_fals	))APP_alert("Bad OGLsetstate argument!","","","");
		if(!intrange(tex,0			,8			))APP_alert("Bad OGLsetstate argument!","","","");
		if(!intrange(lit,lit_true	,lit_fals	))APP_alert("Bad OGLsetstate argument!","","","");

		if(o_fog!=(glIsEnabled(GL_FOG		)?fog_true	:fog_fals))APP_alert("Out of sync OGL state!","Somewhere I changed GL_FOG"			,(o_fog?"got turned OFF somehow!":"got turned ON somehow!"),"");
		if(o_tex!=(glIsEnabled(GL_TEXTURE_2D)?num_units	:0       ))APP_alert("Out of sync OGL state!","Somewhere I changed GL_TEXTURE_2D"	,(o_tex?"got turned OFF somehow!":"got turned ON somehow!"),"");
		if(o_lit!=(glIsEnabled(GL_LIGHTING	)?lit_true	:lit_fals))APP_alert("Out of sync OGL state!","Somewhere I changed GL_LIGHTING"		,(o_lit?"got turned OFF somehow!":"got turned ON somehow!"),"");
	#endif

	if(o_fog!=fog){if(fog==fog_true)glEnable(GL_FOG); else glDisable(GL_FOG);}
	if(o_tex!=tex)
	{
		if(tex>0)	glEnable (GL_TEXTURE_2D);
		else		glDisable(GL_TEXTURE_2D);

					tex=intlim(tex,0,8);	// glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB,&num_tex_units); if i start using a ton of texes
		num_units=	tex;

		if(tex>1 || o_tex>1)
		{
			if(o_tex< 1 && tex>=1){glActiveTextureARB(GL_TEXTURE0_ARB); glEnable (GL_TEXTURE_2D);}
			if(o_tex< 2 && tex>=2){glActiveTextureARB(GL_TEXTURE1_ARB); glEnable (GL_TEXTURE_2D);}
			if(o_tex< 3 && tex>=3){glActiveTextureARB(GL_TEXTURE2_ARB); glEnable (GL_TEXTURE_2D);}
			if(o_tex< 4 && tex>=4){glActiveTextureARB(GL_TEXTURE3_ARB); glEnable (GL_TEXTURE_2D);}
			if(o_tex< 5 && tex>=5){glActiveTextureARB(GL_TEXTURE4_ARB); glEnable (GL_TEXTURE_2D);}
			if(o_tex< 6 && tex>=6){glActiveTextureARB(GL_TEXTURE5_ARB); glEnable (GL_TEXTURE_2D);}
			if(o_tex< 7 && tex>=7){glActiveTextureARB(GL_TEXTURE6_ARB); glEnable (GL_TEXTURE_2D);}
			if(o_tex< 8 && tex>=8){glActiveTextureARB(GL_TEXTURE7_ARB); glEnable (GL_TEXTURE_2D);}

			if(o_tex>=1 && tex< 1){glActiveTextureARB(GL_TEXTURE0_ARB); glDisable(GL_TEXTURE_2D);}
			if(o_tex>=2 && tex< 2){glActiveTextureARB(GL_TEXTURE1_ARB); glDisable(GL_TEXTURE_2D);}
			if(o_tex>=3 && tex< 3){glActiveTextureARB(GL_TEXTURE2_ARB); glDisable(GL_TEXTURE_2D);}
			if(o_tex>=4 && tex< 4){glActiveTextureARB(GL_TEXTURE3_ARB); glDisable(GL_TEXTURE_2D);}
			if(o_tex>=5 && tex< 5){glActiveTextureARB(GL_TEXTURE4_ARB); glDisable(GL_TEXTURE_2D);}
			if(o_tex>=6 && tex< 6){glActiveTextureARB(GL_TEXTURE5_ARB); glDisable(GL_TEXTURE_2D);}
			if(o_tex>=7 && tex< 7){glActiveTextureARB(GL_TEXTURE6_ARB); glDisable(GL_TEXTURE_2D);}
			if(o_tex>=8 && tex< 8){glActiveTextureARB(GL_TEXTURE7_ARB); glDisable(GL_TEXTURE_2D);}

						   		  glActiveTextureARB(GL_TEXTURE0_ARB);	// return all future control to texture unit 0.
		}
	}
	if(o_lit!=lit)
	{
		if(lit==lit_true)	{glEnable (GL_LIGHTING);	glEnable (GL_LIGHT0);}
		else				{glDisable(GL_LIGHTING);	glDisable(GL_LIGHT0);}
	}

	o_fog=fog;
	o_tex=tex;
	o_lit=lit;
}

inline void OGL_setstate_2d_notex()
{
	OGL_setstate(fog_fals,0,lit_fals,al_blend,deR_fals,deW_fals);
}

#endif
