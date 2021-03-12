#pragma once

#include <chrono>
#include <windows.h>
#include "Paddle.h"
#include "Ball.h"

#define ENG_DO_NOT_EXCEED_FPS 1

#define ENG_HIT_TOP 1
#define ENG_HIT_BOTTOM 2
#define ENG_HIT_LEFT 4
#define ENG_HIT_RIGHT 8

#define ENG_HIT_PAD_FRONT 1
#define ENG_HIT_PAD_FRONT 2

class Engine {
public:
	Engine(int FPSMax, DWORD options);
	float diffTime(std::chrono::steady_clock::time_point time1, std::chrono::steady_clock::time_point time2);
	float getTimeAdjustedSpeed(float psec);
	void recordTime();
	void wait(unsigned int msec);

	void drawFilledRect(HDC hDC, int x, int y, int cx, int cy, HBRUSH brush);
	void drawFilledCircle(HDC hDC, int x, int y, int radius, HBRUSH brush);

	DWORD hitSide(RECT rc, int x, int y, int dist);

	BOOL isKeyDown(int vKey);

	BOOL within(float val, float rangeMin, float rangeMax);

private:
	std::chrono::steady_clock::time_point curTime;
	std::chrono::steady_clock::time_point timeLast;

	int maxFPS;
	float leastFrameTime;
	BOOL doNotExceedMaxFPS;

	void avoidExceedFrameRate();
};

class PongGameEngine : Engine {
public:
	using Engine::Engine;
	using Engine::diffTime;
	using Engine::getTimeAdjustedSpeed;
	using Engine::recordTime;
	using Engine::wait;

	using Engine::drawFilledRect;
	using Engine::drawFilledCircle;

	void drawPaddle(HDC hDC, Paddle paddle, HBRUSH brush);

	using Engine::hitSide;
	BOOL ballHitPaddle(Paddle paddle, Ball ball);

	using Engine::isKeyDown;

private:
	
};