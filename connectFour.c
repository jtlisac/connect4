//trying without curses
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

int stringConverter(char str[]);
int findDimension(char option);
int gameMode();
void buildBoard(int boardX,int boardY,int board[boardX][boardY]);

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
int findDimension(char option)
{
	int dimension=0;
	char mesg[20];
	if (option=='y')
		strcpy(mesg,"How many rows? ");
	if (option=='x')
		strcpy(mesg,"How many columns? ");
	char inputStr[100]={'a'};
	//ask for input
	printf(mesg);
	//bad input
	if (scanf("%d",&dimension)<=0)
	{
		//clear buffer
		int buffer=0;
		while (buffer=getchar()!='\n')
		{}
		//ask again
		return findDimension(option);
	}
	//clear buffer while/check for more input
	int bufferCheck=0;
	int buffer=0;
	while (buffer=getchar()!='\n')
		bufferCheck++;
	//too much input
	if (bufferCheck!=0)
		return findDimension(option);
	//test getting input
	//effective, has size issues
	//int has maximum possible value, size restriction
	//work into recursive call
	/*
	int i=0;
	for (i=0;i<10;i++)
	{
		printf("\n");
		printf(mesg);
		
		if (scanf("%d",&dimension)>0)
		{
			printf("%d",dimension);
			printf(" ");
			dimension=dimension+1;
			printf("%d",dimension);
			printf("\n");
		}
		else	//bad input
		{
			printf("Bad input\n");
			char buffer;
			//clear buffer
			int bufferChar;
			while (getchar()!='\n')
			{}
		}
	}
	*/
	//dimension=stringConverter(inputStr);
	//invalid input
	if (dimension<=0)
		return findDimension(option);
	else
		return dimension;
}

//returns 1 or 0, 1=1v1,0=1vAI
int gameMode()
{
	char mode;
	printf("Play against the machine? (Y/N) ");
	//need to check buffer for extra, then clear it
	//bad input, otherwise good character is stored
	if (scanf("%c",&mode)<=0)
	{
		//clear buffer
		int buffer=0;
		while (buffer=getchar()!='\n')
		{}
		return gameMode();
	}
	//check for more input/clear buffer
	int buffer=0;
	int bufferCheck=0;
	while (buffer=getchar()!='\n')
		bufferCheck++;
	//too much input
	if (bufferCheck!=0)
		return gameMode();
	//Y=play against the machine
	if (mode=='Y' || mode=='y')
		return 0;
	//N=play against a person
	if (mode=='N' || mode=='n')
		return 1;
	
	//no vaild input, ask again
	return gameMode();
}

//displays the board to the screen
void buildBoard(int boardX,int boardY,int board[boardX][boardY])
{
	int column=0;
	int row=0;
	//cycle through available y, top down
	for (row=boardY-1;row>=0;row--)
	{
		//cycle through available x left right
		for (column=0;column<boardX;column++)
		{
			//print contents of array
			printf("|");
			switch (board[column][row])
			{
				//empty
				case 0:
					printf("-");
					break;
				//player 1
				case 1:
					printf("X");
					break;
				//player 2
				case 2:
					printf("Y");
					break;
			}
		}
		//close out row
		printf("|\n");
	}
	//board contents displayed
	//print footer
	for (column=0;column<boardX;column++)
	{
		printf("|");
		printf("%d",column+1);
	}
	//close out row
	printf("|\n");
}

int main()
{
	//board dimensions
	int rows,columns=0;
	//game mode: 1=pvp, 0=1vAI
	int player=0;
	//win count
	int playerOneWins=0;
	int playerTwoWins=0;
	//variable to reset while running
	int resetMarker=1;

	//ask for board dimensions
	//ask for y
	rows=findDimension('y');
	//ask for x
	columns=findDimension('x');
	//ask for 1v1 or 1vAI
	player=gameMode();
	int board[columns][rows];
	//loop until board filled or someone wins
	//reset board in loop
	if (resetMarker==1)
	{
		int i=0;
		int j=0;
		for (i=0;i<columns;i++)
			for (j=0;j<rows;j++)
				board[i][j]=0;
	}
	//display win count
	printf("%d",playerOneWins);
	printf("-");
	printf("%d",playerTwoWins);
	printf("\n");
	//build board
	buildBoard(columns,rows,board);
	//ask for player input
	//win check, find next move
	//change player
	//end loop
	
	return 0;
}
