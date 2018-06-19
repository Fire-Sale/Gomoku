#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N	15
#define DEPTH 6

int chessboard[N + 1][N + 1] = { 0 };
int dirx[9]={0,0,1,0,-1,1,1,-1,-1};
int diry[9]={0,1,0,-1,0,1,-1,-1,1};
int score[N+1][N+1]={};
int human[10]={},AI[10]={},empty[10]={};
int past[10]={};
int whoseTurn = 0, OptX, OptY;
int tot = 0;
void initGame();
void printChessboard();
void playChess();
int EvaluateScore();
int judge(int, int);
int MAX(int, int, int, int, int);
int MIN(int, int, int, int, int);
int MAX_MIN(int);

int main()
{
	initGame();
	printf("If you want to play black, please input 1.");
	printf("If you want to play white, please input 2.");
	scanf("%d", &whoseTurn);
	if(whoseTurn == 1) whoseTurn -=1 ;
	else if(whoseTurn == 2) whoseTurn -=3;
	else printf("Wrong mode. Enter Crtl+C to exit.");
	while (1)
	{
		whoseTurn++;
		playChess();
		tot = 0;
	}
	return 0;
}

void initGame()
{
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
				printf("%4d", j);
			else if (j == 0)
				printf("%4d", i);
			else if (1 == chessboard[i][j])
				printf("   ●");
			else if (2 == chessboard[i][j])
				printf("   ○");
			else
				printf("   .");

		}
		printf("\n");
		printf("\n");
	}
	printf("%d\n", tot);
}

