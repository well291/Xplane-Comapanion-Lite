#include "hl_types.h"
#include "UTL_thread.h"
#include <errno.h>
#include <string.h>

inet_class inet;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// SEND DATA
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
xint inet_class::inet_send(const xchr label[5],const xchr* data_in,xint data_len,const xint p_index)
{
	if(data_len>0						)
	if(data_len>udp_buffer_size_snd-5	){
		APP_alert("Trying to inet send too large a message:",int_to_str(data_len),"Max is:",int_to_str(udp_buffer_size_snd));
		return 0;}

	static xchr data_send[udp_buffer_size_snd]={0};

	if(label!=NULL)
	{
							  data_send[0]=label[0]				;	// package the data with the label, and also a 0/1 for apple/windows.
							  data_send[1]=label[1]				;	// ALSO PUT ANOTHER apple/windows AT THE END FOR A DOUBLE-CHECK TO MAKE SURE THE
							  data_send[2]=label[2]				;	// STRUCTURE WAS NOT STEPPED ON PARTWAY THROUGH THE SEND, AS CAN HAPPEN ON WINDOWS MACHINES!
							  data_send[3]=label[3]				;	// THIS IS BECAUSE WHILE MACS ONLY TAKE KEYBOARD EVENTS AT THE UPDATE PROC, WINDOWS TAKES
							  data_send[4]=51	/* IOS */		;	// KEYBOARD EVENTS ASYNC, BRINGING IN USER-MESSAGES, MODE-CHANGES, OR WHO-KNOWS WHAT
		if(data_len>0)memcpy(&data_send[5],data_in,data_len)	;	// FIRST COPY THEN DATA INTO THE BUFFER, THEN REVERSE BYTE-ORDER THE BUFFER!
												   data_len+=5	;
	}
	else if(data_len>0)memcpy(&data_send,data_in,data_len);

	xint ret=0;

	for(xint b=0;b<becn.other_becns.size();b++)
	if(becn.other_becns[b].beacon_used)
	{
		static PCSBSocketUDP	send_socket	;
		static xint				send_inited=0;
		if(					   !send_inited	){
								send_inited=1;
								send_socket.udp_init(port_ConPad_sends_from,blocking_setup);}	//  block here so we don't step on our own sends and mess up the early sends with the later ones.

		xint send = (is_pro_use() || p_index<0);								// for pro use or lab use with stuff like weather, send to all machines
		
		if(is_lab_use()														)	// for the lab use,
		if(p_index>=0														)	// if we are sending some stuff to only 1 machine, like moving a single aircraft,
		if(becn.other_becns[b].other_addy == xios.acft_p(p_index)->acft_addy)	// then only send to the beacons with the ip addy of the plane we are sending to now
			send=1;																// for stuff like init-flt
	
		if(send)
		{
			ret=(xint)send_socket.udp_write(data_send						,	// REMEMBER, IF THIS MAC HAS BOTH ETHERNET AND AIRPORT TURNED ON,
											data_len						,	// THEN IT MAY BE SENDING TO ITSELF, SINCE THE AIRPORT WILL BE A DIFFERENT ADDY THAN THE ETHERNET!
											becn.other_becns[b].other_addy	,	// THIS HAS CAUSED HOURS OF WASTED TIME AS I TRIED TO FIGURE OUT WHY THE MACHINE WAS SENDING TO ITSLEF, EVEN THOUGH SENDING TO A DIFFERENT IP ADDY!
											port_XPlane_listens_on			);	// WE ARE SENDING TO THIS PORT ON THE TARGET MACHINE.
		}

		const string label_str = {label[0],label[1],label[2],label[3]};

		if(ret > 0)
		{
			udp_str_sent = label_str+" to ip "+becn.other_becns[b].other_addy.toString();
			udp_tim_sent = xtim.real_time;
		}
		else
		{
			const char * error_msg = strerror(errno);

			udp_str_sent = label_str+" to ip "+becn.other_becns[b].other_addy.toString()+" failed with error "+error_msg;
			udp_tim_sent = xtim.real_time;
		}
	}

	pref.prefs_txt(IO_write);	// whatever we just sent... save it!

	return ret;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// UM DATA
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
const	long tcp_size=48000000		;
		char tcp_data[tcp_size]={0}	;
		long tcp_scanned_so_far=0	;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// RECEIVE DATA
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void inet_class::inet_per_frame()
{
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// RECEIVE ANY BEACONS TO BUILD UP THE COPIES OF X-PLANE WE CAN CHOOSE FROM
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(xtim.puff1)	becn.send_becn(iPhoneDeviceName_from_cpp(),becn_ConPad);
					becn.recv_becn();

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// RECEIVE DATA FROM X-PLANE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	enum tcp_download_enum
	{
		socket_not_inited	=0,
		download_not_inited	=1,
		download_requested	=2
	};

	static PCSBSocketUDP	UDP_socket	;
	static xint				UDP_inited=0;
	if(					   !UDP_inited	){
							UDP_inited=1;
							UDP_socket.udp_init(port_ConPad_listens_on,non_blocking_setup);}

	static tcp_download_enum	TCP_status		= socket_not_inited				;	// TCB socket status
	static PCSBSocket*			TCP_socket		= new PCSBSocket(port_NULL,false);	// init of the TCB socket
	static string				TCP_connect_ip	= "Not connected yet!";

	xint   done=xfals;
	while(!done)
	{
		IPAddress	from_ip								;
		xbyt		data_read[udp_buffer_size_rcv]={0}	;

		const xint len = (xint)UDP_socket.udp_read(data_read,sizeof(data_read),&from_ip);

		if(len<=0)
		{
			done=xtrue;
		}
		else if(len>5)
		{
			xchr label[5]={0};
			C_eq_C(label,(xchr*)data_read,4);
			udp_str_rcvd = (string)label+" from ip "+from_ip.toString();
			udp_tim_rcvd = xtim.real_time;

			#if NET_OZ_DIAG
				const string label_str	= {label[0],label[1],label[2],label[3]};
				const string last_recvd	= label_str+" from ip "+from_ip.toString();
				cout<<"xios_inet_rcv last_recvd="<<last_recvd<<"\n";	// too much log data when getting this once per FIX!
			#endif

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// HANDLE DATA FROM ANY COPY OF X-PLANE, TO BUILD LIST OF X-PLANES SENDING DATA TO US, EVEN IF THEIR BEACONS ARE BLOCKED, AS SOME LANS ARE INDEED DOING
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
			if(strncmp((xchr*)data_read,"xACF",4)==0)becn.recv_becn_synthetic(from_ip,(xchr*)data_read);
			if(strncmp((xchr*)data_read,"acf0",4)==0)becn.recv_becn_synthetic(from_ip,(xchr*)data_read);
			if(strncmp((xchr*)data_read,"acfX",4)==0)becn.recv_becn_synthetic(from_ip,(xchr*)data_read);
			#if SUPPORT_1208
			if(strncmp((xchr*)data_read,"xCON",4)==0)becn.recv_becn_synthetic(from_ip,(xchr*)data_read);
			if(strncmp((xchr*)data_read,"xLOC",4)==0)becn.recv_becn_synthetic(from_ip,(xchr*)data_read);
			#endif

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// HANDLE DATA FROM SELECTED COPIES OF X-PLANE, INCLUDING STARTING TCP
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// SUBTLE POINT: THIS WILL ONLY GRAB DATA FROM THE FIST SIM IT CONNECTS TO, BECAUSE WHEN THAT CONNECTION IS DONE,
// THE SOCKET DOES NOT GET SET BACK TO socket_not_inited
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
			for(xint b=0;b<becn.other_becns.size();b++)									// go thru all beacons
			if(becn.other_becns[b].beacon_used)											// that we use
			if(becn.other_becns[b].other_addy==from_ip)									// if we are using the one that is coming in, then listen to the signal
			{																			// then
				xios.xios_inet_rcv(from_ip,(xchr*)data_read,(xchr*)data_read+5,len-5);	// read the data from it, whatever UDP message it is
																						// and since we are talking to them
				if(	TCP_status==socket_not_inited){										// grab the TCP data as well
					TCP_socket->Connect(TCP_socket->LookupAddress(becn.other_becns[b].other_addy.toString().c_str()),port_ConPad_sends_from);
					TCP_status=download_not_inited;
					TCP_connect_ip=becn.other_becns[b].other_addy.toString();}
			}
		}
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// GRAB THE HI-SPEED TCP NAV DATA HERE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if		(TCP_socket)									// if we got a socket connection
	if		(TCP_socket->GetStatus()==pcsbstatus_Connected)	// if we got a socket connection
	switch	(TCP_status)
	{
		case socket_not_inited:
		break;

		case download_not_inited:
			if(TCP_socket->WriteData("CON1\n",5) == 5)
			{
				TCP_status = download_requested;

				tcp_str_sent = "CON1 to ip "+TCP_connect_ip+" sent OK";
				tcp_tim_sent = xtim.real_time;
			}
			else
			{
				tcp_str_sent = "CON1 to ip "+TCP_connect_ip+" failed with error "+strerror(errno);
				tcp_tim_sent = xtim.real_time;
			}
		break;

		case download_requested:
			xint  error_found	= 0;
			long  received		= 1;					// OK I'm an optimist.
			while(received		> 0 && !error_found)	// while no error!
			{
				received = TCP_socket->ReadData(tcp_data + tcp_scanned_so_far,	// if received == 0, because waiting for data, then we just drop out of this loop!
												tcp_size - tcp_scanned_so_far);	// if received == 0, because waiting for data, then we just drop out of this loop!

				shar.shar_per_init("TCP_socket->ReadData",int_to_str((xint)received));

				if(tcp_scanned_so_far >= tcp_size)
				{
					tcp_str_rcvd = "From ip "+TCP_connect_ip+", TCP buffer too small!";
					tcp_tim_rcvd = xtim.real_time;
					error_found = 1;
				}

				if(received < 0)
				{
					tcp_str_rcvd = "From ip "+TCP_connect_ip+", "+int_to_str((xint)received)+" bytes, "+strerror(errno);
					tcp_tim_rcvd = xtim.real_time;
					error_found = 1;
				}

				if(received > 0)	// got data!
				{
					tcp_str_rcvd = "From ip "+TCP_connect_ip;
					tcp_tim_rcvd = xtim.real_time;

					tcp_scanned_so_far += received;

					if(					tcp_scanned_so_far>8)
					if(memcmp(tcp_data+	tcp_scanned_so_far-8,"DONECODE",8)==0)
					{
						shar.shar_per_init("DONECODE");

								TCP_socket->Release();
						delete	TCP_socket;
								TCP_socket=nullptr;

						vec_fix				.clear();		// ok, we have a connection! let's clear the data to grab in the new stuff!
						vec_nav				.clear();		// ok, we have a connection! let's clear the data to grab in the new stuff!
						vec_apt				.clear();		// ok, we have a connection! let's clear the data to grab in the new stuff!
						vec_awy				.clear();		// ok, we have a connection! let's clear the data to grab in the new stuff!
						vec_hol				.clear();		// ok, we have a connection! let's clear the data to grab in the new stuff!
						set_cmd				.clear();		// ok, we have a connection! let's clear the data to grab in the new stuff!
						set_cus				.clear();		// ok, we have a connection! let's clear the data to grab in the new stuff!
						apt_icao_to_index	.clear();		// ok, we have a connection! let's clear the data to grab in the new stuff!

						xint  ctr=0;
						while(ctr<tcp_scanned_so_far)
						{
							int32_t sz=0;
							memcpy(&sz,tcp_data+ctr+4,4);	// label is 4, then size is 4, then data

							xios.xios_inet_rcv(IPAddress(),(xchr*)tcp_data+ctr,(xchr*)tcp_data+ctr+8,sz);

							ctr+=8+sz;						// add 4 label, 4 size, and the data
						}

						pref.prefs_bin(IO_write);			// save to the disk so we have them right away next time! even if tcp fails!
						tcp_scanned_so_far=0;				// reset in case we do this again
						break;								// break out of while !error loop!
					}
				}
			}
		break;
	}
}
