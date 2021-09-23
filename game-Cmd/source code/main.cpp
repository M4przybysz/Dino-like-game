#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <time.h>

using namespace std;

unsigned long long score;
int Ry = 0, np = 0, jclock = 0, npclock = 3;
bool over = false;
char pole[30][3];

void wyczysc()
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 30; j++)
        {
            pole[j][i] = ' ';
        }
}

void wyswietl()
{
    system("cls");
    if (over) cout << "GAME OVER" << endl << endl;

    cout << "Score: " << score << endl << endl;
    cout << "-------------------------------------------------------------" << endl;
    for (int i = 2; i >= 0; i--)
    {
        cout << " ";
        for (int j = 0; j < 30; j++)
        {
            cout << pole[j][i] << " ";
        }
        cout << endl;
    }
    cout << "-------------------------------------------------------------" << endl;
}

void przejscie()
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 29; j++)
        {
            pole[j][i] = pole[j+1][i];
        }

    pole[29][0] = ' ';
    pole[29][2] = ' ';

    if (jclock != 0)
    {
        if (jclock == 1) { Ry = 1; jclock++; }
        else if (jclock == 2) { Ry = 2; jclock++; }
        else if (jclock == 3) jclock++;
        else if (jclock == 4) jclock++;
        else if (jclock == 5) { Ry = 1; jclock++; }
        else if (jclock == 6) { Ry = 0; jclock = 0; }
    }

    if (Ry == 0) { pole[3][0] = 'R'; pole[2][0] = ' '; pole[2][1] = ' '; }
    else if (Ry == 1) { pole[3][1] = 'R'; pole[2][0] = ' '; pole[2][2] = ' '; pole[2][1] = ' '; }
    else if (Ry == 2) { pole[3][2] = 'R'; pole[2][1] = ' '; pole[2][2] = ' '; }

    np = rand() % 3;
    if (npclock == 0)
    {
        if (np == 0) pole[29][2] = 'v';
        if (np == 1) pole[29][0] = '^';
        if (np == 2) { pole[29][0] = '^'; pole[28][0] = '^'; }
        npclock = rand()%10+6;
    }
    npclock--;

    if ((Ry == 0 && pole[4][0] == '^') || (Ry == 2 && pole[4][2] == 'v') || 
        (Ry == 1 && pole[3][0] == '^') || (Ry == 1 && pole[3][2] == 'v')) over = true;
}

void start()
{
    int wait = 0;
    while (wait != 13)
    {
        cout << "Press ENTER to play";
        wait = _getch();
        system("cls");
    }
    system("cls");
}

int main()
{
    srand(time(NULL));
    int XD=0;
    Ry = 0; jclock = 0; score = 0;

    wyczysc();
    start();
    while (XD != 27 && !over)
    {
        while (!_kbhit() && !over)
        {
            wyswietl();
            przejscie();
            Sleep(250);
            score++;
        }
        wyswietl();
        XD = _getch();
        //cout << XD << endl;
        if (XD == 32 && jclock == 0) jclock = 1;
        przejscie();
        Sleep(250);
        score++;
    }
    return 0;
}