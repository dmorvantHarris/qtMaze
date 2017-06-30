
#ifndef HELPER_H
#define HELPER_H

#include <QBrush>
#include <QFont>
#include <QPen>
#include <QWidget>

#define WINDOW_WIDTH 1800
#define WINDOW_HEIGHT 900


typedef struct
{
  float x;
  float y;
} Point;

typedef struct
{
  float x;
  float y;
} Vel;

typedef struct
{
  int x;
  int y;
  char wall;
  char visited;
  char solver_visited;
} pixel;

typedef struct
{
  int x;
  int y;
  int last_dir;
}solver;


class Helper
{
public:
    Helper();

public:
    void paint(QPainter *painter, QPaintEvent *event, int elapsed, short *isFirst, Point *points, Vel *velocitys);
    Point myPoints[5000];
    Vel myVels[5000];
    pixel *grid[WINDOW_WIDTH+3];
    solver mySolver;
    solver mySolver2;
    short isFirst = 1;
    long count = 0;

private:
    //QBrush background;
    QBrush circleBrush;
    QBrush solveBrush;
    QFont textFont;
    QPen circlePen;
    QPen solvePen;
    QPen textPen;
    QImage myImage;
    QRgb background;
    QRgb path;
    QRgb solverPath;
    QRgb solverPath2;
    QRgb solverPathCombined;
    void move(Point* p, Vel *v);
    void genMaze();
    void solve(int index);
    bool moveUp(int index);
    bool moveRight(int index);
    bool moveDown(int index);
    bool moveLeft(int index);
    bool moveUp2(int index);
    bool moveRight2(int index);
    bool moveDown2(int index);
    bool moveLeft2(int index);
    bool trueFirst = 1;
};

#endif
