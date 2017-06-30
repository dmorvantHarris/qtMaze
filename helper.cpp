#include "helper.h"
#include <stdlib.h>
#include <stdio.h>
#include <stack>
#include <unistd.h>
#include <time.h>
#include <malloc.h>

#include <QPainter>
#include <QPaintEvent>
#include <QWidget>
#include <QImage>



Helper::Helper()
{
    QLinearGradient gradient(QPointF(50, -20), QPointF(80, 20));
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(1.0, QColor(0xff, 0xff, 0xff));

    myImage = QImage(WINDOW_WIDTH,WINDOW_HEIGHT,QImage::Format_RGB32);
    background = qRgb(0,0,0);
    path = qRgb(255,255,255);
    solverPath = qRgb(0,0,255);
    solverPath2 = qRgb(255,0,0);
    solverPathCombined = qRgb(2,255,2);
    //background = QBrush(QColor(0, 0, 0));
    circleBrush = QBrush(Qt::SolidPattern);
    //circlePen = QPen(Qt::black);
    /*circlePen = QPen(Qt::white);
    circlePen.setWidth(1);
    solveBrush = QBrush(Qt::SolidPattern);
    solvePen = QPen(Qt::blue);
    solvePen.setWidth(1);
    textPen = QPen(Qt::white);
    textFont.setPixelSize(50);*/
}

void Helper::paint(QPainter *painter, QPaintEvent *event, int elapsed, short *isFirst, Point *points, Vel *velocitys)
{
    //painter->fillRect(event->rect(), background);
    //painter->translate(300, 300);

    //painter->save();
    //painter->setBrush(circleBrush);
    //painter->setPen(circlePen);
  if(*isFirst)
  {
    //printf("First loop\n"); 
    srand(time(NULL));
    genMaze();
    trueFirst = 0;
    mySolver.x = 0;
    mySolver.y = 0;
    mySolver.last_dir = 0;
    mySolver2.x = 0;
    mySolver2.y = 0;
    mySolver2.last_dir = 0;
    *isFirst = 0;
  }
  else
  {
    for (int i=0; i < 2; i++)
    {
      //printf("Solving 0 \n");
      solve(0);
      //printf("Solving 1 \n");
      solve(1);
    }
  }
  count ++; 
   
  /*if (count)
  {  
    for(int i=0; i < 300; i++)
    {
      for(int j=0; j < 300; j++)
      {
        if (grid[i][j].solver_visited)
        {
          painter->setBrush(solveBrush);
          painter->setPen(solvePen);
          painter->drawRect(1+i*2, 1+j*2, 2, 2);
          painter->setBrush(circleBrush);
          painter->setPen(circlePen);
        }
        else if(grid[i][j].wall)
        {
          painter->drawRect(1+i*2, 1+j*2, 2, 2);
        }
      }
    }
  }*/
  //painter->begin(&myImage);
  painter->drawImage(0,0,myImage);
  //painter->end();

  //printf("Paint done\n");
  //painter->restore();

  //painter->setPen(textPen);
  //painter->setFont(textFont);
  //painter->drawText(QRect(-50, -50, 100, 100), Qt::AlignCenter, QStringLiteral("Qt"));
}

void Helper::move(Point *p, Vel *v)
{
  //printf("%d %d\n",v->x,v->y);
  p->x = p->x + v->x; 
  if(p->x >= WINDOW_WIDTH || p->x < 0)
  {
    v->x = -v->x + v->x*0.5;
    if(p->x >= WINDOW_WIDTH)
    {
      p->x = WINDOW_WIDTH;
    }
    if(p->x < 0 )
    {
      p->x = 0;
    }
  }
  
  
  p->y = p->y + v->y; 
  if(p->y >= WINDOW_HEIGHT || p->y < 0)
  {
    v->y = -v->y + v->y*0.5;
    if(p->y >= WINDOW_HEIGHT)
    {
      p->y = WINDOW_HEIGHT;
    }
    if(p->y < 0 )
    {
      p->y = 0;
    }
  }
}

