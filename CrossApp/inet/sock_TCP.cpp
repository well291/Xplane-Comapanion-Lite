#include "hl_types.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netdb.h>

#define NON_BLOCKING 1

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// CON AND DE STRUCT
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
PCSBSocket::PCSBSocket(unsigned short inPort, bool inServer)
{
	mSocketStatus		=pcsbstatus_Ready;
	mSocketError		=0;
	mReceivedRelease	=false;
	mSentRelease		=false;
	mIsAServer			=false;

	struct sockaddr_in	sIn;
						sIn.sin_family		=AF_INET		;
						sIn.sin_addr.s_addr	=INADDR_ANY		;	// Let windows pick a suitable addy
						sIn.sin_port		=htons(inPort)	;	// Set the local port or if 0 let windows assign one

	mLinSocket=socket(PF_INET,SOCK_STREAM,0);	// start the socket for TCP/IP comms

	if(mLinSocket==-1)
	{
		fprintf(stderr, "LINUX: NET: I could not even create the socket! Uh-oh!\n");
		mSocketStatus=pcsbstatus_Error;
		mSocketError=errno;
		return;
	}

	if(::bind(mLinSocket, (struct sockaddr*)&sIn, sizeof(sIn))!=0) // Bind the socket to the local addy
	{
		fprintf(stderr, "LINUX: NET: I could not even bind the socket! Uh-oh!\n");
		mSocketStatus=pcsbstatus_Error;
		mSocketError=errno;
		return;
	}

	unsigned long nSetSocketType=NON_BLOCKING;
	int nResult=ioctl(mLinSocket,FIONBIO,&nSetSocketType);	// set to non-blocking
	if(nResult==-1)
	{
		fprintf(stderr, "LINUX: NET: I could not set the socket non-blocking! Uh-oh!\n");
		mSocketStatus=pcsbstatus_Error;
		mSocketError=errno;
		return;
	}

	if(inServer)	// We're a server so start listening!
	{
		mIsAServer=true;
		if(listen(mLinSocket, SOMAXCONN))
		{
			fprintf(stderr, "LINUX: NET: I could not start listening! Uh-oh!\n");
			mSocketStatus=pcsbstatus_Error;
			mSocketError=errno;
			return;
		}
	}
}

