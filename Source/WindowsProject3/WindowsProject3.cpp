// WindowsProject1.cpp : Defines the entry point for the application.
//


#include "stdafx.h"
#include "WindowsProject3.h"
#include<stdlib.h>
#include<windows.h>
#include<time.h>
#include "Types.h"
#define ID_TIMER  1
#define NUM 200
#define _CRT_SECURE_NO_WARNINGS
#define EXAMPLE_MACRO_NAME


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("MyTerics");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("Program requires Windows NT!"),

			szAppName, MB_ICONERROR);

		return 0;
	}

	hwnd = CreateWindow(szAppName,
		TEXT("MyTerics"),
		WS_OVERLAPPED | WS_SYSMENU | WS_BORDER,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	HBRUSH brush;
	static HWND hwndButton[3];
	static int cxChar, cyChar;
	int i, ID;

	switch (message)
	{
	case WM_CREATE:
		//MessageBox(hwnd,TEXT("Have a good time!"),TEXT("Welcome"),MB_OK);

		cxChar = LOWORD(GetDialogBaseUnits());
		cyChar = HIWORD(GetDialogBaseUnits());
		for (i = 0; i<3; i++)
			
			hwndButton[i] = CreateWindow(TEXT("button"), button[i].szText, WS_CHILD | WS_VISIBLE | button[i].iStyle, cxChar * 55, cyChar*(15 + 5 * i),
				20 * cxChar, 7 * cyChar / 4, hwnd, (HMENU)i, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		break;

	case WM_TIMER:
		hdc = GetDC(hwnd);
		if (Players == 1)
		{
			if (!IsOutOfRegion(CurrentTericsInfo.shape, CurrentTericsInfo.dir,
				CurrentTericsInfo.x, (CurrentTericsInfo.y + 1)))
			{
				++CurrentTericsInfo.y;
				EraseGameRegion(hdc);
				DrawGameRegion(hdc);
				LoadCurrentTerics(hdc);
				DrawTerics(hdc, CurrentTericsInfo.shape, CurrentTericsInfo.dir,
					CurrentTericsInfo.x, CurrentTericsInfo.y);
			}
			else
			{
				SaveStateOfTerics(CurrentTericsInfo.shape, CurrentTericsInfo.dir,
					CurrentTericsInfo.x, CurrentTericsInfo.y);
				InvalidateRect(hwnd, NULL, FALSE);
			}
		}
		else
		{
			if (!IsOutOfRegionPlayer1(CurrentTericsInfo_Player1.shape, CurrentTericsInfo_Player1.dir,
				CurrentTericsInfo_Player1.x, (CurrentTericsInfo_Player1.y + 1)))
			{
				++CurrentTericsInfo_Player1.y;
				EraseGameRegionPlayer1(hdc);
				DrawGameRegionPlayer1(hdc);
				LoadCurrentTericsPlayer1(hdc);
				DrawTericsPlayer1(hdc, CurrentTericsInfo_Player1.shape, CurrentTericsInfo_Player1.dir,
					CurrentTericsInfo_Player1.x, CurrentTericsInfo_Player1.y);
			}
			else
			{
				SaveStateOfTericsPlayer1(CurrentTericsInfo_Player1.shape, CurrentTericsInfo_Player1.dir,
					CurrentTericsInfo_Player1.x, CurrentTericsInfo_Player1.y);
				invalidatePlayer1 = TRUE;
			}
			if (!IsOutOfRegionPlayer2(CurrentTericsInfo_Player2.shape, CurrentTericsInfo_Player2.dir,
				CurrentTericsInfo_Player2.x, (CurrentTericsInfo_Player2.y + 1)))
			{
				++CurrentTericsInfo_Player2.y;
				EraseGameRegionPlayer2(hdc);
				DrawGameRegionPlayer2(hdc);
				LoadCurrentTericsPlayer2(hdc);
				DrawTericsPlayer2(hdc, CurrentTericsInfo_Player2.shape, CurrentTericsInfo_Player2.dir,
					CurrentTericsInfo_Player2.x, CurrentTericsInfo_Player2.y);
			}
			else
			{
				SaveStateOfTericsPlayer2(CurrentTericsInfo_Player2.shape, CurrentTericsInfo_Player2.dir,
					CurrentTericsInfo_Player2.x, CurrentTericsInfo_Player2.y);
				invalidatePlayer2 = TRUE;
			}
			if (invalidatePlayer2 && !invalidatePlayer1)
			{
				InvalidateRect(hwnd, &rect_Player2, FALSE);
			}
			if (invalidatePlayer1 && !invalidatePlayer2)
				InvalidateRect(hwnd, &rect_Player1, FALSE);
			if (invalidatePlayer1 || invalidatePlayer2)
			{
				InvalidateRect(hwnd, &rect_Player1, FALSE);
				InvalidateRect(hwnd, &rect_Player2, FALSE);
			}
		}
		ReleaseDC(hwnd, hdc);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		if (Start == TRUE)
		{
			brush = CreateSolidBrush(RGB(0, 0, 0));
			GetClientRect(hwnd, &rect);
			FillRect(hdc, &rect, brush);
			Start = FALSE;
		}
		else
		{
			if (Players == 1)
			{
				brush = CreateSolidBrush(RGB(0, 180, 255));
				GetClientRect(hwnd, &rect);
				FillRect(hdc, &rect, brush);
				EraseGameRegion(hdc);
				DrawGameRegion(hdc);
				DrawInterface(hdc);
				PrintTetris(hdc);
				InitTericsInfo();
				PrintInfo(hdc, szshape);
				if (level != 0)
				{
					KillTimer(hwnd, ID_TIMER);
					SetTimer(hwnd, ID_TIMER, speed - 100 * level, NULL);

				}
				LoadCurrentTerics(hdc);
				if (!(IsGameOver(CurrentTericsInfo.shape, CurrentTericsInfo.dir,
					CurrentTericsInfo.x, CurrentTericsInfo.y)))
				{
					DrawTerics(hdc, CurrentTericsInfo.shape, CurrentTericsInfo.dir,
						CurrentTericsInfo.x, CurrentTericsInfo.y);
				}
				else
				{
					char szscore[200] = {};
					wsprintf(szscore, "Score: %d.", score);
					KillTimer(hwnd, ID_TIMER);
					MessageBox(hwnd, szscore, TEXT("GameOver. Hit enter"), MB_OK);
				}
			}
			else
			{
				if (FirstDraw == TRUE)
				{
					brush = CreateSolidBrush(RGB(165, 180, 255));
					GetClientRect(hwnd, &rect);
					FillRect(hdc, &rect, brush);
					FirstDraw = FALSE;
				}
				if (invalidatePlayer1)
				{
					brush = CreateSolidBrush(RGB(165, 180, 255));
					FillRect(hdc, &rect_Player1, brush);
					EraseGameRegionPlayer1(hdc);
					DrawGameRegionPlayer1(hdc);
					DrawInterfacePlayer1(hdc);
					PrintTetris(hdc);
					InitTericsInfoPlayer1();
					PrintInfoPlayer1(hdc, szshape_Player1);
					LoadCurrentTericsPlayer1(hdc);
					if (!(IsGameOverPlayer1(CurrentTericsInfo_Player1.shape, CurrentTericsInfo_Player1.dir,
						CurrentTericsInfo_Player1.x, CurrentTericsInfo_Player1.y)))
					{
						DrawTericsPlayer1(hdc, CurrentTericsInfo_Player1.shape, CurrentTericsInfo_Player1.dir,
							CurrentTericsInfo_Player1.x, CurrentTericsInfo_Player1.y);
					}
					else
					{
						KillTimer(hwnd, ID_TIMER);
						MessageBox(hwnd, TEXT("Player2 is the winner! Hit enter"), TEXT("GameOver"), MB_OK);
					}
					invalidatePlayer1 = FALSE;
				}
				if (invalidatePlayer2)
				{
					brush = CreateSolidBrush(RGB(165, 180, 255));
					FillRect(hdc, &rect_Player2, brush);
					EraseGameRegionPlayer2(hdc);
					DrawGameRegionPlayer2(hdc);
					DrawInterfacePlayer2(hdc);
					PrintTetris(hdc);
					InitTericsInfoPlayer2();
					PrintInfoPlayer2(hdc, szshape_Player2);
					LoadCurrentTericsPlayer2(hdc);
					if (!(IsGameOverPlayer2(CurrentTericsInfo_Player2.shape, CurrentTericsInfo_Player2.dir,
						CurrentTericsInfo_Player2.x, CurrentTericsInfo_Player2.y)))
					{
						DrawTericsPlayer2(hdc, CurrentTericsInfo_Player2.shape, CurrentTericsInfo_Player2.dir,
							CurrentTericsInfo_Player2.x, CurrentTericsInfo_Player2.y);
					}
					else
					{
						KillTimer(hwnd, ID_TIMER);
						MessageBox(hwnd, TEXT("Player1 is the winner! Hit enter"), TEXT("GameOver"), MB_OK);
					}
					invalidatePlayer2 = FALSE;
				}
			}
		}
		EndPaint(hwnd, &ps);
		break;


	case WM_KEYDOWN:
		hdc = GetDC(hwnd);
		switch (wParam)
		{
		case VK_UP:
			if (Players == 1)
			{
				if (Stop == FALSE)
				{
					if (!IsOutOfRegion(CurrentTericsInfo.shape, (CurrentTericsInfo.dir + 1) % 4,
						CurrentTericsInfo.x, CurrentTericsInfo.y))
					{
						CurrentTericsInfo.dir = ++CurrentTericsInfo.dir % 4;
						EraseGameRegion(hdc);
						DrawGameRegion(hdc);
						LoadCurrentTerics(hdc);
						DrawTerics(hdc, CurrentTericsInfo.shape, CurrentTericsInfo.dir,
							CurrentTericsInfo.x, CurrentTericsInfo.y);
					}
				}
			}
			else
			{
				if (Stop == FALSE)
				{
					if (!IsOutOfRegionPlayer2(CurrentTericsInfo_Player2.shape, (CurrentTericsInfo_Player2.dir + 1) % 4,
						CurrentTericsInfo_Player2.x, CurrentTericsInfo_Player2.y))
					{
						CurrentTericsInfo_Player2.dir = ++CurrentTericsInfo_Player2.dir % 4;
						EraseGameRegionPlayer2(hdc);
						DrawGameRegionPlayer2(hdc);
						LoadCurrentTericsPlayer2(hdc);
						DrawTericsPlayer2(hdc, CurrentTericsInfo_Player2.shape, CurrentTericsInfo_Player2.dir,
							CurrentTericsInfo_Player2.x, CurrentTericsInfo_Player2.y);
					}
				}
			}
			break;

		case VK_DOWN:
			if (Players == 1)
			{
				if (Stop == FALSE)
				{
					if (!IsOutOfRegion(CurrentTericsInfo.shape, CurrentTericsInfo.dir,
						CurrentTericsInfo.x, (CurrentTericsInfo.y + 1)))
					{
						++CurrentTericsInfo.y;
						EraseGameRegion(hdc);
						DrawGameRegion(hdc);
						LoadCurrentTerics(hdc);
						DrawTerics(hdc, CurrentTericsInfo.shape, CurrentTericsInfo.dir,
							CurrentTericsInfo.x, CurrentTericsInfo.y);
					}
					else
					{
						SaveStateOfTerics(CurrentTericsInfo.shape, CurrentTericsInfo.dir,
							CurrentTericsInfo.x, CurrentTericsInfo.y);
						InvalidateRect(hwnd, NULL, FALSE);
					}
				}
			}
			else
			{
				if (Stop == FALSE)
				{
					if (!IsOutOfRegionPlayer2(CurrentTericsInfo_Player2.shape, CurrentTericsInfo_Player2.dir,
						CurrentTericsInfo_Player2.x, (CurrentTericsInfo_Player2.y + 1)))
					{
						++CurrentTericsInfo_Player2.y;
						EraseGameRegionPlayer2(hdc);
						DrawGameRegionPlayer2(hdc);
						LoadCurrentTericsPlayer2(hdc);
						DrawTericsPlayer2(hdc, CurrentTericsInfo_Player2.shape, CurrentTericsInfo_Player2.dir,
							CurrentTericsInfo_Player2.x, CurrentTericsInfo_Player2.y);
					}
					else
					{
						SaveStateOfTericsPlayer2(CurrentTericsInfo_Player2.shape, CurrentTericsInfo_Player2.dir,
							CurrentTericsInfo_Player2.x, CurrentTericsInfo_Player2.y);
						invalidatePlayer2 = TRUE;
						InvalidateRect(hwnd, &rect_Player2, FALSE);
					}
				}
			}
			break;

		case VK_LEFT:
			if (Players == 1)
			{
				if (Stop == FALSE)
				{
					if (!IsOutOfRegion(CurrentTericsInfo.shape, CurrentTericsInfo.dir,
						(CurrentTericsInfo.x - 1), CurrentTericsInfo.y))
					{
						--CurrentTericsInfo.x;
						EraseGameRegion(hdc);
						DrawGameRegion(hdc);
						LoadCurrentTerics(hdc);
						DrawTerics(hdc, CurrentTericsInfo.shape, CurrentTericsInfo.dir,
							CurrentTericsInfo.x, CurrentTericsInfo.y);
					}
				}
			}
			else
			{
				if (Stop == FALSE)
				{
					if (!IsOutOfRegionPlayer2(CurrentTericsInfo_Player2.shape, CurrentTericsInfo_Player2.dir,
						(CurrentTericsInfo_Player2.x - 1), CurrentTericsInfo_Player2.y))
					{
						--CurrentTericsInfo_Player2.x;
						EraseGameRegionPlayer2(hdc);
						DrawGameRegionPlayer2(hdc);
						LoadCurrentTericsPlayer2(hdc);
						DrawTericsPlayer2(hdc, CurrentTericsInfo_Player2.shape, CurrentTericsInfo_Player2.dir,
							CurrentTericsInfo_Player2.x, CurrentTericsInfo_Player2.y);
					}
				}
			}
			break;

		case VK_RIGHT:
			if (Players == 1)
			{
				if (Stop == FALSE)
				{
					if (!IsOutOfRegion(CurrentTericsInfo.shape, CurrentTericsInfo.dir,
						(CurrentTericsInfo.x + 1), CurrentTericsInfo.y))
					{
						++CurrentTericsInfo.x;
						EraseGameRegion(hdc);
						DrawGameRegion(hdc);
						LoadCurrentTerics(hdc);
						DrawTerics(hdc, CurrentTericsInfo.shape, CurrentTericsInfo.dir,
							CurrentTericsInfo.x, CurrentTericsInfo.y);
					}
				}
			}
			else
			{
				if (Stop == FALSE)
				{
					if (!IsOutOfRegionPlayer2(CurrentTericsInfo_Player2.shape, CurrentTericsInfo_Player2.dir,
						(CurrentTericsInfo_Player2.x + 1), CurrentTericsInfo_Player2.y))
					{
						++CurrentTericsInfo_Player2.x;
						EraseGameRegionPlayer2(hdc);
						DrawGameRegionPlayer2(hdc);
						LoadCurrentTericsPlayer2(hdc);
						DrawTericsPlayer2(hdc, CurrentTericsInfo_Player2.shape, CurrentTericsInfo_Player2.dir,
							CurrentTericsInfo_Player2.x, CurrentTericsInfo_Player2.y);
					}
				}
			}
			break;

		case VK_SPACE:
			if (Stop == FALSE)
			{
				KillTimer(hwnd, ID_TIMER);
				Stop = TRUE;
			}
			else
			{
				SetTimer(hwnd, ID_TIMER, 500, NULL);
				Stop = FALSE;
			}
			break;


		case VK_RETURN:
			// You can only restart on game over
			if ((IsGameOver(CurrentTericsInfo.shape, CurrentTericsInfo.dir,
				CurrentTericsInfo.x, CurrentTericsInfo.y)))
			{
				InvalidateRect(hwnd, NULL, TRUE);
				reset(hwnd,hdc);
			}
			break;
	
		}
		ReleaseDC(hwnd, hdc);
		break;

	case WM_CHAR:
		hdc = GetDC(hwnd);
		switch (wParam)
		{
		case 'w':
			if (Stop == FALSE)
			{
				if (!IsOutOfRegionPlayer1(CurrentTericsInfo_Player1.shape, (CurrentTericsInfo_Player1.dir + 1) % 4,
					CurrentTericsInfo_Player1.x, CurrentTericsInfo_Player1.y))
				{
					CurrentTericsInfo_Player1.dir = ++CurrentTericsInfo_Player1.dir % 4;
					EraseGameRegionPlayer1(hdc);
					DrawGameRegionPlayer1(hdc);
					LoadCurrentTericsPlayer1(hdc);
					DrawTericsPlayer1(hdc, CurrentTericsInfo_Player1.shape, CurrentTericsInfo_Player1.dir,
						CurrentTericsInfo_Player1.x, CurrentTericsInfo_Player1.y);
				}
			}
			break;
		case 's':
			if (Stop == FALSE)
			{
				if (!IsOutOfRegionPlayer1(CurrentTericsInfo_Player1.shape, CurrentTericsInfo_Player1.dir,
					CurrentTericsInfo_Player1.x, (CurrentTericsInfo_Player1.y + 1)))
				{
					++CurrentTericsInfo_Player1.y;
					EraseGameRegionPlayer1(hdc);
					DrawGameRegionPlayer1(hdc);
					LoadCurrentTericsPlayer1(hdc);
					DrawTericsPlayer1(hdc, CurrentTericsInfo_Player1.shape, CurrentTericsInfo_Player1.dir,
						CurrentTericsInfo_Player1.x, CurrentTericsInfo_Player1.y);
				}
				else
				{
					SaveStateOfTericsPlayer1(CurrentTericsInfo_Player1.shape, CurrentTericsInfo_Player1.dir,
						CurrentTericsInfo_Player1.x, CurrentTericsInfo_Player1.y);
					invalidatePlayer1 = TRUE;
					InvalidateRect(hwnd, &rect_Player1, FALSE);
				}
			}
			break;
		case 'a':
			if (Stop == FALSE)
			{
				if (!IsOutOfRegionPlayer1(CurrentTericsInfo_Player1.shape, CurrentTericsInfo_Player1.dir,
					(CurrentTericsInfo_Player1.x - 1), CurrentTericsInfo_Player1.y))
				{
					--CurrentTericsInfo_Player1.x;
					EraseGameRegionPlayer1(hdc);
					DrawGameRegionPlayer1(hdc);
					LoadCurrentTericsPlayer1(hdc);
					DrawTericsPlayer1(hdc, CurrentTericsInfo_Player1.shape, CurrentTericsInfo_Player1.dir,
						CurrentTericsInfo_Player1.x, CurrentTericsInfo_Player1.y);
				}
			}
			break;
		case 'd':
			if (Stop == FALSE)
			{
				if (!IsOutOfRegionPlayer1(CurrentTericsInfo_Player1.shape, CurrentTericsInfo_Player1.dir,
					(CurrentTericsInfo_Player1.x + 1), CurrentTericsInfo_Player1.y))
				{
					++CurrentTericsInfo_Player1.x;
					EraseGameRegionPlayer1(hdc);
					DrawGameRegionPlayer1(hdc);
					LoadCurrentTericsPlayer1(hdc);
					DrawTericsPlayer1(hdc, CurrentTericsInfo_Player1.shape, CurrentTericsInfo_Player1.dir,
						CurrentTericsInfo_Player1.x, CurrentTericsInfo_Player1.y);
				}
			}
			break;
		}



	case WM_COMMAND:
		ID = LOWORD(wParam);
		if (!(IsGameOver(CurrentTericsInfo.shape, CurrentTericsInfo.dir,
			CurrentTericsInfo.x, CurrentTericsInfo.y)))
		{
			if (ID == 0)
			{
				Players = 1;
				InvalidateRect(hwnd, NULL, FALSE);
				SetTimer(hwnd, ID_TIMER, speed, NULL);
				for (i = 0; i<3; i++)
					DestroyWindow(hwndButton[i]);
			}
		}
		if (!(IsGameOver(CurrentTericsInfo.shape, CurrentTericsInfo.dir,
			CurrentTericsInfo.x, CurrentTericsInfo.y)))
		{
			if (ID == 1)
			{
				Players = 2;
				InvalidateRect(hwnd, NULL, FALSE);
				SetTimer(hwnd, ID_TIMER, speed2, NULL);
				for (i = 0; i<3; i++)
					DestroyWindow(hwndButton[i]);
			}
		}
		if (ID == 2)
		{
			DestroyWindow(hwnd);
		}
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
		break;
	}

	return 0;
}

//********************************************************************************************
void DrawGameRegion(HDC hdc)
{
	HBRUSH brush;
	int LeftX, LeftY, RightX, RightY;
	brush = CreateSolidBrush(RGB(0, 0, 0));
	SelectObject(hdc, brush);
	LeftX = GameSizeX;
	LeftY = GameSizeY;
	RightX = LeftX + GameRegionWidth * SquareSize;
	RightY = LeftY + GameRegionHeight * SquareSize;
	Rectangle(hdc, LeftX, LeftY, RightX, RightY);
	DeleteObject(brush);
}

void DrawGameRegionPlayer1(HDC hdc)
{
	HBRUSH brush;
	int LeftX, LeftY, RightX, RightY;
	brush = CreateSolidBrush(RGB(0, 0, 0));
	SelectObject(hdc, brush);
	LeftX = GameSizeX_Player1;
	LeftY = GameSizeY_Player1;
	RightX = LeftX + GameRegionWidth * SquareSize;
	RightY = LeftY + GameRegionHeight * SquareSize;
	Rectangle(hdc, LeftX, LeftY, RightX, RightY);
	DeleteObject(brush);
}

void DrawGameRegionPlayer2(HDC hdc)
{
	HBRUSH brush;
	int LeftX, LeftY, RightX, RightY;
	brush = CreateSolidBrush(RGB(0, 0, 0));
	SelectObject(hdc, brush);
	LeftX = GameSizeX_Player2;
	LeftY = GameSizeY_Player2;
	RightX = LeftX + GameRegionWidth * SquareSize;
	RightY = LeftY + GameRegionHeight * SquareSize;
	Rectangle(hdc, LeftX, LeftY, RightX, RightY);
	DeleteObject(brush);
}

void DrawInterface(HDC hDc)
{
	// Draw a gray area at the right
	HBRUSH rush = CreateSolidBrush(RGB(255, 215, 0));
	int LeftX, LeftY, RightX, RightY;
	LeftX = GameInterFaceX;
	LeftY = GameInterFaceY;
	RightX = LeftX + GameRegionWidth1 * SquareSize;
	RightY = LeftY + GameRegionHeight1 * SquareSize;
	SelectObject(hDc, rush);
	Rectangle(hDc, LeftX, LeftY, RightX, RightY);
	DeleteObject(rush);
};

void DrawInterfacePlayer1(HDC hDc) 
{
	HBRUSH rush = CreateSolidBrush(RGB(255, 215, 0));
	int LeftX, LeftY, RightX, RightY;
	LeftX = GameInterFaceX_Player1;
	LeftY = GameInterFaceY_Player1;
	RightX = LeftX + GameRegionWidth1 * SquareSize;
	RightY = LeftY + GameRegionHeight1 * SquareSize;
	SelectObject(hDc, rush);
	Rectangle(hDc, LeftX, LeftY, RightX, RightY);
	DeleteObject(rush);
};

void DrawInterfacePlayer2(HDC hDc)
{
	HBRUSH rush = CreateSolidBrush(RGB(255, 215, 0));
	int LeftX, LeftY, RightX, RightY;
	LeftX = GameInterFaceX_Player2;
	LeftY = GameInterFaceY_Player2;
	RightX = LeftX + GameRegionWidth1 * SquareSize;
	RightY = LeftY + GameRegionHeight1 * SquareSize;
	SelectObject(hDc, rush);
	Rectangle(hDc, LeftX, LeftY, RightX, RightY);
	DeleteObject(rush);
};

void DrawRectangle(HDC hdc, int shape, int x, int y)  
{
	HBRUSH brush;
	int leftx = GameSizeX + SquareSize * x;
	int lefty = GameSizeY + SquareSize * y;
	int rightx = leftx + SquareSize;
	int righty = lefty + SquareSize;
	brush = CreateSolidBrush(color[shape]);
	SelectObject(hdc, brush);
	Rectangle(hdc, leftx, lefty, rightx, righty);
	DeleteObject(brush);
}

void DrawRectanglePlayer1(HDC hdc, int shape, int x, int y)  
{
	HBRUSH brush;
	int leftx = GameSizeX_Player1 + SquareSize * x;
	int lefty = GameSizeY_Player1 + SquareSize * y;
	int rightx = leftx + SquareSize;
	int righty = lefty + SquareSize;
	brush = CreateSolidBrush(color[shape]);
	SelectObject(hdc, brush);
	Rectangle(hdc, leftx, lefty, rightx, righty);
	DeleteObject(brush);
}

void DrawRectanglePlayer2(HDC hdc, int shape, int x, int y) 
{
	HBRUSH brush;
	int leftx = GameSizeX_Player2 + SquareSize * x;
	int lefty = GameSizeY_Player2 + SquareSize * y;
	int rightx = leftx + SquareSize;
	int righty = lefty + SquareSize;
	brush = CreateSolidBrush(color[shape]);
	SelectObject(hdc, brush);
	Rectangle(hdc, leftx, lefty, rightx, righty);
	DeleteObject(brush);
}

void DrawTerics(HDC hdc, int shape, int dir, int x, int y)
{
	int index;
	for (index = 0; index<4; index++)
	{
		nx[index] = Terics[shape][dir][index].x + x;
		ny[index] = Terics[shape][dir][index].y + y;
		DrawRectangle(hdc, shape, nx[index], ny[index]);
	}
}

void DrawTericsPlayer1(HDC hdc, int shape, int dir, int x, int y)
{
	int index;
	for (index = 0; index<4; index++)
	{
		nx_Player1[index] = Terics[shape][dir][index].x + x;
		ny_Player1[index] = Terics[shape][dir][index].y + y;
		DrawRectanglePlayer1(hdc, shape, nx_Player1[index], ny_Player1[index]);
	}
}

void DrawTericsPlayer2(HDC hdc, int shape, int dir, int x, int y)
{
	int index;
	for (index = 0; index<4; index++)
	{
		nx_Player2[index] = Terics[shape][dir][index].x + x;
		ny_Player2[index] = Terics[shape][dir][index].y + y;
		DrawRectanglePlayer2(hdc, shape, nx_Player2[index], ny_Player2[index]);
	}
}

BOOL IsOutOfRegion(int shape, int dir, int x, int y)
{
	int index;
	int nx, ny;
	for (index = 0; index<4; index++)
	{
		nx = Terics[shape][dir][index].x + x;
		ny = Terics[shape][dir][index].y + y;
		if (nx<0 || nx >= GameRegionWidth || ny<0 || ny >= GameRegionHeight || GameMapStatesInfo[nx][ny].states != 0)
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL IsOutOfRegionPlayer1(int shape, int dir, int x, int y)
{
	int index;
	int nx, ny;
	for (index = 0; index<4; index++)
	{
		nx = Terics[shape][dir][index].x + x;
		ny = Terics[shape][dir][index].y + y;
		if (nx<0 || nx >= GameRegionWidth || ny<0 || ny >= GameRegionHeight || GameMapStatesInfo_Player1[nx][ny].states != 0)
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL IsOutOfRegionPlayer2(int shape, int dir, int x, int y)
{
	int index;
	int nx, ny;
	for (index = 0; index<4; index++)
	{
		nx = Terics[shape][dir][index].x + x;
		ny = Terics[shape][dir][index].y + y;
		if (nx<0 || nx >= GameRegionWidth || ny<0 || ny >= GameRegionHeight || GameMapStatesInfo_Player2[nx][ny].states != 0)
		{
			return TRUE;
		}
	}
	return FALSE;
}

void InitTericsInfo()
{
	srand((unsigned)time(NULL));
	CurrentTericsInfo.shape = szshape;
	CurrentTericsInfo.dir = 0;
	CurrentTericsInfo.x = 4;
	CurrentTericsInfo.y = 0;
	szshape = rand() % 7;
}

void InitTericsInfoPlayer1()
{
	srand((unsigned)time(NULL));
	CurrentTericsInfo_Player1.shape = szshape_Player1;
	CurrentTericsInfo_Player1.dir = 0;
	CurrentTericsInfo_Player1.x = 4;
	CurrentTericsInfo_Player1.y = 0;
	szshape_Player1 = rand() % 7;
}

void InitTericsInfoPlayer2()
{
	srand((unsigned)time(NULL));
	CurrentTericsInfo_Player2.shape = szshape_Player2;
	CurrentTericsInfo_Player2.dir = 0;
	CurrentTericsInfo_Player2.x = 4;
	CurrentTericsInfo_Player2.y = 0;
	szshape_Player2 = (rand() + 2) % 7;
}

void EraseGameRegion(HDC hdc)
{
	RECT rect;
	HBRUSH brush;
	rect.left = GameSizeX;
	rect.top = GameSizeY;
	rect.right = GameSizeX + GameRegionWidth * SquareSize;
	rect.bottom = GameSizeY + GameRegionHeight * SquareSize;
	brush = CreateSolidBrush(RGB(255, 255, 255));
	FillRect(hdc, &rect, brush);
	DeleteObject(brush);
}

void EraseGameRegionPlayer1(HDC hdc)
{
	RECT rect;
	HBRUSH brush;
	rect.left = GameSizeX_Player1;
	rect.top = GameSizeY_Player1;
	rect.right = GameSizeX_Player1 + GameRegionWidth * SquareSize;
	rect.bottom = GameSizeY_Player1 + GameRegionHeight * SquareSize;
	brush = CreateSolidBrush(RGB(255, 255, 255));
	FillRect(hdc, &rect, brush);
	DeleteObject(brush);
}

void EraseGameRegionPlayer2(HDC hdc)
{
	RECT rect;
	HBRUSH brush;
	rect.left = GameSizeX_Player2;
	rect.top = GameSizeY_Player2;
	rect.right = GameSizeX_Player2 + GameRegionWidth * SquareSize;
	rect.bottom = GameSizeY_Player2 + GameRegionHeight * SquareSize;
	brush = CreateSolidBrush(RGB(255, 255, 255));
	FillRect(hdc, &rect, brush);
	DeleteObject(brush);
}


void SaveStateOfTerics(int shape, int dir, int x, int y)
{
	int index;
	int nx, ny;
	int indexX, indexY;
	BOOL flag;
	int m, n;
	for (index = 0; index<4; ++index)
	{
		nx = Terics[shape][dir][index].x + x;
		ny = Terics[shape][dir][index].y + y;
		GameMapStatesInfo[nx][ny].states = 1;
		GameMapStatesInfo[nx][ny].shapes = shape;
	}
	for (indexX = 0; indexX<GameRegionHeight; indexX++)
	{
		flag = TRUE;
		for (indexY = 0; indexY<GameRegionWidth; indexY++)
		{
			if (GameMapStatesInfo[indexY][indexX].states != 1)
			{
				flag = FALSE;
			}
		}
		if (flag)
		{
			for (m = indexX; m >= 1; --m)
			{
				for (n = 0; n<GameRegionWidth; ++n)
				{
					GameMapStatesInfo[n][m].states = GameMapStatesInfo[n][m - 1].states;
					GameMapStatesInfo[n][m].shapes = GameMapStatesInfo[n][m - 1].shapes;
				}
			}
			++count;
		}
	}
}

void SaveStateOfTericsPlayer1(int shape, int dir, int x, int y)
{
	int index;
	int nx, ny;
	int indexX, indexY;
	BOOL flag;
	int m, n;
	for (index = 0; index<4; ++index)
	{
		nx = Terics[shape][dir][index].x + x;
		ny = Terics[shape][dir][index].y + y;
		GameMapStatesInfo_Player1[nx][ny].states = 1;
		GameMapStatesInfo_Player1[nx][ny].shapes = shape;
	}
	for (indexX = 0; indexX<GameRegionHeight; indexX++)
	{
		flag = TRUE;
		for (indexY = 0; indexY<GameRegionWidth; indexY++)
		{
			if (GameMapStatesInfo_Player1[indexY][indexX].states != 1)
			{
				flag = FALSE;
			}
		}
		if (flag)
		{
			for (m = indexX; m >= 1; --m)
			{
				for (n = 0; n<GameRegionWidth; ++n)
				{
					GameMapStatesInfo_Player1[n][m].states = GameMapStatesInfo_Player1[n][m - 1].states;
					GameMapStatesInfo_Player1[n][m].shapes = GameMapStatesInfo_Player1[n][m - 1].shapes;
				}
			}
		}
	}
}

void SaveStateOfTericsPlayer2(int shape, int dir, int x, int y)
{
	int index;
	int nx, ny;
	int indexX, indexY;
	BOOL flag;
	int m, n;
	for (index = 0; index<4; ++index)
	{
		nx = Terics[shape][dir][index].x + x;
		ny = Terics[shape][dir][index].y + y;
		GameMapStatesInfo_Player2[nx][ny].states = 1;
		GameMapStatesInfo_Player2[nx][ny].shapes = shape;
	}
	for (indexX = 0; indexX<GameRegionHeight; indexX++)
	{
		flag = TRUE;
		for (indexY = 0; indexY<GameRegionWidth; indexY++)
		{
			if (GameMapStatesInfo_Player2[indexY][indexX].states != 1)
			{
				flag = FALSE;
			}
		}
		if (flag)
		{
			for (m = indexX; m >= 1; --m)
			{
				for (n = 0; n<GameRegionWidth; ++n)
				{
					GameMapStatesInfo_Player2[n][m].states = GameMapStatesInfo_Player2[n][m - 1].states;
					GameMapStatesInfo_Player2[n][m].shapes = GameMapStatesInfo_Player2[n][m - 1].shapes;
				}
			}
		}
	}
}

void LoadCurrentTerics(HDC hdc)
{
	int indexX, indexY;
	for (indexX = 0; indexX<GameRegionWidth; indexX++)
	{
		for (indexY = 0; indexY<GameRegionHeight; indexY++)
		{
			if (GameMapStatesInfo[indexX][indexY].states == 1)
			{
				DrawRectangle(hdc, GameMapStatesInfo[indexX][indexY].shapes, indexX, indexY);
			}
		}
	}
}

void LoadCurrentTericsPlayer1(HDC hdc)
{
	int indexX, indexY;
	for (indexX = 0; indexX<GameRegionWidth; indexX++)
	{
		for (indexY = 0; indexY<GameRegionHeight; indexY++)
		{
			if (GameMapStatesInfo_Player1[indexX][indexY].states == 1)
			{
				DrawRectanglePlayer1(hdc, GameMapStatesInfo_Player1[indexX][indexY].shapes, indexX, indexY);
			}
		}
	}
}

void LoadCurrentTericsPlayer2(HDC hdc)
{
	int indexX, indexY;
	for (indexX = 0; indexX<GameRegionWidth; indexX++)
	{
		for (indexY = 0; indexY<GameRegionHeight; indexY++)
		{
			if (GameMapStatesInfo_Player2[indexX][indexY].states == 1)
			{
				DrawRectanglePlayer2(hdc, GameMapStatesInfo_Player2[indexX][indexY].shapes, indexX, indexY);
			}
		}
	}
}

void PrintInfo(HDC hdc, int shape)
{
	char szscore[200] = "Score : ";
	char strscore[20];
	score = count * 10;
	_itoa_s(score, strscore, 10);
	strcat_s(szscore, strscore);
	SetBkColor(hdc, RGB(165, 180, 255));
	TextOut(hdc, 550, 200, szscore, strlen(szscore));

	level = score / 20;
	strcpy_s(szscore, "Level : ");
	_itoa_s(level, strscore, 10);
	strcat_s(szscore, strscore);
	SetBkColor(hdc, RGB(165, 180, 255));
	TextOut(hdc, 550, 225, szscore, strlen(szscore));
	DrawTerics(hdc, shape, 0, 12, 2);
}

void PrintInfoPlayer1(HDC hdc, int shape)
{
	char text[20] = "Player1";
	char szscore1[200] = "Score : ";
	char strscore1[20];
	score1 = count * 10;
	_itoa_s(score1, strscore1, 10);
	strcat_s(szscore1, strscore1);
	SetBkColor(hdc, RGB(165, 180, 255));
	TextOut(hdc, 350, 120, szscore1, strlen(szscore1));
	level1 = score1 / 20;
	strcpy_s(szscore1, "Level : ");
	_itoa_s(level1, strscore1, 10);
	strcat_s(szscore1, strscore1);
	SetBkColor(hdc, RGB(165, 180, 255));
	TextOut(hdc, 350, 140, szscore1, strlen(szscore1));
	SetBkColor(hdc, RGB(165, 180, 255));
	TextOut(hdc, 350, 100, text, strlen(text));
	DrawTericsPlayer1(hdc, shape, 0, 13, 7);
}

void PrintInfoPlayer2(HDC hdc, int shape)
{
	char text[20] = "Player2";
	char szscore2[200] = "Score : ";
	char strscore2[20];
	score2 = count * 10;
	_itoa_s(score2, strscore2, 10);
	strcat_s(szscore2, strscore2);
	SetBkColor(hdc, RGB(165, 180, 255));
	TextOut(hdc, 850, 120, szscore2, strlen(szscore2));
	level2 = score2 / 20;
	strcpy_s(szscore2, "Level : ");
	_itoa_s(level2, strscore2, 10);
	strcat_s(szscore2, strscore2);
	SetBkColor(hdc, RGB(165, 180, 255));
	TextOut(hdc, 850, 140, szscore2, strlen(szscore2));
	SetBkColor(hdc, RGB(165, 180, 255));
	TextOut(hdc, 850, 100, text, strlen(text));
	DrawTericsPlayer2(hdc, shape, 0, 13, 7);
}

void PrintTetris(HDC hdc) 
{
	TCHAR szBuffer[20];
	int len = wsprintf(szBuffer, TEXT("MY TETRIS"));
	SetBkMode(hdc, OPAQUE);
	TextOut(hdc, 550, 500, szBuffer, len);
	SetBkMode(hdc, TRANSPARENT);
}

BOOL IsGameOver(int shape, int dir, int x, int y)
{
	int index;
	int nx, ny;
	for (index = 0; index<4; index++)
	{
		nx = Terics[shape][dir][index].x + x;
		ny = Terics[shape][dir][index].y + y;
		if (ny == 0 && GameMapStatesInfo[nx][ny].states == 1)
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL IsGameOverPlayer1(int shape, int dir, int x, int y)
{
	int index;
	int nx, ny;
	for (index = 0; index<4; index++)
	{
		nx = Terics[shape][dir][index].x + x;
		ny = Terics[shape][dir][index].y + y;
		if (ny == 0 && GameMapStatesInfo_Player1[nx][ny].states == 1)
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL IsGameOverPlayer2(int shape, int dir, int x, int y)
{
	int index;
	int nx, ny;
	for (index = 0; index<4; index++)
	{
		nx = Terics[shape][dir][index].x + x;
		ny = Terics[shape][dir][index].y + y;
		if (ny == 0 && GameMapStatesInfo_Player2[nx][ny].states == 1)
		{
			return TRUE;
		}
	}
	return FALSE;
}

void reset(HWND hwnd, HDC hdc)
{
	level = 0;
	InvalidateRect(hwnd, NULL, TRUE);
	SetTimer(hwnd, ID_TIMER, speed, NULL);
	EraseGameRegion(hdc);
	DrawGameRegion(hdc);
	LoadCurrentTerics(hdc);
	DrawTerics(hdc, CurrentTericsInfo.shape, CurrentTericsInfo.dir,
	CurrentTericsInfo.x, CurrentTericsInfo.y);
}

void reset_Player1(HWND hwnd, HDC hdc)
{
	level = 0;
	InvalidateRect(hwnd, NULL, TRUE);
	SetTimer(hwnd, ID_TIMER, speed, NULL);
	EraseGameRegionPlayer1(hdc);
	DrawGameRegionPlayer1(hdc);
	LoadCurrentTericsPlayer1(hdc);
	DrawTericsPlayer1(hdc, CurrentTericsInfo.shape, CurrentTericsInfo.dir,
		CurrentTericsInfo.x, CurrentTericsInfo.y);
}

void reset_Player2(HWND hwnd, HDC hdc)
{
	level = 0;
	InvalidateRect(hwnd, NULL, TRUE);
	SetTimer(hwnd, ID_TIMER, speed, NULL);
	EraseGameRegionPlayer2(hdc);
	DrawGameRegionPlayer2(hdc);
	LoadCurrentTericsPlayer2(hdc);
	DrawTericsPlayer2(hdc, CurrentTericsInfo.shape, CurrentTericsInfo.dir,
		CurrentTericsInfo.x, CurrentTericsInfo.y);
}