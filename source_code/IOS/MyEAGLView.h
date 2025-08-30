#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

// CLASSES:
@class MyEAGLView;

// PROTOCOLS:
@protocol MyEAGLViewDelegate <NSObject>
- (void) didResizeEAGLSurfaceForView:(MyEAGLView*)view; //Called whenever the EAGL surface has been resized
@end

// CLASS INTERFACE:
@interface MyEAGLView : UIView
{
@private

	// The pixel dimensions of the backbuffer
	GLint backingWidth;
	GLint backingHeight;

	EAGLContext *context;

	// OpenGL names for the renderbuffer and framebuffers used to render to this view
	GLuint viewRenderbuffer, viewFramebuffer;

	// OpenGL name for the depth buffer that is attached to viewFramebuffer, if it exists (0 if it does not exist)
	GLuint depthRenderbuffer;

	BOOL animating;
//	BOOL displayLinkSupported;
	NSInteger animationFrameInterval;

	// Use of the CADisplayLink class is the preferred method for controlling your animation timing.
	// CADisplayLink will link to the main display and fire every vsync when added to a given run-loop.
	// The NSTimer class is used only as fallback when running on a pre 3.1 device where CADisplayLink
	// isn't available.
	id displayLink;
//	NSTimer *animationTimer;
	id<MyEAGLViewDelegate>	_delegate;
}

//- (id)initWithCoder:(NSCoder*)coder;

@property (readonly, nonatomic, getter=isAnimating) BOOL animating;
@property (nonatomic) NSInteger animationFrameInterval;

-(void)startAnimation;
-(void)stopAnimation;
-(void)drawView;
- (EAGLSharegroup*) getSharegroup;

@property(assign) id<MyEAGLViewDelegate> delegate;

@end




