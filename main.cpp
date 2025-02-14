#include<iostream>
#include<thread>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
using namespace std;
bool gameover;
const int width = 20;
const int height = 20;
int x,y,fruitX,fruitY,score;
int tailX[100],tailY[100];
int nTail;
enum direction{STOP = 0, LEFT, RIGHT, UP, DOWN};
direction dir;

int getch(void) {
    struct termios oldt, newt;
    int ch;
    
    // Get current terminal attributes
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    
    // Disable canonical mode and echo
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    // Read a single character
    ch = getchar();
    
    // Restore original terminal attributes
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    
    return ch;
}


int kbhit() {
    struct termios oldt, newt;
    int ch;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

void setup()
{
    gameover = false;
    dir = STOP;
    x = width/2;
    y = height/2;
    score = 0;
}
void fruit()
{
    fruitX = rand()%width;
    fruitY = rand()%height;
}
void draw()
{
    system("clear");
    cout<<"\t\t\tT H E  S N A K E  G A M E"<<endl;
    cout<<"PRESS:\nW to move UP"<<endl;
    cout<<"S to move DOWN"<<endl;
    cout<<"A to move LEFT"<<endl;
    cout<<"D to move RIGHT"<<endl;
    cout<<"X to move EXIT"<<endl;


    cout<<"\t\t\t ";
    for (int i = 0; i < width+2; i++){ cout<<"#"; }
    cout<<endl;

    for (int i = 0; i < height; i++)
    {
        cout<<"\t\t\t ";
        for (int j = 0; j < width; j++)
        {
            if(j == 0){cout<<"#";}

            if(i == y && j == x){cout<<"O";}
            else if(i == fruitY && j == fruitX){cout<<"F";}
            else
            {
                bool print = false;
                for (int k = 0; k < nTail; k++)
                {
                    if(tailX[k] == j && tailY[k] == i)
                    {
                        cout<<"o";
                        print = true;
                    }
                }
                if(!print){cout<<" ";}
                
            }

            if(j == width-1){cout<<"#";}
        }
        cout<<endl;
    }
    
    cout<<"\t\t\t ";
    for (int i = 0; i < width+2; i++){ cout<<"#"; }
    cout<<endl;
    cout<<"SCORE:"<<score<<endl;
}
void input()
{
    if(kbhit())
    {
        switch (getch())
        {
            case 'a': dir = LEFT;break;
            case 'w': dir = UP;break;
            case 'd': dir = RIGHT;break;
            case 's': dir = DOWN;break;
            case 'x': gameover = true;break;
        }
    }
}
void logic()
{
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X ,prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < nTail; i++)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    

    switch(dir)
    {
        case LEFT: x--; break;
        case RIGHT: x++; break;
        case UP: y--; break;
        case DOWN: y++; break;
    }
    if(/*x > width || x < 0 ||*/ y > height || y < 0){ gameover = true;}
    if(x >= width){x = 0;} else if(x < 0){x = width - 1;}
    for ( int i = 0; i < nTail; i++)
    {
        if(tailX[i] == x && tailY[i] == y){gameover = true;}
    }
    
    if( x == fruitX && y == fruitY){ score += 10; fruit(); nTail++;}

}
int main(){
    setup();
    fruit();
    while (!gameover)
    {
        draw();
        input();
        logic();
        this_thread::sleep_for(chrono::milliseconds(10));
    }
    
    return 0;
}