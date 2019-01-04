#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "winsys.h"
#include "cpoint.h"
#include <list>

enum GameState { help, pause, play};

class CSnake:public CFramedWindow
{
  list<CPoint> body;  //list of snake segments positions relative to CSnake window
  CPoint head_direction = CPoint(0,1);
  GameState game_state = help;

public:
  CSnake(CRect r, char _c = ' ');
  void paint();
  void paintSnake();
  void moveSnakeByOne(const CPoint & delta);


};

#endif

