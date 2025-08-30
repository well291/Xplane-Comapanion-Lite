#include "hl_types.h"
#import <AppKit/AppKit.h>

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// EVENT-HANDLER
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
xplane_window_delegate::xplane_window_delegate(WIN_ogl_window* wptr,xint which_win_num_am_i) : win(wptr),w(which_win_num_am_i)
{
	base.self			=this;
	base.mouse_wheel	=mouse_wheel;
	base.mouse_move		=mouse_move	;
	base.mouse_down		=mouse_down	;
	base.mouse_up		=mouse_up	;
}

void xplane_window_delegate::mouse_wheel(void* self,xint x,xint y,xint axis,xint delta)
{
	mnw.mouse_wheel_this_frame+=delta;
}

void xplane_window_delegate::mouse_move(void* self,xint x,xint y)
{
	mnw.mx=(				x);
	mnw.my=(mnw.YRES_now()-	y);
}

void xplane_window_delegate::mouse_down(void* self,xint x,xint y)
{
	mnw.mx=(		 		x);
	mnw.my=(mnw.YRES_now()-	y);
	mnw.mouse_clicked=xtrue;
}

void xplane_window_delegate::mouse_up(void* self,xint x,xint y)
{
	mnw.mx=(				x);
	mnw.my=(mnw.YRES_now()-	y);
	mnw.mouse_lifted=xtrue;			// see the dec on this for explanation
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// EVENT-FINDER
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void APP_get_events()
{
	mnw.key_target_char			=0		;	// we use a key just once! reset per-frame!
	mnw.mouse_clicked			=xfals	;	// avoid multi-clicks with one mouse-press... must be at beginning not end of proc for alert procs
	mnw.mouse_lifted 			=xfals	;	// avoid multi-lifts  with one mouse-press... must be at beginning not end of proc for alert procs
	mnw.mouse_wheel_this_frame	=0		;

	NSAutoreleasePool* pool=[[NSAutoreleasePool alloc] init];
	NSEvent* e=[[NSApplication sharedApplication] nextEventMatchingMask:NSAnyEventMask untilDate:[NSDate distantPast] inMode:NSDefaultRunLoopMode dequeue:TRUE];

	if(e)
	{
		[[NSApplication sharedApplication] sendEvent:e];
	}

	[pool drain];

}

