#include "GameObjects.h"
#include "Graphic.h"

#include <ctime>

#define DEFAULT_GAME_WIDTH  25 // must be odd
#define DEFAULT_GAME_HEIGHT 13  // must be odd

bool isKeyClicked(sf::Keyboard::Key key) {
  if (sf::Keyboard::isKeyPressed(key)) {
    while (sf::Keyboard::isKeyPressed(key));
    //
    return true;
  }
  return false;
}

int main() {
  srand(time(nullptr));

  auto plane = GameObjects::Plane(DEFAULT_GAME_WIDTH, DEFAULT_GAME_HEIGHT);
       plane.generate();
  auto snake = GameObjects::Snake(DEFAULT_GAME_WIDTH / 2, 0, plane);
       snake.findWay();
  auto display = GameObjects::Display(plane, snake);

  display.renderObstacles();
  display.renderSnake(150);
  display.clearBuffer();
  display.renderSnake();
  display.printMessage("Your turn!", 5000);
  // clear snake movements
  snake.clearMovements();

  bool stateWin       = false;
  bool stateHitWall   = false;
  bool stateWrongHole = false;

  display.clearBuffer();
  display.renderObstacles();
  display.renderSnake();
  while (display.window->isOpen() and not stateWin) {
    sf::Event event{};
    while (display.window->pollEvent(event))
      if (event.type == sf::Event::Closed)
        display.window->close();
    //
    // process movement
    int oX = 0, oY = 0;
    if (isKeyClicked(sf::Keyboard::W))
      oY = -1;
    else if (isKeyClicked(sf::Keyboard::S))
      oY = 1;
    else if (isKeyClicked(sf::Keyboard::A))
      oX = -1;
    else if (isKeyClicked(sf::Keyboard::D))
      oX = 1;
    // check if player moves
    if (oX or oY) {
      int x = snake.x + oX;
      int y = snake.y + oY;
      // check if movement is ok
      int result = snake.moveCheck(oX, oY);
      if (result == plane.obstacles.size() - 1)
        stateWin = true;
      //
      if (result == -1) {
        stateWrongHole = true;
        break;
      }
      if (not snake.move(oX, oY, display.isEmpty(x, y))) {
        stateHitWall = true;
        break;
      }
      display.clearBuffer();
      display.renderObstacles();
      display.renderSnake();
    }
  }
  // show result
  if (stateWin) {
    display.printMessage("You WON the game", 5000);
  }
  else if (stateHitWall)
    display.printMessage("LOOSE: Hit the wall", 5000);
  else if (stateWrongHole)
    display.printMessage("LOOSE: Wrong hole", 5000);
}