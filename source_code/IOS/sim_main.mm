#import <UIKit/UIKit.h>
#import "XPlaneAppDelegate.h"

// obj c:
//	x =  i.method_name("value",   3,   4)	C++
//	x = [i method_name:"value" p1:3 p2:4]	obj C

void sighandler(int sig)
{
	printf("Sig-handler alert! This means some networking failed somewhere.\n");
	printf("One time Space Camp had same IP addy for the routers and a sim on the network.\n");
	printf("Much confusion ensued, and this alert was hit!\n");
}

int main(int argc, char *argv[])
{
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	int retVal=-1;

	struct sigaction ss;
	ss.sa_handler = sighandler;
	sigaction(13, &ss, NULL);

	@try																							// THE DELEGATE GETS CALLED WHEN AN EVENT HAPPENS LIKE GOING TO BACKGROUND, FIINISHING LOAD, ETC.
	{
		retVal=UIApplicationMain(argc, argv, nil, NSStringFromClass([XPlaneAppDelegate class]));	// get the class object for the view controller... the last arg is a string indicating who the delegate is.
	}

	@catch(NSException*e)
	{
		NSString *		n = [e name		];
		NSString *		r = [e reason	];
		NSDictionary *	d = [e userInfo	];
		printf("%p %p %p %p\n",e,n,r,d);
	}

	[pool release];
	return retVal;
}
