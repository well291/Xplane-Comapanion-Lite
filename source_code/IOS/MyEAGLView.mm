#import <QuartzCore/QuartzCore.h>
#import "MyEAGLView.h"
#import <OpenGLES/EAGLDrawable.h>

@implementation MyEAGLView
@synthesize animating;
@dynamic animationFrameInterval;
@synthesize delegate=_delegate;

extern void OGL_init();

// Implement this to override the default layer class (which is [CALayer class]).
// We do this so that our view will be backed by a layer that is capable of OpenGL ES rendering.
+ (Class) layerClass{return [CAEAGLLayer class];}
-(void)setupView{OGL_init();}

// init if we are not coming from a nib file
- (id)init
{
	self = [super init];
	if (self)
	{

		CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;

		eaglLayer.opaque = YES;
		eaglLayer.drawableProperties =	[NSDictionary dictionaryWithObjectsAndKeys:
										[NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];

		context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

		if (!context || ![EAGLContext setCurrentContext:context]) {
			[self release];
			return nil;
		}

		animating = FALSE;
//		displayLinkSupported = FALSE;
		animationFrameInterval = 1;		// divisions of 60 fps. default to max fps
		displayLink = nil;
//		animationTimer = nil;

		[self setupView];
		[self setMultipleTouchEnabled:TRUE];
	}

	return self;
}

// If our view is resized, we'll be asked to layout subviews.
// This is the perfect opportunity to also update the framebuffer so that it is
// the same size as our display area.
- (BOOL)createFramebuffer
{
	// Generate IDs for a framebuffer object and a color renderbuffer
	glGenFramebuffersOES(1, &viewFramebuffer);
	glGenRenderbuffersOES(1, &viewRenderbuffer);

	glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
	// This call associates the storage for the current render buffer with the EAGLDrawable (our CAEAGLLayer)
	// allowing us to draw into a buffer that will later be rendered to screen wherever the layer is (which corresponds with our view).
	[context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(id<EAGLDrawable>)self.layer];
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, viewRenderbuffer);

	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);

	// For this sample, we also need a depth buffer, so we'll create and attach one via another renderbuffer.
	glGenRenderbuffersOES(1, &depthRenderbuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);
	glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, backingWidth, backingHeight);
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthRenderbuffer);

	if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES)
	{
		// err=glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES);
		APP_alert("failed to make complete framebuffer object","","","");
		return NO;
	}

	return YES;
}

- (void)destroyFramebuffer
{
	glDeleteFramebuffersOES(1, &viewFramebuffer);
	viewFramebuffer = 0;
	glDeleteRenderbuffersOES(1, &viewRenderbuffer);
	viewRenderbuffer = 0;

	if(depthRenderbuffer)
	{
		glDeleteRenderbuffersOES(1, &depthRenderbuffer);
		depthRenderbuffer = 0;
	}
}

-(void)layoutSubviews
{
	[EAGLContext setCurrentContext:context];
	[self destroyFramebuffer];
	[self createFramebuffer];
	[self drawView];
}

- (EAGLSharegroup*) getSharegroup
{
    return [context sharegroup];
}

- (void)dealloc
{
	if(	[EAGLContext currentContext] == context)
		[EAGLContext setCurrentContext:nil];

	[context release];
	[super dealloc];
}


// Clean up any buffers we have allocated.
- (NSInteger) animationFrameInterval
{
	return animationFrameInterval;
}

- (void)stopAnimation
{
	if (animating)
	{
		[displayLink invalidate];
		 displayLink = nil;
		animating = FALSE;
	}
}

