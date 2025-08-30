#include "hl_types.h"

#ifndef IPV6_ADD_MEMBERSHIP
#define IPV6_ADD_MEMBERSHIP IPV6_JOIN_GROUP
#endif

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
int PCSBSocketUDP::udp_init(const port_enum inPort,const xint blocking_enum,IPAddress multi_group4,IPAddress multi_group6)	// this is a blocking call... slower, but always works
{
	int nResult;
	sIn6.sin6_family = AF_INET6;
	sIn6.sin6_addr = in6addr_any;							// Let linux pick a suitable addy
	sIn6.sin6_port = htons(inPort);							// Set the local port or if0 let windows assign one

	mWinSocket = socket(PF_INET6,SOCK_DGRAM,IPPROTO_UDP);	// start the socket for UDP comms
	if(mWinSocket == -1)
	{
		return 0;
	}

	int off = 0;
	if(setsockopt(mWinSocket,IPPROTO_IPV6,IPV6_V6ONLY,(char *)&off, sizeof(off)))
	{
		// We cannot receive dual stack.
		close(mWinSocket);
		return 0;
	}
	#define kReuseOptions SO_REUSEPORT	// init code

	int reuse = 1;
	if(setsockopt(mWinSocket, SOL_SOCKET, kReuseOptions, (char *)&reuse, sizeof(reuse)) < 0)
	{
		// We couldn't set reuse address, which is important.
		close(mWinSocket);
		return 0;
	}

	if(::bind(mWinSocket, (sockaddr*)&sIn6, sizeof(sIn6))) // Bind the socket to the local addy, which here is in6addr_any
	{
		// This is almost always because our port is in use and reuse addr didn't work
		close(mWinSocket);
		return 0;
	}

	if(multi_group6 && !multi_group6.isv4())
	{
		struct ipv6_mreq	mreq6;
		mreq6.ipv6mr_interface = 0;
		memcpy(&mreq6.ipv6mr_multiaddr,
				&multi_group6.in_.in6_,
				sizeof(struct in6_addr));

		if(setsockopt(mWinSocket, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP, &mreq6, sizeof(mreq6)) < 0)
		{
			close(mWinSocket);
			return 0;
		}
	}

	if(multi_group4 && multi_group4.isv4())
	{
		struct ip_mreq mreq4;
		inet_aton("0.0.0.0", &mreq4.imr_interface);
		memcpy(&mreq4.imr_multiaddr,
				&multi_group4.in_.in4_,
				sizeof(struct in_addr));

		if(setsockopt(mWinSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq4, sizeof(mreq4)) < 0)
		{
			struct ipv6_mreq	mreq6;
			mreq6.ipv6mr_interface = 0;
			in6_addr mapped;
			multi_group4.toV4Mapped(&mapped);
			memcpy(&mreq6.ipv6mr_multiaddr,
					&mapped,
					sizeof(struct in6_addr));

			if(setsockopt(mWinSocket, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP, &mreq6, sizeof(mreq6)) < 0)
			{
				close(mWinSocket);
				return 0;
			}
		}
	}

	if(blocking_enum==non_blocking_setup)
	{
		nResult = fcntl(mWinSocket, F_SETFL, O_NONBLOCK); // set to non-blocking
		if(nResult != 0)
		{
			APP_alert("Could not set net to non-blocking!","","","");
			close(mWinSocket);
			return 0;
		}
	}

	nResult = fcntl(mWinSocket, F_SETFD, FD_CLOEXEC);

	dev_assert(nResult == 0);

	socklen_t act_len=0;
	socklen_t act_len_size=sizeof(act_len);

	xint set_result1=setsockopt(mWinSocket,SOL_SOCKET,SO_SNDBUF,(char*)&udp_buffer_size_snd,sizeof(udp_buffer_size_snd));
	xint set_result2=setsockopt(mWinSocket,SOL_SOCKET,SO_RCVBUF,(char*)&udp_buffer_size_rcv,sizeof(udp_buffer_size_rcv));
	xint get_result1=getsockopt(mWinSocket,SOL_SOCKET,SO_SNDBUF,(char*)&act_len,&act_len_size);
	xint get_result2=getsockopt(mWinSocket,SOL_SOCKET,SO_RCVBUF,(char*)&act_len,&act_len_size);

	if(set_result1!=0)APP_alert("Could not set socket size!","","","");
	if(get_result1!=0)APP_alert("Could not set socket size!","","","");
	if(set_result2!=0)APP_alert("Could not set socket size!","","","");
	if(get_result2!=0)APP_alert("Could not set socket size!","","","");

	return 1;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void PCSBSocketUDP::udp_exit(void)
{
	if(mWinSocket != -1)
		close(mWinSocket);
	mWinSocket = -1;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
long PCSBSocketUDP::udp_read(
				void*		outBuf,
				long		outBufLength,
				IPAddress*	outSrcIP)
{
	xint result;
	sockaddr_in6 remoteAddress;
	int fromlen						= sizeof(remoteAddress);
	remoteAddress.sin6_family		= AF_INET6;
	remoteAddress.sin6_port			= 0;
	remoteAddress.sin6_addr		 = in6addr_any;

	if(mWinSocket<0)APP_alert("Bad mWinSocket!","","","");

	result=(xint)recvfrom(mWinSocket,
					 (char*)outBuf,
					 outBufLength,
					 0,
					 (sockaddr*)&remoteAddress,
					 (socklen_t*)&fromlen);

	if(IN6_IS_ADDR_V4MAPPED(&(remoteAddress.sin6_addr)))
	{
		outSrcIP->proto_ = AF_INET;
		memcpy(&outSrcIP->in_.in4_, &remoteAddress.sin6_addr.s6_addr[12], sizeof(uint32_t));
	}
	else
	{
		outSrcIP->proto_ = AF_INET6;
		outSrcIP->in_.in6_ = remoteAddress.sin6_addr;
	}

	return result;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
long PCSBSocketUDP::udp_write(
				const void*			inBuf,
				const long			inBufLength,
				const IPAddress&	inDstIP,
				const port_enum		inDstPort)
{
	int result;
	sockaddr_in6 destAddress;
	memset(&destAddress, 0, sizeof(sockaddr_in6));
	int tolen					= sizeof(destAddress);
	destAddress.sin6_family		= AF_INET6;
	destAddress.sin6_port		= htons(inDstPort);

	if(inDstIP.proto_ == AF_INET6)
	{
		destAddress.sin6_addr = inDstIP.in_.in6_;
	}
	else
	{
		memset(&destAddress.sin6_addr.s6_addr[10],255,sizeof(uint16_t));
		memcpy(&destAddress.sin6_addr.s6_addr[12], &inDstIP.in_.in4_, sizeof(uint32_t));
	}

	result=(xint)sendto(mWinSocket,
						(char*)inBuf,
						inBufLength,
						0,
						(sockaddr*)&destAddress,
						tolen);

	return result;
}