void Helper::genMaze()
{
  printf("Gen MAZE\n");
  std::stack<pixel*> myStack;
  pixel *curPixel;
  int dir;
  for (int i=0; i < WINDOW_WIDTH; i++)
  {
    if(trueFirst)
    {
      grid[i] = (pixel*)malloc(sizeof(pixel)*(WINDOW_HEIGHT));
    }
    for (int j=0; j < WINDOW_HEIGHT; j++)
    {
      grid[i][j].x = i;
      grid[i][j].y = j;
      grid[i][j].wall = 0;
      grid[i][j].visited = 0;
      grid[i][j].solver_visited = 0;
      myImage.setPixel(i,j,background);
    }
  }
  printf("grid Initilized\n");
  grid[0][0].wall = 1;
  grid[0][0].visited = 1;
  myStack.push(&grid[0][0]);
  while(myStack.size() > 0)
  {
    //printf("Loop start\n");
    curPixel = myStack.top();
    //printf("%d:%d\n", curPixel->x, curPixel->y);
    if(   (curPixel->y - 2 <= 0 || grid[curPixel->x][curPixel->y-2].visited) 
       && (curPixel->y + 2 >= WINDOW_HEIGHT || grid[curPixel->x][curPixel->y+2].visited)
       && (curPixel->x - 2 <= 2 || grid[curPixel->x-2][curPixel->y].visited)
       && (curPixel->x + 2 >= WINDOW_WIDTH || grid[curPixel->x+2][curPixel->y].visited))
    {
    //  printf("Popped\n");
      myStack.pop();
    }
    dir = (rand()%4); 
    //printf("%d\n", dir);
    //printf("%d:%d:%d:%d\n", grid[curPixel->x][curPixel->y-2].visited, grid[curPixel->x][curPixel->y+2].visited, grid[curPixel->x-2][curPixel->y].visited, grid[curPixel->x+2][curPixel->y].visited);
    curPixel->visited = 1;
    // jumping 2 
    if(dir == 0 && curPixel->y-1 > 0)
    {
      if (!grid[curPixel->x][curPixel->y-2].visited)
      {
	//printf("Pushed 0\n");
        grid[curPixel->x][curPixel->y-2].wall = 1;
        myImage.setPixel(curPixel->x,curPixel->y-2,path);
        grid[curPixel->x][curPixel->y-1].wall = 1;
        myImage.setPixel(curPixel->x,curPixel->y-1,path);
        myStack.push(&grid[curPixel->x][curPixel->y-2]);
      }
    }
    else if(dir == 1 && curPixel->x+2 < WINDOW_WIDTH)
    {
      if (!grid[curPixel->x+2][curPixel->y].visited)
      {
	//printf("Pushed 1\n");
        grid[curPixel->x+1][curPixel->y].wall = 1;
        myImage.setPixel(curPixel->x+1,curPixel->y,path);
        grid[curPixel->x+2][curPixel->y].wall = 1;
        myImage.setPixel(curPixel->x+2,curPixel->y,path);
        myStack.push(&grid[curPixel->x+2][curPixel->y]);
      }
    }
    else if(dir == 2 && curPixel->y+2 < WINDOW_HEIGHT)
    {
      if (!grid[curPixel->x][curPixel->y+2].visited)
      {
   //printf("Pushed 2\n");
        grid[curPixel->x][curPixel->y+2].wall = 1;
        myImage.setPixel(curPixel->x,curPixel->y+2,path);
        grid[curPixel->x][curPixel->y+1].wall = 1;
        myImage.setPixel(curPixel->x,curPixel->y+1,path);
        myStack.push(&grid[curPixel->x][curPixel->y+2]);
      }
    }
    else if(dir == 3 && curPixel->x-1 > 0)
    {
      if (!grid[curPixel->x-2][curPixel->y].visited)
      {
	//printf("Pushed 3\n");
        grid[curPixel->x-1][curPixel->y].wall = 1;
        myImage.setPixel(curPixel->x-1,curPixel->y,path);
        grid[curPixel->x-2][curPixel->y].wall = 1;
        myImage.setPixel(curPixel->x-2,curPixel->y,path);
        myStack.push(&grid[curPixel->x-2][curPixel->y]);
      }
    }
    //printf("\r%d",myStack.size());
    //fflush(stdout); 
  }
   
  printf("Generation Complete\n");
}

