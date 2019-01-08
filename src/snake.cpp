#include "snake.h"
#include "winsys.h"
#include "cpoint.h"
#include "screen.h"
#include <time.h>
#include <unistd.h>


CSnake::CSnake(CRect r, char _c):CFramedWindow(r, _c)
{
  restart();
}


void CSnake::paint()
{
  CFramedWindow::paint();
  paintFruit();
  paintSnake();
  if(help) paintHelp();
  if(!dead) paintScore();
  else paintGameOver();
}


void CSnake::paintSnake()
{
  if(body.size() == 0) return;

  for(auto it = body.begin(); it != body.end(); it++)
  {
    gotoyx((*it).y + geom.topleft.y, (*it).x + geom.topleft.x);
    if(it == body.begin()) printl("%c", '*');
    else printl("%c", '+');
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
  printl("h - toggle help information");
  gotoyx(geom.topleft.y + 5, geom.topleft.x + 3);
  printl("p - toggle pause/play mode");
  gotoyx(geom.topleft.y + 6, geom.topleft.x + 3);
  printl("r - restart game");
  gotoyx(geom.topleft.y + 7, geom.topleft.x + 3);
  printl("arrows - move snake (in play mode) or");
  gotoyx(geom.topleft.y + 8, geom.topleft.x + 3);
  printl("         move window (in pause mode)");
}


void CSnake::paintScore()
{
  gotoyx(geom.topleft.y-1, geom.topleft.x);
  printl("|SCORE: %d |", score);
}


void CSnake::paintGameOver()
{
    gotoyx(geom.topleft.y + 2, geom.topleft.x + 5);
    printl("GAME OVER, result: %d", score);
}


void CSnake::moveSnakeByOne()
{
  if(paused)return;
  checkForFood();

  // deletes last segment and adds new at the front
  body.pop_back();
  CPoint head = body.front();
  head += head_direction;
  body.push_front(head);

  // check for crossing borders
  if(body.front().x > geom.size.x - 2)
    body.front().x = 1;
  else if(body.front().x < 1)
    body.front().x = geom.size.x - 2;
  else if(body.front().y > geom.size.y - 2)
    body.front().y = 1;
  else if(body.front().y < 1)
    body.front().y = geom.size.y - 2;

  if(checkForCollision())
    dead = true;
}


bool CSnake::checkForCollision()
{
  CPoint head = body.front();
  auto it = body.begin();
  it++;
  while(it != body.end())
  {
    if(head.x == (*it).x && head.y == (*it).y) return true;
    it++;
  }

  return false;
}


bool CSnake::checkFor180(CPoint direction)
{
  if(direction.x == -head_direction.x && direction.y == -head_direction.y)
    return true;
  return false;
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
    score++;
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
  do
  {
    srand(time(NULL));
    int x = rand()%(geom.size.x-2)+1;
    int y = rand()%(geom.size.y-2)+1;
    fruit = CPoint(x,y);
  } while(find(fruit));     // in case fruit spawns in snake body
}


void CSnake::restart()
{
  body.clear();
  body.push_back(starting_point);
  CPoint next_seg = starting_point;
  next_seg += LEFT;
  body.push_back(next_seg);
  next_seg += LEFT;
  body.push_back(next_seg);

  fruit = CPoint(geom.size.x/2, geom.size.y/2);

  score = 0;
  dead = false;
  help = true;
  paused = false;
  head_direction = RIGHT;
}


void CSnake::runS()
{
  while(true)
  {
    int key = ngetch();

    switch(key)
    {
      case 'h':
        help = !help;
        return;
      case 'p':
        paused = !paused;
        return;
      case 'r':
        restart();
        break;
      case 'w':
        if(checkFor180(UP)) break;
        head_direction = UP;
        moveSnakeByOne();
        break;
      case 's':
        if(checkFor180(DOWN)) break;
        head_direction = DOWN;
        moveSnakeByOne();
        break;
      case 'a':
        if(checkFor180(LEFT)) break;
        head_direction = LEFT;
        moveSnakeByOne();
        break;
      case 'd':
        if(checkFor180(RIGHT)) break;
        head_direction = RIGHT;
        moveSnakeByOne();
        break;
    }

    usleep(update_delay);
    if(dead) return;
    if(!paused || help) moveSnakeByOne();
    paint();
  }
}


bool CSnake::handleEvent(int key)
{
  if(CFramedWindow::handleEvent(key))
    return true;

  if(dead)
  {
    if(key == 'r') restart();
    else if(key == 'h') help = !help;
    return true;
  }

  switch(key)
  {
    case 'h':
      help = !help;
      if(!paused)
        runS();
      break;
    case 'p':
      paused = ! paused;
      if(!help)
        runS();
      break;
    case 'r':
      restart();
      break;
  }

  return true;
}
