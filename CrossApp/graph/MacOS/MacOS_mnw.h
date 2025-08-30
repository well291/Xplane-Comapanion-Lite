#ifndef hl_mnw_h
#define hl_mnw_h

const xint XRES_default=1024;
const xint YRES_default= 768;

//ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
// MNW
//ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
class mnw_class
{
public:
   ~mnw_class(){}
	mnw_class()
	{
		memset(this,0,sizeof(*this));
		XRES_priv=XRES_default;
		YRES_priv=YRES_default;
	}

	xint XRES_priv;		inline xint XRES_now(){return XRES_priv;}
	xint YRES_priv;		inline xint YRES_now(){return YRES_priv;}

	xint mouse_clicked			;	// this gets hit ONCE each time we click
	xint now_dragging			;	// this gets hit as long as the mouse is down
	xint mouse_lifted			;	// this gets hit ONCE each time we lift
	xflt mx,my,drag_x,drag_y	;	// drag is pix dragged this frame
	xint mouse_wheel_this_frame	;	// how many clicks has mouse wheel been turned?

//ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
// MOUSE-BOX PROCS
//ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
	inline xint mouse_over		(xflt x1,xflt y1,xflt x2,xflt y2){return (fltbox(mx,my,x1,y1,x2-1,y2-1)					);}	// -1 or we get an overlap in the menuing system
	inline xint mouse_down		(xflt x1,xflt y1,xflt x2,xflt y2){return (fltbox(mx,my,x1,y1,x2-1,y2-1) && mouse_clicked);}	// the boxes look perfect but the mouse-clicks overlap
	inline xint mouse_grabbed	(xint x1,xint y1,xint x2,xint y2)															// grabbed that is reset as soon as the mouse is lifted for things like scroll bars
	{
		static xint			crt_code=0;	// local to keep this object tight
		if(!now_dragging)	crt_code=0;	// reset at end of frame

		if(mouse_down(x1,y1,
					  x2,y2))crt_code= (y1*XRES_priv+x1) ;
		return			    (crt_code==(y1*XRES_priv+x1));
	}

	inline xint mouse_up		(xint x1,xint y1,xint x2,xint y2)
	{
		static xint xdn=0;	if(mouse_clicked)xdn=mx;
		static xint ydn=0;	if(mouse_clicked)ydn=my;
		static xint xup=0;	if(mouse_lifted	)xup=mx;
		static xint yup=0;	if(mouse_lifted	)yup=my;

		if(mouse_lifted)
		if(xdn==xup)
		if(ydn==yup)
		if(fltbox(mx,my,x1,y1,x2-1,y2-1))	// only ret true if we clicked without dragging! needed to place RPs on the map for example
			return xtrue;

		return xfals;
	}

//ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
// TARGET TEXT ENTRY
//ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
	xint key_target_code;	// key-entry, grabbed until we hit enter key or click to a new menu or something
	xchr key_target_char;	// key-entry

	inline xint get_target(xflt cx1,xflt cy1,xflt cx2,xflt cy2,xint* just_sel_this_click)
	{
		*just_sel_this_click=0;

		xint this_code=intround(cy1*XRES_priv+cx1);

		if(mouse_down(cx1,cy1,cx2,cy2))		// clicked IN the box? ok text entry will go to this one
		{
			*just_sel_this_click=(key_target_code!=this_code);
								  key_target_code =this_code ;
		}

		if(key_target_code==this_code)		// clicked AWAY from this selected box? go to no-code
		if(mouse_clicked && !mouse_down(cx1,cy1,cx2,cy2))
			key_target_code=0;

		if(	key_target_char==13)			// hit enter? go to no-code
			key_target_code=0;

		return (this_code==key_target_code);
	}

	inline void edit_target(string* s)
	{
			 if(key_target_char==  0){}		// no key hit this frame... char used up
		else if(key_target_char== 13){}		// carriage-return... don't build that into our names... get_target will set away when we hit this
		else if(key_target_char==127)		// delete a char
		{
			if(	s->size()>0)
				s->erase(
				s->size()-1,1);
		}
		else
		{
			*s=*s+key_target_char;
		}
	}

//ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
// PER-FRAME PROC
//ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
	inline void mnw_per_frame()
	{
		if(mouse_clicked)now_dragging=xtrue;
		if(mouse_lifted )now_dragging=xfals;

		drag_x=0;
		drag_y=0;

		static xflt ox=0.0;
		static xflt oy=0.0;

		if(!mouse_clicked)
		if( now_dragging )
		{
			drag_x=mx-ox;
			drag_y=my-oy;
		}

		ox=mx;
		oy=my;	
	}
};
extern class mnw_class mnw;

#endif