// follow last wall
void Helper::solve(int index)
{
  if((mySolver.x >= WINDOW_WIDTH-2 && mySolver.y >= WINDOW_HEIGHT-2) ||
      (mySolver2.x >= WINDOW_WIDTH-2 && mySolver2.y >= WINDOW_HEIGHT-2))
  {
    if (mySolver2.x >= WINDOW_WIDTH-2 && mySolver2.y >= WINDOW_HEIGHT-2)
    {
      printf("Red First\n");
    }
    else
    {
      printf("Blue First\n");
    }
    //sleep(1);
    isFirst = 1;
    return;
  } 
  //last_dir is last direction
  if (index == 0)
  {
    if(mySolver.last_dir == 0)
    {
      if (!moveRight(index))
      {
        if(!moveUp(index))
        {
          if(!moveLeft(index))
          {
            moveDown(index);
          }
        }
      }
    }
    else if(mySolver.last_dir == 1)
    {
      if (!moveDown(index))
      {
        if(!moveRight(index))
        {
          if(!moveUp(index))
          {
            moveLeft(index);
          }
        }
      }
    }
    else if(mySolver.last_dir == 2)
    {
      if (!moveLeft(index))
      {
        if(!moveDown(index))
        {
          if(!moveRight(index))
          {
            moveUp(index);
          }
        }
      }
    }
    else if(mySolver.last_dir == 3)
    {
      if (!moveUp(index))
      {
        if(!moveLeft(index))
        {
          if(!moveDown(index))
          {
            moveRight(index);
          }
        }
      }
    }
  }
  else
  {
    if(mySolver2.last_dir == 0)
    {
      if (!moveLeft2(index))
      {
        if(!moveUp2(index))
        {
          if(!moveRight2(index))
          {
            moveDown2(index);
          }
        }
      }
    }
    else if(mySolver2.last_dir == 1)
    {
      if (!moveUp2(index))
      {
        if(!moveRight2(index))
        {
          if(!moveDown2(index))
          {
            moveLeft2(index);
          }
        }
      }
    }
    else if(mySolver2.last_dir == 2)
    {
      if (!moveRight2(index))
      {
        if(!moveDown2(index))
        {
          if(!moveLeft2(index))
          {
            moveUp2(index);
          }
        }
      }
    }
    else if(mySolver2.last_dir == 3)
    {
      if (!moveDown2(index))
      {
        if(!moveLeft2(index))
        {
          if(!moveUp2(index))
          {
            moveRight2(index);
          }
        }
      }
    }
  }

  
}


bool Helper::moveUp(int index)
{
  //printf("Attempting Up\n");
  if(mySolver.y <= 0)
  {
    return false;
  }
  if (!grid[mySolver.x][mySolver.y-1].wall)
  {
    return false;
  }
  //printf("Moved Up\n");
  mySolver.y -= 1;
  grid[mySolver.x][mySolver.y].solver_visited |= 1;
  if (grid[mySolver.x][mySolver.y].solver_visited & 2)
  {
    myImage.setPixel(mySolver.x,mySolver.y,solverPathCombined);
  }
  else
  {
    myImage.setPixel(mySolver.x,mySolver.y,solverPath);
  }
  mySolver.last_dir = 0;
  return true;
}

bool Helper::moveRight(int index)
{
  //printf("Attempting Right\n");
  if(mySolver.x >= WINDOW_WIDTH)
  {
    return false;
  }
  if (!grid[mySolver.x+1][mySolver.y].wall)
  {
    return false;
  }
  //printf("Moved Right\n");
  mySolver.x += 1;
  grid[mySolver.x][mySolver.y].solver_visited |= 1;
  if (grid[mySolver.x][mySolver.y].solver_visited & 2)
  {
    myImage.setPixel(mySolver.x,mySolver.y,solverPathCombined);
  }
  else
  {
    myImage.setPixel(mySolver.x,mySolver.y,solverPath);
  }
  mySolver.last_dir = 1;
  return true;
}


