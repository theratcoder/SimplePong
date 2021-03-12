#include "Engine.h"

Engine::Engine(int FPSMax, DWORD options) {
	curTime = std::chrono::steady_clock::now();
	timeLast = std::chrono::steady_clock::now();

	doNotExceedMaxFPS = options & ENG_DO_NOT_EXCEED_FPS;
	
	if (doNotExceedMaxFPS) {
		maxFPS = FPSMax;
		leastFrameTime = (1 / maxFPS) * 1000;
	}
	else {
		maxFPS = NULL;
		leastFrameTime = NULL;
	}
}

void Engine::wait(unsigned int msec) {
	Sleep(msec);
}

void Engine::avoidExceedFrameRate() {
	float frameTime = std::chrono::duration<float, std::milli>(curTime - timeLast).count();
	if (frameTime < leastFrameTime) {
		wait(leastFrameTime - frameTime);
	}
}

float Engine::diffTime(std::chrono::steady_clock::time_point time1, std::chrono::steady_clock::time_point time2) {
	return std::chrono::duration<float, std::milli>(time2 - time1).count();
}

void Engine::recordTime() {
	timeLast = curTime;
	curTime = std::chrono::steady_clock::now();

	if (doNotExceedMaxFPS) avoidExceedFrameRate();
}

float Engine::getTimeAdjustedSpeed(float psec) {
	return (diffTime(curTime, timeLast) / 1000) * psec;
}

void Engine::drawFilledRect(HDC hDC, int x, int y, int cx, int cy, HBRUSH brush) {
	RECT rc;
	SetRect(&rc, x, y, x + cx, y + cy);
	FillRect(hDC, &rc, brush);
}

void Engine::drawFilledCircle(HDC hDC, int x, int y, int radius, HBRUSH brush) {
	SelectObject(hDC, brush);
	
	RECT rc;
	SetRect(&rc, x + radius, y - radius, x - radius, y + radius);

	Ellipse(hDC, rc.left, rc.top, rc.right, rc.bottom);
}

DWORD Engine::hitSide(RECT rc, int x, int y, int dist) {
	BYTE hits[2] = {0, 0};

	if (x < dist) {
		hits[0] = ENG_HIT_LEFT;
	}
	else if (x > rc.right - dist) {
		hits[0] = ENG_HIT_RIGHT;
	}

	if (y < dist) {
		hits[1] = ENG_HIT_TOP;
	}
	else if (y > rc.bottom - dist) {
		hits[1] = ENG_HIT_BOTTOM;
	}

	return hits[0] | hits[1];
}

BOOL Engine::isKeyDown(int vKey) {
	return GetKeyState(vKey) & 0x8000;
}

BOOL Engine::within(float val, float rangeMin, float rangeMax) {
	return val < rangeMax && val > rangeMin;
}

void PongGameEngine::drawPaddle(HDC hDC, Paddle paddle, HBRUSH brush) {
	if (paddle.vertical) drawFilledRect(hDC, paddle.x, paddle.y, paddle.width, paddle.length, brush);
	else drawFilledRect(hDC, paddle.x, paddle.y, paddle.length, paddle.width, brush);
}

BOOL PongGameEngine::ballHitPaddle(Paddle paddle, Ball ball) {
	POINT ptLeft = POINT{ (LONG)(ball.x - ball.size), (LONG)ball.y };
	POINT ptRight = POINT{ (LONG)(ball.x + ball.size), (LONG)ball.y };

	if (PtInRect(&paddle.rect, ptLeft) || PtInRect(&paddle.rect, ptRight)) {
		return TRUE;
	}
	return FALSE;
}