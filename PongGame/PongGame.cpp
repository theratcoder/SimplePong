// PongGame.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "PongGame.h"
#include "Engine.h"
#include "Paddle.h"
#include "Ball.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
PongGameEngine engine = PongGameEngine(100, ENG_DO_NOT_EXCEED_FPS);
Ball ball = Ball(400, 200, 0, 0);
Paddle paddle1 = Paddle(75, 300, 125, PAD_VERTICAL);
Paddle paddle2 = Paddle(800, 300, 125, PAD_VERTICAL);

struct scores {
    int p1 = 0;
    int p2 = 0;
    int max = 10;
} scores;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PONGGAME, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PONGGAME));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PONGGAME));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PONGGAME);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 900, 600, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        {
            srand(time(NULL));
            float xPercent = (float)rand() / RAND_MAX;
            float yPercent = 1 - xPercent;

            float ballSpeedx = (100 * xPercent) + 50;
            float ballSpeedy = (100 * yPercent) + 50;

            ball.vX = ballSpeedx;
            ball.vY = ballSpeedy;
        }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            BOOL mvRight1 = FALSE;
            BOOL mvLeft1 = FALSE;
            BOOL mvRight2 = FALSE;
            BOOL mvLeft2 = FALSE;

            int stuck = 0;

            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            HDC hdcMem = CreateCompatibleDC(hdc);

            RECT rcClient;
            GetClientRect(hWnd, &rcClient);

            HBITMAP hbmMem = CreateCompatibleBitmap(hdc, rcClient.right, rcClient.bottom);

            HANDLE hOld = SelectObject(hdcMem, hbmMem);
            
            if (paddle1.vertical) {
                paddle1.x = 75;
                paddle2.x = rcClient.right - 95;
            }
            else {
                paddle1.y = 75;
                paddle2.y = rcClient.right - 95;
            }

            // for vertical paddles, left = up and right = down
            BOOL goneTooFarLeft1 = paddle1.vertical ? paddle1.y < 0 : paddle1.x < 0;
            BOOL goneTooFarRight1 = paddle1.vertical ? paddle1.y > rcClient.bottom - paddle1.length : paddle1.x > rcClient.right - paddle1.length;

            BOOL goneTooFarLeft2 = paddle2.vertical ? paddle2.y < 0 : paddle2.x < 0;
            BOOL goneTooFarRight2 = paddle2.vertical ? paddle2.y > rcClient.bottom - paddle2.length : paddle2.x > rcClient.right - paddle2.length;

            if (engine.isKeyDown((int)'A') && !goneTooFarLeft1) {
                paddle1.move(engine.getTimeAdjustedSpeed(paddle1.speed));
                mvLeft1 = TRUE;
            }
            else if (engine.isKeyDown((int)'D') && !goneTooFarRight1) {
                paddle1.move(-engine.getTimeAdjustedSpeed(paddle1.speed));
                mvRight1 = TRUE;
            }

            if (engine.isKeyDown(VK_LEFT) && !goneTooFarLeft2) {
                paddle2.move(engine.getTimeAdjustedSpeed(paddle2.speed));
                mvLeft2 = TRUE;
            }
            else if (engine.isKeyDown(VK_RIGHT) && !goneTooFarRight2) {
                paddle2.move(-engine.getTimeAdjustedSpeed(paddle2.speed));
                mvRight2 = TRUE;
            }

            SetDCBrushColor(hdcMem, RGB(0, 0, 255));
            engine.drawPaddle(hdcMem, paddle1, (HBRUSH)GetStockObject(DC_BRUSH));

            SetDCBrushColor(hdcMem, RGB(0, 255, 0));
            engine.drawPaddle(hdcMem, paddle2, (HBRUSH)GetStockObject(DC_BRUSH));

            DWORD hits = engine.hitSide(rcClient, ball.x, ball.y, ball.size);

            BOOL vertical = hits & ENG_HIT_TOP || hits & ENG_HIT_BOTTOM;
            BOOL horizontal = hits & ENG_HIT_LEFT || hits & ENG_HIT_RIGHT;

            if (vertical && horizontal) {
                ball.bounce(BALL_VERTICAL | BALL_HORIZONTAL);
            }
            else if (vertical) {
                if (paddle1.vertical) {
                    ball.bounce(BALL_VERTICAL);
                }
                else {
                    if (hits & ENG_HIT_TOP)
                        scores.p2++;
                    else
                        scores.p1++;
                    ball.reset(rcClient);
                }
            }
            else if (horizontal) {
                if (!paddle1.vertical) {
                    ball.bounce(BALL_HORIZONTAL);
                }
                else {
                    if (hits & ENG_HIT_LEFT)
                        scores.p2++;
                    else
                        scores.p1++;

                    ball.reset(rcClient);
                }
            }
            else {
                stuck++;
            }

            DWORD padHits1 = engine.ballHitPaddle(paddle1, ball);

            if (engine.ballHitPaddle(paddle1, ball) && ball.stuckCounter < 2) {
                ball.vX += 20;
                ball.vY += 20;
                paddle1.speed += 5;
                paddle2.speed += 5;

                if (paddle1.vertical) {
                    if (mvLeft1) {
                        ball.vY += 5;
                    }
                    else if (mvRight1) {
                        ball.vY -= 5;
                    }
                    ball.bounce(BALL_HORIZONTAL);
                }
                else {
                    if (mvLeft1) {
                        ball.vX += 5;
                    }
                    else if (mvRight1) {
                        ball.vX -= 5;
                    }
                    ball.bounce(BALL_VERTICAL);
                }
            }
            else if (engine.ballHitPaddle(paddle2, ball) && ball.stuckCounter < 2) {
                ball.vX += 20;
                ball.vY += 20;
                paddle1.speed += 5;
                paddle2.speed += 5;

                if (paddle2.vertical) {
                    if (mvLeft2) {
                        ball.vY += 5;
                    }
                    else if (mvRight2) {
                        ball.vY -= 5;
                    }
                    ball.bounce(BALL_HORIZONTAL);
                }
                else {
                    if (mvLeft2) {
                        ball.vX += 5;
                    }
                    else if (mvRight2) {
                        ball.vX -= 5;
                    }
                    ball.bounce(BALL_VERTICAL);
                }
            }
            else {
                stuck++;
            }

            if (stuck == 2) {
                ball.stuckCounter = 0;
            }

            // lpszScore = null-terminated, bc it may have variable length later
            LPWSTR lpszScore = (LPWSTR)malloc(sizeof(wchar_t) * 27);
            wsprintfW(lpszScore, L"Player 1: %d   Player 2: %d", scores.p1, scores.p2);

            DrawText(hdcMem, lpszScore, -1, &rcClient, DT_CENTER | DT_TOP);

            if (scores.p1 >= scores.max) {
                MessageBox(hWnd, L"Player 1 (A and D) won!", L"Game Results", MB_OK);
                DestroyWindow(hWnd);
            }
            else if (scores.p2 >= scores.max) {
                MessageBox(hWnd, L"Player 2 (arrow keys) won!", L"Game Results", MB_OK);
                DestroyWindow(hWnd);
            }
            
            engine.recordTime();
            ball.updateVelocity(engine.getTimeAdjustedSpeed(ball.vX), engine.getTimeAdjustedSpeed(ball.vY));
            ball.move();

            SetDCBrushColor(hdcMem, RGB(255, 0, 0));
            engine.drawFilledCircle(hdcMem, ball.x, ball.y, ball.size, (HBRUSH)GetStockObject(DC_BRUSH));

            // Transfer the off-screen DC to the screen
            BitBlt(hdc, 0, 0, rcClient.right, rcClient.bottom, hdcMem, 0, 0, SRCCOPY);

            // Free-up the off-screen DC
            SelectObject(hdcMem, hOld);

            DeleteObject(hbmMem);
            DeleteDC(hdcMem);

            EndPaint(hWnd, &ps);

            // free lpszScore to prevent leak
            free(lpszScore);

            InvalidateRect(hWnd, NULL, FALSE);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}