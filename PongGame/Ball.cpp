#include "Ball.h"

const char* BallInvalidBounceDirEx::what() const throw () {
	return "Invalid bounce direction";
}

Ball::Ball(float _x, float _y, float initVX, float initVY) {
	x = _x;
	y = _y;

	vX = initVX;
	vY = initVY;

	dX = 0;
	dY = 0;
}

void Ball::reset(RECT rc) {
	x = rc.right / 2;
	y = rc.bottom / 2;

	resetRect = rc;

	stuckCounter = 0;
}

void Ball::updateVelocity(float _dX, float _dY) {
	dX = _dX;
	dY = _dY;
}

void Ball::bounce(DWORD direction) {
	if (stuckCounter > 5) reset(resetRect);
	else if (stuckCounter > 0) return;

	stuckCounter++;

	if (direction & BALL_VERTICAL && direction & BALL_HORIZONTAL) {
		vX = -vX;
		vY = -vY;
	}
	else if (direction & BALL_VERTICAL) {
		vY = -vY;
	}
	else if (direction & BALL_HORIZONTAL) {
		vX = -vX;
	}
	else {
		throw BallInvalidBounceDirEx();
	}
}

void Ball::move() {
	x += dX;
	y += dY;
}

