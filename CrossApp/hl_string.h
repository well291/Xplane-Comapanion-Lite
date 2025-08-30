#ifndef hl_string_h
#define hl_string_h

#include <stdarg.h>
#include <stdio.h>

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
#define  _10_ setw( 1)<<setprecision(0)
#define  _62_ setw( 6)<<setprecision(2)
#define  _52_ setw( 5)<<setprecision(2)
#define  _42_ setw( 4)<<setprecision(2)
#define  _72_ setw( 7)<<setprecision(2)
#define  _53_ setw( 5)<<setprecision(3)
#define  _51_ setw( 5)<<setprecision(1)
#define  _50_ setw( 5)<<setprecision(0)
#define  _20_ setw( 2)<<setprecision(0)
#define  _30_ setw( 3)<<setprecision(0)
#define  _32_ setw( 3)<<setprecision(2)
#define  _40_ setw( 4)<<setprecision(0)
#define  _60_ setw( 6)<<setprecision(0)
#define  _62_ setw( 6)<<setprecision(2)
#define  _64_ setw( 6)<<setprecision(4)
#define  _76_ setw( 7)<<setprecision(6)
#define  _82_ setw( 8)<<setprecision(2)
#define  _80_ setw( 8)<<setprecision(0)
#define  _94_ setw( 9)<<setprecision(4)
#define  _84_ setw( 8)<<setprecision(4)
#define _126_ setw(12)<<setprecision(6)
#define _168_ setw(16)<<setprecision(8)
#define _162_ setw(16)<<setprecision(2)
#define _400_ setw(40)<<setprecision(0)

enum enum_str_format
{
	str_show_with_commas=0	,
	str_lead_with_spaces	,
	str_lead_with_zeros		,
	str_lead_with_pos_sign	,
	str_just_the_numbers
};

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
#define dirchar		'/'
#define dirstring	"/"

inline xint is_a_bad_dir_char	(const xchr& c		){return (c=='\\' || c==':');}
inline xint is_a_dir_char		(const xchr& c		){return (c==dirchar);}
inline xchr chr_is_int			(const xchr& input	){return (input>='0' && input<='9');}
inline xchr int_to_chr			(const xint	 input	){if(input<0  || input>9 )return '0';	return '0'+input;}

inline xint chr_to_int(const xchr& input){
	if(input=='+'		 )return  1;			// this is used for first dig
	if(input=='-'		 )return -1;			// of lat/lon env filename
	if(!chr_is_int(input))return  0;
						  return  input-'0';}

inline xflt chr_to_flt(const xchr& input){
	if(!chr_is_int(input))return  0;
						  return  input-'0';}

