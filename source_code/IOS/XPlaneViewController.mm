#import "XPlaneViewController.h"

@implementation XPlaneViewController

- (UIView *) view
{
	return [self eaglView];
}

- (MyEAGLView *) eaglView
{
	if (!eaglView)
	{
	 	eaglView = [[MyEAGLView alloc] init];
		self.view = eaglView;
	}
	return eaglView;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••

int IOS_is_allowed_to_rotate=NO;	// we do not want the thing to rotate by itself when it detects a tilt! but we need to enable rotation when switching to HUD mode, wich is landscape!

- (BOOL)shouldAutorotate{return IOS_is_allowed_to_rotate;}	// the plist specifies what we allow here... then this turns it on and off


- (void)viewDidLoad
{
	[super viewDidLoad];
}

- (void)didReceiveMemoryWarning
{
	[super didReceiveMemoryWarning];
}

- (void)viewDidUnload
{
}

- (void)dealloc
{
	[eaglView release];
	[super dealloc];
}

- (void)viewWillAppear:(BOOL)animated
{
	[super viewWillAppear:animated];
}

- (void)viewDidDisappear:(BOOL)animated
{
	[super viewDidDisappear:animated];
}

- (BOOL)prefersStatusBarHidden	// this keeps the battery from showing up on the top of the screen all the time
{
    return YES;
}

@end
