//trying without curses
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>


int findDimension(char option);
int gameMode();
void buildBoard(int boardX,int boardY,int board[boardX][boardY]);
int playerChoice(int player);
int findRow(int columnChoice,int boardX,int boardY,int board[boardX][boardY],int mode);
int vertCheck(int boardX,int boardY,int board[boardX][boardY],int player,int column,int row);
int horizontalCheck(int boardX,int boardY,int board[boardX][boardY],int player,int column,int row);
int diagUpCheck(int boardX,int boardY,int board[boardX][boardY],int player,int column,int row);
int diagDownCheck(int boardX,int boardY,int board[boardX][boardY],int player,int column,int row);
void stateSearch(int boardX,int boardY,int board[boardX][boardY],int player,int* search);
int winCheck(int boardX,int boardY,int board[boardX][boardY]);
int fullCheck(int boardX,int boardY,int board[boardX][boardY]);
void winCount(int playerOneWins,int playerTwoWins);
int askForRematch();



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
					printf("O");
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

//returns chosen column number
int playerChoice(int player)
{
	int choice=0;
	printf("Enter a column number player ");
	printf("%d",player);
	switch(player)
	{
		case 1:
		     printf("(X)");
		     break;
		case 2:
		     printf("(O)");
		     break;
	}
	printf(" ");
	//bad input
        if (scanf("%d",&choice)<=0)
        {
                //clear buffer
                int buffer=0;
                while (buffer=getchar()!='\n')
                {}
                //ask again
                return playerChoice(player);
        }
        //clear buffer/check for more input
        int bufferCheck=0;
        int buffer=0;
        while (buffer=getchar()!='\n')
                bufferCheck++;
        //too much input
        if (bufferCheck!=0)
                return playerChoice(player);
	//good input
	return choice;
}

//finds open row, if mode==1, function is used for input
//if mode==0, function is used for checking board state
int findRow(int columnChoice,int boardX,int boardY, int board[boardX][boardY],int mode)
{
	//increments before being used
	int rowNumber=-1;
	do
	{
		//i will start at zero and iterate through rows
		rowNumber++;
		//check for empty space
		if (board[columnChoice][rowNumber]==0)
		{
			return rowNumber;
		}
	} while (board[columnChoice][rowNumber]!=0 && rowNumber<boardY-1);
	
	//being used for input
	if (mode==1)
	{
		//column full
		printf("Column is full, choose a different one\n");
	}

	return -1;
}

//returns number of sequential vertical blocks of the same player
int vertCheck(int boardX,int boardY,int board[boardX][boardY],int player,int column,int row)
{
	int vertMax=1;
	//no room,invalid chain
	if (row+3>=boardY)
		return 0;
	//1st space matches
	if (row+1<boardY && board[column][row+1]==player)
	{
		vertMax++;
		//2nd space matches
		if (row+2<boardY && board[column][row+2]==player)
		{
			vertMax++;
			if (row+3<boardY && board[column][row+3]==player)
			{
				vertMax++;
			}
			//no more spaces or space taken by other player
			else if	(row+3>=boardY || board[column][row+3]==(player%2+1))
				return 0;
			//otherwise empty space exists
		}
		//no more spaces or space taken by other player
		else if (row+2>=boardY || board[column][row+2]==(player%2+1))
			return 0;
		//otherwise empty space exists
	}
	return vertMax;
}

//returns number of sequential horizontal blocks of the same player, including one open space
int horizontalCheck(int boardX,int boardY,int board[boardX][boardY],int player,int column,int row)
{
	int horizontalMax=1;
	int openSpace=1;
	//no room, invalid chain
	if (column+3>=boardX)
		return 0;
	//1st space matches or is open
	if (board[column+1][row]!=(player%2+1) && (openSpace>0 || board[column+1][row]==player))
	{
		//there was an open space, something needs to be on other side
		if (board[column+1][row]==0 && board[column+2][row]==player)
			openSpace--;
		//chain already ended
		else
			return horizontalMax;
		//passed
		horizontalMax++;
		//2nd space matches or is open
		if (board[column+2][row]!=(player%2+1) && (openSpace>0 || board[column+2][row]==player))
		{
			//if there was an open space, somthing needs to be on other side
			if (board[column+2][row]==0 && board[column+3][row]==player)
				openSpace--;
			//chain ended earlier
			else
				return horizontalMax;
			//passed
			horizontalMax++;
			//3rd space matches
			if (board[column+3][row]==player)
			{
				//passed
				horizontalMax++;
				//if there is a hole in the chain
				if (openSpace==0)
					horizontalMax--;
			}
			//invalid chain
			else if (board[column+3][row]==(player%2+1))
				return 0;
		}
		//invalid chain
		else
			return 0;
	}
	//invalid chain
	else
		return 0;

	return horizontalMax;
}


