#ifndef _PCSBSocket_h_
#define _PCSBSocket_h_

#include <sys/socket.h>
#include <netinet/in.h>

enum PCSB_Status
{
	pcsbstatus_Ready		=1,	// The socket is instantiated, bound, and ready	to connect.
	pcsbstatus_Connecting	=2,	// The socket is attempting to connect, but has not yet.  Or an incoming connection has come in.
	pcsbstatus_Connected	=3,	// The socket is connected and ready to send data.
	pcsbstatus_Disconnected	=4,	// The other side is disconnected, either because the connection failed, the connection was disconnected in a disorderly manner from either side, or because both sides issued an orderly disconnect.
	pcsbstatus_Error		=5	// An unknown error has happened; the socket is hosed.
};

class PCSBSocket
{
public:
	 PCSBSocket(unsigned short inPort,bool inServer);	// Construct a new socket.  If the socket is to be a server listening port, Pass 0 to dynamically allocate a port or a positive number to pick a well-known one.
	~PCSBSocket();										// Close the socket. you will be forcefully disconnected if necessary.

	static unsigned long	LookupAddress	(const char* inAddress						);	// static means we don't need to have a variable of this type declared and allocated in RAM to call this
	PCSB_Status				GetStatus		(void										);	// Get the socket's internal status.

	void					Connect			(unsigned long inIP,unsigned short inPort	);	// Connect to a server at a port and IP as a client.
	void					Disconnect		(void										);	// Disconnect the socket now.  This will be a disorderly disconnect
	void					Release			(void										);	// Indicate that you have no more data to send.  The socket will be  closed after the other side finishes sending data.  You can still call disconnect to force the issue.

	long					ReadData		(void*			buf,long len				);	// Read as much data from the socket as you want and is possible. Positive numbers indicate a read, 0 indicates that no data is available (either due to a lack of data sent or an orderly release). A negative number indicates an error, either an unknown error ora disconnect.  To further diagnose 0 or -1 responses, use GetStatus() and ReceivedRelease().
	long					WriteData		(const void*	buf,long len				);	// Write as much data to the socket as you want and is possible.  Positive numbers indicate a write, zero indicates that no data was written due to flow control.  -1 indicates that an error occured, either a socket closure or otherwise.

private:
	PCSBSocket		(int inWorkerSocket);

	int				mLinSocket		;
	PCSB_Status		mSocketStatus	;
	int				mSocketError	;
	bool			mReceivedRelease;
	bool			mSentRelease	;
	bool			mIsAServer		;

	PCSBSocket()							=delete;	// no public construct with no arguments
	PCSBSocket(const PCSBSocket&)			=delete;	// no public copy
	PCSBSocket& operator=(const PCSBSocket&)=delete;	// no public assign
};

#endif
