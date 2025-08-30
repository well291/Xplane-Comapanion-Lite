#import <AVFoundation/AVFoundation.h>
#import <mach/mach_time.h>
#import "XPlaneAppDelegate.h"
#import "XPlaneViewController.h"
#import "MyEAGLView.h"
#include <ifaddrs.h>
#include <arpa/inet.h>

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void write_app_review()
{
	NSString *appStoreReviewLink=@"itms-apps://itunes.apple.com/app/id1645141064";	// 1645141064 is the App ID from itunes connect... WE HAVE TO PUT THAT ID BEFORE THE NUMBER! HAR!
   [[UIApplication sharedApplication] openURL:[NSURL URLWithString:appStoreReviewLink]];
}

XPlaneAppDelegate *	gXPAppDelegate;

@implementation XPlaneAppDelegate

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
+ (void) initialize
{
	if(self==[XPlaneAppDelegate class]){}
}

- (void) dealloc
{
	[glView release];
	[device_window release];
	[super dealloc];
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//•••LEAVE/ENTER FOREGROUND/BACKGROUND••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// I CANNOT GET THE THING TO RUN PROPERLY IN THE BACKGROUND, AND IT KILLS BATTERY AND WOULD KILL CPU, AND APPLE DOES NOT LIKE US DOING STUFF IN THE BACKGROUND.
// SO, TO SAVE BATTERY AND STOP PEOPLE FROM TRYING TO RUN IT IN THE BACKGROUND AND JUST MISSING SOLUTIONS, I CURRENTLY JUST TERMINATE US IF WE ENTER THE BACKGROUND.
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
- (void)applicationWillResignActive			:(UIApplication*)application{[self stopDeviceMotion	];}	// this is called when the phone rings.
- (void)applicationDidBecomeActive			:(UIApplication*)application{[self startDeviceMotion];}
- (void)applicationDidReceiveMemoryWarning	:(UIApplication*)application{apps.got_ram_warn=xtrue;}

- (void)applicationWillTerminate			:(UIApplication*)application{							if(xios.prefs_save_armed)apps.apps_save();}
- (void)applicationDidEnterBackground		:(UIApplication*)application{[glView stopAnimation	];	if(xios.prefs_save_armed)apps.apps_save();}
- (void)applicationWillEnterForeground		:(UIApplication*)application{[glView startAnimation	];}	// This is called when we re-enter the foreground because we survived a fast app switch without being killed off for being memory pigs. All we have to do is RESTART OpenGL rendering.

- (void)beginInterruption	{soun.interrupt_start();	if(xios.prefs_save_armed)apps.apps_save();}
- (void)endInterruption		{soun.interrupt_stop ();	NSError *activationError=nil;	[[AVAudioSession sharedInstance] setActive: YES error: &activationError];}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// TIME TO INIT!
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
  void	make_worker_thread_ogl_from_cpp(void * self){[(id) self make_worker_thread_ogl_from_obj_c];}

-(void)make_worker_thread_ogl_from_obj_c
{
	EAGLSharegroup * grp = [glView getSharegroup];
    EAGLContext * async_ctx = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2 sharegroup:grp];
    [EAGLContext setCurrentContext:async_ctx];
}

#include <sys/types.h>
#include <sys/sysctl.h>

  void	my_set_pixel_spec_from_cpp(void* self){[(id)self my_set_pixel_spec_from_obj_c];}