//returns number of sequential diagonal up blocks of the same player, including one open space
int diagUpCheck(int boardX,int boardY,int board[boardX][boardY],int player,int column,int row)
{
	int diagUpMax=1;
	int openSpace=1;
        //no room, invalid chain
        if (column+3>=boardX || row+3>=boardY)
                return 0;
        //1st space matches or is open
        if (board[column+1][row+1]!=(player%2+1) && (openSpace>0 || board[column+1][row+1]==player))
        {
                //there was an open space, something needs to be on other side
                if (board[column+1][row+1]==0 && board[column+2][row+2]==player)
                        openSpace--;
                //chain already ended
                else
                        return diagUpMax;
                //passed
                diagUpMax++;
                //2nd space matches or is open
                if (board[column+2][row+2]!=(player%2+1) && (openSpace>0 || board[column+2][row+2]==player))
                {
                        //if there was an open space, somthing needs to be on other side
                        if (board[column+2][row+2]==0 && board[column+3][row+3]==player)
                                openSpace--;
                        //chain ended earlier
                        else
                                return diagUpMax;
                        //passed
                        diagUpMax++;
                        //3rd space matches
                        if (board[column+3][row+3]==player)
                        {
                                //passed
                                diagUpMax++;
                                //if there is a hole in the chain
                                if (openSpace==0)
                                        diagUpMax--;
                        }
                        //invalid chain
                        else if (board[column+3][row+3]==(player%2+1))
                                return 0;
                }
                //invalid chain
                else
                        return 0;
        }
        //invalid chain
        else
                return 0;

        return diagUpMax;
}

//returns number of sequential diagonal down blocks of the smae player, including one open space
int diagDownCheck(int boardX,int boardY,int board[boardX][boardY],int player,int column,int row)
{
	int diagDownMax=1;
        int openSpace=1;
        //no room, invalid chain
        if (column+3>=boardX || row-3<=0)
                return 0;
        //1st space matches or is open
        if (board[column+1][row-1]!=(player%2+1) && (openSpace>0 || board[column+1][row-1]==player))
        {
                //there was an open space, something needs to be on other side
                if (board[column+1][row-1]==0 && board[column+2][row-2]==player)
                        openSpace--;
                //chain already ended
                else
                        return diagDownMax;
                //passed
                diagDownMax++;
                //2nd space matches or is open
                if (board[column+2][row-2]!=(player%2+1) && (openSpace>0 || board[column+2][row-2]==player))
                {
                        //if there was an open space, somthing needs to be on other side
                        if (board[column+2][row-2]==0 && board[column+3][row-3]==player)
                                openSpace--;
                        //chain ended earlier
                        else
                                return diagDownMax;
                        //passed
                        diagDownMax++;
                        //3rd space matches
                        if (board[column+3][row-3]==player)
                        {
                                //passed
                                diagDownMax++;
                                //if there is a hole in the chain
                                if (openSpace==0)
                                        diagDownMax--;
                        }
                        //invalid chain
                        else if (board[column+3][row-3]==(player%2+1))
                                return 0;
                }
                //invalid chain
                else
                        return 0;
        }
        //invalid chain
        else
                return 0;

        return diagDownMax;

}