inline xint strpow10(xint i)	// a little faster than pow
{								// SORT OF A REDEC OF THE MATH.H FUN SINCE MATH.H NOT INCLUDED YET!
	xint			ret = 1;	// when do we get negative or zero values? airspeed at zero, rpm at negative as the prop kicks back, stuff like that!
	while(i>0){i--;	ret*=10;}	// windows machines cannot handle negative input! so i handle it this way... this is a physics sim after all.
	return			ret	   ;	// windows machines cannot handle 0 input! amazing and strange but true!
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline void s_eq_s(string s1,const string s2)	// CONST arg up front means we cannot change the source var, messing it up my mistake.
{
	s1=s2;
}

inline void C_eq_C(xchr* c1,const xchr* c2,const xint c1_dim)	// CONST arg up front means we cannot change the source var, messing it up my mistake.
{
	if(c1 && c2)
	{
		strncpy(c1,c2,c1_dim);
	}
	else
	{
		if(!c1)APP_alert("Trying to copy TO a null sring!","","","");

		if(	c1 )
			c1[0]=0;
	}
}

inline void app_C(xchr* c1,const xchr* c2,const xint c1_dim)	// CONST arg up front means we cannot change the source var, messing it up my mistake.
{
	if(c1 && c2 && strlen(c1)+strlen(c2) < c1_dim)
	{
		strcat(c1,c2);
	}
	else if(c1)
	{
		c1[0]=0;
	}
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline xint check_ext(string s1,string s2)
{
	if(			s1.size()<4							)	return 0;
	if(			s2.size()<4							)	return 0;
	if(s1[(xint)s1.size()-4]!=s2[(xint)s2.size()-4]	)	return 0;
	if(s1[(xint)s1.size()-3]!=s2[(xint)s2.size()-3]	)	return 0;
	if(s1[(xint)s1.size()-2]!=s2[(xint)s2.size()-2]	)	return 0;
	if(s1[(xint)s1.size()-1]!=s2[(xint)s2.size()-1]	)	return 0;
														return 1;
}

inline void nuke_ext(string& s)
{
	if(	s.size()>4)
		s.resize(
		s.size()-4);
}

inline xint find_string(string s,string find)
{
	return (s.find(find)!=s.npos);
}

inline xint find_string_no_case(const xchr* str,const xchr* pat)
{
	if(str==NULL || *str==0)return xfals;
	if(pat==NULL || *pat==0)return xfals;

	const xint len=(xint)strlen(pat);
	while(*str)
	{
		if(strncasecmp(str,pat,len)==0)return xtrue;
		str++;
	}

	return xfals;
}

inline void mod_string(string& s,string find,string replace)
{
	size_t found=s.find(find);
	if(found!=s.npos)
	{
		s.erase(s.begin()+found,s.begin()+found+find.size());
		s.insert(found,replace);
	}
}

inline void half_name(const string s,string& s1,string& s2)
{
	s1 = s	;
	s2 = " ";

	xint c_ctr =  0;
	xint c_del = 40;

	for(xint c=0;c<s.size();c++)
	if(s[c]==' ')
	{
		const xint del = abs(c-(xint)s.size()/2);

		if(del<c_del)
		{
			c_del = del;
			c_ctr = c  ;
		}
	}

	if(c_ctr)
	{
		s1 = s;		s1.erase(			s1.begin()+c_ctr,s1.end()	);
		s2 = s;		s2.erase(s2.begin(),s2.begin()+c_ctr+1			);
	}
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline xchr str_is_int(const string& str)		// the & means that we will not COPY the string at all, but simply use it itself
{
	if(str.size()==0)return xfals;				// null string is no number at all! remember we access string [0] in the str_to_int call if we determine this to be a string!
	for(xint c=0;c<str.size();c++)
	if(!chr_is_int(str[c]))
	if(str[c]!='-')
	if(str[c]!='+')return xfals;
				   return xtrue;
}

inline xint C_is_flt(const xchr* cs)
{
	if(cs[0]==0)return xfals;	// cannot do strlen on null string

	xint ret=0;

	for(xint c=0;c<strlen(cs);c++)
	if(	(c==0 &&	cs[c]=='+'	)||
		(c==0 &&	cs[c]=='-'	)||
		(			cs[c]=='.'	)||
		(chr_is_int(cs[c])		) )	{ret=1			;}
	else							{return xfals	;}

	return ret;
}

inline xint str_to_int(const string& str)		// the & means that we will not COPY the string at all, but simply use it itself
{
	if(!str_is_int(str))return -1;

	xint			neg= 1;	xint c =0;
	if(str[0]=='+'){neg=+1;		 c+=1;}
	if(str[0]=='-'){neg=-1;		 c+=1;}

	if(str.size()-c==1)return neg*(chr_to_int(str[c+0])*	    1);
	if(str.size()-c==2)return neg*(chr_to_int(str[c+0])*	   10 + chr_to_int(str[c+1])*		1);
	if(str.size()-c==3)return neg*(chr_to_int(str[c+0])*	  100 + chr_to_int(str[c+1])*	   10 + chr_to_int(str[c+2])*	   1);
	if(str.size()-c==4)return neg*(chr_to_int(str[c+0])*	 1000 + chr_to_int(str[c+1])*	  100 + chr_to_int(str[c+2])*     10 + chr_to_int(str[c+3]));
	if(str.size()-c==5)return neg*(chr_to_int(str[c+0])*	10000 + chr_to_int(str[c+1])*	 1000 + chr_to_int(str[c+2])*    100 + chr_to_int(str[c+3])*	10 + chr_to_int(str[c+4]));
	if(str.size()-c==6)return neg*(chr_to_int(str[c+0])*   100000 + chr_to_int(str[c+1])*	10000 + chr_to_int(str[c+2])*   1000 + chr_to_int(str[c+3])*   100 + chr_to_int(str[c+4])*   10 + chr_to_int(str[c+5]));
	if(str.size()-c==7)return neg*(chr_to_int(str[c+0])*  1000000 + chr_to_int(str[c+1])*  100000 + chr_to_int(str[c+2])*  10000 + chr_to_int(str[c+3])*  1000 + chr_to_int(str[c+4])*  100 + chr_to_int(str[c+5])*  10 + chr_to_int(str[c+6]));
	if(str.size()-c==8)return neg*(chr_to_int(str[c+0])* 10000000 + chr_to_int(str[c+1])* 1000000 + chr_to_int(str[c+2])* 100000 + chr_to_int(str[c+3])* 10000 + chr_to_int(str[c+4])* 1000 + chr_to_int(str[c+5])* 100 + chr_to_int(str[c+6])* 10 + chr_to_int(str[c+7]));
	if(str.size()-c==9)return neg*(chr_to_int(str[c+0])*100000000 + chr_to_int(str[c+1])*10000000 + chr_to_int(str[c+2])*1000000 + chr_to_int(str[c+3])*100000 + chr_to_int(str[c+4])*10000 + chr_to_int(str[c+5])*1000 + chr_to_int(str[c+6])*100 + chr_to_int(str[c+7])*10 + chr_to_int(str[c+8]));
	return -1;
}

inline xdob str_to_flt(const string& str)		// the & means that we will not COPY the string at all, but simply use it itself
{
	xint negative		=xfals	;
	xint decimals		=0		;
	xint found_decimal	=0		;
	xdob ret_val		=0.0	;	// this needs to be dob to give the proper lat and lon for the new zealnd fixes for sure

	for(xint c=0;c<str.size();c++)
	{
		if(!found_decimal		)
		if(chr_is_int(str[c])	)
			ret_val*=10.0;

		if(str[c]=='+')negative		=xfals;
		if(str[c]=='-')negative		=xtrue;
		if(str[c]=='.')found_decimal=xtrue;

		if(str[c]=='0'){if(found_decimal)decimals++;	ret_val+=0.0/strpow10(decimals);}
		if(str[c]=='1'){if(found_decimal)decimals++;	ret_val+=1.0/strpow10(decimals);}
		if(str[c]=='2'){if(found_decimal)decimals++;	ret_val+=2.0/strpow10(decimals);}
		if(str[c]=='3'){if(found_decimal)decimals++;	ret_val+=3.0/strpow10(decimals);}
		if(str[c]=='4'){if(found_decimal)decimals++;	ret_val+=4.0/strpow10(decimals);}
		if(str[c]=='5'){if(found_decimal)decimals++;	ret_val+=5.0/strpow10(decimals);}
		if(str[c]=='6'){if(found_decimal)decimals++;	ret_val+=6.0/strpow10(decimals);}
		if(str[c]=='7'){if(found_decimal)decimals++;	ret_val+=7.0/strpow10(decimals);}
		if(str[c]=='8'){if(found_decimal)decimals++;	ret_val+=8.0/strpow10(decimals);}
		if(str[c]=='9'){if(found_decimal)decimals++;	ret_val+=9.0/strpow10(decimals);}
	}

	return ret_val*((negative)?-1.0:1.0);
}

inline void int_to_str_internal(long input,xchr* str,const xint min_len=1,const enum_str_format format=str_show_with_commas)	// we use long not int to handle flite-plan serial numbers, which are long
{
	xint current=0		;
	xint neg=(input<0)	;
	if  (neg) input*=-1	;

	const xint show_commas	= (format==str_show_with_commas	|| format==str_lead_with_pos_sign	);
	const xint show_zeros	= (format==str_lead_with_zeros	|| format==str_lead_with_pos_sign	);
	const xint show_sign	= (format==str_lead_with_pos_sign									);

	if(				 neg			){str[current]='-';										current++;}
	if(show_sign && !neg			){str[current]='+';										current++;}
	if(input>999999999 || min_len>9	){str[current]=int_to_chr((xint)(input/1000000000)%10);	current++;	if(show_commas && input>999999999){str[current]=',';	current++;}}
	if(input> 99999999 || min_len>8	){str[current]=int_to_chr((xint)(input/100000000)%10);	current++;}
	if(input>  9999999 || min_len>7	){str[current]=int_to_chr((xint)(input/10000000)%10);	current++;}
	if(input>	999999 || min_len>6	){str[current]=int_to_chr((xint)(input/1000000)%10);	current++;	if(show_commas && input>   999999){str[current]=',';	current++;}}
	if(input>	 99999 || min_len>5	){str[current]=int_to_chr((xint)(input/100000)%10);		current++;}
	if(input>	  9999 || min_len>4	){str[current]=int_to_chr((xint)(input/10000)%10);		current++;}
	if(input>	   999 || min_len>3	){str[current]=int_to_chr((xint)(input/1000)%10);		current++;	if(show_commas && input>	  999){str[current]=',';	current++;}}
	if(input>		99 || min_len>2	){str[current]=int_to_chr((xint)(input/100)%10);		current++;}
	if(input>		 9 || min_len>1	){str[current]=int_to_chr((xint)(input/10)%10);			current++;}
									 {str[current]=int_to_chr((xint)(input/1)%10);			current++;}
									  str[current]=0;
	if(!show_zeros)
	for(xint i=0;i<(xint)strlen(str)-1;i++){		// strlen(str)-1 so we still get ONE zero!
		if(str[i]!='0' && str[i]!='-')break;		// ok we got to the number itself
		if(str[i]=='0'				 )str[i]=' ';}	// nuke the leading zeros and replace with SPACES.

	if(neg)											// now walk the - back to the right as much as we can
	for(xint i=1;i<(xint)strlen(str)-1;i++)
	if(	str[i  ]==' '){
		str[i  ]= '-';
		str[i-1]= ' ';}
}

inline void dob_to_str_internal(const xdob input,xchr* num_str,xint dig_and_dec,xint dec,const enum_str_format format_type=str_show_with_commas)
{
	if(dec==0)
	{
		int_to_str_internal(round(input),num_str,dig_and_dec,format_type);	// round the input rather than floor it so that 599.99999 shows up as 600, like we prolly want!
	}
	else
	{
		long p10  =pow(10,dec);				// WE NEED A LONG TO HANDLE LAT AND LON WITH TONS OF DECIMAL PLACES
		long dnum =round(input*(xdob)p10);	// 17.988 displayed 3.1 now goes to dnum = 180
		long inum =dnum/p10;				// inum is now 18
			 dnum-=inum*p10;				// dnum is now 0, so 17.988 gets displayed as 18.0, like we want.

										int_to_str_internal(abs(inum),	num_str,dig_and_dec-dec,format_type);	// do NOT try to build the sign in here... -0.001 will appear as 0.001 since the integer part is NOT NEGATIVE!
		xint sl=(xint)strlen(num_str);									num_str[sl]='.';						// use strlen here to find the end of the string, cause the length of the string is unknown coming back from the int_to_str call, since the sign, extra digits, etc could make it who-knows-what size.
										int_to_str_internal(abs(dnum),	num_str+sl+1,dec,str_lead_with_zeros);	// need the leading 0's for sure!
		if(input<0.0)
		{
			if(format_type != str_lead_with_pos_sign)	// if we have to ADD a neg sign, as additional string len,
			{											// then make the sting one unit longer, pushing everything back 1 to the right
				xint sl=(xint)strlen(num_str);
				for(xint i=sl+1;i>0;i--)
					num_str[i]=num_str[i-1];
			}
			num_str[0]='-';

			for(xint i=1;i<(xint)strlen(num_str)-1;i++)	// then walk the - back to the right thru any spaces
			if(	num_str[i  ]==' '){
				num_str[i  ]= '-';
				num_str[i-1]= ' ';}
		}
	}
}

inline string int_to_str(xint input,const xint min_len=1,const enum_str_format format_type=str_show_with_commas)
{
	xchr						c_str[str_dim]				;	// do not make anything here static or it will not be thread-safe
	int_to_str_internal(input,	c_str,min_len,format_type)	;
	string s=					c_str						;
	return s												;
}

inline string lon_to_str(long input,const xint min_len=1,const enum_str_format format_type=str_show_with_commas)
{
	xchr						c_str[str_dim]				;	// do not make anything here static or it will not be thread-safe
	int_to_str_internal(input,	c_str,min_len,format_type)	;
	string s=					c_str						;
	return s												;
}

inline string flt_to_str(const xflt input,xint dig_and_dec=1,xint dec=0,const enum_str_format format_type=str_show_with_commas)
{
	xchr						c_str[str_dim]						;	// do not make anything here static or it will not be thread-safe
	dob_to_str_internal(input,	c_str,dig_and_dec,dec,format_type)	;
	string s=					c_str								;
	return s														;
}

inline string dob_to_str(const xdob input,xint dig_and_dec=1,xint dec=0,const enum_str_format format_type=str_show_with_commas)
{
	xchr						c_str[str_dim]						;	// do not make anything here static or it will not be thread-safe
	dob_to_str_internal(input,	c_str,dig_and_dec,dec,format_type)	;
	string s=					c_str								;
	return s														;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline string get_file_with_path(const string& path)			// the & means that we will not COPY the string at all, but simply use it itself
{
	xint  c=(xint)path.size()-1;								// start at the very trailing edge of the string
	if   (c>1 &&  is_a_dir_char(path[c]))c-=1;					// skip any DIRCHAR at the very end if we already have a path but just want a shorter path!
	while(c>0 && !is_a_dir_char(path[c]))c-=1;					// find the last DIRCHAR

	string							file;
		 if(c<0					  )	file.erase(0,file.length());// send in a NULL? get back a NULL!
	else if(is_a_dir_char(path[c]))	file=path.substr(c+1);		// we found a DIRCHAR... base our string on everything AFTER it
	else							file=path.substr(c  );		// we did NOT find a dirchar! whatever we found IS the file... it sits in no directory.
	return							file;
}

inline string get_path_with_path(const string& full_path)				// the & means that we will not COPY the string at all, but simply use it itself
{
	string file=get_file_with_path(	full_path)						;	// SO WE FIND THE FILE, TAKE IT OUT OF THE PATH, AND THAT MUST BE WHAT IS LEFT!
	string	parent_path=			full_path						;
			parent_path.erase(		full_path.size()-file.size())	;
	return	parent_path;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline xint date_str_to_exp_int(const string s)
{
	if(s.size()>9)
	{
		return	10000000*chr_to_int(s[0])+
				 1000000*chr_to_int(s[1])+
				  100000*chr_to_int(s[2])+
				   10000*chr_to_int(s[3])+
					1000*chr_to_int(s[5])+
					 100*chr_to_int(s[6])+
					  10*chr_to_int(s[8])+
					   1*chr_to_int(s[9]);
	}
	return 0;
}

inline xint time_str_to_exp_int(const string s)
{
	if(s.size()==4)
	{
		return 1000*chr_to_int(s[0])+
				100*chr_to_int(s[1])+
				 10*chr_to_int(s[2])+
				  1*chr_to_int(s[3]);
	}
	return 0;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline string nrt_str_hours(const xflt fhours)
{
	xflt fsec	=fhours*3600.0;
	xint isec	=fsec;
	xint hours	=isec/3600;		isec-=hours*3600;
	xint mins	=isec/  60;		isec-=mins *  60;

	return int_to_str(hours,1,str_show_with_commas)+":"+int_to_str(mins,2,str_lead_with_zeros);
}

inline string nrt_str_sec(xint secs,const xint draw_seconds)
{
	xint neg=	 (secs<0);
		 secs=abs(secs	);
	xint hrs=secs/3600		;
	xint min=secs/60-hrs*60	;
	xint sec=secs%60		;
	if(	hrs>99){
		hrs=99;
		min=59;
		sec=59;}

	string label_h = int_to_str(hrs							);
	string label_m = int_to_str(min,2,str_lead_with_zeros	);
	string label_s = int_to_str(sec,2,str_lead_with_zeros	);

	if(neg)			  label_h="-"+label_h;	// we not not want to show neg signs for the hrs, mins, and secs. just a single neg sign is needed in front of the hours
	string			s=label_h+":"+label_m;
	if(draw_seconds)s+=		  ":"+label_s;
	return s;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline const string month_name(const xint m)	// m is 0->11!!!
{
	string   s		;
	if(m== 0)s="Jan";
	if(m== 1)s="Feb";
	if(m== 2)s="Mar";
	if(m== 3)s="Apr";
	if(m== 4)s="May";
	if(m== 5)s="Jun";
	if(m== 6)s="Jul";
	if(m== 7)s="Aug";
	if(m== 8)s="Sep";
	if(m== 9)s="Oct";
	if(m==10)s="Nov";
	if(m==11)s="Dec";
	if(m==12)s="Jan";	// 12 wraps right back around to 0 on the date-slider in the time window
	return   s		;
}

inline string int_to_date_str(xint d)
{
	xint day	=d%100;	d/=100;
	xint month	=d%100;	d/=100;
	xint year	=d;

	string	year_str=int_to_str(year,4,str_lead_with_zeros);
	string month_str;
	string	 day_str=int_to_str(day,2,str_lead_with_zeros);

	if(month== 1)month_str="Jan";	// keep these tite so they fit on the data currency page on the phone!
	if(month== 2)month_str="Feb";	// keep these tite so they fit on the data currency page on the phone!
	if(month== 3)month_str="Mar";	// keep these tite so they fit on the data currency page on the phone!
	if(month== 4)month_str="Apr";	// keep these tite so they fit on the data currency page on the phone!
	if(month== 5)month_str="May";	// keep these tite so they fit on the data currency page on the phone!
	if(month== 6)month_str="Jun";	// keep these tite so they fit on the data currency page on the phone!
	if(month== 7)month_str="Jul";	// keep these tite so they fit on the data currency page on the phone!
	if(month== 8)month_str="Aug";	// keep these tite so they fit on the data currency page on the phone!
	if(month== 9)month_str="Sep";	// keep these tite so they fit on the data currency page on the phone!
	if(month==10)month_str="Oct";	// keep these tite so they fit on the data currency page on the phone!
	if(month==11)month_str="Nov";	// keep these tite so they fit on the data currency page on the phone!
	if(month==12)month_str="Dec";	// keep these tite so they fit on the data currency page on the phone!

	string	ret;
			ret =month_str	+" "	;
			ret+=day_str	+", "	;
			ret+=year_str			;
	 return ret						;
}

inline void timet_to_date_str(string* s,time_t* time_t_in,const xint include_day=xtrue)
{
	struct tm* t=localtime(time_t_in);	// turn this to local time.

	string	year_str=int_to_str(1900+t->tm_year,4,str_lead_with_zeros);
	string month_str;
	string	 day_str=int_to_str(	 t->tm_mday,2,str_lead_with_zeros);

	if(t->tm_mon== 0)month_str="January"	;
	if(t->tm_mon== 1)month_str="February"	;
	if(t->tm_mon== 2)month_str="March"		;
	if(t->tm_mon== 3)month_str="April"		;
	if(t->tm_mon== 4)month_str="May"		;
	if(t->tm_mon== 5)month_str="June"		;
	if(t->tm_mon== 6)month_str="July"		;
	if(t->tm_mon== 7)month_str="August"		;
	if(t->tm_mon== 8)month_str="September"	;
	if(t->tm_mon== 9)month_str="October"	;
	if(t->tm_mon==10)month_str="November"	;
	if(t->tm_mon==11)month_str="December"	;

					*s=  month_str+" "		;
	if(include_day)	*s=*s+ day_str+", "		;
					*s=*s+year_str			;
}

inline string timet_to_day_date_str(time_t var)
{
	struct tm* tm_local=localtime(&var);

	string s;
	if(tm_local->tm_wday==0)s="Sunday"		;
	if(tm_local->tm_wday==1)s="Monday"		;
	if(tm_local->tm_wday==2)s="Tuesday"		;
	if(tm_local->tm_wday==3)s="Wednesday"	;
	if(tm_local->tm_wday==4)s="Thursday"	;
	if(tm_local->tm_wday==5)s="Friday"		;
	if(tm_local->tm_wday==6)s="Saturday"	;
							s+=", "			;
	if(tm_local->tm_mon== 0)s+="January"	;
	if(tm_local->tm_mon== 1)s+="February"	;
	if(tm_local->tm_mon== 2)s+="March"		;
	if(tm_local->tm_mon== 3)s+="April"		;
	if(tm_local->tm_mon== 4)s+="May"		;
	if(tm_local->tm_mon== 5)s+="June"		;
	if(tm_local->tm_mon== 6)s+="July"		;
	if(tm_local->tm_mon== 7)s+="August"		;
	if(tm_local->tm_mon== 8)s+="September"	;
	if(tm_local->tm_mon== 9)s+="October"	;
	if(tm_local->tm_mon==10)s+="November"	;
	if(tm_local->tm_mon==11)s+="December"	;
							s+=" "			;	s+=int_to_str(tm_local->tm_mday		,1,str_show_with_commas);
							s+=", "			;	s+=int_to_str(tm_local->tm_year+1900,1,str_show_with_commas);
	return s;
}

inline string timet_to_number_date_str(time_t var)
{
	struct tm*					local_t=localtime(&var);								// turn this to local time.
	string var_str	=int_to_str(local_t->tm_mon	+1		,1,str_lead_with_zeros)+"-"		// months since January [0-11]
					+int_to_str(local_t->tm_mday		,1,str_lead_with_zeros)+"-"		// day of the month [1-31]
					+int_to_str(local_t->tm_year+1900	,1,str_lead_with_zeros)+", "	// years since 1900
					+int_to_str(local_t->tm_hour		,2,str_lead_with_zeros)+":"		// hours since midnight [0-23]
					+int_to_str(local_t->tm_min			,2,str_lead_with_zeros);		// minutes after the hour [0-59]
	return var_str;
}

inline string timet_to_megatime(const struct tm* time_ptr)
{
	const string s	=int_to_str(time_ptr->tm_year+1900	,4,str_lead_with_zeros)
					+int_to_str(time_ptr->tm_mon +   1	,2,str_lead_with_zeros)
					+int_to_str(time_ptr->tm_mday		,2,str_lead_with_zeros)+"_"
					+int_to_str(time_ptr->tm_hour		,2,str_lead_with_zeros)
					+int_to_str(time_ptr->tm_min		,2,str_lead_with_zeros);

	return s;
}

inline string timet_to_time_str(time_t var)
{
	string		ret		=timet_to_day_date_str	( var);
	struct tm*	local_t	=localtime  			(&var);					// turn this to local time.

	ret	+=											  			" at "
		+int_to_str(local_t->tm_hour,2,str_show_with_commas)+	":"		// hours since midnight [0-23]
		+int_to_str(local_t->tm_min	,2,str_show_with_commas);			// minutes after the hour [0-59]

	return ret;
}

inline xint scan_number(string& str,const xint digits_in_number)
{
	xint ret			=0;
	xint digits_we_found=0;

	while((xint)str.size()>0)
	{
		xchr c=str[0];
		str.erase(str.begin(),str.begin()+1);

		if(chr_is_int(c))
		{
			ret=(10*ret)+chr_to_int(c);

				digits_we_found++;
			if(	digits_we_found==digits_in_number)break;
		}
		else if(digits_we_found>0)
		{
			break;
		}
	}

	return ret;
}

inline time_t date_time_str_to_timet(string s)
{
	time_t						   time_t_local=time(NULL)	;
	struct tm* tm_local=localtime(&time_t_local)			;

	tm_local->tm_mon	=scan_number(s,2)-1		;
	tm_local->tm_mday	=scan_number(s,2)		;
	tm_local->tm_year	=scan_number(s,4)-1900	;
	tm_local->tm_hour	=scan_number(s,2)		;
	tm_local->tm_min	=scan_number(s,2)		;
	tm_local->tm_sec	=0						;

	time_t							time_t_return=mktime(tm_local);
	struct tm* tm_return=localtime(&time_t_return);

	if(	tm_return->tm_isdst)
		tm_return->tm_hour-=1;

	return mktime(tm_return);
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// SAFE-ENCODE URLS TO BE SENT OVER THE NET WITH SPACES AND STUFF NOT MESSING ANYTHING UP
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
#include <sstream>

inline string url_encode(const string value)
{
	ostringstream	escaped;
					escaped.fill('0');
					escaped << hex;

	for(xint i=0;i<value.size();i++)
	{
		xchr c=value[i];

			 if(isalnum(c) || c=='-' || c=='_' || c=='.' || c=='~'	){escaped << c										;}
		else if(c==' '												){escaped << '+'									;}
		else														 {escaped << '%' << setw(2) << ((int) c) << setw(0)	;}
	}

	return escaped.str();
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// IF I TRY TO TURN A NIL NS-STRING* TO A C-STRING, THIS WILL IPAD-ALERT US RATHER THAN CRASH THE APP!
// USE THIS WHEREVER I SEE A UTF8String TYPE-COERCION TO GET AN IPAD ALERT INSTEAD OF A CRASH IF THE STRING IS NIL!
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
#if __OBJC__
	@class NSString;
	#import <Foundation/Foundation.h>

	inline const char* __ns_to_c_safe(NSString* obj,const char* file,int line)
	{
		const char* cstr=[obj UTF8String]; // may return NULL!

		if(cstr)return cstr;

		// handle the nil NSString* case!
		std::string file_str=			file	;
		std::string line_str=int_to_str(line)	;
		APP_alert("Got a nil string","",file_str,line_str);
		return ""; // return a safe empty string
	}

	#define ns_to_c_safe(obj) __ns_to_c_safe((obj),__FILE__,__LINE__)
#endif
#endif

