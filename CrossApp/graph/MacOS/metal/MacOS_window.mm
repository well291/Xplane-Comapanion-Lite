#include "hl_types.h"

#import <AppKit/AppKit.h>
#include <Carbon/Carbon.h>	// we use this for vkeys.
#include <OpenGL/glext.h>

static int fsaa_req=0;
static int fsaa_act=0;

void process_key_for_delegate(NSEvent* evt, WIN_ogl_window_delegate* delegate)
{
	TISInputSourceRef		currentKeyboard =				TISCopyCurrentKeyboardLayoutInputSource();
	CFDataRef				uchr_data		=(CFDataRef)	TISGetInputSourceProperty(currentKeyboard, kTISPropertyUnicodeKeyLayoutData);
	const UCKeyboardLayout* uchr			=(uchr_data)?	(const UCKeyboardLayout*)CFDataGetBytePtr(uchr_data):NULL;

	CFRelease(currentKeyboard);
 
	if(uchr)
	{
		NSString* keystr	=[evt characters];
		unichar firstChar	=[keystr characterAtIndex:0];

		if([evt type]==NSKeyDown)APP_event(firstChar);
	}
}

static NSOpenGLPixelFormat* pixel_format=NULL;

@interface WIN_Window : NSWindow {
};
@end

@implementation WIN_Window
-(BOOL) canBecomeKeyWindow {return YES;}
-(BOOL) canBecomeMainWindow{return YES;}
@end

@interface	WIN_OpenGLView : NSOpenGLView <NSWindowDelegate>
{
@private
	WIN_ogl_window*		window;
	NSOpenGLContext*	prev_ctx;
}

- (id) initWithFrame:(NSRect)	frameRect pixelFormat:(NSOpenGLPixelFormat*)format;
- (void)						setWindow			 :(WIN_ogl_window*)window;

@end

struct WIN_ogl_window
{
	WIN_ogl_window_delegate*	delegate;
	WIN_ogl_window_style		style;
	WIN_Window*					window;
	WIN_OpenGLView*				view;
};

@implementation WIN_OpenGLView 

- (void) lockFocus
{
	prev_ctx = [NSOpenGLContext currentContext];
	[super lockFocus];
}

- (void) unlockFocus
{
	[super unlockFocus];
	if(prev_ctx)
	{
		[prev_ctx makeCurrentContext];
	}
	else
	{
		[NSOpenGLContext clearCurrentContext];
	}
	prev_ctx = nil;
}

- (id) initWithFrame:(NSRect)frameRect pixelFormat:(NSOpenGLPixelFormat*)format
{
	self = [super initWithFrame:frameRect pixelFormat:format];
	window = NULL;
	return self;
}

- (void)windowDidResize:(NSNotification*)notification
{
	static int depth=0;
	if(window->delegate && depth==0)
	{
		++depth;
		NSRect nf = [window->view frame];

		mnw.XRES_priv=nf.size.width ;
		mnw.YRES_priv=nf.size.height;
		OGL_viewport(2);

		--depth;
	}
}

- (void) setWindow:(WIN_ogl_window*)w
{
	window=w;
}

- (void)mouseMoved:(NSEvent*)theEvent
{
	NSPoint where = [theEvent locationInWindow];
	where.y = [window->view frame].size.height - where.y;
	if(window->delegate)
		window->delegate->mouse_move(window->delegate->self,where.x,where.y);
}

- (void)mouseDown:(NSEvent*)theEvent
{
	NSPoint where = [theEvent locationInWindow];
	where.y = [window->view frame].size.height - where.y;
	if(window->delegate)
		window->delegate->mouse_down(window->delegate->self,where.x,where.y);
}

- (void)rightMouseDown:(NSEvent*)theEvent{}
- (void)otherMouseDown:(NSEvent*)theEvent{}

- (void)mouseUp:(NSEvent*)theEvent
{
	NSPoint where = [theEvent locationInWindow];
	where.y = [window->view frame].size.height - where.y;
	if(window->delegate)
		window->delegate->mouse_up(window->delegate->self,where.x,where.y);
}

- (void)rightMouseUp:(NSEvent*)theEvent{}

- (void)otherMouseUp:(NSEvent*)theEvent{}

- (void)scrollWheel:(NSEvent*)theEvent
{
	NSPoint where = [theEvent locationInWindow];
	where.y = [window->view frame].size.height - where.y;
	if(window->delegate)
		window->delegate->mouse_wheel(window->delegate->self,where.x, where.y, 0,[theEvent deltaY]);
}

- (void)mouseDragged:(NSEvent*)theEvent
{
	NSPoint where = [theEvent locationInWindow];
	where.y = [window->view frame].size.height - where.y;
	if(window->delegate)
		window->delegate->mouse_move(window->delegate->self,where.x,where.y);
}

- (void)rightMouseDragged:(NSEvent*)theEvent
{
	NSPoint where = [theEvent locationInWindow];
	where.y = [window->view frame].size.height - where.y;
	if(window->delegate)
		window->delegate->mouse_move(window->delegate->self,where.x,where.y);
}

- (void)otherMouseDragged:(NSEvent*)theEvent
{
	NSPoint where = [theEvent locationInWindow];
	where.y = [window->view frame].size.height - where.y;
	if(window->delegate)
		window->delegate->mouse_move(window->delegate->self,where.x,where.y);
}

