#include "snake.h"
#include "winsys.h"
#include "cpoint.h"
#include "screen.h"

CSnake::CSnake(CRect r, char _c /*=' '*/):CFramedWindow(r, _c)
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

void CSnake::moveSnakeByOne(const CPoint & dir = CPoint(0,0))//=0
{
  CPoint direction;
  for(auto it = body.begin(); it != body.end(); it++)
  {
    auto it_tmp = it;
    if(++it_tmp != body.end())
    {
      if((*(it++)).x > (*it).x)
        direction = CPoint(1,0);
      else if((*(it++)).x < (*it).x)
        direction = CPoint(-1,0);
      else if((*(it++)).y > (*it).y)
        direction = CPoint(0,1);
      else if((*(it++)).y < (*it).y)
        direction = CPoint(0,-1);
      else
        exit(1); // should never come to this
    }
    else
    {
      if(dir.x != 0 && dir.y != 0)
        head_direction = dir;
      direction = head_direction;
    }

    *it += direction;
  }

}

