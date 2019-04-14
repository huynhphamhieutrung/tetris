#ifndef TYPES_H
#define TYPES_H

#include "stdafx.h"
#include "WindowsProject3.h"
#include<stdlib.h>
#include<windows.h>

struct
{
	int iStyle;
	TCHAR * szText;
}

button[] =
{
	BS_PUSHBUTTON,    (_TCHAR*)TEXT("1 Player"),
	BS_PUSHBUTTON,    (_TCHAR*)TEXT("2 Player"),
	BS_PUSHBUTTON,    (_TCHAR*)TEXT("Quit game"),
};

struct CurrentTerics
{
	int shape, dir, x, y;
} CurrentTericsInfo, CurrentTericsInfo_Player1, CurrentTericsInfo_Player2;



struct GameMapStates
{
	int states;
	int shapes;
} GameMapStatesInfo[10][20], GameMapStatesInfo_Player1[10][20], GameMapStatesInfo_Player2[10][20];

const COLORREF color[7] =
{
	RGB(255,0,0),
	RGB(240,100,5),
	RGB(150,250,60),
	RGB(27,229,50),
	RGB(10,125,145),
	RGB(20,12,184),
	RGB(116,34,156)
};

const POINT Terics[7][4][4] =
{
	{

		0,0,1,0,0,1,-1,1,  //   **
						   //  **

						   0,0,0,1,1,1,1,2,   //  *
											  //  **
											  //   *
											  0,0,1,0,0,1,-1,1,  //同一

											  0,0,0,1,1,1,1,2    //同二

	},
	{

		0,0,1,0,1,1,2,1,  //   **
						  //    **

						  0,0,0,1,-1,1,-1,2,  //   *
											  //  **
											  //  *

											  0,0,1,0,1,1,2,1,    //同一

											  0,0,0,1,-1,1,-1,2   //同二

	},
	{

		0,0,0,1,0,2,1,2,    //  *
							//   *
							//   **

							0,0,0,1,-1,1,-2,1,  //    *
												//  ***

												0,0,1,0,1,1,1,2,    //  **
																	//    *
																	//    *

																	0,0,0,1,1,0,2,0     //    ***
																						//    *

	},
	{

		0,0,0,1,0,2,-1,2,   //    *
							//    *
							//   **

							0,0,1,0,2,0,2,1,    //   ***
												//     *

												0,0,1,0,0,1,0,2,    //   **
																	//   *
																	//   *

																	0,0,0,1,1,1,2,1     //   *
																						//   ***

	},
	{

		0,0,0,1,0,2,0,3,     //  *
							 //  *
							 //  *
							 //  *

							 0,0,1,0,2,0,3,0,     //  ****

							 0,0,0,1,0,2,0,3,     //  *
												  //  *
												  //  *
												  //  *

												  0,0,1,0,2,0,3,0      //  ****

	},
	{

		0,0,1,0,0,1,1,1,      //   **
							  //   **

							  0,0,1,0,0,1,1,1,      //   **
													//   **

													0,0,1,0,0,1,1,1,

													0,0,1,0,0,1,1,1

	},
	{

		0,0,1,0,2,0,1,1,       //   ***
							   //    *

							   0,0,0,1,0,2,1,1,       //    *
													  //    **
													  //    *
													  0,0,0,1,-1,1,1,1,      //     *
																			 //    ***

																			 0,0,0,1,0,2,-1,1       //    *
																									//   **
																									//    *

	}
};

#endif // TYPES_H