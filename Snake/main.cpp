#include<stdio.h>
#include<Windows.h>
#include<random>
#include<ctime>
#include "tinyXML/tinyxml.h"

#define KEY_A 0x41
#define KEY_D 0x44
#define KEY_W 0x57
#define KEY_S 0x53

#define WITH  50
#define HEIGHT 30

HANDLE consoleHandle;

bool endGame;

struct Pos2D
{
	int x, y, ox, oy;
};

int snakeLength, point;

Pos2D snake[100], direction, food;

short range;

void readFile() {
	TiXmlDocument doc("SavingData.xml");
	if (!doc.LoadFile())
	{
		printf("%s",doc.ErrorDesc());
	}

	TiXmlElement* root = doc.RootElement();
	int id;
	TiXmlElement* child1 = root->FirstChildElement();
	//child1->QueryIntAttribute("id",&id);

}

void ShowCur(bool CursorVisibility)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor = { 1, CursorVisibility };
	SetConsoleCursorInfo(handle, &cursor);
}

void gotoxy(short x, short y)
{
	COORD coord = { x,y };
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(consoleHandle, coord);
}

bool checkKey(short key)
{
	return GetAsyncKeyState(key);
}
/*---------------------- Init ----------------------*/
void init() {

	endGame = 1;
	snake[0].x = 5; snake[0].y = 5;

	snakeLength = 15;

	direction.x = 0; direction.y = 1;

	food.x = 5; food.y = 10;
}

void moveSnake(Pos2D dir)
{
	// move snake head and snake tails
	for (int i = 0; i < snakeLength; i++) {
		if (i == 0) {
			snake[0].ox = snake[0].x; snake[0].oy = snake[0].y;
			snake[0].x += dir.x; snake[0].y += dir.y;
			//printf("\nsnake[0].x : %d \n",snake[0].x);
			//printf("snake[0].y : %d \n", snake[0].y);
		}
		else {
			snake[i].ox = snake[i].x; snake[i].oy = snake[i].y;
			snake[i].x = snake[i - 1].ox; snake[i].y = snake[i - 1].oy;
			//printf("snake[0].ox : %d ", snake[i].ox);
			//printf("snake[0].oy : %d", snake[i].oy);
		}

		// Check game over when snake hit the wall 
		if (snake[0].x == 0 ||snake[0].x == WITH - 1 || snake[0].y == 0 || snake[0].y == HEIGHT - 1) { endGame = 0; }

		// Check game over when snake head hit snake tails
		if (snake[0].x == snake[snakeLength - i].ox && snake[0].y == snake[snakeLength - i].oy) { endGame = 0; }

	}

	// check snake eat food and grow
	if (snake[0].x == food.x && snake[0].y == food.y) {
		snake[snakeLength].x = snake[snakeLength -1].ox; snake[snakeLength].y = snake[snakeLength -1].oy;
		snakeLength++;
		//printf(":: %d ::",snakeLength);
		srand(NULL);
		food.x = range;
		food.y = range;

		point++;
	}
}
/*-------------- Draw Snake ------------------------*/
void drawSnake() {
	for (int i = 0; i < snakeLength; i++)
	{
		gotoxy(snake[i].x, snake[i].y);
		printf("%c", 12);
	}
	gotoxy(snake[snakeLength - 1].ox, snake[snakeLength - 1].oy);
	printf(" ");
}
/*----------------- Draw Food --------------------*/
void drawFood() {
	gotoxy(food.x, food.y);
	printf("*");
}
/*----------------- Show point ------------------*/
void showPoint() {
		printf("\n-------------------");
		printf("\n|   Points : %d    |", point);
		printf("\n-------------------");
}

/*---------------- Message gameover ------------*/
void showMessageStatusGame(bool status) {
	if (status == 0) {
		printf("\n-----------------------");
		printf("\n|  Status : Gameover  |");
		printf("\n-----------------------");
	}
}

/*------------------- Draw Maps -----------------*/
void drawMap() {
	for (int i = 0; i < WITH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			if (i == 0 || i == WITH - 1 || j == 0 || j == HEIGHT - 1)
			{
				gotoxy(i, j);
				printf("%c", 219);
			}

			if (i != 0 || i != WITH - 1 || j != 0 || j != HEIGHT - 1)
			{
				range = rand() % 30 + 1;
			}
		}
	}
	showPoint();
}
/*------------------- Check input click to move snake -----------*/
void getAsyncKeyState()
{
	moveSnake(direction);

	if (checkKey(KEY_A))
	{
		if (direction.x != 1)
		{
			direction.x = -1;
			direction.y = 0;
		}
	}
	else if (checkKey(KEY_D)) {
		if (direction.x != -1)
		{
			direction.x = 1; 
			direction.y = 0;
		}
	}
	else if (checkKey(KEY_W))
	{
		if (direction.y != 1)
		{
			direction.y = -1;
			direction.x = 0;
		}
	}
	else if (checkKey(KEY_S))
	{
		if (direction.y != -1)
		{
			direction.y = 1;
			direction.x = 0;
		}
	}
}

void clear() {
	system("cls");
}

/*------  -----*/
void draw() {

	drawFood();

	drawMap();

	getAsyncKeyState();

	drawSnake();

}


int main() {
	
	init();

	ShowCur(0);
	while (1) {
		while (endGame) {

			draw();

			Sleep(200);
		}
		clear();
		showMessageStatusGame(endGame);
	}
	return 0;
}