//finds available column number for best size chain
//returns avaiable column number in search[0]
//returns size in search[1]
void stateSearch(int boardX,int boardY, int board[boardX][boardY],int player,int* search)
{
	search[0]=0;
	search[1]=0;
	//beginning of loop, will cycle through board
	int column=0;
	int row=0;
	for (row=0;row<boardY;row++)
	{
		for (column=0;column<boardX;column++)
		{
			//checks to be made
			int vertical=0;
			int horizontal=0;
			int diagUp=0;
			int diagDown=0;
			if (board[column][row]==player)
			{
				//functions to find max of possible available chains
				//has to have available space to be valid
				vertical=vertCheck(boardX,boardY,board,player,column,row);
				horizontal=horizontalCheck(boardX,boardY,board,player,column,row);
				diagUp=diagUpCheck(boardX,boardY,board,player,column,row);
				diagDown=diagDownCheck(boardX,boardY,board,player,column,row);

				/*
				//test code
				printf("Player: ");
				printf("%d",player);
				printf("\n");
				printf("%d",vertical);
				printf("\n");
				printf("%d",horizontal);
                                printf("\n");
				printf("%d",diagUp);
                                printf("\n");
				printf("%d",diagDown);
                                printf("\n");
				*/

				
				//find maximum
				int sortArray[4]={vertical,horizontal,diagUp,diagDown};
				int i=0;
				int j=0;
				//bubble sort, descending order
				for (i=0;i<3;i++)
				{
					for (j=0;j<3-i;j++)
					{
						if (sortArray[j]<sortArray[j+1])
						{
							int temp=sortArray[j+1];
							sortArray[j+1]=sortArray[j];
							sortArray[j]=temp;
						}
					}
				}

				/*
				//test code
                                printf("Sorted for player: ");
                                printf("%d",player);
                                printf("\n");
                                printf("%d",vertical);
                                printf("\n");
                                printf("%d",horizontal);
                                printf("\n");
                                printf("%d",diagUp);
                                printf("\n");
                                printf("%d",diagDown);
                                printf("\n");
				*/


				//someone won
				if (sortArray[1]==4)
				{
					//test
					printf("someone won\n");

					search[1]=4;
					//win marker
					search[0]=-1;
					return;
				}
				//game continues, look if current run has higher max than other runs
				if (sortArray[0]>search[1])
				{
					//new maximum
					search[1]=sortArray[0];

					//find which way the chain goes
					//priorities: vert->horizontal->diagUp->diagDown
					if (sortArray[0]==vertical)
					{
						//chain is in same column
						search[0]=column;
					}
					else if (sortArray[0]==horizontal)
					{
						//chain is in same row, somewhere in next 3 spaces
						//check for open spaces
						//check 1st space
						if (row==findRow(column+1,boardX,boardY,board,0))
							search[0]=column+1;
						//check 2nd space
						else if (row==findRow(column+2,boardX,boardY,board,0))
							search[0]=column+2;
						//check 3rd space
						else if (row==findRow(column+3,boardX,boardY,board,0))
							search[0]=column+3;
					}
					else if (sortArray[0]==diagUp)
					{
						//chain is in column+1,row+1 order
						//check for open spaces
						//check 1st space
						if (row+1==findRow(column+1,boardX,boardY,board,0))
							search[0]=column+1;
						//check 2nd row
						else if (row+2==findRow(column+2,boardX,boardY,board,0))
							search[0]=column+2;
						//check 3rd row
						else if (row+3==findRow(column+3,boardX,boardY,board,0))
							search[0]=column+3;
					}
					else if (sortArray[0]==diagDown)
					{
						//chain is in column+1,row-1 order
						//check for open spaces
						//check 1st space
						if (row-1==findRow(column+1,boardX,boardY,board,0))
							search[0]=column+1;
						//check 2nd space
						if (row-2==findRow(column+2,boardX,boardY,board,0))
							search[0]=column+2;
						//check 3rd space
						if (row-3==findRow(column+3,boardX,boardY,board,0))
							search[0]=column+3;
					}
				}
			}
		}
	}
	return;
}

//returns AI column number move choice, or -1 if someone won
int winCheck(int boardX,int boardY,int board[boardX][boardY])
{
	int maxArray[2]={0};
	int playerOneMax=0;
	int playerOneBestMove=0;
	int playerTwoMax=0;
	int playerTwoBestMove=0;
	//search player 1 for size and column number
	stateSearch(boardX,boardY,board,1,maxArray);
	playerOneMax=maxArray[1];
	playerOneBestMove=maxArray[0];
	/*
	//test contents
	printf("%d",maxArray[1]);
	printf("\n");
	printf("%d",maxArray[0]);
	printf("\n");
	*/
	
	//search player 2 for size and column number
	stateSearch(boardX,boardY,board,2,maxArray);
	playerTwoMax=maxArray[1];
	playerTwoBestMove=maxArray[0];
	/*
	//test contents
	printf("%d",maxArray[1]);
        printf("\n");
        printf("%d",maxArray[0]);
        printf("\n");
	*/

	
	//determine best choice based on case
	//someone won
	if (playerOneMax==4 || playerTwoMax==4)
		return -1;
	//if player 2 longer chain
	if (playerTwoMax>playerOneMax)
		return playerTwoBestMove;
	//player 1 is a better position
	//block
	else
		return playerOneBestMove;
}

