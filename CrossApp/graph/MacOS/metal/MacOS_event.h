#ifndef WIN_event_loop_h
#define WIN_event_loop_h

class xplane_window_delegate
{
public:
	WIN_ogl_window_delegate base;
	xplane_window_delegate(WIN_ogl_window* win,xint which_win_num_am_i);

	static void mouse_wheel	(void* self,xint x,xint y,xint axis,xint delta);
	static void mouse_move	(void* self,xint x,xint y);
	static void mouse_down	(void* self,xint x,xint y);
	static void mouse_up	(void* self,xint x,xint y);

private:
	WIN_ogl_window*	win	;
	xint			w	;
};

void APP_get_events();

#endif
