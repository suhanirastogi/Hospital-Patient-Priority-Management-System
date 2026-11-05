#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <ctime>

using namespace std;

// using constant variables
//width
const int width = 30;
//height
const int height = 20;

// ---------------- PLAYER ------------------
class Player
{
    int x;

public:
    Player()
     {
        x = width / 2;
     }
//left control
    void moveLeft()
    {
        if (x > 0) x--;
    }
//right control
    void moveRight()
     {
        if (x < width - 1) x++;
    }

    int getX()
     {
        return x;
    }
};

// ----------------- BULLET------------------
class Bullet
{
    int x, y;
    bool active;

public:
    Bullet()
    {
        active = false;
     }

//shoot control
    void shoot(int px)
    {
        if (!active) {
            x = px;
            y = height - 2;
            active = true;
        }
    }

    void move()
    {
        if (active)
        {
            y--;
            if (y < 0) active = false;
        }
    }

    int getX() { return x; }
    int getY() { return y; }
    bool isActive() { return active; }

    void deactivate() { active = false; }
};

//------------------ ENEMY ----------------
class Enemy
{
    int x, y;

public:
    Enemy()
    {
        reset();
    }

//reset control
    void reset()
    {
        x = rand() % width;
        y = 0;
    }

    void move()
     {
        y++;
        if (y >= height) reset();
    }

    int getX()
     {
        return x;
     }
    int getY()
    {
        return y;
    }
};

// --------------- GAME -------------
class Game
{
    //player
    Player player;

    //bullet
    Bullet bullet;

    vector<Enemy> enemies;


    bool gameOver;
    bool paused;
    int score;

public:
    Game()
    {
        gameOver = false;
        paused = false;
        score = 0;

        for (int i = 0; i < 3; i++)
            enemies.push_back(Enemy());
    }

    void draw()
    {
        system("cls");

        for (int i = 0; i < width + 2; i++) cout << "#";
        cout << endl;

        for (int i = 0; i < height; i++) {
            cout << "#";

            for (int j = 0; j < width; j++)
            {

                if (i == height - 1 && j == player.getX())
                    cout << "^";

                else if (bullet.isActive() &&
                         i == bullet.getY() &&
                         j == bullet.getX())
                    cout << "|";

                else {
                    bool printed = false;

                    for (int k = 0; k < (int)enemies.size(); k++)
                    {
                        if (i == enemies[k].getY() &&
                            j == enemies[k].getX())
                            {
                            cout << "V";
                            printed = true;
                            break;
                            }
                    }

                    if (!printed)
                         cout << " ";
                }
            }
            cout << "#" << endl;
        }

        for (int i = 0; i < width + 2; i++) cout << "#";
        cout << endl;

        cout << "Score: " << score << endl;
        cout << "L=Left R=Right S=Shoot P=Pause X=Exit\n";

        if (paused)
            cout << "PAUSED\n";
    }
//input
    void input()
    {
        if (_kbhit())
        {
            char key = _getch();

            if (key == 'l' || key == 'L') player.moveLeft();
            else if (key == 'r' || key == 'R') player.moveRight();
            else if (key == 's' || key == 'S') bullet.shoot(player.getX());
            else if (key == 'p' || key == 'P') paused = !paused;
            else if (key == 'x' || key == 'X') gameOver = true;
        }
    }
//main logic
    void logic()
    {
        if (paused) return;

        bullet.move();

        for (int i = 0; i < (int)enemies.size(); i++)
        {

            enemies[i].move();

            if (bullet.isActive() &&
                bullet.getX() == enemies[i].getX() &&
                bullet.getY() == enemies[i].getY())
                {

                score++;
                bullet.deactivate();
                enemies[i].reset();
            }

            if (enemies[i].getY() == height - 1 &&
                enemies[i].getX() == player.getX())
                {

                gameOver = true;
            }
        }
    }
//run program
    void run()
    {
        while (!gameOver)
        {
            draw();
            input();
            logic();
            Sleep(150);
        }

        system("cls");
        cout << "GAME OVER\nScore: " << score << endl;
        cout << "Press any key...";
        _getch();
    }
};

//------------------ MENU ----------------
void menu()
{
    while (true)
    {
        system("cls");

        cout << "==== SHOOTING GAME ====\n";
        cout << "1. Start Game\n";
        cout << "2. Exit\n";
        cout << "Enter choice: ";

        char choice = _getch();

        if (choice == '1')
        {
            Game g;
            g.run();
        }
        else if (choice == '2')
        {
            break;
        }
        else
        {
            cout << "\nInvalid choice!";
            Sleep(1000);
        }
    }
}

// --------------MAIN-----------------
int main()
{
    srand(time(0));
    menu();
    return 0;
}
//end of program


