#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreMotion/CoreMotion.h>
#import "XPlaneViewController.h"

@class MyEAGLView;

@interface XPlaneAppDelegate : NSObject <
								UIApplicationDelegate,
								UIAlertViewDelegate,
								UITextFieldDelegate,
								AVAudioSessionDelegate>
{
@public
	IBOutlet XPlaneViewController	*viewController;
@protected
	IBOutlet UIWindow				*device_window;
	MyEAGLView						*glView;
	CMMotionManager					*mMotionManager;
}

-(void)my_set_pixel_spec_from_obj_c;
-(void)make_worker_thread_ogl_from_obj_c;
@end

extern XPlaneAppDelegate *	gXPAppDelegate;

#if defined __cplusplus
extern "C" {
#endif

    const char* iPhoneDeviceName();

#if defined __cplusplus
};
#endif
