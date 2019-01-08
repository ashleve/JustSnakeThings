#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "screen.h"
#include "winsys.h"
#include "cpoint.h"
#include <list>


class CSnake:public CFramedWindow
{
  CPoint UP = CPoint(0,-1);
  CPoint DOWN = CPoint(0,1);
  CPoint LEFT = CPoint(-1,0);
  CPoint RIGHT = CPoint(1,0);

  list<CPoint> body;  //  list of snake segments positions relative to CSnake window
  CPoint head_direction;
  CPoint starting_point = CPoint(4,2);
  CPoint fruit;
  bool paused;
  bool help;
  bool dead;
  int score;
  int start_speed = 300;
  int speed;
  int acceleration = 10;

public:
  CSnake(CRect r, char _c = ' ');
  void paint();
  void paintSnake();
  void paintFruit();
  void paintHelp();
  void paintScore();
  void paintGameOver();
  void moveSnakeByOne();

  bool handleEvent(int key);
  void runS();

  void restart();
  bool checkForCollision();
  bool checkFor180(const CPoint& direction);
  bool checkForFood();
  void generateFood();
  bool find(const CPoint& point);  //supplemental function for generateFood()
  void grow();
  void accelerate();
};

#endif
