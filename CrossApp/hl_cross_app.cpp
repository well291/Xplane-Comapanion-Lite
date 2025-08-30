#include "hl_types.h"

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// PROCS WHERE AVIONICS AND ATC SHOULD AGREE, EVEN THOUGH THE AVIONICS MIGHT NOT USE THE SAME HOOPS FORMAT AS THE ATC
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline xint check_long(const long val,const long val_lo,const long val_hi)
{
	if(check_nan(	val			))	return 0;
	if(				val<val_lo	 )	return 0;
	if(				val>val_hi	 )	return 0;
									return 1;
}

inline xint check_xdob(const xdob val,const xdob val_lo,const xdob val_hi)
{
	if(check_nan(	val			))	return 0;
	if(				val<val_lo	 )	return 0;
	if(				val>val_hi	 )	return 0;
									return 1;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// CONF COMPRESS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
xint conf_class::CONF_compress(buff_class* buff,const xsho version,const xchr acft_id_name[name_dim64])
{
	header_compress(buff,version,acft_id_name);
	buff->com_xdim(conf_serial_number,name_dim64);
	return buff->perfect_scan();
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// CONF EXTRACT
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
xint conf_class::CONF_extract(buff_class* buff,const xsho version,xchr acft_id_name[name_dim64])
{
	if(!header_extract(buff,version,acft_id_name))return 0;
	buff->ext_xdim(conf_serial_number,name_dim64);
	return buff->perfect_scan();
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// DONE COMPRESS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
xint done_class::DONE_compress(buff_class* buff,const xsho version,const xchr acft_id_name[name_dim64])
{
	header_compress(buff,version,acft_id_name);
	return buff->perfect_scan();
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// DONE EXTRACT
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
xint done_class::DONE_extract(buff_class* buff,const xsho version,xchr acft_id_name[name_dim64])
{
	if(!header_extract(buff,version,acft_id_name))return 0;
	return buff->perfect_scan();
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// WAIT COMPRESS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
xint wait_class::WAIT_compress(buff_class* buff,const xsho version,const xchr acft_id_name[name_dim64])
{
	header_compress(buff,version,acft_id_name);

												buff->com_xdim(wait_serial_number,name_dim64);
	if(check_nan(wait_Unix_sec_dep))return 0;	buff->com_xdob(wait_Unix_sec_dep			);

	return buff->perfect_scan();
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// WAIT EXTRACT
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
xint wait_class::WAIT_extract(buff_class* buff,const xsho version,xchr acft_id_name[name_dim64])
{
	if(!header_extract(buff,version,acft_id_name))return 0;

						buff->ext_xdim(wait_serial_number,name_dim64);
	wait_Unix_sec_dep=	buff->ext_xdob();	if(check_nan(wait_Unix_sec_dep))return 0;

	return buff->perfect_scan();
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// HERE COMPRESS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
xint here_class::HERE_compress(buff_class* buff,const xsho version,const xchr acft_id_name[name_dim64])
{
	header_compress(buff,version,acft_id_name);

	if(!check_xdob	(plan_mode				,    0,       255)){APP_alert("Out of range plan_mode"				,"HERE_compress",int_to_str(plan_mode				),"");	return 0;}	// this is BYTE to must go 0-255
//	if(!check_xdob	(Unix_sec				,    0,4102444800)){APP_alert("Out of range Unix_sec"				,"HERE_compress",int_to_str(Unix_sec				),"");	return 0;}	// JAN 1, YEAR 10,000
	if(!check_xdob	(acft_lat_deg			,  -90,        90)){APP_alert("Out of range acft_lat_deg"			,"HERE_compress",int_to_str(acft_lat_deg			),"");	return 0;}
	if(!check_xdob	(acft_lon_deg			, -180,       180)){APP_alert("Out of range acft_lon_deg"			,"HERE_compress",int_to_str(acft_lon_deg			),"");	return 0;}
	if(!check_xdob	(acft_ele_mtr			,-1000,   1000000)){APP_alert("Out of range acft_ele_mtr"			,"HERE_compress",int_to_str(acft_ele_mtr			),"");	return 0;}
	if(!check_xdob	(acft_psi_deg_true		,    0,       360)){APP_alert("Out of range acft_psi_deg_true"		,"HERE_compress",int_to_str(acft_psi_deg_true		),"");	return 0;}
	if(!check_xdob	(acft_hpath_deg_true	,    0,       360)){APP_alert("Out of range acft_hpath_deg_true"	,"HERE_compress",int_to_str(acft_hpath_deg_true		),"");	return 0;}
	if(!check_xdob	(acft_psi_deg_true_dot	, -360,       360)){APP_alert("Out of range acft_psi_deg_true_dot"	,"HERE_compress",int_to_str(acft_psi_deg_true_dot	),"");	return 0;}
	if(!check_xdob	(acft_gs_2d_msc			,    0,     10000)){APP_alert("Out of range acft_gs_2d_msc"			,"HERE_compress",int_to_str(acft_gs_2d_msc			),"");	return 0;}
	if(!check_xdob	(acft_vh_msc			,-1000,      1000)){APP_alert("Out of range acft_vh_msc"			,"HERE_compress",int_to_str(acft_vh_msc				),"");	return 0;}

	buff->com_xbyt	(plan_mode				);
	buff->com_xdob	(acft_Unix_sec			);
	buff->com_xdob	(acft_lat_deg			);
	buff->com_xdob	(acft_lon_deg			);
	buff->com_xflt	(acft_ele_mtr			);
	buff->com_xflt	(acft_psi_deg_true		);
	buff->com_xflt	(acft_hpath_deg_true	);
	buff->com_xflt	(acft_psi_deg_true_dot	);
	buff->com_xflt	(acft_gs_2d_msc			);
	buff->com_xflt	(acft_vh_msc			);

	return buff->perfect_scan();
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// HERE EXTRACT
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
xint here_class::HERE_extract(buff_class* buff,const xsho version,xchr acft_id_name[name_dim64])
{
	if(!header_extract(buff,version,acft_id_name))return 0;

					plan_mode				=(RADAR_mode_enum)	buff->ext_xbyt();
					acft_Unix_sec			=					buff->ext_xdob();
					acft_lat_deg			=					buff->ext_xdob();
					acft_lon_deg			=					buff->ext_xdob();
					acft_ele_mtr			=					buff->ext_xflt();
					acft_psi_deg_true		=					buff->ext_xflt();
					acft_hpath_deg_true		=					buff->ext_xflt();
					acft_psi_deg_true_dot	=					buff->ext_xflt();
					acft_gs_2d_msc			=					buff->ext_xflt();
					acft_vh_msc				=					buff->ext_xflt();

	if(!check_xdob(	plan_mode				,    0,       255)){APP_alert("Out of range plan_mode"				,"HERE_compress",int_to_str(plan_mode				),"");	return 0;}	// this is BYTE to must go 0-255
//	if(!check_xdob(	Unix_sec				,    0,4102444800)){APP_alert("Out of range Unix_sec"				,"HERE_compress",int_to_str(Unix_sec				),"");	return 0;}	// JAN 1, YEAR 10,000
	if(!check_xdob(	acft_lat_deg			,  -90,        90)){APP_alert("Out of range acft_lat_deg"			,"HERE_compress",int_to_str(acft_lat_deg			),"");	return 0;}
	if(!check_xdob(	acft_lon_deg			, -180,       180)){APP_alert("Out of range acft_lon_deg"			,"HERE_compress",int_to_str(acft_lon_deg			),"");	return 0;}
	if(!check_xdob(	acft_ele_mtr			,-1000,   1000000)){APP_alert("Out of range acft_ele_mtr"			,"HERE_compress",int_to_str(acft_ele_mtr			),"");	return 0;}
	if(!check_xdob(	acft_psi_deg_true		,    0,       360)){APP_alert("Out of range acft_psi_deg_true"		,"HERE_compress",int_to_str(acft_psi_deg_true		),"");	return 0;}
	if(!check_xdob(	acft_hpath_deg_true		,    0,       360)){APP_alert("Out of range acft_hpath_deg_true"	,"HERE_compress",int_to_str(acft_hpath_deg_true		),"");	return 0;}
	if(!check_xdob(	acft_psi_deg_true_dot	, -360,       360)){APP_alert("Out of range acft_psi_deg_true_dot"	,"HERE_compress",int_to_str(acft_psi_deg_true_dot	),"");	return 0;}
	if(!check_xdob(	acft_gs_2d_msc			,    0,     10000)){APP_alert("Out of range acft_gs_2d_msc"			,"HERE_compress",int_to_str(acft_gs_2d_msc			),"");	return 0;}
	if(!check_xdob(	acft_vh_msc				,-1000,      1000)){APP_alert("Out of range acft_vh_msc"			,"HERE_compress",int_to_str(acft_vh_msc				),"");	return 0;}

	return buff->perfect_scan();
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// PLAN COMPRESS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
xint plan_class::PLAN_compress(buff_class* buff,const xsho version,const xchr acft_id_name[name_dim64],const xsho gates_per_frame)
{
	if(gates.size()>0)
	{
		header_compress(buff,version,acft_id_name);

		if(!check_xdob(plan_prio_person_days	,0, 100000000	)){APP_alert("Out of range plan_prio_person_days"		,"PLAN_compress",int_to_str(plan_prio_person_days	),"");	return 0;}	// 1,000 people at 100,000 each
		if(!check_xdob(plan_buffer_lon_time_sec	,0, 3600.0		)){APP_alert("Out of range plan_buffer_lon_time_sec"	,"PLAN_compress",int_to_str(plan_buffer_lon_time_sec),"");	return 0;}
		if(!check_xdob(plan_buffer_lat_radius_m	,0,10000.0		)){APP_alert("Out of range plan_buffer_lat_radius_m"	,"PLAN_compress",int_to_str(plan_buffer_lat_radius_m),"");	return 0;}
		if(!check_xdob(plan_buffer_vrt_radius_m	,0, 1000.0		)){APP_alert("Out of range plan_buffer_vrt_radius_m"	,"PLAN_compress",int_to_str(plan_buffer_vrt_radius_m),"");	return 0;}
		if(!check_xdob(gates.size()				,0,65535		)){APP_alert("Out of range gates.size()"				,"PLAN_compress",int_to_str((xint)gates.size()		),"");	return 0;}	// this is SHORT so must go 0-65535

		buff->com_xdim(			plan_serial_number		,name_dim64);
		buff->com_xint(			plan_prio_person_days	);
		buff->com_xflt(			plan_buffer_lon_time_sec);
		buff->com_xflt(			plan_buffer_lat_radius_m);
		buff->com_xflt(			plan_buffer_vrt_radius_m);
		buff->com_xdim(			plan_dep_apt_id,id_dim8	);
		buff->com_xdim(			plan_arr_apt_id,id_dim8	);
		buff->com_xsho((xsho)	gates.size()			);

		for(xint g_loop=0;g_loop<gates_per_frame;g_loop++)
		{
			gate_index_to_send=intwrap(						// MUST DO THIS FIRST IN CASE THE GATE DIM CHANGED,
			gate_index_to_send+1,0,(xsho)gates.size()-1);	// LEAVING THE gate_index_to_send TOO HIGH!

			if(!check_xdob(	    gate_index_to_send						,                 0,gates.size()-1)	){APP_alert("Out of range gate_index_to_send"	,"PLAN_compress",int_to_str(	  gate_index_to_send					),"");	return 0;}	// this is SHORT so must go 0-65535
			if(!check_xdob(	    gate_index_to_send						,                 0,     65535		)){APP_alert("Out of range gate_index_to_send"	,"PLAN_compress",int_to_str(	  gate_index_to_send					),"");	return 0;}	// this is SHORT so must go 0-65535
			if(!check_xdob(gates[gate_index_to_send].gate_type			,                 0,       255		)){APP_alert("Out of range gate_type"			,"PLAN_compress",int_to_str(gates[gate_index_to_send].gate_type			),"");	return 0;}	// this is BYTE to must go 0-255
			if(!check_xdob(gates[gate_index_to_send].gate_lat_deg		,               -90,        90		)){APP_alert("Out of range lat_deg"				,"PLAN_compress",int_to_str(gates[gate_index_to_send].gate_lat_deg		),"");	return 0;}
			if(!check_xdob(gates[gate_index_to_send].gate_lon_deg		,              -180,       180		)){APP_alert("Out of range lon_deg"				,"PLAN_compress",int_to_str(gates[gate_index_to_send].gate_lon_deg		),"");	return 0;}
			if(!check_xdob(gates[gate_index_to_send].gate_ele_mtr		,             -1000,   1000000		)){APP_alert("Out of range ele_mtr"				,"PLAN_compress",int_to_str(gates[gate_index_to_send].gate_ele_mtr		),"");	return 0;}
//			if(!check_xdob(gates[gate_index_to_send].gate_Unix_sec		,                 0,4102444800		)){APP_alert("Out of range gate_Unix_sec"		,"PLAN_compress",int_to_str(gates[gate_index_to_send].gate_Unix_sec		),"");	return 0;}	// JAN 1, YEAR 10,000

			buff->com_xsho(		 gate_index_to_send						);
			buff->com_xdim(gates[gate_index_to_send].gate_serial_number	,name_dim64);
			buff->com_xbyt(gates[gate_index_to_send].gate_type			);
			buff->com_xdob(gates[gate_index_to_send].gate_lat_deg		);
			buff->com_xdob(gates[gate_index_to_send].gate_lon_deg		);
			buff->com_xflt(gates[gate_index_to_send].gate_ele_mtr		);
			buff->com_xdob(gates[gate_index_to_send].gate_Unix_sec		);
		}

		return buff->perfect_scan();
	}

	return 0;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// PLAN EXTRACT
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
xint plan_class::PLAN_extract(buff_class* buff,const xsho version,xchr acft_id_name[name_dim64])
{
	if(!header_extract(buff,version,acft_id_name))return 0;

	buff->ext_xdim(	plan_serial_number			,name_dim64);
					plan_prio_person_days		=buff->ext_xint();
					plan_buffer_lon_time_sec	=buff->ext_xflt();
					plan_buffer_lat_radius_m	=buff->ext_xflt();
					plan_buffer_vrt_radius_m	=buff->ext_xflt();
												 buff->ext_xdim(plan_dep_apt_id,id_dim8);
												 buff->ext_xdim(plan_arr_apt_id,id_dim8);
	const xsho number_gates						=buff->ext_xsho();

	if(!check_xdob(plan_prio_person_days	,0					, 100000000	)){APP_alert("Out of range plan_prio_person_days"	,"PLAN_extract",int_to_str(plan_prio_person_days	),"");	return 0;}	// 1,000 people at 100,000 each
	if(!check_xdob(plan_buffer_lon_time_sec	,0					, 3600.0	)){APP_alert("Out of range plan_buffer_lon_time_sec","PLAN_extract",int_to_str(plan_buffer_lon_time_sec	),"");	return 0;}
	if(!check_xdob(plan_buffer_lat_radius_m	,0					,10000.0	)){APP_alert("Out of range plan_buffer_lat_radius_m","PLAN_extract",int_to_str(plan_buffer_lat_radius_m	),"");	return 0;}
	if(!check_xdob(plan_buffer_vrt_radius_m	,0					, 1000.0	)){APP_alert("Out of range plan_buffer_vrt_radius_m","PLAN_extract",int_to_str(plan_buffer_vrt_radius_m	),"");	return 0;}
	if(!check_xdob(number_gates				,0					,65535		)){APP_alert("Out of range gates.size()"			,"PLAN_extract",int_to_str(number_gates				),"");	return 0;}	// this is SHORT so must go 0-65535
	gates.resize  (number_gates);

	while(buff->buff_can_scan())
	{
		const xsho			  scan_index					=					buff->ext_xsho();	if(!check_xdob(scan_index,0,number_gates-1)){APP_alert("scan_index>number_gates-1","PLAN_extract",int_to_str(gate_index_to_send),"");	return 0;}
		buff->ext_xdim(	gates[scan_index].gate_serial_number,					name_dim64);
						gates[scan_index].gate_type			=(gate_type_enum)	buff->ext_xbyt();
						gates[scan_index].gate_lat_deg		=					buff->ext_xdob();
						gates[scan_index].gate_lon_deg		=					buff->ext_xdob();
						gates[scan_index].gate_ele_mtr		=					buff->ext_xflt();
						gates[scan_index].gate_Unix_sec		=					buff->ext_xdob();

		if(!check_xdob(		  scan_index					,                 0,     65535)){APP_alert("Out of range gate_index_to_send","PLAN_extract",int_to_str(      gate_index_to_send						),"");	return 0;}	// this is SHORT so must go 0-65535
		if(!check_xdob(	gates[scan_index].gate_type			,                 0,       255)){APP_alert("Out of range gate_type"			,"PLAN_extract",int_to_str(gates[gate_index_to_send].gate_type			),"");	return 0;}	// this is BYTE to must go 0-255
		if(!check_xdob(	gates[scan_index].gate_lat_deg		,               -90,        90)){APP_alert("Out of range lat_deg"			,"PLAN_extract",int_to_str(gates[gate_index_to_send].gate_lat_deg		),"");	return 0;}
		if(!check_xdob(	gates[scan_index].gate_lon_deg		,              -180,       180)){APP_alert("Out of range lon_deg"			,"PLAN_extract",int_to_str(gates[gate_index_to_send].gate_lon_deg		),"");	return 0;}
		if(!check_xdob(	gates[scan_index].gate_ele_mtr		,             -1000,   1000000)){APP_alert("Out of range ele_mtr"			,"PLAN_extract",int_to_str(gates[gate_index_to_send].gate_ele_mtr		),"");	return 0;}
//		if(!check_xdob(	gates[scan_index].gate_Unix_sec		,                 0,4102444800)){APP_alert("Out of range gate_Unix_sec"		,"PLAN_extract",int_to_str(gates[gate_index_to_send].gate_Unix_sec		),"");	return 0;}	// JAN 1, YEAR 10,000
	}

	return buff->perfect_scan();
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// PLAN APPLY
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void plan_class::PLAN_apply(plan_class* plan_source)	// PASS IN POINTER TO AVOID MAKING A COPY OF THIS THING! THAT IS BAD SINCE IT CONTAINS VECTORS!!!!
{
	C_eq_C(	plan_serial_number		,plan_source->plan_serial_number , name_dim64)	;
			plan_prio_person_days	=plan_source->plan_prio_person_days				;
			plan_buffer_lon_time_sec=plan_source->plan_buffer_lon_time_sec			;
			plan_buffer_lat_radius_m=plan_source->plan_buffer_lat_radius_m			;
			plan_buffer_vrt_radius_m=plan_source->plan_buffer_vrt_radius_m			;
	C_eq_C(	plan_dep_apt_id			,plan_source->plan_dep_apt_id,id_dim8)			;
	C_eq_C(	plan_arr_apt_id			,plan_source->plan_arr_apt_id,id_dim8)			;
			gates.resize			(plan_source->gates.size())						;

	for(xint g=0;g<	plan_source->gates.size();g++)
	if(				plan_source->gates[g].gate_type!=gate_type_init)
	{
		C_eq_C(	gates[g].gate_serial_number	,plan_source->gates[g].gate_serial_number , name_dim64)	;
				gates[g].gate_type			=plan_source->gates[g].gate_type						;
				gates[g].gate_lat_deg		=plan_source->gates[g].gate_lat_deg						;
				gates[g].gate_lon_deg		=plan_source->gates[g].gate_lon_deg						;
				gates[g].gate_ele_mtr		=plan_source->gates[g].gate_ele_mtr						;
				gates[g].gate_Unix_sec		=plan_source->gates[g].gate_Unix_sec					;
	}

	PLAN_update_bounds_double_buffer();	// update bounds based on newest info!
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// UPDATE THE PLAN BOUNDS WHENEVER ANYTHING CHANGES THE PLAN BOUNDS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void plan_class::PLAN_update_bounds_double_buffer()	// this updates to double-buffer so we can check the CENTER of each gate against it for SPEED!
{
	check_tim_lo_double_buffer=			plan_dep_tim_sec()						;
	check_tim_hi_double_buffer=			plan_arr_tim_sec()						;
	check_lat_lo_double_buffer=dobmin2(	plan_dep_lat_deg(),plan_arr_lat_deg())	;
	check_lat_hi_double_buffer=dobmax2(	plan_dep_lat_deg(),plan_arr_lat_deg())	;
	check_lon_lo_double_buffer=dobmin2(	plan_dep_lon_deg(),plan_arr_lon_deg())	;
	check_lon_hi_double_buffer=dobmax2(	plan_dep_lon_deg(),plan_arr_lon_deg())	;

	for(xint g=0;g<gates.size();g++)
	{
		if(gates[g].gate_Unix_sec	<check_tim_lo_double_buffer)check_tim_lo_double_buffer=gates[g].gate_Unix_sec	;
		if(gates[g].gate_Unix_sec	>check_tim_hi_double_buffer)check_tim_hi_double_buffer=gates[g].gate_Unix_sec	;
		if(gates[g].gate_lat_deg	<check_lat_lo_double_buffer)check_lat_lo_double_buffer=gates[g].gate_lat_deg	;
		if(gates[g].gate_lat_deg	>check_lat_hi_double_buffer)check_lat_hi_double_buffer=gates[g].gate_lat_deg	;
		if(gates[g].gate_lon_deg	<check_lon_lo_double_buffer)check_lon_lo_double_buffer=gates[g].gate_lon_deg	;
		if(gates[g].gate_lon_deg	>check_lon_hi_double_buffer)check_lon_hi_double_buffer=gates[g].gate_lon_deg	;
	}

	const xdob tunnel_radius_deg_lat=plan_buffer_lat_radius_m*mtrtodeg;
	const xdob tunnel_radius_deg_lon=plan_buffer_lat_radius_m*mtrtodeg/cos(fltmax2(	fltabs(check_lat_lo_double_buffer),
																					fltabs(check_lat_hi_double_buffer))*degtorad);

	check_tim_lo_double_buffer-=plan_buffer_lon_time_sec*2.0;	// we use double-buffer so we can check the center of the target gate against these bounds for speed!
	check_tim_hi_double_buffer+=plan_buffer_lon_time_sec*2.0;	// we use double-buffer so we can check the center of the target gate against these bounds for speed!
	check_lat_lo_double_buffer-=tunnel_radius_deg_lat	*2.0;	// we use double-buffer so we can check the center of the target gate against these bounds for speed!
	check_lat_hi_double_buffer+=tunnel_radius_deg_lat	*2.0;	// we use double-buffer so we can check the center of the target gate against these bounds for speed!
	check_lon_lo_double_buffer-=tunnel_radius_deg_lon	*2.0;	// we use double-buffer so we can check the center of the target gate against these bounds for speed!
	check_lon_hi_double_buffer+=tunnel_radius_deg_lon	*2.0;	// we use double-buffer so we can check the center of the target gate against these bounds for speed!
}
