#ifndef PROFILE_SCOPE_H
#define PROFILE_SCOPE_H

class ProfileScope
{
public:
	ProfileScope(const string &_tag);

	~ProfileScope();

private:
	double start;
	string tag;
};

#define PROFILE(tag) ProfileScope _arfox_profiler_(tag);

#endif
