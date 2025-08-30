//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void _APP_alert(const xchr* func,const xchr* file,xint line,const string& c1,const string& c2,const string& c3,const string& c4)
{
	cout.flush();
	alert_message1=c1;
	alert_message2=c2;
	alert_message3=c3;
	alert_message4=c4;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void apps_class::apps_event(xint et,xint m)
{
	if(et==click_type_click		){set_mou_clicked (m,xtrue);	set_mou_dragging(m,xtrue);	dragged_pix[m]=0.0;	soun.play_sound(soun_click);}
	if(et==click_type_release	){set_mou_released(m,xtrue);	set_mou_dragging(m,xfals);													}
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
xint apps_class::mouse_box_internal(const xflt x1,const xflt y1,const xflt x2,const xflt y2,const click_type_enum click_type)
{
//	OGL_framerect(col_red,1.0,x1,y1,x2-x1,y2-y1);
//	OGL_fillrect (col_red,1.0,mou_drag_x[0]-5,mou_drag_y[0]-5,10,10);

	const xint m=0;

//	for(xint m=0;m<2;m++)
	if(	(click_type==click_type_click	&& mou_clicked	(m)							) ||
		(click_type==click_type_drag	&& mou_dragging	(m)							) ||
		(click_type==click_type_release	&& mou_released	(m) && dragged_pix[m]<20.0	) )	// act on release... since that is what gives us time to tell a click from a drag! if he dragged then no that was not a click it was a scrolling drag!
	{
		if(mou_drag_x[m]>x1)
		if(mou_drag_x[m]<x2)
		if(mou_drag_y[m]>y1)
		if(mou_drag_y[m]<y2)

		if(!graf.sci_on || mou_drag_x[m]>graf.sci_x1)		// since our UI draws and checks for mouse clicks at the same time, scissors should apply equally to both... easy!
		if(!graf.sci_on || mou_drag_x[m]<graf.sci_x2)
		if(!graf.sci_on || mou_drag_y[m]>graf.sci_y1)
		if(!graf.sci_on || mou_drag_y[m]<graf.sci_y2)
		{
			if(mou_clicked	(m))set_mou_clicked (m,xfals);	// only 1 click plz to not fall thru to next screen!
			if(mou_released	(m))set_mou_released(m,xfals);	// only 1 click plz to not fall thru to next screen!

			return xtrue;
		}
	}

	return xfals;
}

xint apps_class::mouse_box_click	(const xflt x1,const xflt y1,const xflt x2,const xflt y2){return mouse_box_internal(x1,y1,x2,y2,click_type_click	);}
xint apps_class::mouse_box_drag		(const xflt x1,const xflt y1,const xflt x2,const xflt y2){return mouse_box_internal(x1,y1,x2,y2,click_type_drag		);}
xint apps_class::mouse_box_release	(const xflt x1,const xflt y1,const xflt x2,const xflt y2){return mouse_box_internal(x1,y1,x2,y2,click_type_release	);}
xint apps_class::mouse_btn_slide	(const xint x1,const xint y1,const xint x2,const xint y2,void* new_var,const xint reset)
{
	static void*	crt_var=0;	// local to keep this object tight
	if(reset)		crt_var=0;	// reset at end of frame

	if(mouse_box_internal(	x1,y1,
							x2,y2,click_type_click)) crt_var= new_var;	// this is for grabbing sliders, so we grab on the click!
	return									 		(crt_var==new_var);
}

