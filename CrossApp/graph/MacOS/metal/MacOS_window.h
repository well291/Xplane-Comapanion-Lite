#ifndef WIN_ogl_window_h
#define WIN_ogl_window_h

struct WIN_ogl_window_delegate
{
	void* self;
	void (* mouse_down)(void* self, int x, int y);
	void (* mouse_up)(void* self, int x, int y);
	void (* mouse_move)(void* self, int x, int y);
	void (* mouse_wheel)(void* self, int x, int y, int axis, int delta);
	void (* repaint)(void* self);
	void (* resized)(void* self, int width, int height);
};

class WIN_ogl_window;
class OGL_context	;

enum WIN_ogl_window_style
{
	style_Drag			,
	style_Drag_Resize	,
	style_FullScreen
};
					
WIN_ogl_window*	WIN_ogl_window_create(
									int						x,
									int						y,
									int						dx,
									int						dy,
									int						min_x,
									int						min_y,
									WIN_ogl_window_style	style,
									WIN_ogl_window*			share_ogl_junk);
void WIN_initialize(				int						fsaa_level);
void WIN_ogl_window_set_delegate(	WIN_ogl_window*			win,
									WIN_ogl_window_delegate*delegate);
void WIN_ogl_window_destroy		(	WIN_ogl_window*			win);
void WIN_ogl_window_set_vsync	(	WIN_ogl_window*			win,
									int						vsync);
void WIN_ogl_window_swap(			WIN_ogl_window*			win);

void*								WIN_ogl_window_get_native_handle	(WIN_ogl_window*	win);
OGL_context*						WIN_ogl_context_create_with_window	(WIN_ogl_window*	win);
int									WIN_ogl_context_attach_to_thread	(OGL_context*		ctx);

int	 WIN_ogl_window_get_actual_fsaa(void);
void WIN_ogl_window_set_actual_fsaa(int fsaa);

#endif