void playChess()
{
	int i, j, winner;
	if (1 == whoseTurn % 2)
	{
		printf("Turn to player 1, please input the position:");
		scanf("%d %d", &i, &j);
		if(i==0)
		{
			chessboard[past[1]][past[2]] = chessboard[past[3]][past[4]] = 0;
			system("clear");
			printChessboard();
			printf("Turn to player 1, please input the position again:");
			scanf("%d %d", &i, &j);
		}
		while(chessboard[i][j] != 0)
		{
			printf("your position is taken,choose another:"); 
			scanf("%d %d", &i, &j);
		}
		chessboard[i][j] = 1;
		past[3] = i;
		past[4] = j;
	}
	else
	{
		printf("Turn to player 2\n");
		int ans = MAX_MIN(1);
		i = ans/(N+1);
		j = ans-i*(N+1);
		if(whoseTurn == 0)
		{
			i = (N+1)/2;
			j = (N+1)/2;
		}
		past[1] = i;
		past[2] = j;
		chessboard[i][j] = 2;
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

int MAX_MIN(int deep)
{
	int best = -1e9;
	int bestPos;
	int pos[200];
	int numPos = 0;
	for(int i=2;i<=N-1;i++)
		for(int j=2;j<=N-1;j++)
		{
			// if(chessboard[i+1][j] != 0)  pos[++numPos] = (i+1)*(N+1)+j;
			// if(chessboard[i+1][j+1] != 0)  pos[++numPos] = (i+1)*(N+1)+j+1;
			// if(chessboard[i+1][j-1] != 0)  pos[++numPos] = (i+1)*(N+1)+j-1;

			// if(chessboard[i][j+1] != 0)  pos[++numPos] = (i)*(N+1)+j+1;
			// if(chessboard[i][j-1] != 0)  pos[++numPos] = (i)*(N+1)+j-1;

			// if(chessboard[i-1][j] != 0)  pos[++numPos] = (i-1)*(N+1)+j;
			// if(chessboard[i-1][j+1] != 0)  pos[++numPos] = (i-1)*(N+1)+j+1;
			// if(chessboard[i-1][j-1] != 0)  pos[++numPos] = (i-1)*(N+1)+j-1;

			if(chessboard[i+1][j] != 0 || chessboard[i-1][j] != 0 || chessboard[i][j+1] != 0 || chessboard[i][j-1] != 0 || 
			   chessboard[i+1][j+1] != 0 || chessboard[i+1][j-1] != 0 || chessboard[i-1][j+1] != 0 || chessboard[i-1][j-1] != 0)
				pos[++numPos] = i*(N+1)+j;

		}
	for(int i=1;i<=numPos;i++)
	{
		int x = pos[i]/(N+1);
		int y = pos[i]-x*(N+1);
		if(chessboard[x][y] == 0)
			chessboard[x][y] = 0;
		else
			continue;
		int mark = MIN(deep+1, x, y, 1e9, best > -1e9 ? best : -1e9);
		chessboard[x][y] = 0;
		if(mark > best) 
		{
			best = mark;
			bestPos = pos[i];
		}
	}
	return bestPos;
}

int MIN(int deep, int x, int y, int alpha, int beta)
{
	int mark = EvaluateScore(x, y);
	chessboard[x][y] = 2;
	if(deep>=DEPTH || judge(x, y))
		return mark;
	tot++;
	int best = 1e9;
	int pos[200];
	int numPos = 0;
	for(int i=2;i<=N-1;i++)
		for(int j=2;j<=N-1;j++)
		{
			if(chessboard[i+1][j] != 0 || chessboard[i-1][j] != 0 || chessboard[i][j+1] != 0 || chessboard[i][j-1] != 0 || 
			   chessboard[i+1][j+1] != 0 || chessboard[i+1][j-1] != 0 || chessboard[i-1][j+1] != 0 || chessboard[i-1][j-1] != 0)
				pos[++numPos] = i*(N+1)+j;
		}
	for(int i=1;i<=numPos;i++)
	{
		int x = pos[i]/(N+1);
		int y = pos[i]-x*(N+1);
		if(chessboard[x][y] == 0)
			chessboard[x][y] = 0;
		else
			continue;
		int score = MAX(deep+1, x, y, score < alpha ? score : alpha, beta);
		chessboard[x][y] = 0;
		if(score < best) 
		{
			best = score;
		}
		if(score < beta)
		{
			//cut++;
			break;
		}
	}
	return best;
}

int MAX(int deep, int x, int y, int alpha, int beta)
{
	int score = EvaluateScore(x, y);
	chessboard[x][y] = 1;
	if(deep>=DEPTH || judge(x,y))
		return score;
	tot++;
	int best = -1e9;
	int pos[200];
	int numPos = 0;
	for(int i=2;i<=N-1;i++)
		for(int j=2;j<=N-1;j++)
		{
			if(chessboard[i+1][j] != 0 || chessboard[i-1][j] != 0 || chessboard[i][j+1] != 0 || chessboard[i][j-1] != 0 || 
			   chessboard[i+1][j+1] != 0 || chessboard[i+1][j-1] != 0 || chessboard[i-1][j+1] != 0 || chessboard[i-1][j-1] != 0)
				pos[++numPos] = i*(N+1)+j;
		}
	for(int i=1;i<=numPos;i++)
	{
		int x = pos[i]/(N+1);
		int y = pos[i]-x*(N+1);
		if(chessboard[x][y] == 0)
			chessboard[x][y] = 0;
		else
			continue;
		int score = MIN(deep+1, x, y, alpha, score > beta ? score : beta);
		chessboard[x][y] = 0;
		if(score > best) 
		{
			best = score;
		}
		if(score > alpha)
		{
			//cut ++;
			break;
		}
	}
	return best;
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
            for(int k=1;k<=4;++k)
			{
                int row=x+k*d[j]*step[i][0];
                int col=y+k*d[j]*step[i][1];
                if(row >=1 && row <=N && col >=1 && col <= N && chessboard[x][y] == chessboard[row][col])
                    count += 1;
                else
                    break;
            }
        if(count >= 5)
            return 1;
    }
	return 0;
}

// int EvaluateScore(int x, int y)
// {
// 	for(int k=1;k<=8;k++)
// 	{
// 		int humanNum = 0;
// 		int AINum = 0;
// 		int emptyNum = 0;
	
// 		for(int step = -2;step <= 4;++step)
// 		{
// 			int curX = x+step*dirx[k];
// 			int curY = y+step*diry[k];

// 			if(curX>=1 && curX<=N && curY>=1 && curY<=N)
// 			{
// 				if(chessboard[curX][curY] == 0) empty[++emptyNum] = step;
// 				if(chessboard[curX][curY] == 1) human[++humanNum] = step;
// 				if(chessboard[curX][curY] == 2) AI[++AINum] = step;
// 			}

// 			if(curX>=1 && curX<=N && curY>=1 && curY<=N)
// 			{
// 				if(chessboard[curX][curY] == 0) empty[++emptyNum] = step;
// 				if(chessboard[curX][curY] == 1) human[++humanNum] = step;
// 				if(chessboard[curX][curY] == 2) AI[++AINum] = step;
// 			}

// 			if(humanNum == 5) score[x][y] += 101000;
// 			if(humanNum == 4) score[x][y] += 101000;
// 		}
// 	}
// 	return score[x][y];
// }

int EvaluateScore(int x, int y)
{
	int mark = 0;
	for(int k=1;k<=8;k++)
	{
		int humanNum = 0;
		int AINum = 0;
		int emptyNum = 0;
		for(int step = 0;step <= 4;++step)
		{
			int curX = x+step*dirx[k];
			int curY = y+step*diry[k];
			
			if(curX>=1 && curX<=N && curY>=1 && curY<=N)
			{
				if(chessboard[curX][curY] == 0) emptyNum++;
				if(chessboard[curX][curY] == 1) humanNum++;
				if(chessboard[curX][curY] == 2) AINum++;
			}

			if(humanNum == 1)                      // 杀二
				 mark += 10;
			else if (humanNum == 2)                 // 杀三
			{
				if (emptyNum == 1)
					 mark += 30;
				else if (emptyNum == 2)
					 mark += 60;
			}
			else if (humanNum == 3)                 // 杀四
			{
				if (emptyNum == 1)
					mark += 600;
				else if (emptyNum == 2)
					 mark += 1100;
			}
			else if (humanNum == 4)                 // 杀五
				 mark += 10100;
			// else if (humanNum == 5)                 // 杀五
			// 	score[x][y] += 101000;



			if (AINum == 0)                      // 普通下子
				mark += 5;
			else if (AINum == 1)                 // 活二
				mark += 10;
			else if (AINum == 2)
			{
				if (emptyNum == 1)                // 死三
					mark += 25;
				else if (emptyNum == 2)
					//score[x][y] += 50;  // 活三
					mark += 50;  // 活三
			}
			else if (AINum == 3)
			{
				if (emptyNum == 1)                // 死四
					mark += 550;
				else if (emptyNum == 2)
					//score[x][y] += 100; // 活四
					mark += 1000; // 活四
			}
			else if (AINum == 4)
				mark += 10000;   // 活五
			// else if (AINum == 5)
			// 	score[x][y] += 100000;   // 活五
		}
	}
	return mark;
}