-(void)my_set_pixel_spec_from_obj_c
{
	size_t		 size ;		sysctlbyname("hw.machine",NULL		,&size,NULL,0);
	char machine[size];		sysctlbyname("hw.machine",machine	,&size,NULL,0);
	NSString *hardwareString=[NSString stringWithCString:machine encoding:NSUTF8StringEncoding];

	const xflt pix_scale_max	=[[UIScreen mainScreen] scale]			;	// old=1, retina (starting with iPhone 4!)=2, retina HD=3
	const xflt dx_original_pix	=device_window.bounds.size.width 		;
	const xflt dy_original_pix	=device_window.bounds.size.height		;

	graf.is_phone	= ([hardwareString containsString:@"iPhone"]);	// all the phones have notches now
	graf.pix_scale	= pix_scale_max;
	graf.dev_dx_raw	= dx_original_pix*graf.pix_scale;				// and realize that we did it
	graf.dev_dy_raw	= dy_original_pix*graf.pix_scale;				// and realize that we did it

	[viewController.view setContentScaleFactor:graf.pix_scale];		// set the scale so we use all the pixels
	 device_window.rootViewController=[viewController retain];		// possible leak
	[device_window addSubview:viewController.view];
	[device_window makeKeyAndVisible];
	 glView=[viewController eaglView];
	[glView setFrame:device_window.bounds];							// as it turns out, we need this.
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
- (BOOL)application						:(UIApplication*)application
		didFinishLaunchingWithOptions	:(NSDictionary *)launchOptions
{
	gXPAppDelegate = self;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	NSString* ns_app=[NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory,NSUserDomainMask,YES)lastObject];

	NSError* error;
	if([[NSFileManager defaultManager] createDirectoryAtPath:ns_app withIntermediateDirectories:YES attributes:nil error:&error]){}

	NSString* ns_prfs=[ns_app stringByAppendingString:@"/prfs"];	file_path_prfs=*new std::string([ns_prfs UTF8String]);	file_path_prfs+="/";
	NSString* ns_sits=[ns_app stringByAppendingString:@"/sits"];	file_path_sits=*new std::string([ns_sits UTF8String]);	file_path_sits+="/";

	if([[NSFileManager defaultManager] createDirectoryAtPath:ns_prfs	withIntermediateDirectories:YES attributes:nil error:&error]){}	// we made the prefs directory ok
	if([[NSFileManager defaultManager] createDirectoryAtPath:ns_sits	withIntermediateDirectories:YES attributes:nil error:&error]){}	// we made the prefs directory ok

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	device_window=[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];	// init the window to the main screen size now

	 viewController=[[XPlaneViewController alloc] init];
	[viewController.view setBounds:device_window.bounds];

	[(id)self my_set_pixel_spec_from_obj_c];

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	[application setIdleTimerDisabled:(YES)];

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	soun.init_sound();

	NSError *activationError = nil;																			// This "inits" our audio session - necessary to do audio that isn't hosed by a phone call.
	[[AVAudioSession sharedInstance] setActive:YES error:&activationError];

	NSError *setCategoryError = nil;																		// And - change our category from solo ambient to ambient.  This will allow a user to keep listening to other audio like an mp3, in case a user wants to fly to techno or something.
	[[AVAudioSession sharedInstance] setCategory: AVAudioSessionCategoryAmbient error: &setCategoryError];	// AVAudioSession is the object, sharedInstance is the function, setCategory is the function we call on the returned value from sharedInstance!
																											// and then AVAudioSessionCategoryAmbient is an argument to that function, error: is the label for the argument going in, setCategoryError is the argument going in
	[[UIDevice currentDevice] setBatteryMonitoringEnabled:YES];												// and then AVAudioSessionCategoryAmbient is an argument to that function, error: is the label for the argument going in, setCategoryError is the argument going in

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// GET OUR IP ADDY
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	struct ifaddrs* interfaces=NULL;
	if(getifaddrs(&interfaces)==0)					// retrieve the current interfaces – returns 0 on success
	{
		struct ifaddrs* temp_addr=interfaces;		// loop through linked list of interfaces
		xint i=0;
		while(temp_addr!=NULL && i<this_ip_dim)
		{
			if(temp_addr->ifa_addr->sa_family==AF_INET)
			{
				unsigned int ip_uint=((struct sockaddr_in*)temp_addr->ifa_addr)->sin_addr.s_addr;

				if(ip_uint!=16777343)	// this is 127.0.0.1 that shows up all the time with no point at all
				{
					C_eq_C(inet.our_ip_addies[i],inet_ntoa(((struct sockaddr_in*)temp_addr->ifa_addr)->sin_addr),name_dim64);
					i++;
				}
			}
			temp_addr=temp_addr->ifa_next;
		}
	}
	freeifaddrs(interfaces);

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	apps.apps_init(self,0);
	[glView startAnimation];
	return 1;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
- (void)startDeviceMotion
{
		mMotionManager=[[CMMotionManager alloc]init];	// BEGIN MOTION MANAGEMENT

	if(mMotionManager.isDeviceMotionAvailable)
	{
        [mMotionManager startDeviceMotionUpdates];
	}
	if(mMotionManager.gyroAvailable && mMotionManager.accelerometerAvailable)
	{
		[mMotionManager startGyroUpdates];
		[mMotionManager startAccelerometerUpdates];
	}
}

- (void)stopDeviceMotion
{
	[mMotionManager stopGyroUpdates];
	[mMotionManager stopAccelerometerUpdates];
	[mMotionManager release];
	 mMotionManager = nil;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// HARDWARE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
xflt my_battery_level_from_cpp	(				){return [[UIDevice currentDevice]	batteryLevel]*100.0						;}
xflt my_get_brightness_from_cpp	(				){return [[UIScreen mainScreen]		brightness]								;}
void my_set_brightness_from_cpp	(xflt brite_rat	){		 [[UIScreen mainScreen]		setBrightness:fltlim(brite_rat,0.0,1.0)];}

char gUIDeviceName[1024]="";

char* iPhoneDeviceName_from_cpp()
{
    NSString *deviceUDID = [[UIDevice currentDevice] name];
    strcpy(gUIDeviceName, [deviceUDID UTF8String]);
    return gUIDeviceName;
}

char ghardwareString[1024]="";

char* hardwareString_from_cpp()
{
	size_t		 size ;		sysctlbyname("hw.machine",NULL		,&size,NULL,0);
	char machine[size];		sysctlbyname("hw.machine",machine	,&size,NULL,0);
	NSString *hardwareString=[NSString stringWithCString:machine encoding:NSUTF8StringEncoding];
    strcpy(ghardwareString, [hardwareString UTF8String]);
    return ghardwareString;
}
@end
