#include <QPainter>
#include <QTime>
#include "snake.h"

Snake::Snake(QWidget *parent) : QWidget(parent)
{
  setStyleSheet("background-color:black;");
  leftDirection = false;
  rightDirection = false;
  upDirection = true;
  downDirection = false;
  inGame = true;

  setFixedSize(B_WIDTH, B_HEIGHT);
  loadImages();
  initGame();
}

void Snake::loadImages()
{
  dot.load("/home/japonamat/programming/Snake/Snake/dot.png");
  head.load("/home/japonamat/programming/Snake/Snake/head.png");
  apple.load("/home/japonamat/programming/Snake/Snake/apple.png");
}

void Snake::initGame()
{
  dots = 3;

  for (int i = 0; i < dots; i++){
    x[i] = 50 - i*10;
    y[i] = 50;
  }

  locateApple();

  timerId = startTimer(DELAY);
}

void Snake::paintEvent(QPaintEvent *e)
{
  Q_UNUSED(e);
  doDrawing();
}

void Snake::doDrawing()
{
  QPainter qp(this);

  if (inGame){
    qp.drawImage(apple_x, apple_y, apple);

    for (int i = 0; i < dots; i++){
      if (i == 0) qp.drawImage(x[i], y[i], head);
      else qp.drawImage(x[i], y[i], dot);
    }
  } else gameOver(qp);
}

void Snake::gameOver(QPainter &qp)
{
  QString messege = "Game over!";
  QFont font("Courier", 15, QFont::DemiBold);
  QFontMetrics fm(font);
  int textWidth = fm.horizontalAdvance(messege);

  qp.setFont(font);
  int h = height();
  int w = width();

  qp.translate(QPoint(w/2, h/2));
  qp.drawText(-textWidth/2, 0, messege);
}

void Snake::checkApple()
{
  if ((x[0] == apple_x) && (y[0] == apple_y)){
    dots++;
    locateApple();
  }
}

void Snake::move()
{
  for (int i = dots; i > 0; i--){
    x[i] = x[(i-1)];
    y[i] = y[(i-1)];
  }
  if (leftDirection){
    x[0] -= DOT_SIZE;
  }

  if (rightDirection){
    x[0] += DOT_SIZE;
  }

  if (upDirection){
    y[0] -= DOT_SIZE;
  }
  if (downDirection){
    y[0] += DOT_SIZE;
  }
}

void Snake::checkCollision()
{
  for (int i = dots; i > 0; i--){

      if ((i > 4) && (x[0] == x[i]) && (y[0] == y[i])) inGame = false;
  }

  if (y[0] >= B_HEIGHT || y[0] < 0) inGame = false;

  if (x[0] >= B_WIDTH || x[0] < 0) inGame = false;

  if (!inGame) killTimer(timerId);
}

void Snake::locateApple()
{
  QTime time = QTime::currentTime();
  qsrand((uint) time.msec());

  int r = qrand() % RAND_POS;
  apple_x = (r * DOT_SIZE);

  r = qrand() % RAND_POS;
  apple_y = (r * DOT_SIZE);
}

void Snake::timerEvent(QTimerEvent *e)
{
  Q_UNUSED(e);

  if (inGame){
    checkApple();
    checkCollision();
    move();
  }

  repaint();
}

void Snake::keyPressEvent(QKeyEvent *e)
{
  int key = e->key();

  if ((key == Qt::Key_Left) && (!rightDirection)){
    leftDirection = true;
    upDirection = false;
    downDirection = false;
  }

  if ((key == Qt::Key_Right) && (!leftDirection)){
    rightDirection = true;
    upDirection = false;
    downDirection = false;
    }

  if ((key == Qt::Key_Up) && (!downDirection)) {
      upDirection = true;
      rightDirection = false;
      leftDirection = false;
  }

  if ((key == Qt::Key_Down) && (!upDirection)) {
      downDirection = true;
      rightDirection = false;
      leftDirection = false;
  }

  QWidget::keyPressEvent(e);
}
