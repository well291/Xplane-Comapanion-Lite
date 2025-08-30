#ifndef hl_cross_app_h
#define hl_cross_app_h

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// MESSAGE VERSIONS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
const xsho message_version_HERE	= 1;	// transponder ping: here I am. can be sent on the ground, but flight mode status could get loaded in here
const xsho message_version_RQST	= 3;	// AIRPLANE to ATC: the flight plan the plane wants to fly
const xsho message_version_WAIT	= 4;	// ATC to AIRPLANE: just wait this long and then fly your requested route
const xsho message_version_ROUT	= 5;	// ATC to AIRPLANE: the route the plane needs to fly
const xsho message_version_CONF	= 6;	// confirm
const xsho message_version_DONE	= 7;	// confirm

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// ENUMS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
enum RADAR_mode_enum
{
	plan_mode_awaiting_FMS				=0,
	plan_mode_sending_RQST_until_ROUT	=1,
	plan_mode_holding_for_hoop_time		=2,
	plan_mode_flying_plan				=3,
	plan_mode_cleared_active			=4	// this means remove from the system now
};

enum gate_type_enum		// sent as BYTE so keep under 256
{
	gate_type_init	=0,	// not set
	gate_type_dep	=1,	// may only rotate heading around the start-gate, nothing else
	gate_type_clb	=2,
	gate_type_crz	=3,	// this is what can be adjusted: climb, turn, etc
	gate_type_des	=4,
	gate_type_arr	=5	// may only rotate heading around the end-gate, nothing else
};

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// UTILS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline xint plan_priority(	xint num_pax_normal	,	// unit is: 1 person-day of life
							xint num_pax_prio	,
							xint num_pax_emer	,
							xint in_flight_now	)
{
	return	 num_pax_normal			// flight diversion costs each person on board a day
			+num_pax_prio*100		// prio ranks a life-flight with a 737
			+num_pax_emer*10000;	// emer assumes 30 years per person on average
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// THE DATA IS SIMPLY STREAMS OF BYTES WITH 100% PACKING EFFICIENCY: IT DOES NOT GET MORE CLEAR OR SIMPLE OR TIGHT THAN THAT!
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
class buff_class
{
public:
   ~buff_class(){}
	buff_class(){}

	vector <xchr>	m_data		;
	xint			m_index		=0;
	bool			m_overrun	=0;

	inline xint buff_can_scan	(){return (m_index <m_data.size()				);}	// we can scan some more
	inline xint perfect_scan	(){return (m_index==m_data.size() && !m_overrun	);}	// the input message size was the perfect size to scan

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// LO-LEVEL ACCESSORS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
private:
	inline void	com_data(const xchr* in_data,const xint in_len)
	{
		m_data.insert(m_data.end(),in_data,(const xchr*)(in_data+in_len));
		m_index += in_len;
	 }

	inline void ext_data(xchr* in_data,const xint in_len)
	{
		if(m_index+in_len > m_data.size())
		{
			memset(in_data,0,in_len);
			m_overrun = xtrue;

			APP_alert("extract-data over-run!","","","");
		}
		else
		{
			memcpy(in_data,&m_data[m_index],in_len);
								   m_index+=in_len ;
		}
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// NET COMPRESSION AND EXTRACTION
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
public:
	inline void load_buff(const xchr* data_in,const xint len)
	{
								m_data.resize(len)	;
		for(xint i=0;i<len;i++)	m_data[i]=data_in[i];
	}

	inline void		com_xdim(const xchr*	d,const xint dim){for(xint c=0;c<dim;c++)	com_data((xchr*)&d[c]	,sizeof(xchr	));				}
	inline void		com_xchr(const xchr		d				){							com_data((xchr*)&d   	,sizeof(xchr	));				}
	inline void		com_xbyt(const xbyt		d				){							com_data((xchr*)&d   	,sizeof(xbyt	));				}
	inline void		com_xsho(const xsho		d				){							com_data((xchr*)&d   	,sizeof(xsho	));				}
	inline void		com_xint(const xint		d				){							com_data((xchr*)&d   	,sizeof(xint	));				}
	inline void		com_long(const long		d				){							com_data((xchr*)&d   	,sizeof(long	));				}
	inline void		com_xflt(const xflt		d				){							com_data((xchr*)&d   	,sizeof(xflt	));				}
	inline void		com_xdob(const xdob		d				){							com_data((xchr*)&d   	,sizeof(xdob	));				}
	inline void		com_xu08(const uint8_t	d				){							com_data((xchr*)&d   	,sizeof(uint8_t	));				}
	inline void		com_xu16(const uint16_t	d				){							com_data((xchr*)&d   	,sizeof(uint16_t));				}
	inline void		com_xu32(const uint32_t	d				){							com_data((xchr*)&d   	,sizeof(uint32_t));				}
	inline void		com_xu64(const xu64  	d				){							com_data((xchr*)&d   	,sizeof(xu64	));				}

	inline void		ext_xdim(xchr* d,const xint dim			){for(xint c=0;c<dim;c++)	ext_data((xchr*)&d[c]	,sizeof(d[c]	));				}
	inline xchr		ext_xchr(								){xchr		 d;				ext_data((xchr*)&d		,sizeof(d		));	return d;	}
	inline xbyt		ext_xbyt(								){xbyt		 d;				ext_data((xchr*)&d		,sizeof(d		));	return d;	}
	inline xsho		ext_xsho(								){xsho		 d;				ext_data((xchr*)&d		,sizeof(d		));	return d;	}
	inline xint		ext_xint(								){xint		 d;				ext_data((xchr*)&d		,sizeof(d		));	return d;	}
	inline long		ext_long(								){long		 d;				ext_data((xchr*)&d		,sizeof(d		));	return d;	}
	inline xflt		ext_xflt(								){xflt		 d;				ext_data((xchr*)&d		,sizeof(d		));	return d;	}
	inline xdob		ext_xdob(								){xdob		 d;				ext_data((xchr*)&d		,sizeof(d		));	return d;	}
	inline uint8_t	ext_xu08(								){uint8_t	 d;				ext_data((xchr*)&d		,sizeof(d		));	return d;	}
	inline uint16_t	ext_xu16(								){uint16_t	 d;				ext_data((xchr*)&d		,sizeof(d		));	return d;	}
	inline uint32_t	ext_xu32(								){uint32_t	 d;				ext_data((xchr*)&d		,sizeof(d		));	return d;	}
	inline xu64		ext_xu64(								){xu64		 d;				ext_data((xchr*)&d		,sizeof(d		));	return d;	}
	inline void		ext_cstr(xchr* d,const xint dim			)
	{
		for(xint c=0;c<dim;c++)
		{
			ext_data((xchr*)&d[c],sizeof(d[c]));
			if(d[c] == 0)return;	// um that's all we got! this is the case for beacons coming in from x-plane, which have variable-length strings for the PC name!
		}
	}
};

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// MESSAGE HEADER EXCHANGE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline void header_compress(buff_class* buff,const xsho version,const xchr acft_id_name[name_dim64])
{
		buff->com_xsho(version					);
		buff->com_xdim(acft_id_name,name_dim64	);
}

inline xint header_extract(buff_class* buff,const xsho version,xchr acft_id_name[name_dim64])
{
	if(	buff->ext_xsho(						)!=version)	return 0;
		buff->ext_xdim(acft_id_name,name_dim64);		return 1;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// CONF
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
class conf_class
{
public:
   ~conf_class(){}
	conf_class(){}

	xchr conf_serial_number[name_dim64] = {0};

	xint CONF_compress(buff_class* buff,const xsho version,const xchr acft_id_name[name_dim64]);
	xint CONF_extract (buff_class* buff,const xsho version,		 xchr acft_id_name[name_dim64]);
};

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// WHENEVER THE HERE MESSAGE FROM THE ACF TO THE ATC HAS plan_mode == plan_mode_cleared_active, THEN THAT MEANS PLAN IS CANCELLED AND WE ARE DONE. ATC SENDS THIS MESSAGE IN THAT CASE.
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
class done_class
{
public:
   ~done_class(){}
	done_class(){}

	xint DONE_compress(buff_class* buff,const xsho version,const xchr acft_id_name[name_dim64]);
	xint DONE_extract (buff_class* buff,const xsho version,		 xchr acft_id_name[name_dim64]);
};

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// HERE: THIS IS SENT WITH acft_mode == acft_mode_offline WHEN NOT FLYING, WHICH CAUSES ATC TO PICK UP THE PLANE WITH THE HERE MESSAGE, AND DISCARD IT BASED ON THE MODE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
class here_class
{
public:
   ~here_class(){}
	here_class(){}

	RADAR_mode_enum plan_mode	=plan_mode_awaiting_FMS;	// this is sent as size byte, so keep it in range 0 to 255

	xdob acft_Unix_sec			=0.0;
	xdob acft_lat_deg			=0.0;
	xdob acft_lon_deg			=0.0;
	xflt acft_ele_mtr			=0.0;

	xflt acft_psi_deg_true		=0.0;	// for plotting
	xflt acft_hpath_deg_true	=0.0;	// for location extrapolation
	xflt acft_psi_deg_true_dot	=0.0;	// for location extrapolation
	xflt acft_gs_2d_msc			=0.0;	// for location extrapolation
	xflt acft_vh_msc			=0.0;	// for location extrapolation

	xint HERE_compress(buff_class* buff,const xsho version,const xchr acft_id_name[name_dim64]);
	xint HERE_extract (buff_class* buff,const xsho version,		 xchr acft_id_name[name_dim64]);
};

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// DELAY
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
class wait_class
{
public:
   ~wait_class(){}
	wait_class(){}

	xchr wait_serial_number[name_dim64]	={0};	// unique plan serial number
	xdob wait_Unix_sec_dep				=0.0;	// seconds the pan is pushed back

	xint WAIT_compress(buff_class* buff,const xsho version,const xchr acft_id_name[name_dim64]);
	xint WAIT_extract (buff_class* buff,const xsho version,		 xchr acft_id_name[name_dim64]);
};

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// FLIGHT PLAN EXCHANGE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
class gate_class
{
public:
   ~gate_class(){}
	gate_class(){}

	xchr			gate_serial_number[name_dim64] ={0};
	gate_type_enum	gate_type			=gate_type_init;	// gate type for mods by controller
	xdob			gate_lat_deg		=0.0;
	xdob			gate_lon_deg		=0.0;
	xflt			gate_ele_mtr		=0.0;
	xdob			gate_Unix_sec		=0.0;	// time we plan to pass the gate
};

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// FLIGHT PLAN, WHICH LIVES WITHIN AN AIRPLANE, WHICH CONTAINS THE AIRPLANE ID
// SO WE DO NOT HAVE INFO CONNECTING THIS PLAN TO AN AIRPLANE, SINCE THIS STRUCT LIVES WITHIN AN AIRPLANE STRUCT!
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
class plan_class	// RQST from airplane and ROUT from ATC
{
public:
   ~plan_class(){}
	plan_class(){}

	xchr plan_serial_number[name_dim64] = {0};

	xint plan_prio_person_days		=0;						// prio in person-days of life

	xflt plan_buffer_lon_time_sec	=0.0;
	xflt plan_buffer_lat_radius_m	=0.0;
	xflt plan_buffer_vrt_radius_m	=0.0;

	xchr plan_dep_apt_id[id_dim8]	={0};	// just a note for ATC is all... we fly to lat and lon not apt ID
	xchr plan_arr_apt_id[id_dim8]	={0};	// just a note for ATC is all... we fly to lat and lon not apt ID

	xsho gate_index_to_send			=0;		// next gate to send across the net to the other guy for this plan
	vector <gate_class> gates;				// if i want accessor like dep and arrival locations and times then just have accessors to the gates here!

	xdob check_tim_lo_double_buffer	=0.0;
	xdob check_lat_lo_double_buffer	=0.0;
	xdob check_lon_lo_double_buffer	=0.0;

	xdob check_tim_hi_double_buffer	=0.0;
	xdob check_lat_hi_double_buffer	=0.0;
	xdob check_lon_hi_double_buffer	=0.0;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// ACCESSORS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	inline xdob plan_dep_lat_deg(){if(gates.size()==0)return 0.0;	return gates[					0].gate_lat_deg	;}
	inline xdob plan_dep_lon_deg(){if(gates.size()==0)return 0.0;	return gates[					0].gate_lon_deg	;}
	inline xdob plan_dep_ele_mtr(){if(gates.size()==0)return 0.0;	return gates[					0].gate_ele_mtr	;}
	inline xdob plan_dep_tim_sec(){if(gates.size()==0)return 0.0;	return gates[					0].gate_Unix_sec;}

	inline xdob plan_arr_lat_deg(){if(gates.size()==0)return 0.0;	return gates[(xint)gates.size()-1].gate_lat_deg	;}
	inline xdob plan_arr_lon_deg(){if(gates.size()==0)return 0.0;	return gates[(xint)gates.size()-1].gate_lon_deg	;}
	inline xdob plan_arr_ele_mtr(){if(gates.size()==0)return 0.0;	return gates[(xint)gates.size()-1].gate_ele_mtr	;}
	inline xdob plan_arr_tim_sec(){if(gates.size()==0)return 0.0;	return gates[(xint)gates.size()-1].gate_Unix_sec;}

	inline xint plan_is_loaded_for_flight()	// we have received this entire plan by serial number
	{
		if((xint)gates.size()==0)
			return 0;

		if(strlen(plan_serial_number)==0)
			return 0;

		for(xint g=0;g<(xint)	gates.size();g++)
		if(strcmp(				gates[g].gate_serial_number,plan_serial_number))
			return 0;

		return 1;
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// PLAN PROCS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	xint PLAN_push_back_or_cancel_the_others(const xint a_this,plan_class* p_them,const xdob delay_sec,const string delay_reason,string* delay_reasons);
	void PLAN_update_bounds_double_buffer	();
	void PLAN_check							(const xint this_a);
	void PLAN_apply							(plan_class* plan_source);
	xint PLAN_compress						(buff_class* buff,const xsho version,const	xchr acft_id_name[name_dim64],const xsho gates_per_frame);
	xint PLAN_extract						(buff_class* buff,const xsho version,		xchr acft_id_name[name_dim64]);

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// PLAN MODS DONE BY ATC
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	void flight_delay_holding(const xint gate_index_to_hold);
	xdob flight_delay_dep_sec(plan_class* p_them,const xchr* delay_name,string* delay_reason);	// has to be & to pass the vector by reference to allow mods
	xdob flight_delay_arr_sec(plan_class* p_them,const xchr* delay_name,string* delay_reason);	// has to be & to pass the vector by reference to allow mods
	xdob flight_delay_crz_sec(plan_class* p_them,const xchr* delay_name,string* delay_reason);	// has to be & to pass the vector by reference to allow mods
};
#endif
