#include "hl_types.h"

shar_class shar;

vector <string>	all_label1	;
vector <string>	all_label2	;
vector <xflt>	all_tot_time;
vector <xflt>	all_num_time;

void shar_class::shar_per_frame()
{
	#if TEST_OZ_SHARK
		if(xtim.puff1)
		{
			cout<<"\n=======================================================================\n";
			cout<<"frame-rate="<<_62_<<1.0/xtim.prd_act<<"\n";
			cout<<"=======================================================================\n";

			for(xint i=0;i<all_label1.size();i++)
			{
				const xint ave_time = intround(all_tot_time[i] / all_num_time[i]);
				if		  (ave_time > 10)cout<<_50_<<ave_time<<" :"<<all_label1[i]+" "+all_label2[i]<<"\n";
			}

			all_label1	.clear();
			all_label2	.clear();
			all_tot_time.clear();
			all_num_time.clear();
		}
	#endif
}

void shar_class::shar_per_init(string label1,string label2)
{
	#if TEST_OZ_INIT
		static	xdob		elapsed_time=xtim.time_sec();
		const	xdob fltval=elapsed_time-xtim.time_sec();
							elapsed_time=xtim.time_sec();

		cout<<label1<<" "<<label2<<", dt="<<-fltval<<" sec\n";
	#endif
}

void shar_class::shar_per_check(string label1,string label2)
{
	#if TEST_OZ_SHARK
		if(label1.size()<1)
			APP_alert("shar_per_check no label","","","");

		static xdob				elapsed_time=xtim.time_sec() ;
		xint fltval=-100000.0*(	elapsed_time-xtim.time_sec());
								elapsed_time=xtim.time_sec() ;
		xint index=-1;
		for(xint i=0;i<all_label1.size();i++)
		if(all_label1[i]==label1)
		{
			index=i;
			all_tot_time[i]+=fltval	;
			all_num_time[i]+=1.0	;
			break;
		}

		if(index==-1)
		{
			index		=(xint)all_label1.size();
			all_label1	.push_back(label1);
			all_label2	.push_back(label2);
			all_tot_time.push_back(fltval);
			all_num_time.push_back(1.0   );
		}
	#endif
}
