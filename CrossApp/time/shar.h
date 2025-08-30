#ifndef shar_h
#define shar_h

class shar_class
{
public:
   ~shar_class(){};
	shar_class(){};

void shar_per_frame();
void shar_per_init	(string label1,string label2=" ");	// my perf-testing: shows every call to test init
void shar_per_check	(string label1,string label2=" ");	// my perf-testing: shows on puff 1 to tradck perf or over time
};
extern shar_class shar;

#endif
