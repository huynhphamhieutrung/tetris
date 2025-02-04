#pragma once

#include "resource.h"
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#define ID_TIMER  1
#define NUM 200
#define _CRT_SECURE_NO_WARNINGS
#define EXAMPLE_MACRO_NAME

const int SquareSize = 20;
const int GameSizeX = 300;
const int GameSizeY = 50;
const int GameInterFaceX = 500;
const int GameInterFaceY = 50;
const int GameInterFaceX_Player1 = 300;
const int GameInterFaceY_Player1 = 50;
const int GameInterFaceX_Player2 = 800;
const int GameInterFaceY_Player2 = 50;
const int GameSizeX_Player1 = 100;
const int GameSizeY_Player1 = 50;
const int GameSizeX_Player2 = 600;
const int GameSizeY_Player2 = 50;
const int GameRegionWidth = 10;
const int GameRegionHeight = 20;
const int GameRegionWidth1 = 7;
const int GameRegionHeight1 = 20;
const RECT rect_Player1 = { 100,50,500,500 };
const RECT rect_Player2 = { 600,50,1000,500 };
BOOL Stop = FALSE;
BOOL Start = TRUE;
int Players;
BOOL invalidatePlayer1 = TRUE;
BOOL invalidatePlayer2 = TRUE;
BOOL FirstDraw = TRUE;
HINSTANCE hInst;

int nx[4], ny[4];
int nx_Player1[4], ny_Player1[4];
int nx_Player2[4], ny_Player2[4];

int count = 0;
int score = 0;
int score1 = 0;
int score2 = 0;
int level = 0;
int level1 = 0;
int level2 = 0;
int speed = 500;
int speed2 = 350;
int szshape = 0;
int szshape_Player1 = 1;
int szshape_Player2 = 2;
int X, Y;
HBITMAP hBmp, hBmp2;
BITMAP bmpObj, bmpObj2;


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawGameRegion(HDC hdc);
void DrawGameRegionPlayer1(HDC hdc);
void DrawGameRegionPlayer2(HDC hdc);

void DrawInterface(HDC hdc);
void DrawInterfacePlayer1(HDC hdc);
void DrawInterfacePlayer2(HDC hdc);

void EraseGameRegion(HDC hdc);
void EraseGameRegionPlayer1(HDC hdc);
void EraseGameRegionPlayer2(HDC hdc);

void DrawRectangle(HDC hdc, int shape, int x, int y);
void DrawRectanglePlayer1(HDC hdc, int shape, int x, int y);
void DrawRectanglePlayer2(HDC hdc, int shape, int x, int y);

void DrawTerics(HDC hdc, int shape, int dir, int x, int y);
void DrawTericsPlayer1(HDC hdc, int shape, int dir, int x, int y);
void DrawTericsPlayer2(HDC hdc, int shape, int dir, int x, int y);

BOOL IsOutOfRegion(int shape, int dir, int x, int y);
BOOL IsOutOfRegionPlayer1(int shape, int dir, int x, int y);
BOOL IsOutOfRegionPlayer2(int shape, int dir, int x, int y);

void InitTericsInfo();
void InitTericsInfoPlayer1();
void InitTericsInfoPlayer2();

void SaveStateOfTerics(int shape, int dir, int x, int y);
void SaveStateOfTericsPlayer1(int shape, int dir, int x, int y);
void SaveStateOfTericsPlayer2(int shape, int dir, int x, int y);

void LoadCurrentTerics(HDC hdc);
void LoadCurrentTericsPlayer1(HDC hdc);
void LoadCurrentTericsPlayer2(HDC hdc);

void PrintInfo(HDC hdc, int shape);
void PrintInfoPlayer1(HDC hdc, int shape);
void PrintInfoPlayer2(HDC hdc, int shape);
void PrintTetris(HDC hdc);

BOOL IsGameOver(int shape, int dir, int x, int y);
BOOL IsGameOverPlayer1(int shape, int dir, int x, int y);
BOOL IsGameOverPlayer2(int shape, int dir, int x, int y);

void reset(HWND hwnd, HDC hdc);
void reset_Player1(HWND hwnd, HDC hdc);
void reset_Player2(HWND hwnd, HDC hdc);
