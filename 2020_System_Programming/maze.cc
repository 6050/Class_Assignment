#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "maze.h"

// Game 관련 함수
void TextColorChange(int);
void GotoXY(int, int);
void DrawMap(int, int);
void RunGame(int, int);
int Checker(int, int);
int GameOverChecker(int, int);

int main(int argc, char* argv[])
{
	int circlePosX = 4, circlePosY = 3;

	DrawMap(circlePosX, circlePosY);

	RunGame(circlePosX, circlePosY);

	return 0;
}

void TextColorChange(int colorNumber)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNumber);
}

void GotoXY(int x, int y)
{
	COORD pos = {(SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void DrawMap(int circlePosX, int circlePosY)
{
	int idx = 0, idy = 0;

	system("mode con cols=37 lines=24");

	GotoXY(8, 1), TextColorChange(15), printf("★★  미로 찾기  ★★");

	GotoXY(0, 2), TextColorChange(12);

	for (idx = 0; idx < HEIGHT; idx++) {

		for (idy = 0; idy < WIDTH; idy++) {

			switch (map[idx][idy]) {

			case 0:
				printf("  ");
				break;
			case 1:
				TextColorChange(8);
				printf("■");
				break;
			case 2:
				TextColorChange(12);
				printf("★");
				break;
			default:
				break;
			}

		}

		printf("\n");

	}

	TextColorChange(14);
	GotoXY(circlePosX, circlePosY + 2);
	printf("●");

}

int Checker(int x, int y)
{
	switch (map[y][x]) {

	case 0:
	case 2:
		return 1;
		break;
	default:
		return 0;
		break;

	}
}

void RunGame(int circlePosX, int circlePosY)
{
	int cmd = 0;

	while (1) {

		cmd = _getch();

		TextColorChange(14);

		switch (cmd) {

		case UP:

			GotoXY(circlePosX, circlePosY + 2);
			printf("  ");

			circlePosY = circlePosY - 1;

			if (!Checker(circlePosX, circlePosY)) {
				circlePosY = circlePosY + 1;
			}

			break;

		case DOWN:

			GotoXY(circlePosX, circlePosY + 2);
			printf("  ");

			circlePosY = circlePosY + 1;

			if (!Checker(circlePosX, circlePosY)) {
				circlePosY = circlePosY - 1;
			}

			break;

		case LEFT:

			GotoXY(circlePosX, circlePosY + 2);
			printf("  ");

			circlePosX = circlePosX - 2;

			if (!Checker(circlePosX, circlePosY)) {
				circlePosX = circlePosX + 2;
			}

			break;

		case RIGHT:

			GotoXY(circlePosX, circlePosY + 2);
			printf("  ");

			circlePosX = circlePosX + 2;

			if (!Checker(circlePosX, circlePosY)) {
				circlePosX = circlePosX - 2;
			}

			break;

		case EXIT:
			return;

		}

		GotoXY(circlePosX, circlePosY + 2);
		printf("●");

		if (GameOverChecker(circlePosX, circlePosY)) {
			break;
		}
		else {
			continue;
		}

	}
}

int GameOverChecker(int circlePosX, int circlePosY)
{
	if (32 == circlePosX && 3 == circlePosY || 33 == circlePosX && 3 == circlePosY) {

		system("cls");

		int color = 0;

		for (; color <= 15; color++) {
			GotoXY(10, 11);
			TextColorChange(color);
			printf("미로 탈출 성공!");
			Sleep(100);
		}

		return 1;

	}

	return 0;

}