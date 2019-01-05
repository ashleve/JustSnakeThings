#include "snake.h"
#include "winsys.h"
#include "cpoint.h"
#include "screen.h"
#include <time.h>
#include <unistd.h>


CSnake::CSnake(CRect r, char _c):CFramedWindow(r, _c)
{
  CPoint starting_point = CPoint(
    geom.size.x/2,
    geom.size.y/2
  );

  body.push_back(starting_point);
  starting_point+=1;
  body.push_back(starting_point);
  body.push_back(starting_point+=1);
}

void CSnake::paint()
{
  CFramedWindow::paint();
  paintSnake();
}

void CSnake::paintSnake()
{
  for(auto it = body.cbegin(); it != body.cend(); it++)
  {
    gotoyx((*it).y + geom.topleft.y, (*it).x + geom.topleft.x);
    printl("%c", '*');
  }
}

void CSnake::moveSnakeByOne()
{
  //if(paused)return;

  CPoint direction;
  for(auto it = body.begin(); it != body.end(); ++it)
  {
    auto it_tmp = it;
    if(++it_tmp != body.end())
    {
      if((*(it_tmp)).x > (*it).x)
        direction = RIGHT;
      else if((*(it_tmp)).x < (*it).x)
        direction = LEFT;
      else if((*(it_tmp)).y < (*it).y)
        direction = UP;
      else if((*(it_tmp)).y > (*it).y)
        direction = DOWN;
      else
        exit(1); // should never come to this
    }
    else direction = head_direction;


    *it += direction;
  }
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