bool Helper::moveDown(int index)
{
  //printf("Attempting Down\n");
  if(mySolver.y >= WINDOW_HEIGHT)
  {
    return false;
  }
  if (!grid[mySolver.x][mySolver.y+1].wall)
  {
    return false;
  }
  //printf("Moved Down\n");
  mySolver.y += 1;
  grid[mySolver.x][mySolver.y].solver_visited |= 1;
  if (grid[mySolver.x][mySolver.y].solver_visited & 2)
  {
    myImage.setPixel(mySolver.x,mySolver.y,solverPathCombined);
  }
  else
  {
    myImage.setPixel(mySolver.x,mySolver.y,solverPath);
  }
  mySolver.last_dir = 2;
  return true;
}


bool Helper::moveLeft(int index)
{
  //printf("Attempting Left\n");
  if(mySolver.x <= 0)
  {
    return false;
  }
  if (!grid[mySolver.x-1][mySolver.y].wall)
  {
    return false;
  }
  //printf("Moved Left\n");
  mySolver.x -= 1;
  grid[mySolver.x][mySolver.y].solver_visited |= 1;
  if (grid[mySolver.x][mySolver.y].solver_visited & 2)
  {
    myImage.setPixel(mySolver.x,mySolver.y,solverPathCombined);
  }
  else
  {
    myImage.setPixel(mySolver.x,mySolver.y,solverPath);
  }
  mySolver.last_dir = 3;
  return true;
}








bool Helper::moveUp2(int index)
{
  //printf("Attempting Up\n");
  if(mySolver2.y <= 0)
  {
    return false;
  }
  if (!grid[mySolver2.x][mySolver2.y-1].wall)
  {
    return false;
  }
  //printf("Moved Up\n");
  mySolver2.y -= 1;
  grid[mySolver2.x][mySolver2.y].solver_visited |= 2;
  if (grid[mySolver2.x][mySolver2.y].solver_visited & 1)
  {
    myImage.setPixel(mySolver2.x,mySolver2.y,solverPathCombined);
  }
  else
  {
    myImage.setPixel(mySolver2.x,mySolver2.y,solverPath2);
  }
  mySolver2.last_dir = 0;
  return true;
}

bool Helper::moveRight2(int index)
{
  //printf("Attempting Right\n");
  if(mySolver2.x >= WINDOW_WIDTH)
  {
    return false;
  }
  if (!grid[mySolver2.x+1][mySolver2.y].wall)
  {
    return false;
  }
  //printf("Moved Right\n");
  mySolver2.x += 1;
  grid[mySolver2.x][mySolver2.y].solver_visited |= 2;
  if (grid[mySolver2.x][mySolver2.y].solver_visited & 1)
  {
    myImage.setPixel(mySolver2.x,mySolver2.y,solverPathCombined);
  }
  else
  {
    myImage.setPixel(mySolver2.x,mySolver2.y,solverPath2);
  }
  mySolver2.last_dir = 1;
  return true;
}


bool Helper::moveDown2(int index)
{
  //printf("Attempting Down\n");
  if(mySolver2.y >= WINDOW_HEIGHT)
  {
    return false;
  }
  if (!grid[mySolver2.x][mySolver2.y+1].wall)
  {
    return false;
  }
  //printf("Moved Down\n");
  mySolver2.y += 1;
  grid[mySolver2.x][mySolver2.y].solver_visited |= 2;
  if (grid[mySolver2.x][mySolver2.y].solver_visited & 1)
  {
    myImage.setPixel(mySolver2.x,mySolver2.y,solverPathCombined);
  }
  else
  {
    myImage.setPixel(mySolver2.x,mySolver2.y,solverPath2);
  }
  mySolver2.last_dir = 2;
  return true;
}


bool Helper::moveLeft2(int index)
{
  //printf("Attempting Left\n");
  if(mySolver2.x <= 0)
  {
    return false;
  }
  if (!grid[mySolver2.x-1][mySolver2.y].wall)
  {
    return false;
  }
  //printf("Moved Left\n");
  mySolver2.x -= 1;
  grid[mySolver2.x][mySolver2.y].solver_visited |= 2;
  if (grid[mySolver2.x][mySolver2.y].solver_visited & 1)
  {
    myImage.setPixel(mySolver2.x,mySolver2.y,solverPathCombined);
  }
  else
  {
    myImage.setPixel(mySolver2.x,mySolver2.y,solverPath2);
  }
  mySolver2.last_dir = 3;
  return true;
}





