#ifndef cons_h
#define cons_h

class xplane_window_delegate;

struct monitor_res_struct{	// This is a vector of all possible modes that the monitor can run in, including string names.
	xint	width	;		// We always have "default" as an option, with all items set to zero.
	xint	height	;
	xint	bpp		;
	xint	hz		;
	string	name	;};
extern vector <monitor_res_struct> g_res_vec;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
class cons_class
{
public: 
	cons_class(){memset(this,0,sizeof(*this));}	// THIS is a POINTER. so the first arg is just "this" (the pointer) the third arg is the thing the pointer points to, thus the '*'. as well, we CAN memset vectors... just not STL strings!
   ~cons_class(){}

WIN_ogl_window*			window	;
xplane_window_delegate*	delegate;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void cons_nuke					();
xint cons_make					();
OGL_context* cons_make_loader	();
};
extern cons_class cons;

#endif