- (void)startAnimation
{
	if(!animating)
	{
		 displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(drawView)];
		[displayLink setFrameInterval:animationFrameInterval];
		[displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
		animating = TRUE;
	}
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// CALL MY RENDER PROC
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
- (void)drawView
{
	shar.shar_per_frame();
	shar.shar_per_check("drawView start");

	[EAGLContext setCurrentContext:context];						// make sure that you are drawing to the current context
	glBindFramebufferOES(GL_FRAMEBUFFER_OES,viewFramebuffer);		// make sure that you are drawing to the current context

	shar.shar_per_check("app start");
	apps.apps_frame();
	shar.shar_per_check("app stop");

	glBindRenderbufferOES(GL_RENDERBUFFER_OES,viewRenderbuffer);	// swap buffers. this will happen at the vsync (60 htz / frame interval)
 	[context presentRenderbuffer:GL_RENDERBUFFER_OES];				// swap buffers. this will happen at the vsync (60 htz / frame interval)

	shar.shar_per_check("drawView stop");
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void device_xy_to_app_xy(CGFloat* x,CGFloat* y)
{
	xflt xin=*x;
	xflt yin=*y;

	xin=				xin*graf.pix_scale;
	yin=graf.dev_dy_raw-yin*graf.pix_scale;

	if(graf.dev_orient_now()==orient_portrait	){*x=					xin;	*y=						yin;}
	if(graf.dev_orient_now()==orient_invert		){*x=graf.dev_dx_use()-	xin;	*y=graf.dev_dy_use()-	yin;}
	if(graf.dev_orient_now()==orient_land_rgt	){*x=					yin;	*y=graf.dev_dy_use()-	xin;}
	if(graf.dev_orient_now()==orient_land_lft	){*x=graf.dev_dx_use()-	yin;	*y=						xin;}
}

xint dragging_none  (){return(!apps.mou_dragging(0) && !apps.mou_dragging(1));}
xint dragging_0_only(){return( apps.mou_dragging(0) && !apps.mou_dragging(1));}
xint dragging_1_only(){return(!apps.mou_dragging(0) &&  apps.mou_dragging(1));}
xint dragging_both  (){return( apps.mou_dragging(0) &&  apps.mou_dragging(1));}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
	id value;
	NSEnumerator* enumerator=[touches objectEnumerator];
	while((value=[enumerator nextObject]))
	{
		UITouch* touch=value;
		CGPoint p=[touch locationInView:self];

		device_xy_to_app_xy(&p.x,&p.y);

		xint				 i=-1;
		if(dragging_none  ())i= 0;
		if(dragging_0_only())i= 1;
		if(dragging_1_only())i= 0;
		if(dragging_both  ())i=-1;

		if(i>=0)
		{
			apps.mou_drag_x[i]=p.x;
			apps.mou_drag_y[i]=p.y;
			apps.apps_event(click_type_click,i);
		}
	}
}

- (void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{
	id value;
	NSEnumerator* enumerator=[touches objectEnumerator];
	while((value=[enumerator nextObject]))
	{
		UITouch* touch=value;
		CGPoint p=[touch locationInView:self];
		device_xy_to_app_xy(&p.x,&p.y);

		const xint i=(apps.mou_dragging(1) && (	pythag(p.x-apps.mou_drag_x[1] , p.y-apps.mou_drag_y[1])<
												pythag(p.x-apps.mou_drag_x[0] , p.y-apps.mou_drag_y[0])))?1:0;
		apps.mou_drag_x[i]=p.x;
		apps.mou_drag_y[i]=p.y;
	}
}

- (void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event
{
	id value;
	NSEnumerator* enumerator=[touches objectEnumerator];
	while((value=[enumerator nextObject]))
	{
		UITouch* touch=value;
		CGPoint p=[touch locationInView:self];
		device_xy_to_app_xy(&p.x,&p.y);

		const xint i=(apps.mou_dragging(1) && (	pythag(p.x-apps.mou_drag_x[1] , p.y-apps.mou_drag_y[1])<
												pythag(p.x-apps.mou_drag_x[0] , p.y-apps.mou_drag_y[0])))?1:0;

		apps.apps_event(click_type_release,i);
	}
}

- (void)touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event	// phone_call
{
	apps.apps_event(click_type_release,0);
	apps.apps_event(click_type_release,1);
}
@end
