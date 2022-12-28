#pragma once

#include <utility>
#include <vector>
#include <iostream>
#include <fcntl.h>
#include <windows.h>

using namespace std;

#define DEFAULT_HOLE_SIZE 3
#define RANDOM_GEN(a, b) ((rand() % ((b) - (a))) + (a))

namespace GameObjects {
  class Obstacle {
   public:
    struct Hole {
      int x;
      int size;
    };

   public:
    int y;
    int width;
    vector<Hole> holes;

   public:
    explicit Obstacle(int y, int width) : y(y), width(width) {
    }

    Obstacle &generate() {
      Hole hole{0, DEFAULT_HOLE_SIZE};
      // generate holes randomly
      do {
        hole.x += RANDOM_GEN(2, 10);
        if (hole.x + hole.size >= width - 2)
          break;
        // add current hole to all holes
        holes.push_back(hole);
        // move to next hole
        hole.x += hole.size;
      } while (true);
      // return result
      return *this;
    }

    Obstacle &addHole(Hole hole) {
      holes.push_back(hole);
      // return result
      return *this;
    }
  };

  class Plane {
   public:
    int width;
    int height;
    vector<Obstacle> obstacles;

   public:
    explicit Plane(int width, int height) : width(width), height(height) {
    }

    Plane &generate() {
      int y = 2;
      //
      obstacles.push_back(Obstacle(0, width)
                          .addHole({width / 2, 1}));
      do {
        Obstacle obstacle(y, width);
                 obstacle.generate();
        obstacles.push_back(obstacle);
        // move to next obstacle
        y += 2;
      } while (y < height - 2);
      obstacles.push_back(Obstacle(height - 1, width)
                          .addHole({(width - DEFAULT_HOLE_SIZE) / 2, DEFAULT_HOLE_SIZE}));
      // return result
      return *this;
    }
  };

  class Snake {
   public:
    struct Pos {
      int x;
      int y;
    };

   public:
    int x;
    int y;
    //
    Plane &plane;
    //
    vector<int> indices;
    vector<Pos> movements;

   public:
    Snake(int x, int y, Plane &plane) : x(x), y(y), plane(plane) {
      movements.push_back({x, y});
    }

    void findWay() {
      int lastX = 0;
      for (int i = 0; i < plane.obstacles.size(); ++i) {
        auto index = RANDOM_GEN(0, (int) plane.obstacles[i].holes.size());
        indices.push_back(index);
        // get hole
        auto &hole = plane.obstacles[i].holes[index];
        //
        int nextX = hole.x + hole.size / 2;
        if (i != 0) {
          movements.push_back({lastX, 2 * (i - 1)});
          for (int path = lastX; (lastX < nextX ? path <= nextX : path >= nextX);
                                 (lastX < nextX ? ++path        : --path))
            movements.push_back({path, 2 * i - 1});
        }
        lastX = nextX;
      }
      movements.push_back({lastX, (int) (2 * (plane.obstacles.size() - 1))});
    }

    int moveCheck(int oX, int oY) {
      int tempX = x + oX;
      int tempY = y + oY;

      bool holeFind = false;
      // check for hole index
      int index = 0;
      for (auto const& obstacle : plane.obstacles) {
        if (obstacle.y == tempY) {
          int holeIndex = 0;
          for (auto const& hole : obstacle.holes) {
            if (hole.x <= tempX and tempX < hole.x + hole.size) {
              holeFind = true;
              // check if current hole index is same as answer index
              if (indices[index] == holeIndex)
                return index;
            }
            // move to next hole
            ++holeIndex;
          }
        }
        // move to next obstacle
        ++index;
      }
      // return result
      if (holeFind)
        return -1; // -1 means snake isn't in correct hole
      return -2;   // -2 means snake isn't in hole
    }

    bool move(int oX, int oY, bool canGo) {
      x += oX;
      y += oY;
      if (not movements.empty()) {
        auto &lastMove = movements[movements.size() - 2];
        if (x == lastMove.x and y == lastMove.y) {
          movements.pop_back();
          return true;
        }
      }
      if (canGo)
        movements.push_back({x, y});
      else {
        x -= oX;
        y -= oY;
      }
      return canGo;
    }

    void clearMovements() {
      movements.erase(movements.begin() + 1, movements.end());
    }
  };
}