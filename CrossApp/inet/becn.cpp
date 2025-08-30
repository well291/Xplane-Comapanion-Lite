#include "hl_types.h"

becn_class becn;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// SEND BEACON: THE BEACON GOES TO ALL MACHINES, INCLUDING OURS, SO WE CAN RUN OTHER APPS ON THE SAME MACHINE THAT USE THE BEACON!
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
xint becn_class::send_becn(const xchr* becn_name_in,const becn_type_enum becn_type)
{
	xchr	becn_name[name_dim64]={0};
	C_eq_C(	becn_name,becn_name_in,name_dim64);	// we need to have name_dim64 filled in for sure to compress, so we apply random in put to a string of that size here.

	buff_class	buff;
				buff.com_xdim("BECN",5);
				buff.com_xu08(1);				// data used by X-Plane, not us, and must be 1 to be listened to by X-Plane
				buff.com_xu08(0);				// data used by X-Plane, not us, and set per-version
				buff.com_xu32(becn_type);
				buff.com_xu32(0);				// data used by X-Plane, not us, and set per-version
				buff.com_xu32(0);				// data used by X-Plane, not us, and set per-version
				buff.com_xu16(0);				// data used by X-Plane, not us, and set per-version
				buff.com_xdim(becn_name,name_dim64);

	IPAddress				becn_send_addy;
							becn_send_addy.fromString("239.255.1.1");	// broadcast IP
	static PCSBSocketUDP	becn_send_socket	;
	static xint				becn_send_inited=0	;
	if(					   !becn_send_inited	){
							becn_send_inited=1	;
							becn_send_socket.udp_init(port_beacon_sends_from,blocking_setup);}	//  block here so we don't step on our own sends and mess up the early sends with the later ones.

	const xint ret=(xint)becn_send_socket.udp_write(&buff.m_data[0]			,	// REMEMBER, IF THIS MAC HAS BOTH ETHERNET AND AIRPORT TURNED ON,
													 buff.m_index			,	// THEN IT MAY BE SENDING TO ITSELF, SINCE THE AIRPORT WILL BE A DIFFERENT ADDY THAN THE ETHERNET!
													 becn_send_addy			,	// THIS HAS CAUSED HOURS OF WASTED TIME AS I TRIED TO FIGURE OUT WHY THE MACHINE WAS SENDING TO ITSELF, EVEN THOUGH SENDING TO A DIFFERENT IP ADDY!
													 port_beacon_listens_on	);	// WE ARE SENDING TO THIS PORT ON THE TARGET MACHINE... X-Plane wants 49000
	return ret;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// RECV REAL BEACON
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void becn_class::recv_becn()
{
	static IPAddress		becn_recv_addy;
							becn_recv_addy.fromString("239.255.1.1");	// 239.255.x.x is the address range for multi-casting that you can just use without registering at IANA.
	static PCSBSocketUDP	becn_recv_socket	;
	static xint				becn_recv_inited=0	;
	if(					   !becn_recv_inited	){
							becn_recv_inited=1	;
							becn_recv_socket.udp_init(port_beacon_listens_on,non_blocking_setup,becn_recv_addy);}

	static char	bcn_buf[1000]={0};

	IPAddress		becn_addy							;
	becn_type_enum	becn_type				=becn_none	;
	xchr			becn_name[name_dim64]	={0}		;

	const long len = becn_recv_socket.udp_read(	bcn_buf		,
										sizeof(	bcn_buf)	,
											   &becn_addy	);

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// FOR DIAGNOSTICS: ARE WE GETTTING ANYTHING?
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(len>4)
	{
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// ADD REAL BEACON
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		if(strncmp(bcn_buf,"BECN",5)==0)
		{
			becn_rcvd=bcn_buf;
			becn_rcvd.resize(4);
			becn_rcvd+=" from "+becn_addy.toString();
			becn_time =xtim.real_time;

			xint its_me_hi_im_the_problem_its_me = 0;

// not avail in tec...
#if XAVION
			for(xint i=0;i<inet.our_ip_addies.size();i++)
			if((string)inet.our_ip_addies[i] == becn_addy.toString())
				its_me_hi_im_the_problem_its_me=1;
#endif

			if(!its_me_hi_im_the_problem_its_me)
			{
				buff_class					buff;
											buff.load_buff(bcn_buf,(xint)len);
				xchr label[str_dim]={0};	buff.ext_xdim(label,5);
											buff.ext_xu08();	// data used by X-Plane, not us, and set per-version
											buff.ext_xu08();	// data used by X-Plane, not us, and set per-version
				becn_type=(becn_type_enum)	buff.ext_xu32();
											buff.ext_xu32();	// data used by X-Plane, not us, and set per-version
											buff.ext_xu32();	// data used by X-Plane, not us, and set per-version
											buff.ext_xu16();	// data used by X-Plane, not us, and set per-version
											buff.ext_cstr(becn_name,name_dim64);

				for(xint o=0;o<	other_becns.size();o++)
				if(				other_becns[o].other_addy==becn_addy)
				if(				other_becns[o].other_type==becn_type)
								other_becns[o].other_name= becn_name;	// if we have any synthetic beacons (when I made a beacon from UDP input, as I need to do since multicast does not work on some peoples LANs), then get them updated with the proper name

				xint already_have_it=0;
				for(xint o=0;o<	other_becns.size();o++)
				if(				other_becns[o].other_addy==becn_addy)
				if(				other_becns[o].other_type==becn_type)
				if(				other_becns[o].other_name==becn_name){
					already_have_it=1;}

				if(!already_have_it)
				{
					other_becn_class		other_becn;
											other_becn.beacon_used	= auto_connect;
											other_becn.other_addy	= becn_addy;
											other_becn.other_type	= becn_type;
											other_becn.other_name	= becn_name;
					other_becns.push_back(	other_becn);

					for(xint b1=0;b1<=		other_becns.size()		;b1++)
					for(xint b2=0;b2<(xint)(other_becns.size())-1	;b2++)
					{
						if(	other_becns[b2+1].other_name <	// alphabetize beacons so we get the same PCs in the same slots every time in the lab
							other_becns[b2  ].other_name )	// alphabetize beacons so we get the same PCs in the same slots every time in the lab
						{
							const other_becn_class other_becn =	other_becns[b2  ]						;
																other_becns[b2  ] = other_becns[b2+1]	;
																other_becns[b2+1] = other_becn			;
						}
					}
				}
			}
		}
	}
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// RECV SYNTHETIC BEACON
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// SOME LAN SETUPS DO NOT UNDERSTAND MULTICAST!
// HAVE SEEN THIS BEFORE!!!
// THE BEACONS NEVER MAKE IT, BUT THE UDP MESSAGES DO!
// THE LANS SEEM TO HAVE NO IDEA WHAT MULTICAST IS, SO THEY THINK THAT THE MULTICAST IP ADDY IS SIMPLY A WRONG IP ADDY THAT GOES NOWHERE!
// BUT THEY STILL SEND UDP JUST FINE!
// SO IN THAT CASE, IF I GET UDP FROM A COPY OF X-PLANE WHERE SOMEONE ENTERED THE IP ADDY MANUALLY IN THE ABSENCE OF BEACONS,
// THEN MAKE A SYNTHETIC BEACON SO WE CAN SEE THE LIST OF BEACONS TO CHOOSE FROM IN THE SETUP MENU IN CON PAD!
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void becn_class::recv_becn_synthetic(const IPAddress from_ip,xchr* label_read)
{
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// FOR DIAGNOSTICS: ARE WE GETTING ANYTHING?
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	becn_rcvd=label_read;
	becn_rcvd.resize(4);
	becn_rcvd+=" from "+from_ip.toString();
	becn_time=xtim.real_time;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// ADD SYNTHETIC BEACON
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	xint already_have_it=0;
	for(xint o=0;o<	other_becns.size();o++)
	if(				other_becns[o].other_addy==from_ip)
	if(				other_becns[o].other_type==becn_XPlane)
		already_have_it=1;
	if(!already_have_it)
	{
		other_becn_class		other_becn;
								other_becn.other_addy=from_ip;
								other_becn.other_type=becn_XPlane;
								other_becn.other_name="X-Plane from UDP";	// some LANs are NOT understanding broadcast IP, so they do NOT send beacons to ConPad! But targeted UDP gets to ConPad just fine. So we make a beacon based on that.
		other_becns.push_back(	other_becn);
	}
}
