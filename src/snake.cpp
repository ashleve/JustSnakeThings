#include "snake.h"
#include "winsys.h"
#include "cpoint.h"
#include "screen.h"
#include <time.h>
#include <unistd.h>


CSnake::CSnake(CRect r, char _c):CFramedWindow(r, _c)
{
  CPoint starting_point = CPoint(4, 1);

  fruit = CPoint(geom.size.x/2, geom.size.y/2);

  body.push_back(starting_point);
  starting_point+=1;
  body.push_back(starting_point);
  body.push_back(starting_point+=1);
}

void CSnake::paint()
{
  CFramedWindow::paint();
  paintSnake();
  paintFruit();
  if(help) paintHelp();
}

void CSnake::paintSnake()
{
  for(auto it = body.cbegin(); it != body.cend(); it++)
  {
    gotoyx((*it).y + geom.topleft.y, (*it).x + geom.topleft.x);
    printl("%c", '*');
  }
}

void CSnake::paintFruit()
{
  gotoyx(fruit.y + geom.topleft.y, fruit.x + geom.topleft.x);
  printl("%c", 'O');
}

void CSnake::paintHelp()
{
  gotoyx(geom.topleft.y + 4, geom.topleft.x + 3);
  printl("%s", "h - toggle help information");
  gotoyx(geom.topleft.y + 5, geom.topleft.x + 3);
  printl("%s", "p - toggle pause/play mode");
  gotoyx(geom.topleft.y + 6, geom.topleft.x + 3);
  printl("%s", "r - restart game");
  gotoyx(geom.topleft.y + 7, geom.topleft.x + 3);
  printl("%s", "arrows - move snake (in play mode) or");
  gotoyx(geom.topleft.y + 8, geom.topleft.x + 3);
  printl("%s", "         move window (in pause mode)");
}

void CSnake::moveSnakeByOne()
{
  if(paused)return;

  checkForFood();

  for(auto it = body.end(); it != body.begin(); it--)
  {
    auto it_tmp = it;
    it_tmp--;
    (*it).x = (*it_tmp).x;
    (*it).y = (*it_tmp).y;
  }
  body.front() += head_direction;
}

void CSnake::grow()
{
  body.push_back(body.back());
}

bool CSnake::checkForFood()
{
  CPoint head = body.front();
  if(head.x + head_direction.x == fruit.x && head.y + head_direction.y == fruit.y)
  {
    grow();
    generateFood();
    //accelerate();
    return true;
  }
  return false;
}


bool CSnake::find(CPoint point)
{
  for(auto it = body.begin(); it != body.end(); ++it)
    if(point.x == (*it).x && point.y == (*it).y) return true;
  return false;
}

void CSnake::generateFood()
{
  srand(time(NULL));
  int x = rand()%(geom.size.x-2)+1;
  int y = rand()%(geom.size.y-2)+1;
  fruit = CPoint(x,y);

  while(find(fruit))  // in case fruit spawns in snake body
    generateFood();

}

void CSnake::runS()
{
  while(true)
  {
    int key = ngetch();
    switch(key)
    {
      case 'h':
        help = true;
        return;
        break;
      case 'p':
        if(paused) paused = false;
        else paused = true;
        break;
      case 'r':
        restart = true;
        break;
      case 'w':
        head_direction = UP;
        moveSnakeByOne();
        break;
      case 's':
        head_direction = DOWN;
        moveSnakeByOne();
        break;
      case 'a':
        head_direction = LEFT;
        moveSnakeByOne();
        break;
      case 'd':
        head_direction = RIGHT;
        moveSnakeByOne();
        break;
    }

    usleep(time_delay);
    //sleep(1);
    if(!paused) moveSnakeByOne();
    paint();
  }
}


bool CSnake::handleEvent(int key)
{
  if(CFramedWindow::handleEvent(key))
    return true;

  switch(key)
  {
    case 'h':
      help = false;
      runS();
      break;
    case 'p':
      if(paused) paused = false;
      else paused = true;
      break;
    case 'r':
      restart = true;
      break;
  }


  return true;
}
