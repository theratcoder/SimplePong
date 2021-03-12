#pragma once

#include <windows.h>
#include <exception>

#define BALL_VERTICAL 1
#define BALL_HORIZONTAL 2

struct BallInvalidBounceDirEx : public std::exception {
	const char* what() const throw ();
};

class Ball {
public:	
	float x;
	float y;
	const unsigned int size = 10;

	float vX;
	float vY;

	BOOL stuckCounter = 0;
	
	Ball(float _x, float _y, float initVX, float initVY);
	void bounce(DWORD direction);
	void updateVelocity(float _vX, float _vY);
	void move();

	void reset(RECT rc);

private:
	float dX;
	float dY;

	RECT resetRect;
};