// Full Name: Leead Arbetman
// ID : 206018574

/*
This program is a game called "snake"
The user maneuvers a line of '@' which grows in length every 5 steps.
The game is over when the snake hit the frame of the board or itself.
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define ROWS 25
#define COLS 75
#define FRAME '#'
#define SNAKE_CHAR '@'
#define LEFT 'a'
#define RIGHT 'd'
#define UP 'w'
#define DOWN 's'

typedef struct node
{
	int x;
	int y;
	struct node* next;
} NODE;

// Note: 
// In this program the head (of the snake) is the last node in the linked list
// and the tail (of the snake) is the first node in the linked list
typedef struct snake
{
	NODE* head;
	NODE* tail;
} SNAKE;

// terminal escape codes/ANSI escape codes (or ANSI escape sequences)
// "\x1B" is the escape character that tells your terminal that what comes next is not meant to be printed on the screen, 
// but rather a command to the terminal (or most likely terminal emulator).

// Turn escape codes ON.
void init(void);

// Moves the cursor to position (x,y) on screen.
// Parameters:
//     x: the row of the position.
//     y: the column of the position.
void gotoxy(int x, int y);

// Delays the execution of the program.
// Parameters:
// secs: the length of the delay in seconds. 
void sleep(float secs);

// This function gets positive integers rows and cols, and a char. 
// The function prints a board of the char. 
void printBoard(int rows, int cols, char ch);

// This function gets 2 integers represent a location in the board.
// The function create a new node and returns the pointer to the node.
NODE* newNode(int x, int y);

// This function gets x and y of a node, and a char that represents the current direction.
// The function returns as an output parameter the new x and y of the new head according to the direction.
void newLocation(int x, int y, char direction, int* newX, int* newY);

// This function gets a pointer to a snake.
// The function initialized the snake, and create the first node of the snake (head)
// (at the start of the game the tail equal to head)
void createSnake(SNAKE* s);

// This function gets a node, and two integers x,y represent a location in the board.
// The function add the new node to the head of the snake, and returns the new head of the snake.
NODE* insertLast(NODE* head, int newX, int newY);

// This function gets a node, and two integers x,y represent a location in the board. 
// The function changes the current x and y of the node to the new x and y.
void changeXY(NODE* change, int x, int y);

// This function gets a tail of snake and delete it (put a space instead of the previous char).
void deleteTail(NODE* tail);

// This function gets the head of the snake and prints the head on the board.
void printHead(NODE* head);

// This function gets a snake, and update the arr:
// put 'false' in the previous tail before the snake continue to the next location
void updateArrTail(bool arr[][COLS], SNAKE* s);

// This function gets a snake ,and two integers x,y represent a location in the board.
// The function makes one step with the snake:
// the function "delete" the tail and makes the old Tail to be the new head.
void continueSnake(SNAKE* s, int newX, int newY);

// This function gets the current speed of the snake, and returns the new speed of 
// the snake. if the snake's speed is 1/10 sec, the function returns the same speed.
float newSpeed(float speed);

// This function gets an array,
// and itialized all the cells in the frame of the array to be 'true'
// and all the rest of the cells to be 'false'.
void arrSnake(bool arr[][COLS]);

// This function gets a char represent the direction of the snake, and a char.
// If the char is right, left, up or down,
// the function returns an output parameter of the new direction.
void isDirectionChange(char* direction, char ch);

// This function gets a snake and checks if the next
// location of the head in the arr has already 'true' in in it.
// If there is true, the game is over and the function returns true.
// If false, the function return false and the game will continue.
bool gameOver(bool arr[][COLS], int newX, int newY);

// This function gets a snake, two integers x,y represent the new head location,
// arr represent the location where the snake is allowed to move, the direction of the snake,
// and contGame (if the game is over or not).
// The function checks if the snake can continue in the requested direction, and increase
// the snake.
void increaseSnakeSize(SNAKE* s, int* newX, int* newY, bool arr[][COLS], bool* contGame, char direction);

// This function delete the linked list (free)
void deleteList(NODE* tail);

// This function delete the snake (free)
void deleteSnake(SNAKE* s);

// This function gets the snake after the game is over,
// deletes the snake and prints "game over" on a clean board
void endGame(SNAKE* s);

int main()
{
	init();
	int count = 1, newX, newY;
	char direction, ch;
	float speed = 1;
	bool contGame = true;
	bool arr[ROWS][COLS];
	SNAKE* s = (SNAKE*)malloc(sizeof(SNAKE));

	createSnake(s);
	arrSnake(arr);	// initialized the board location where the snake can move. 
	printBoard(ROWS, COLS, FRAME); // print the board
	printHead(s->head);		// print the head of the snake
	direction = RIGHT;		// at the beginning of the game, the snake moves right

	while (contGame == true) // while the game is not over
	{
		sleep(speed);
		deleteTail(s->tail); // put space instead of tail
		if (_kbhit())		 // if a keyboard hit, change the direction of the snake to the selected direction
		{
			char ch = _getch();
			isDirectionChange(&direction, ch);
		}
		newLocation(s->head->x, s->head->y, direction, &newX, &newY);
		if (gameOver(arr, newX, newY) == true) // if the game is over
		{
			contGame = false;		// do not continue playing
		}
		else
		{
			updateArrTail(arr, s);		//the old tail in the arr is false now
			continueSnake(s, newX, newY);
			if (count == 5)		// enter every five steps
			{
				speed = newSpeed(speed);
				increaseSnakeSize(s, &newX, &newY, arr, &contGame, direction);
				count = 0;
			}
			count++;
		}
	}
	endGame(s);
}

// Turn escape codes ON.
void init(void) {
	system("cls");
}

// Moves the cursor to position (x,y) on screen.
// Parameters:
//     x: the row of the position.
//     y: the column of the posiiton.
void gotoxy(int x, int y) {
	printf("\x1b[%d;%df", x, y);
}

// Delays the execution of the program.
// Parameters:
//     secs: the length of the delay in seconds. 
void sleep(float secs) {
	clock_t clocks_start = clock();

	while (clock() - clocks_start < secs * CLOCKS_PER_SEC);
}

void printBoard(int rows, int cols, char ch)
{
	int i, j;

	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			if ((i == 0) || (i == rows - 1) || (j == 0) || (j == cols - 1)) // if first or last row, or first or last col
			{
				printf("%c", ch);
			}
			else
			{
				printf(" ");
			}
		}
		printf("\n");
	}
}

NODE* newNode(int x, int y)
{
	NODE* p = (NODE*)malloc(sizeof(NODE));
	if (p)
	{
		p->x = x;
		p->y = y;
		p->next = NULL;
		return p;
	}
	else
	{
		return NULL;
	}
}

void newLocation(int x, int y, char direction, int* newX, int* newY)
{
	switch (direction)
	{
	case LEFT:
		*newX = x;
		*newY = y - 1;
		break;
	case RIGHT:
		*newY = y + 1;
		*newX = x;
		break;
	case UP:
		*newX = x - 1;
		*newY = y;
		break;
	case DOWN:
		*newX = x + 1;
		*newY = y;
		break;
	default:
		*newX = x + 1;
		*newY = y;
	}
}

void createSnake(SNAKE* s)
{
	s->head = newNode(ROWS / 2, COLS / 2); // at the beginning the snake in the mid of the board
	s->tail = s->head; // at the beginning the tail is the head
}

NODE* insertLast(NODE* head, int newX, int newY)
{
	NODE* newHead = newNode(newX, newY); // create a new node
	head->next = newHead;
	return newHead;
}

void changeXY(NODE* change, int x, int y)
{
	change->x = x;
	change->y = y;
}

void deleteTail(NODE* tail)
{
	gotoxy(tail->x, tail->y);
	printf(" "); // put 'space' in the old tail
}

void printHead(NODE* head)
{
	if (head)
	{
		gotoxy(head->x, head->y);
		printf("%c", SNAKE_CHAR);
		gotoxy(COLS, 0);
	}
}

void updateArrTail(bool arr[][COLS], SNAKE* s)
{
	arr[s->tail->x - 1][s->tail->y - 1] = false; //the old tail is false now
}

void continueSnake(SNAKE* s, int newX, int newY)
{
	if (s->tail->next != NULL)
	{
		s->head->next = s->tail;
		changeXY(s->tail, s->head->x, s->head->y);
		s->head = s->tail;
		s->tail = s->tail->next;
		s->head->next = NULL;
	}
	changeXY(s->head, newX, newY); // update the new x, y of the new head
	printHead(s->head);			   // print the new head
}

float newSpeed(float speed)
{
	if (speed <= 0.13)
	{
		speed = 0.10; // max speed
	}
	else
	{
		speed -= 0.03;
	}
	return speed;
}

void arrSnake(bool arr[][COLS])
{
	int i, j;
	for (i = 0; i < ROWS; i++)
	{
		for (j = 0; j < COLS; j++)
		{
			if ((i == 0) || (j == 0) || (i == ROWS - 1) || (j == COLS - 1)) // put 'true' in the frame of the array
			{
				arr[i][j] = true;
			}
			else
			{
				arr[i][j] = false;
			}
		}
	}
}

void isDirectionChange(char* direction, char ch)
{
	if (ch == LEFT || ch == RIGHT || ch == UP || ch == DOWN) //if direction is changed
	{
		*direction = ch;
	}
}

bool gameOver(bool arr[][COLS], int newX, int newY)
{
	bool gameOver = true;
	if (arr[newX - 1][newY - 1] == false) // if the next location of head is empty ( not frame and not snake)
	{
		arr[newX - 1][newY - 1] = true; //the location in the arr of new head is true now
		gameOver = false; // game is not over yet
	}
	return gameOver;
}

void increaseSnakeSize(SNAKE* s, int* newX, int* newY, bool arr[][COLS], bool* contGame, char direction)
{
	newLocation(s->head->x, s->head->y, direction, newX, newY); // cheacks what is the new location of the new head
	if (gameOver(arr, *newX, *newY) == true) // if the game is over
	{
		contGame = false; // do not continue playing
	}
	else
	{
		s->head = insertLast(s->head, *newX, *newY); // insert a new head in the end of the snake
		printHead(s->head);
	}
}

void deleteList(NODE* tail)
{
	if (tail != NULL)
	{
		deleteList(tail->next);
		free(tail);
	}
}

void deleteSnake(SNAKE* s)
{
	free(s);
}

void endGame(SNAKE* s)
{
	deleteList(s->tail);
	deleteSnake(s);
	init();
	printBoard(ROWS, COLS, FRAME);
	gotoxy(ROWS / 2, (COLS / 2) - 4); // go to the middle of the board
	printf("GAME OVER");
	gotoxy(ROWS + 1, 0);

}