PCSBSocket::~PCSBSocket()
{
	shutdown(mLinSocket, SHUT_RDWR);
	close(mLinSocket);
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// UTILS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
unsigned long PCSBSocket::LookupAddress(const char* inAddress)
{
	unsigned long ip=inet_addr(inAddress);		//try it as an IP first
	if(ip!=INADDR_NONE)
		return ip;

	struct hostent* host=gethostbyname(inAddress);
	if(host==NULL)
		return 0;

	struct in_addr* ipAddress=(struct in_addr*)(host->h_addr);
	if(!ipAddress)
		return 0;

	return ipAddress->s_addr;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// STATUS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
PCSB_Status PCSBSocket::GetStatus(void)
{
	struct timeval	timeout;
					timeout.tv_sec	=0;		// no timeout!
					timeout.tv_usec	=0;		// no timeout!

	if((mSocketStatus==pcsbstatus_Ready) && mIsAServer)
	{
		fd_set socketSetRead;
		FD_ZERO(&socketSetRead);
		FD_SET(mLinSocket,&socketSetRead);

		int nReturn=select(mLinSocket+1,&socketSetRead,NULL,NULL,&timeout);
		if(nReturn > 0)
		{
			if(FD_ISSET(mLinSocket,&socketSetRead))	// Able to read on it?
			{
				mSocketStatus=pcsbstatus_Connecting;
				return mSocketStatus;
			}
			else
			{
				mSocketStatus=pcsbstatus_Disconnected;
				return mSocketStatus;
			}
		}
		else if(nReturn==0)		// Didn't have enough time to complete request fully (non-blocking)
		{
		}
		else if(errno!=EINTR)	// Must be a socket error :(
		{
			fprintf(stderr, "LINUX: NET: Some sort of socket error! %d\n", nReturn);
			mSocketStatus=pcsbstatus_Error;
			mSocketError=errno;
		}
	}
	else if(mSocketStatus==pcsbstatus_Connecting)
	{
		fd_set socketSetWrite,socketSetError;
		FD_ZERO(&socketSetWrite);
		FD_ZERO(&socketSetError);
		FD_SET(mLinSocket,&socketSetWrite);		// macro to form set
		FD_SET(mLinSocket,&socketSetError);
		int nReturn=select(mLinSocket+1,NULL,&socketSetWrite,&socketSetError,&timeout);

		if(nReturn > 0)			// Check to see if our connection completed.
		{
			if((FD_ISSET(mLinSocket,&socketSetWrite)) && !(FD_ISSET(mLinSocket,&socketSetError)))
			{
				// If the socket is ready to write with no errors
				mSocketStatus=pcsbstatus_Connected;
				return mSocketStatus;
			}
			else
			{
				mSocketStatus=pcsbstatus_Disconnected;
				return mSocketStatus;
			}
		}
		else if(nReturn==0)		// Didn't have enough time to complete request fully (non-blocking)
		{
		}
		else if(errno!=EINTR)	// Must be a socket error :(
		{
			fprintf(stderr, "LINUX: NET: Some sort of socket error while connecting! %d (errno=%d sock=%d)\n", nReturn,errno,mLinSocket);
			mSocketStatus=pcsbstatus_Error;
			mSocketError=errno;
		}
	}

	return mSocketStatus;
}

void PCSBSocket::Connect(unsigned long inIP, unsigned short inPort)
{
	mSocketStatus=pcsbstatus_Connecting;

	struct sockaddr_in sOut;

	sOut.sin_family=AF_INET;
	sOut.sin_port=htons(inPort);			// set the remote connection port
	sOut.sin_addr.s_addr=(in_addr_t)inIP;	// set the remote IP to connect to

	int nResult=connect(mLinSocket,(sockaddr*)&sOut,sizeof(sOut));	//connect

	if(nResult==-1)
	{
		if(errno!=EAGAIN && errno!=EINPROGRESS)		// If its a mere blocking error, check if connection completed by using select()
		{
			fprintf(stderr, "LINUX: NET: Some sort of connect error! %d\n", errno);

			mSocketStatus=pcsbstatus_Error;
			mSocketError=errno;
		}
	}
	else mSocketStatus=pcsbstatus_Connected;		// If it didn't have an error to begin with
}

void PCSBSocket::Disconnect(void)
{
	shutdown(mLinSocket,SHUT_RDWR);
	mSocketStatus=pcsbstatus_Disconnected;
}

void PCSBSocket::Release(void)
{
	if(shutdown(mLinSocket,SHUT_WR)==-1)
	{
		mSocketStatus=pcsbstatus_Error;
		mSocketError=errno;
	}
	mSentRelease=true;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// READ
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
long PCSBSocket::ReadData(void* outBuf,long inLength)
{
	xint nReturn=(xint)recv(mLinSocket,(char*)outBuf,inLength,0);

	if(nReturn==-1)
	{
		if(errno==EAGAIN)
		{
			return 0;	// No Data on Socket yet!
		}
		else if((errno==ECONNABORTED) || (errno==ECONNREFUSED) || (errno==ECONNRESET) || (errno==EHOSTUNREACH) || (errno==ENETDOWN) || (errno==ENETRESET) || (errno==ENETUNREACH) || (errno==ETIMEDOUT))
		{
			mSocketStatus=pcsbstatus_Disconnected;
			return -1;
		}
		else
		{
			fprintf(stderr, "LINUX: NET: Some sort of error while recving! %d\n", errno);
			mSocketStatus=pcsbstatus_Error;	// signal a real error.
			mSocketError=errno;
			return -1;
		}
	}
	else if(nReturn==0)
	{
		mReceivedRelease=true;

		if(mSentRelease)
		{
			mSocketStatus=pcsbstatus_Disconnected;
			return -1;
		}
		else return 0;
	}
	else
	{
		return nReturn;
	}
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// WRITE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
long PCSBSocket::WriteData(const void* inBuf,long inLength)
{
	#if NET_OZ_DIAG
		char c;
		ssize_t x = recv(mLinSocket, &c, 1, MSG_PEEK);	// from just sid: this tests the socket state!
														// some sort of... PEEK!
		if(x > 0)
		{
			cout<<"WriteData has data, leave it in socket buffer until B connects\n";
		}
		else if(x == 0)
		{
			cout<<"WriteData handle FIN from A\n";
		}
		else
		{
			cout<<"WriteData handle errors, socket closed on other side\n";
		}
	#endif

	const xint nReturn = (xint)send(mLinSocket,(char*)inBuf,inLength,0);

	if(nReturn==-1)
	{
		if(errno==EWOULDBLOCK)
		{
			return 0;	// No Data on Socket yet!
		}
		else if((errno==ECONNABORTED) || (errno==ECONNREFUSED) || (errno==ECONNRESET) || (errno==EHOSTUNREACH) || (errno==ENETDOWN) || (errno==ENETRESET) || (errno==ENETUNREACH) || (errno==ETIMEDOUT))
		{
			mSocketStatus=pcsbstatus_Disconnected;
			return -1;
		}
		else
		{
			fprintf(stderr, "LINUX: NET: Some sort of error while sending! %d\n", errno);
			mSocketStatus=pcsbstatus_Error;	// signal a real error.
			mSocketError=errno;
			return -1;
		}
	}
	else
	{
		return nReturn;
	}
}
