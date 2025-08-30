#ifndef becn_h
#define becn_h

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// BEACON
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
enum becn_type_enum
{
	becn_none	= 0,
	becn_XPlane = 1,	// this must be equal to xplm_Host_XPlane
	becn_Xavion = 8,	// this must be equal to xplm_Host_Xavion
	becn_ConPad = 9,	// this must be equal to xplm_Host_Control_Pad
	becn_PFD_MAP=10,
	becn_RADAR	=11,
	becn_Tech	=12
//	becn_OTTO	=133	// we don't use beacons in the field! it's always hard-wired IP address!
};

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// BEACON
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
class other_becn_class
{
public:
   ~other_becn_class(){}
	other_becn_class(){}

	xint			beacon_used	= 0			;
	IPAddress		other_addy				;
	becn_type_enum	other_type	= becn_none	;
	string			other_name				;
};

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// BEACON
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
class becn_class
{
public:
   ~becn_class(){}
	becn_class(){}

	string	becn_rcvd		;	// ControlPad
	xflt	becn_time=0.0	;	// ControlPad
	xint	auto_connect	;	// ControlPad

	vector <other_becn_class> other_becns;	// all the other apps we are hearing from

	xint send_becn			(const xchr* becn_name_in,const becn_type_enum beacon_en);
	void recv_becn			();
	void recv_becn_synthetic(const IPAddress from_ip,xchr* label_read);
};
extern becn_class becn;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// APP USE-CASE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline xint is_any_use()
{
	xint beacons_used=0;
	for(xint b=0;b<becn.other_becns.size();b++)
	if(becn.other_becns[b].beacon_used)
		beacons_used++;

	return (beacons_used>0);
}

inline xint is_lab_use()	// lab-use: we control P0 on each of MANY computers!
{
	xint beacons_used=0;
	for(xint b=0;b<becn.other_becns.size();b++)
	if(becn.other_becns[b].beacon_used)
		beacons_used++;

	return (beacons_used>1);
}

inline xint is_pro_use()	// pro-use: we control ALL the planes on ONE computer
{
	return (!is_lab_use());
}
#endif
