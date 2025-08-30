#include "hl_types.h"
#include <ApplicationServices/ApplicationServices.h>
#include <IOKit/graphics/IOGraphicsTypes.h>

cons_class cons;

vector <monitor_res_struct> g_res_vec;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// COUNTING AND QUERYING MONITORS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
static map<CGDirectDisplayID, CFDictionaryRef>	s_orig_modes;

static CGDirectDisplayID get_nth_display(xint idx)
{
	CGDisplayCount				n;
	vector<CGDirectDisplayID>	dpy;
	if (CGGetActiveDisplayList(0, NULL, &n) == noErr)
	{
		dpy.resize(n);
		if(CGGetActiveDisplayList(n,&*dpy.begin(),&n) != noErr)
			dpy.clear();
		if(dpy.size() != n) dpy.clear();
	}
	if(dpy.empty()) dpy.push_back(CGMainDisplayID());
	
	dev_assert(!dpy.empty());
	dev_assert(intrange(idx,0,(xint)dpy.size()-1));
	return dpy[idx];
}

static xint GetDictionaryNumber(CFDictionaryRef dict, CFStringRef key)
{
	SInt32 v;
	CFNumberRef	r = (CFNumberRef) CFDictionaryGetValue( dict,key);
	if(r==NULL) return -1;
	if(!CFNumberGetValue(r,kCFNumberSInt32Type,&v)) return -1;
	return v;
}

static size_t freaking_bits_per_pixel(CGDirectDisplayID id)
{
    size_t depth = 0;
    
    CGDisplayModeRef mode = CGDisplayCopyDisplayMode(id);
    CFStringRef pixEnc = CGDisplayModeCopyPixelEncoding(mode);
    if(CFStringCompare(pixEnc, CFSTR(IO32BitDirectPixels), kCFCompareCaseInsensitive) == kCFCompareEqualTo)
        depth = 32;
    else if(CFStringCompare(pixEnc, CFSTR(IO16BitDirectPixels), kCFCompareCaseInsensitive) == kCFCompareEqualTo)
        depth = 16;
    else if(CFStringCompare(pixEnc, CFSTR(IO8BitIndexedPixels), kCFCompareCaseInsensitive) == kCFCompareEqualTo)
        depth = 8;
    
    CFRelease(pixEnc);
    CFRelease(mode);
    
    return depth;
}

void UTL_get_monitor_res(	xint	monitor_index,				
							xint *	x_offset,					
							xint *	y_offset,					
							xint *	width,						
							xint *	height,
							xint *	bits_per_pixel,				
							xint *	freq)
{
	CGDirectDisplayID id = get_nth_display(monitor_index);
	CGRect bounds = CGDisplayBounds(id);
	
	if(x_offset) *x_offset = bounds.origin.x;
	if(y_offset) *y_offset = bounds.origin.y;
	if(width)	 *width = bounds.size.width;
	if(height)	 *height = bounds.size.height;
	if(bits_per_pixel) *bits_per_pixel = (xint)freaking_bits_per_pixel(id);
	if(freq)
	{
		CFDictionaryRef now = CGDisplayCurrentMode(id);
		*freq = GetDictionaryNumber(now, kCGDisplayRefreshRate);
	}
}

OGL_context* cons_class::cons_make_loader()
{
	return WIN_ogl_context_create_with_window(window);		
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void cons_class::cons_nuke()	// This destroys one window.
{
	WIN_ogl_window_destroy(window);
	delete delegate;
	window = 0;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
xint cons_class::cons_make()
{
	xint mon_x, mon_y,mon_col, mon_hz;
	UTL_get_monitor_res(0, NULL, NULL,&mon_x,&mon_y,&mon_col,&mon_hz);

	window = WIN_ogl_window_create(
		0,0,
		mnw.XRES_now(),mnw.YRES_now(),
		XRES_default,YRES_default,
		style_Drag_Resize,  NULL);
	delegate = new xplane_window_delegate(window,0);
	WIN_ogl_window_set_delegate(window, &delegate->base);
	WIN_ogl_window_set_vsync(window,0);

	if(!window)return xfals;

	const GLfloat ogl_base_amb[4]={0.0,0.0,0.0,0.0};							// this is the BASIC LIGHT LEVEL THAT IS ADDED TO EVERYTHING...

	glLightModelfv		(GL_LIGHT_MODEL_AMBIENT,ogl_base_amb);	// ...we want to have NOTHING get in the way of LIGHT0, so we set it to ZERO.
	glPixelStorei		(GL_UNPACK_ALIGNMENT,1				);	// the default is to align the bytes in the bitmap to the nearest 4.
	glPixelStorei		(GL_PACK_ALIGNMENT  ,1				);	// this requires a row width of (3*Width+3)&~3 for access, which is annoying... we therefore align to nearest byte... this is faster anyway
	glAlphaFunc			(GL_GREATER,0						);	// when ALPHA-TESTING  is enabled, we display the GREATEST alpha
	glBlendFunc			(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);	// when ALPHA-BLENDING is enabled, we blend normally
	glDepthFunc			(GL_LEQUAL							);	// less than OR EQUAL plots on top
	glFogi				(GL_FOG_MODE,GL_LINEAR				);	// fog linear for speed
	glHint				(GL_FOG_HINT,GL_FASTEST				);	// fog fastest for speed (Ben says: nicest gives us per-pixel on Win, not Mac.  But we will use pixel shaders for new hardware - leave this fast and shitty since it will be for ANCIENT hardware.
	glFrontFace			(GL_CW								);	// Open-GL normally uses CCW as poly facing you, we want opposite
	glDisable			(GL_CULL_FACE						);	// THIS STUFF MUST BE SET FOR EACH WINDOW, so we put in ogl_reset
	glCullFace			(GL_BACK							);	// nuke backs of airplanes, ships, buildings, etc. for more speed
	glShadeModel		(GL_SMOOTH							);	// we want normals averaged across the polys for smoothness.
	glEnable			(GL_DITHER							);	// we need dithering in 16-bit color
	glDisable			(GL_LOGIC_OP						);
	glDisable			(GL_STENCIL_TEST					);
	glDisable			(GL_TEXTURE_1D						);
	glDisable			(GL_NORMALIZE						);	// do NOT normalize normal vecs to unit length... this saves time, but we must use unit length
	glDisable			(GL_SCISSOR_TEST					);	// faster with no scissors
	glEnable			(GL_COLOR_MATERIAL					);	// GL_COLOR_MATERIAL lets us use OGL_setcolor to set material PROPERTIES FOR LIGHTING TO BOUNCE OFF OF. needed for specularity.
	glColorMaterial		(GL_FRONT,GL_AMBIENT_AND_DIFFUSE	);	// use color material for ambient and diffuse lighting
	glDepthMask			(GL_FALSE							);	// this is our init case in the setstate proc
	glLightModeli		(GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR);
	glEnableClientState (GL_VERTEX_ARRAY					);					// Always leave vertex arrays on - when would we EVER want this off?!?!

	if(WIN_ogl_window_get_actual_fsaa())
		glEnable(GL_MULTISAMPLE_ARB);	// enable this if we got it!

	return xtrue;
}
