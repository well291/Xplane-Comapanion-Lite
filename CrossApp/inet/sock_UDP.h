#ifndef _PCSBSocket_UDP_h_
#define _PCSBSocket_UDP_h_

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

const xint udp_buffer_size_snd=65536;
const xint udp_buffer_size_rcv=65536;

enum
{
		blocking_setup=1,
	non_blocking_setup=2
};

struct IPAddress {
    int proto_;
    union {
        in6_addr	in6_;
        in_addr		in4_;
    } in_;
    string toString() const
    {
        char address[INET6_ADDRSTRLEN];
        memset(address, 0, INET6_ADDRSTRLEN);
        if (proto_ == AF_INET6)
            inet_ntop(proto_, (void*)&in_.in6_, address, INET6_ADDRSTRLEN);
        else
            inet_ntop(proto_, (void*)&in_.in4_, address, INET6_ADDRSTRLEN);
        return address;
    }
    void fromString(const char* input)
    {
        memset(&in_, 0, sizeof(in_));
        if (inet_pton(AF_INET, input, &in_.in4_))
            proto_ = AF_INET;
        else if (inet_pton(AF_INET6, input, &in_.in6_))
            proto_ = AF_INET6;
    }
    void toV4Mapped(in_addr* v4_addr)
    {
        if (isv4mapped())
            memcpy(v4_addr, &in_.in6_.s6_addr[12], sizeof(uint32_t));
    }
    void toV4Mapped(in6_addr* v6_addr)
    {
        if (isv4())
        {
            memset(v6_addr, 0, sizeof(in6_addr));
            memset(&v6_addr->s6_addr[10], 0xff, 2);
            memcpy(&v6_addr->s6_addr[12], &in_.in4_, sizeof(uint32_t));
        }
    }
    IPAddress()
    {
        proto_ = AF_INET6;
        in_.in6_ = in6addr_any;
    }
	IPAddress(const std::string& human_addr)
	{
		fromString(human_addr.c_str());
	}
    bool operator==(const IPAddress& rhs) const
    {
        if (proto_ == AF_INET6)
            return memcmp(&in_.in6_, &rhs.in_.in6_, sizeof(in_.in6_)) == 0;
        return memcmp(&in_.in4_, &rhs.in_.in4_, sizeof(in_.in4_)) == 0;
    }
    operator bool() const
    {
        in6_addr any = in6addr_any;
        return memcmp(&in_.in6_, &any, sizeof(in_.in6_));
    }
    const IPAddress& operator=(const IPAddress& rhs)
    {
        this->in_ = rhs.in_;
        this->proto_ = rhs.proto_;
        return *this;
    }
	bool operator<(const IPAddress& rhs) const
	{
		// For sorting a list of IP-addresses:
		// uninited objects go LAST
		// IPv4 addresses go FIRST
		// IPv4 adresses among themselves are numerically sorted
		// IPv6 adresses among themselves are lexicographically sorted
		if (*this && !rhs)
			return true;
		if (!*this && rhs)
			return false;
		if (proto_==AF_INET6&& rhs.proto_==AF_INET )
			return false;
		if (proto_==AF_INET && rhs.proto_==AF_INET6)
			return true;
		if (proto_==rhs.proto_ && proto_ == AF_INET)
			return ntohl(in_.in4_.s_addr)<ntohl(rhs.in_.in4_.s_addr);
		else
			return toString()<rhs.toString();
	}
    bool isv4mapped() const
    {
        return (proto_ == AF_INET6 && IN6_IS_ADDR_V4MAPPED(&in_.in6_));
    }
    bool isv4() const
    {
        return proto_==AF_INET;
    }
};

class PCSBSocketUDP {
public:
	 PCSBSocketUDP(){mWinSocket = -1;}
	~PCSBSocketUDP(){udp_exit();}

	int udp_init(const port_enum inPort,const xint blocking_enum,IPAddress multi_group4=IPAddress(),IPAddress multi_group6=IPAddress());
	void udp_exit(void);

	long udp_read(	void*				outBuf,
					long				outBufLength,
					IPAddress*			outSrcIP);

	long udp_write(	const void*			inBuf,
					const long			inBufLength,
					const IPAddress&	inDstIP,
					const port_enum		inDstPort);

	int				mWinSocket;
	sockaddr_in6	sIn6;
};

#endif
