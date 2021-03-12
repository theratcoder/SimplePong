#include "Paddle.h"

Paddle::Paddle(float _x, float _y, unsigned int _speed, DWORD options) {
	x = _x;
	y = _y;

	speed = _speed;

	vertical = options & PAD_VERTICAL;

	if (vertical) {
		SetRect(&rect, x, y, x + width, y + length);
	}
	else {
		SetRect(&rect, x, y, x + length, y + width);
	}
}

void Paddle::move(float v) {
	if (vertical) y += v;
	else x += v;

	if (vertical) {
		SetRect(&rect, x, y, x + width, y + length);
	}
	else {
		SetRect(&rect, x, y, x + length, y + width);
	}
}