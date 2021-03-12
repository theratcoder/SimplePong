#pragma once
#include <windows.h>

#define PAD_HORIZONTAL 1
#define PAD_VERTICAL 2

class Paddle {
public:
	unsigned int speed;

	Paddle(float _x, float _y, unsigned int _speed, DWORD options);
	void move(float v);

	float x;
	float y;

	const unsigned int length = 75;
	const unsigned int width = 10;

	BOOL vertical;
	
	RECT rect;
};