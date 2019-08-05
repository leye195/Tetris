/*
73211101
Tetris
*/
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#define randomize()(srand((unsigned)time(NULL)))
#define random(n)(rand()%n)
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ESC  27
#define BX 5
#define BY 1
#define BW 10
#define BH 20

void gotoxy(int x,int y);
void Set_Cursor();
void SetText_Color(int n);
void DrawScreen();
void DrawBoard();
int ProcessKey();
int GetAround(int x,int y,int b,int r);
int MoveDown();
void Print_block(int show);
void Show_Next_block();
void TestFull();
void How_to_play();
int Check_Play_Again();

struct Point{
    int x,y;
};

struct Point Shape[7][4][4]={
  {{{0,0},{1,0},{2,0},{-1,0}},{{0,0},{0,1},{0,-1},{0,-2}},{{0,0},{1,0},{2,0},{-1,0}},{{0,0},{0,1},{0,-1},{0,-2}}},
  {{{0,0},{1,0},{0,1},{1,1}},{{0,0},{1,0},{0,1},{1,1}},{{0,0},{1,0},{0,1},{1,1}},{{0,0},{1,0},{0,1},{1,1}}},
  {{{0,0},{-1,0},{0,-1},{1,-1}},{{0,0},{0,1},{-1,0},{-1,-1}},{{0,0},{-1,0},{0,-1},{1,-1}},{{0,0},{0,1},{-1,0},{-1,-1}}},
  {{{0,0},{-1,-1},{0,-1},{1,0}},{{0,0},{0,1},{1,0},{1,-1}},{{0,0},{-1,-1},{0,-1},{1,0}},{{0,0},{0,1},{1,0},{1,-1}}},
  {{{0,0},{-1,0},{1,0},{-1,-1}},{{0,0},{0,1},{0,-1},{-1,1}},{{0,0},{-1,0},{1,0},{1,1}},{{0,0},{0,1},{0,-1},{1,-1}}},
  {{{0,0},{1,0},{-1,0},{1,-1}},{{0,0},{0,1},{0,-1},{-1,-1}},{{0,0},{-1,0},{1,0},{-1,1}},{{0,0},{0,1},{0,-1},{1,1}}},
  {{{0,0},{-1,0},{1,0},{0,1}},{{0,0},{0,1},{0,-1},{1,0}},{{0,0},{-1,0},{1,0},{0,-1}},{{0,0},{0,1},{0,-1},{-1,0}}}
};
//enum{NOCURSOR,SOLIDCURSOR,NORMALCURSOR};

enum{EMPTY,BRICK,WALL};
char*arTile[]={" ","O","W"};
int board[BW+10][BH+10];//size of Board
int nx,ny;//current position
int brick,rot,next_brick;
int Score;

int main()
{
    int nframe,nstay;
    int x,y;
    int choice;

    printf(" Welcome to BH_Tetris!!!!!!! \n");
    printf(" 1.Start 2.Quit: ");
    scanf("%d",&choice);

    if(choice==1)
    {
    for(;;)
    {
      Score = 0;
      randomize();
      system("cls");
      Set_Cursor();//remove the Cursor on the screen

      for(x=0;x<BW+2;x++)
      {
          for(y=0;y<BH+2;y++)
          {

              board[x][y]=(y==0 || y==BH+1 || x==0 || x==BW+1)?WALL:EMPTY;
              //Set the Space is Empty or Not
          }
      }
      for(x=BW+2;x<BW+9;x++)
      {
          for(y=0;y<6;y++)
          {

              board[x][y]=(y==0 || y==5 || x==0 || x==BW+8)?WALL:EMPTY;
          }
      }
      DrawScreen();
      nframe = 18;//Set Drop speed
      next_brick=random(sizeof(Shape)/sizeof(Shape[0]));//Prepare next brick

      for(;;)
      {

        brick = next_brick;
        nx = BW/2;
        ny = 3;
        rot = 0;
        SetText_Color(-1);
        Print_block(1);
        next_brick=random(sizeof(Shape)/sizeof(Shape[0]));
        Show_Next_block(1);

        if(GetAround(nx,ny,brick,rot)!=EMPTY)break;
        nstay=nframe;//Set the Speed of this Game
        for(;;)
        {
            if(--nstay==0)
            {
                nstay = nframe;
                if(MoveDown())break;
            }
            if(ProcessKey())break;
            Sleep(1000/20);
        }
        Show_Next_block(0);//Show the next bricks on the screen
      }
      if(Check_Play_Again()==0)
          break;

    }
    }
    system("cls");
    return 0;
}

void gotoxy(int x,int y)
{
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}

