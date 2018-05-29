#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N	15

int chessboard[N + 1][N + 1] = { 0 };
int dirx[9]={0,0,1,0,-1,1,1,-1,-1};
int diry[9]={0,1,0,-1,0,1,-1,-1,1};
int score[N+1][N+1]={};
int whoseTurn = 0, OptX, OptY;
void initGame();
void printChessboard();
void playChess();
void CalPos();
void EvaluateScore();
int judge(int, int);

int main()
{
	initGame();
	while (1)
	{
		whoseTurn++;
		playChess();
	}
	return 0;
}

void initGame()
{
	// char c;
	// printf("input Y to enter the game:");
	// c = getchar();
	// if ('y' != c && 'Y' != c)
	// 	exit(0);
	system("clear");
	printChessboard();
}

void printChessboard()
{
	int i, j;
	for (i = 0; i <= N; i++)
	{
		for (j = 0; j <= N; j++)
		{
			if (0 == i)
				printf("%3d", j);
			else if (j == 0)
				printf("%3d", i);
			else if (1 == chessboard[i][j])
				printf("  ○");
			else if (2 == chessboard[i][j])
				printf("  ●");
			else
				printf("   ");
		}
		printf("\n");

	}

}

void playChess()
{
	int i, j, winner;
	if (1 == whoseTurn % 2)
	{
		printf("Turn to player 1, please input the position:");
		scanf("%d %d", &i, &j);
		while(chessboard[i][j] != 0)
		{
			printf("your position is taken,choose another:"); 
			scanf("%d %d", &i, &j);
		}
		chessboard[i][j] = 1;
	}
	else
	{
		printf("Turn to player 2\n");
		int ix = i;
		int jy = j;
		// while(chessboard[i][j] != 0)
		// {
		// 	int r = (int)(rand()%(4-1+1)+1);
		// 	i = ix + dirx[r];
		// 	j = jy + diry[r];
		// }
		CalPos();
		chessboard[OptX][OptY] = 2;
	}

	system("clear");

	printChessboard();
	if (judge(i, j))
	{
		if (1 == whoseTurn % 2)
		{
			printf("player1 win!\n");
			exit(0);
		}
		else
		{
			printf("player2 win!\n");
			exit(0);
		}
	}
}

void CalPos()
{
	EvaluateScore();
	int maxScore = -1e9;
	for(int i=1;i<=N;++i)
		for(int j=1;j<=N;j++)
			if(chessboard[i][j] == 0)
			{
				EvaluateScore();
				//score[i][j] = EvaluateScore(i,j);
				if(maxScore <= score[i][j])
				{
					OptX = i, OptY = j;
					maxScore = score[i][j];
				}
				//else if(score[i][j] == maxScore)
			}
}

void EvaluateScore()
{
	memset(score,0,sizeof(score));
	for(int i=1;i<=N;++i)
		for(int j=1;j<=N;++j)
		{
			if(chessboard[i][j] == 0)
			{
				// int humanNum = 0;
				// int AINum = 0;
				// int emptyNum = 0;
				for(int k=1;k<=8;k++)
				{
					int humanNum = 0;
					int AINum = 0;
					int emptyNum = 0;
					for(int step = -4;step <= 4;++step)
					{
						int curX = i+step*dirx[k];
						int curY = j+step*diry[k];
						
						if(curX>=1 && curX<=N && curY>=1 && curY<=N)
						{
							if(chessboard[curX][curY] == 0) emptyNum++;
							if(chessboard[curX][curY] == 1) humanNum++;
							if(chessboard[curX][curY] == 2) AINum++;
						}
					
					if(humanNum == 1)                      // 杀二
							score[i][j] += 100;
						else if (humanNum == 2)                 // 杀三
						{
								if (emptyNum == 1)
									score[i][j] += 30;
								else if (emptyNum == 2)
									score[i][j] += 40;
						}
						else if (humanNum == 3)                 // 杀四
						{
							// 量变空位不一样，优先级不一样
							if (emptyNum == 1)
								score[i][j] += 60;
							else if (emptyNum == 2)
								score[i][j] += 110;
						}
						else if (humanNum == 4)                 // 杀五
							score[i][j] += 10100;

						if (AINum == 0)                      // 普通下子
							score[i][j] += 5;
						else if (AINum == 1)                 // 活二
							score[i][j] += 10;
						else if (AINum == 2)
						{
							if (emptyNum == 1)                // 死三
								score[i][j] += 25;
							else if (emptyNum == 2)
								//score[i][j] += 50;  // 活三
								score[i][j] += 5000;  // 活三
						}
						else if (AINum == 3)
						{
							if (emptyNum == 1)                // 死四
								score[i][j] += 55;
							else if (emptyNum == 2)
								//score[i][j] += 100; // 活四
								score[i][j] += 11000; // 活四
						}
						else if (AINum >= 4)
							score[i][j] += 10000;   // 活五
					}
				}
			}
		}
}

int judge(int x, int y)
{
	int i, j;
	int t = 2 - whoseTurn % 2;
	const int step[4][2]={{-1,0},{0,-1},{1,1},{1,0}};
    for(int i=0;i<4;++i)
    {
        const int d[2]={-1,1};
        int count=1;
        for(int j=0;j<2;++j)
            for(int k=1;k<=4;++k){
                int row=x+k*d[j]*step[i][0];
                int col=y+k*d[j]*step[i][1];
                if(row>=1&&row<=N&&col>=1&&col<=N&&chessboard[x][y]==chessboard[row][col])
                    count+=1;
                else
                    break;
            }
        if(count>=5)
            return 1;
    }
	return 0;

}