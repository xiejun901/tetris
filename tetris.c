#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <sys/time.h>
#define width 10
#define height 20
typedef struct{
	int loc_x;
	int loc_y;
	int shape_m;
	int shape_n;
} TetrisInfo;
int TetrisShape[7][4][8]={
	{{0,0,1,0,1,1,2,1},{0,0,0,1,-1,1,-1,2},{0,0,1,0,1,1,2,1},{0,0,0,1,-1,1,-1,2}},
	{{0,0,1,0,0,1,-1,1},{0,0,0,1,1,1,1,2},{0,0,1,0,0,1,-1,1},{0,0,0,1,1,1,1,2}},
	{{0,0,0,1,0,2,0,3},{0,0,1,0,2,0,3,0},{0,0,0,1,0,2,0,3},{0,0,1,0,2,0,3,0}},
	{{0,0,1,0,1,1,1,2},{0,0,1,0,2,0,0,1},{0,0,0,1,0,2,1,2},{0,0,0,1,-1,1,-2,1}},
	{{0,0,1,0,0,1,0,2},{0,0,0,1,1,1,2,1},{0,0,0,1,0,2,-1,2},{0,0,0,1,1,1,2,1}},
	{{0,0,1,0,0,1,1,1},{0,0,1,0,0,1,1,1},{0,0,1,0,0,1,1,1},{0,0,1,0,0,1,1,1}},
	{{0,0,0,1,-1,1,1,1},{0,0,0,1,0,2,-1,1},{0,0,1,0,-1,0,0,1},{0,0,0,1,1,1,0,2}}
};
int GameRegion[height][width]={0};
int score;
TetrisInfo CurrentTetris;
void DrawGameRegion(void);
void AddShape(int a[height][width],TetrisInfo Tetristemp);
void DelShape(int a[height][width],TetrisInfo Tetristemp);
void ShapeMove(int a[height][width], unsigned char dir);
void IsScore(int a[height][width]);
void irq_time(int);

int main(void)
{
	int i, j;
	unsigned char ch;
	struct itimerval value,ovalue;
	value.it_value.tv_sec=1;
	value.it_value.tv_usec=0;
	value.it_interval.tv_sec=1;
	value.it_interval.tv_usec=0;
	CurrentTetris.loc_x = 5;
	CurrentTetris.loc_y = 0;
	CurrentTetris.shape_m =1;
	CurrentTetris.shape_n =2;
	score=0;
	signal(SIGALRM,irq_time);
	setitimer(ITIMER_REAL,&value,&ovalue);
	initscr();
	clear();
	i = 0;
	j = 0;
	AddShape(GameRegion,CurrentTetris);
	DrawGameRegion();
	while (1)
	{

		ch=getch();
		if(ch=='q')
			break;
		else
			ShapeMove(GameRegion, ch);
		DrawGameRegion();
	}
	refresh();
	getch();
	endwin();
	return	0;
}

void DrawGameRegion(void)
{
	int i,j;
	clear();
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			if(GameRegion[i][j])
			{
				standout();
				mvaddch(i,j,' ');
			}
			else
			{
				standend();
				mvaddch(i,j,' ');
			}
		}
	}
	mvprintw(22,5,"%d",score);
	standend();
	refresh();
}

void AddShape(int a[height][width],TetrisInfo Tetristemp)
{
	int* p;
	int x,y;
	p=(int *)(&TetrisShape[Tetristemp.shape_m][Tetristemp.shape_n][0]);
	x=Tetristemp.loc_x;
	y=Tetristemp.loc_y;
	a[y+*(p+1)][x+*(p)]=1;
	a[y+*(p+3)][x+*(p+2)]=1;
	a[y+*(p+5)][x+*(p+4)]=1;
	a[y+*(p+7)][x+*(p+6)]=1;
}
void DelShape(int a[height][width],TetrisInfo Tetristemp)
{
	int* p;
	int x,y;
	p=(int *)(&TetrisShape[Tetristemp.shape_m][Tetristemp.shape_n][0]);
	x=Tetristemp.loc_x;
	y=Tetristemp.loc_y;
	a[y+*(p+1)][x+*(p)]=0;
	a[y+*(p+3)][x+*(p+2)]=0;
	a[y+*(p+5)][x+*(p+4)]=0;
	a[y+*(p+7)][x+*(p+6)]=0;
}

void ShapeMove(int a[height][width], unsigned char dir)
{
	int x1,x2,x3,x4,y1,y2,y3,y4;
	int i=0;
	int* p;
	TetrisInfo PreTetris;
	PreTetris=CurrentTetris;
	DelShape(a,PreTetris);
	switch (dir)
	{
	case 'a':
	{
				CurrentTetris.loc_x--;
				break;
	}
	case 'd':
	{
				CurrentTetris.loc_x++;
				break;
	}
	case 's':
	{
				CurrentTetris.loc_y++;
				break;
	}
	case 'w':
	{
				CurrentTetris.loc_y--;
				break;
	}
	case ' ':
	{
				CurrentTetris.shape_n=(CurrentTetris.shape_n+1)%4;
				break;
	}
	default:
		break;
	}
	p=(int *)(&TetrisShape[CurrentTetris.shape_m][CurrentTetris.shape_n][0]);
   	x1 = CurrentTetris.loc_x + *(p+0);
	y1 = CurrentTetris.loc_y + *(p+1);
	x2 = CurrentTetris.loc_x + *(p+2);
	y2 = CurrentTetris.loc_y + *(p+3);
	x3 = CurrentTetris.loc_x + *(p+4);
	y3 = CurrentTetris.loc_y + *(p+5);
	x4 = CurrentTetris.loc_x + *(p+6);
	y4 = CurrentTetris.loc_y + *(p+7);
	if(a[y1][x1]||a[y2][x2]||a[y3][x3]||a[y4][x4]||(x1<0)||(x2<0)||(x3<0)||(x4<0)||(x1>=width)||(x2>=width)||(x3>=width)||(x4>=width)
												   ||(y1<0)||(y2<0)||(y3<0)||(y4<0)||(y1>=height)||(y2>=height)||(y3>=height)||(y4>=height)
													)
	{	
		CurrentTetris=PreTetris;
		if(dir=='s')
		{
			AddShape(a,CurrentTetris);
			IsScore(a);
			CurrentTetris.loc_x=5;
			CurrentTetris.loc_y=0;
			CurrentTetris.shape_m=rand()%7;	
			CurrentTetris.shape_n=rand()%4;	
		}
	}
	AddShape(a,CurrentTetris);
}
void IsScore(int a[height][width])
{
	int i;
	int j;
	int k;
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			if(a[i][j]==0)
				break;
		}
		if(j==width)
		{
			score++;
			for(k=i;k>0;k--)
			{
				for(j=0;j<width;j++)
					a[k][j]=a[k-1][j];
			}
		}

	}
}

void irq_time(int signo)
{
	ShapeMove(GameRegion,'s');
	DrawGameRegion();

}

//void GenNewShape(int a[height][width])
//{
//	CurrentShapeInfo CurrentShape;
//	CurrentShape.x=5;
//	CurrentShape.y=0;
//	CurrentShape.p=(int*)(&shape[rand()%2][rand()%3]);
//}