void Set_Cursor()
{
    CONSOLE_CURSOR_INFO cur;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cur);
    cur.bVisible=0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cur);
}
void SetText_Color(int n)
{
    if(n==-1)
    {
        randomize();
        n = random(9)+5;
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),n);
}
void DrawScreen()
{//Print_Block(1);
    int x,y;

    for(x=0;x<BW+10;x++)
    {
        for(y=0;y<BH+7;y++)
        {
            gotoxy(BX+x*2,BY+y);
            puts(arTile[board[x][y]]);
        }
    }

    How_to_play();
    gotoxy(5,23);SetText_Color(15);
    printf("Score: %d",Score);
}
void DrawBoard()
{
    int x,y;
    for(x=1;x<BW+1;x++)
    {
        for(y=1;y<BH+1;y++)
        {
            gotoxy(BX+x*2,BY+y);
            puts(arTile[board[x][y]]);
        }
    }
    Score+=100;
    gotoxy(5,23);SetText_Color(15);
    printf("Score: %d",Score);
}
int ProcessKey()
{
    int ch,trot;
    if(_kbhit())
    {
        ch=_getch();
        if(ch==0 || ch==0xe0)
        {
            ch = _getch();
            switch(ch)
            {
            case UP:
                trot = (rot==3?0:rot+1);
                if(GetAround(nx,ny,brick,trot)==EMPTY)
                {
                    Print_block(0);
                    rot = trot;
                    Print_block(1);
                }
                break;
            case DOWN:
                if(MoveDown()){
                    return 1;
                }
                break;
            case LEFT:
                if(GetAround(nx-1,ny,brick,rot)==EMPTY)
                {
                    Print_block(0);
                    nx--;
                    Print_block(1);
                }
                break;
            case RIGHT:
                if(GetAround(nx+1,ny,brick,rot)==EMPTY)
                {
                    Print_block(0);
                    nx++;
                    Print_block(1);
                }
                break;
            }
        }
        else
        {
            switch(ch)
            {
                case ' ':
                    while(MoveDown()==0){;}
                    return 1;
                case ESC:
                    system("cls");
                    printf("Good Bye~~\n");
                    exit(0);
                break;
            }
        }
    }

    return 0;
}
int GetAround(int x,int y,int b,int r)
{
    int i,k=EMPTY;

    for(i=0;i<4;i++)
    {
        k=max(k,board[x+Shape[b][r][i].x][y+Shape[b][r][i].y]);
    }
    return k;
}
void Print_block(int show)
{
    int i;

    for(i=0;i<4;i++)
    {
        gotoxy(BX+(Shape[brick][rot][i].x+nx)*2,BY+(Shape[brick][rot][i].y+ny));
        puts(arTile[show?BRICK:EMPTY]);
    }
}
void Show_Next_block(int show)
{
    int i = 0,x =0,y = 0;

    for(i=0;i<4;i++)
    {
        if(show==0)
        {
            for(x=0;x<8;x++)
            {
                for(y=1;y<5;y++)
                {
                    gotoxy(30+x,y+1);
                    SetText_Color(0);
                    puts("   ");
                }
            }
        }
        else
        {
            gotoxy(BX+(Shape[next_brick][rot][i].x+14)*2,BY+(Shape[next_brick][rot][i].y+3));
            puts(arTile[show?BRICK:EMPTY]);
        }
    }
}
int MoveDown()
{
    if(GetAround(nx,ny+1,brick,rot)!=EMPTY)
    {
        TestFull();
        return 1;
    }
    Print_block(0);
    ny++;
    Print_block(1);

    return 0;
}
void TestFull()
{
    int i,x,y,ty;

    for(i=0;i<4;i++)
    {
        board[nx+Shape[brick][rot][i].x][ny+Shape[brick][rot][i].y]=BRICK;
    }
    for(y=1;y<BH+1;y++)
    {
        for(x=1;x<BW+1;x++)
        {
            if(board[x][y]!=BRICK)break;
        }
        if (x == BW+1){
            for (ty=y;ty>1;ty--) {
                for (x=1;x<BW+1;x++) {
                    board[x][ty]=board[x][ty-1];
                }
            }
            DrawBoard();
            Sleep(200);
        }
    }
}

void How_to_play()
{
    gotoxy(40,18);
    printf("Move: Left   Right  ");
    gotoxy(40,20);
    printf("Rotate: Up   Soft Drop: Down");
    gotoxy(40,22);
    printf("Hard Drop: [Space Bar]  Quit: ESC");
}

int Check_Play_Again()
{
    char r;

    system("cls");

    SetText_Color(15);
    printf(" ++++++========Game Over========++++++\n");
    printf(" Score: %d\n",Score);
    printf(" Play Again Y/N ");
    printf(" Y or N?:");

    scanf("%c",&r);
    fflush(stdin);

    if(r=='N'||r=='n')
        return 0;
    return 1;
}
