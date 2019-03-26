//pull request 1
#include <ncurses.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

int stringConverter(char str[]);
int findDimension(int maxY, int maxX, char option);
int gameMode(int maxY,int maxX);
void buildBoard(int boardX,int boardY,int winX,int winY,int maxX,int maxY,int board[boardX][boardY]);

//converts a string into an int
int stringConverter(char str[])
{
	int result=0;
	int digit=0;
	//last useful char
	int i=strlen(str)-2;
	for (i=i;i>0;i--)
	{
		//if input is 'a', nothing was entered
		if (str[i]!='a')
		{
			//input is a number
			if (isdigit(str[i]))
				result+=(str[i]-'0')*pow(10,digit);
			//character input not accepted
			else
			{
				return -1;
			}
			digit++;
		}
	}
	return result;
}

//returns input x or y
int findDimension(int maxY, int maxX, char option)
{
	int dimension=0;
	char mesg[20];
	if (option=='y')
		strcpy(mesg,"How many rows? ");
	if (option=='x')
		strcpy(mesg,"How many columns? ");
	char inputStr[100]={'a'};
	//get input
	//print to middle of screen
	//mvprint(maxY/2,(maxX-strlen(mesg))/2,"%s",mesg);
	move(maxY/2,(maxX-strlen(mesg))/2);
	printw("%s",mesg);
	getstr(inputStr);
	//
	// test getting input
	////mvprintw(LINES-2,0,"You entered: %s", inputStr);
	//move(LINES-2,0);
	//printw("You entered: %s",inputStr);
	//getch();
	//
	endwin();
	dimension=stringConverter(inputStr);
	//invalid input
	if (dimension<=0)
		return findDimension(maxY,maxX,option);
	else
		return dimension;
}

//returns 1 or 0, 1=1v1,0=1vAI
int gameMode(int maxY,int maxX)
{
	char mode=0;
	char mesg[]="Play against the machine? (Y/N) ";
	move(maxY/2,(maxX-strlen(mesg))/2);
	printw("%s",mesg);
	mode=getch();
	endwin();
	//Y=play against the machine
	if (mode=='Y' || mode=='y')
		return 0;
	//N=play against a person
	if (mode=='N' || mode=='n')
		return 1;
	
	//no vaild input, ask again
	return gameMode(maxY,maxX);
}

//displays the board to the screen
void buildBoard(int boardX,int boardY,int winX,int winY,int maxX,int maxY,int board[boardX][boardY])
{
	int i,j=0;
	//cycle through available x
	for (i=winX;i<maxX+winX;i++)
		//cycle through available y
		for (j=winY;j<maxY+winY;j++)
		{
			move(maxY-j,3*i);
			printw("|");
			printw("%d",board[i][j]);
			printw("|");
		}
}

int main()
{
	//screen dimensions
	int maxX,maxY=0;
	//board dimensions
	int rows,columns=0;
	//window bounds
	int winX,winY=0;
	//game mode: 1=pvp, 0=1vAI
	int player=0;

	//start curses
	initscr();
	//screen dimensions
	getmaxyx(stdscr,maxY,maxX);
	//ask for board dimensions
	//ask for y
	rows=findDimension(maxY,maxX,'y');
	//ask for x
	columns=findDimension(maxY,maxX,'x');
	//ask for 1v1 or 1vAI
	player=gameMode(maxY,maxX);
	//build board
	int board[columns][rows];
	
	buildBoard(columns,rows,winX,winY,maxX,maxY,board);
	//test, wait for character input
	getch();
	//start game
	//ask for player input
	//place piece in correct location
	//update chains
	//win check
	//next input
	
	return 0;
}