- (void)keyDown:(NSEvent*)theEvent
{
	if(window->delegate)
		process_key_for_delegate(theEvent, window->delegate);
}

- (void)keyUp:(NSEvent*)theEvent
{
	if(window->delegate)
		process_key_for_delegate(theEvent, window->delegate);
}
@end

extern "C" void instrumentObjcMessageSends(BOOL);

static void WIN_make_pixel_format(void)
{
	if(pixel_format) return;

    if([[NSApplication sharedApplication] respondsToSelector:@selector(setPresentationOptions:)])
		[[NSApplication sharedApplication] setPresentationOptions:((1<<1)|(1<<3))];

	NSOpenGLPixelFormatAttribute attribs[] = {
		NSOpenGLPFADoubleBuffer,
		NSOpenGLPFADepthSize,		32,
		NSOpenGLPFAStencilSize,		8,
		NSOpenGLPFANoRecovery,
		NSOpenGLPFAAllRenderers,
		NSOpenGLPFASampleBuffers,	static_cast<NSOpenGLPixelFormatAttribute>(fsaa_req > 0 ? 1 : 0),
		static_cast<NSOpenGLPixelFormatAttribute>(fsaa_req > 0 ? NSOpenGLPFASamples : 0),
		static_cast<NSOpenGLPixelFormatAttribute>(1 << (fsaa_req)),
		0 };

	pixel_format = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];

	if(pixel_format)
	{
		GLint samp_count = 0;
		[pixel_format getValues:&samp_count forAttribute:NSOpenGLPFASamples forVirtualScreen:0];
		if(samp_count > 0)
			fsaa_act = log2(samp_count);
	}
}

void WIN_initialize(int	fsaa_level)
{
	fsaa_req=fsaa_level;
}

WIN_ogl_window*	WIN_ogl_window_create(	int						x,
										int						y,
										int						dx,
										int						dy,
										int						mindx,
										int						mindy,
										WIN_ogl_window_style	style,
										WIN_ogl_window*			share_ogl_junk)
{
	WIN_ogl_window* win	=new WIN_ogl_window;
	win->delegate		=NULL;
	win->style			=style;
		
	NSUInteger wmask = 0;
	switch(style)
	{
		case style_FullScreen	:wmask=NSBorderlessWindowMask;						break;
		case style_Drag			:wmask=NSTitledWindowMask;							break;
		case style_Drag_Resize	:wmask=NSTitledWindowMask | NSResizableWindowMask;	break;
	}

	if(pixel_format == NULL)
		WIN_make_pixel_format();

	float h = [[[NSScreen screens] objectAtIndex:0] frame].size.height;

	 win->window = [[WIN_Window alloc] initWithContentRect:NSMakeRect(x,h-y-dy,dx,dy) styleMask:wmask backing:NSBackingStoreBuffered defer:TRUE];
	[win->window setAcceptsMouseMovedEvents:YES];	
	[win->window makeKeyAndOrderFront:NSApp];
	 win->view   = [[WIN_OpenGLView alloc] initWithFrame:NSMakeRect(0,0,dx,dy) pixelFormat:pixel_format];

	if(share_ogl_junk)
	{
		NSOpenGLContext* nc = [[NSOpenGLContext alloc] initWithFormat:pixel_format shareContext:[share_ogl_junk->view openGLContext]];
		[nc setView:win->view];
		[win->view setOpenGLContext:nc];
	}

	[win->window setContentView:win->view];	
	[win->window setContentMinSize:NSMakeSize(mindx,mindy)];
	[[win->view openGLContext] makeCurrentContext];
	[win->view setWindow:win];
	[win->view release];
	[win->window setDelegate:win->view];
	[win->window makeFirstResponder:win->view];
	[win->window display];		// This is needed to fully set up the GL stack - only after we do a display will buffer swaps actually show anything!

	return win;
}

void  WIN_ogl_window_set_delegate		(WIN_ogl_window* win,WIN_ogl_window_delegate* delegate	){ win->delegate=delegate;}
void  WIN_ogl_window_destroy			(WIN_ogl_window* win									){[win->window close];	delete win;}
void  WIN_ogl_window_set_vsync			(WIN_ogl_window* win,int v								){GLint vsync=v?1:0;    [[win->view openGLContext] setValues:&vsync forParameter:NSOpenGLCPSwapInterval];}
int	  WIN_ogl_window_get_actual_fsaa	(void													){return fsaa_act;}
void  WIN_ogl_window_set_actual_fsaa	(int fsaa												){fsaa_act=fsaa;}						
void* WIN_ogl_window_get_native_handle	(WIN_ogl_window* win									){return NULL;}
void  WIN_ogl_window_swap				(WIN_ogl_window* win									){[[win->view openGLContext] makeCurrentContext];	glSwapAPPLE();}

OGL_context* WIN_ogl_context_create_with_window(WIN_ogl_window* win)
{
	NSOpenGLContext* ctx = [[NSOpenGLContext alloc] initWithFormat:pixel_format shareContext:[win->view openGLContext]];
	return (OGL_context*) ctx;
}
											
int WIN_ogl_context_attach_to_thread(OGL_context* ctx)
{
	NSOpenGLContext* c = (NSOpenGLContext*) ctx;
	[c makeCurrentContext];
	return 1;
}
