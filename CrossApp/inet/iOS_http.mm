#include "hl_types.h"
#import <Foundation/Foundation.h>

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// DOWNLOAD ITEM
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
@interface DSGDownloadItem : NSObject <NSURLConnectionDataDelegate>

-(void) downloadItemWithURLString:(NSString*)inURL toFile:(NSString*)fileName;

@property (copy)		NSString*	fileName			;
@property (readonly)	NSString*	errorstring			;

@property				long long	size_bytes			;
@property				long long	bytes_read			;

@property (readonly)	BOOL		wrapped_downloading	;
@property (readonly)	BOOL		success				;
@property (readonly)	float		completionPercentage;

@end

struct DSGDownloadItemImpl{DSGDownloadItem* wrapped;};

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// MEMBER FUNCTIONS OF down_class
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
down_class::down_class():impl(new DSGDownloadItemImpl)
{
	impl->wrapped		= [[DSGDownloadItem alloc]init];
	needs_to_scan		= 0;
	downloader_type		= 0;
	downloader_index	= 0;
	download_tries		= 0;
	download_wills		= 0;
}

		down_class::~down_class(){delete impl;}
void	down_class::start_download(std::string URL,std::string fileName)
{
		download_tries++;
	if(	download_tries > 10					)	// we just dont have net. dont spam the logs.
	if(	download_tries > download_wills * 2	)	// we just dont have net. dont spam the logs.
	{
//		// cant do this for customers... we might have app asleep while going to different net
//		if(RUBY_STATION)
//		{
//		//	APP_alert("Net downloads failing.","Stopping attempts to save battery.","","");
//			return;
//		}
	}

    NSString		* URLstring	=[NSString stringWithUTF8String:URL.c_str()		];
    NSString		* filestring=[NSString stringWithUTF8String:fileName.c_str()];
    DSGDownloadItem	* temItem	=impl->wrapped;

    [temItem downloadItemWithURLString:URLstring toFile:filestring];
	needs_to_scan = 1;
}

xflt down_class::percent_done	(){return impl->wrapped.completionPercentage;}
xint down_class::downloading	(){return impl->wrapped.wrapped_downloading	;}
xint down_class::was_successful	()
{
	if(impl->wrapped.success)
	{
		download_wills+=1;
	}
	else
	{
//	ok impl->wrapped.errorstring is crashing due to being 0x0
//	did not used to happen, but restoring old code into this app... it still happened! wha!?!?!?
//		NSLog(@"down_class not successful: %s,%s\n",ns_to_c_safe(impl->wrapped.fileName),ns_to_c_safe(impl->wrapped.errorstring));
	}

	return impl->wrapped.success;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// DOWNLOAD ITEM
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
@implementation DSGDownloadItem

-(id)init
{
    self = [super init];
    if(self)
    {
        _fileName=@"";
        _errorstring=@"";

        _size_bytes=0;
        _bytes_read=0;

        _wrapped_downloading=false;
        _success=false;
        _completionPercentage=0.0f;
 
		[NSURLCache setSharedURLCache:[[NSURLCache alloc]	initWithMemoryCapacity:0	// turn off the cache or we ram-leak like crazy!
															diskCapacity:0
															diskPath:nil]];
    }
    return self;
}

-(void)downloadItemWithURLString:(NSString *)inURL toFile: (NSString *)fileName
{
    _size_bytes=0;
    _bytes_read=0;

    _wrapped_downloading=true;
    _success=false;
    _completionPercentage=0.0f;
 
    self.fileName=fileName;
    NSURL * url = [NSURL URLWithString:inURL];
    NSURLRequest * req = [NSURLRequest requestWithURL:url];
    NSURLConnection * con = [[NSURLConnection alloc] initWithRequest:req delegate:self startImmediately:NO];
    [con start];
}

- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response
{
    NSHTTPURLResponse* newResp = (NSHTTPURLResponse*)response;
    if(newResp.statusCode>=400)
    {
        _wrapped_downloading=false;
        _success=false;
        _completionPercentage=0.0f;
    }
    else
    {
        _size_bytes=response.expectedContentLength;

        NSFileManager *filemgr;
        filemgr = [NSFileManager defaultManager];
        if ([filemgr fileExistsAtPath: _fileName ] == YES)
        {
            //File Exists, remove & reopen
            [filemgr removeItemAtPath:_fileName error:nil];
            [filemgr createFileAtPath:_fileName contents:nil attributes:nil];
        }
        else
        {
            [filemgr createFileAtPath:_fileName contents:nil attributes:nil];
        }
    }
}

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
    NSFileHandle *myHandle = [NSFileHandle fileHandleForWritingAtPath:_fileName];
    if(!myHandle)
    {
        _wrapped_downloading=false;
        _success=false;
        _completionPercentage=0.0f;
        return;
    }

    [myHandle seekToEndOfFile];
    [myHandle writeData:data];
    _bytes_read+=[data length];
    if(_size_bytes>0)
    {
        _completionPercentage=(float)_bytes_read/(float)_size_bytes*100.0f;
    }
    else _completionPercentage=0.0;// using -1 here can be a hint that we do not have the size yet, but it looks annoying on the output
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection
{
      _wrapped_downloading=false;
      _success=true;
      _completionPercentage=100.0f;
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
    _wrapped_downloading=false;
    _success=false;
    _completionPercentage=0.0f;
    _errorstring=[error description];
}
@end