//returns 0 if more space exist, 1 if full
int fullCheck(int boardX,int boardY,int board[boardX][boardY])
{
	int i=0;
	for (i=0;i<boardX;i++)
	{
		//open space, done
		if (findRow(i,boardX,boardY,board,0)!=-1)
			return 0;
		//no open spaces found
		if (i==boardX-1)
			return 1;
	}
}

//displays current win counts
void winCount(int playerOneWins,int playerTwoWins)
{
	printf("%d",playerOneWins);
	printf("-");
	printf("%d",playerTwoWins);
	printf("\n");
}

//retrns 1 if rematch requested, 0 if not
int askForRematch()
{
	char rematchMarker;
        printf("Play another game? (Y/N) ");
        //need to check buffer for extra, then clear it
        //bad input, otherwise good character is stored
        if (scanf("%c",&rematchMarker)<=0)
        {
                //clear buffer
                int buffer=0;
                while (buffer=getchar()!='\n')
                {}
                return askForRematch();
        }
        //check for more input/clear buffer
        int buffer=0;
        int bufferCheck=0;
        while (buffer=getchar()!='\n')
                bufferCheck++;
        //too much input
        if (bufferCheck!=0)
                return askForRematch();
        //Y=play against the machine
        if (rematchMarker=='Y' || rematchMarker=='y')
                return 1;
        //N=play against a person
        if (rematchMarker=='N' || rematchMarker=='n')
                return 0;

        //no vaild input, ask again
        return askForRematch();
}

int main()
{
	//board dimensions
	int rows,columns=0;
	//game mode: 1=pvp, 0=1vAI
	int mode=0;
	//player #, 1=player 1, 2=player 2
	int player=1;
	//win count
	int playerOneWins=0;
	int playerTwoWins=0;
	//variable to reset while running
	int resetMarker=1;
	//game over variable
	int gameOver=0;
	//column number(+1) for next piece insertion
	int choice=0;
	//row number for next piece insertion
	int choiceRow=0;
	//next move for AI
	int nextMove=0;
	//rematch marker
	int rematch=0;

	//ask for board dimensions
	//ask for y
	rows=findDimension('y');
	//ask for x
	columns=findDimension('x');
	//ask for 1v1 or 1vAI
	mode=gameMode();
	int board[columns][rows];

	//loop until board filled or someone wins
	while (!gameOver)
	{
		//display win count
		winCount(playerOneWins,playerTwoWins);
		//reset board in loop
		if (resetMarker==1)
		{
			int i=0;
			int j=0;
			for (i=0;i<columns;i++)
				for (j=0;j<rows;j++)
					board[i][j]=0;
			//display initial board
			buildBoard(columns,rows,board);
			//reset finished
			resetMarker=0;
		}
		//AI makes move
		if (mode==0 && player==2)
		{
			choice=winCheck(columns,rows,board);
			choiceRow=findRow(choice,columns,rows,board,1);
		}
		//2nd player makes move
		else
		{
			//while loop to check open space
			choiceRow=-1;
			while (choiceRow<0)
			{
				//ask for player input, corrected
				choice=playerChoice(player)-1;
				//valid column
				if (choice>=0 && choice<columns)
				{
					//find open position in board
					choiceRow=findRow(choice,columns,rows,board,1);
				}
			}
		}
		//place piece in board
		board[choice][choiceRow]=player;
		//build board
		buildBoard(columns,rows,board);
		//check for win/find next move for AI
		nextMove=winCheck(columns,rows,board);
		//check if game should end
		if (nextMove<0 || fullCheck(columns,rows,board)==1)
		{
			//game finished
			gameOver=1;
			//someone won
			if (nextMove<0)
			{
				switch (player)
				{
					case 1:
						playerOneWins++;
						printf("Player one wins!\n");
						break;
					case 2:
						playerTwoWins++;
						printf("Player two wins!\n");
						break;
				}
			}
			//display win count
			winCount(playerOneWins,playerTwoWins);
			//display board
			buildBoard(columns,rows,board);
			//ask for rematch
			rematch=askForRematch();
			if (rematch==1)
			{
				gameOver=0;
				resetMarker=1;
				//will switch back to player 1 through loop
				player=2;
			}
		}
		//change player
		player=player%2+1;
		//end loop
	}
	
	return 0;
}
