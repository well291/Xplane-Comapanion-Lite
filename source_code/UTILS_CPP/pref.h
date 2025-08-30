#ifndef pref_h
#define pref_h

class pref_class
{
public:
   ~pref_class(){};
	pref_class(){};

	void prefs_txt(const IO_type opr_typ);
	void prefs_bin(const IO_type opr_typ);
};
extern pref_class pref;

#endif
