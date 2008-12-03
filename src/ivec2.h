#ifndef _IVEC2_H
#define _IVEC2_H

struct ivec2
{
	ivec2()
	{
		this->x = 0;
		this->y = 0;
	}

	ivec2(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	ivec2(const ivec2 &v)
	{
		this->x = v.x;
		this->y = v.y;
	}

	int x,y;
};

#endif
