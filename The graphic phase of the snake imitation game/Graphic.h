#pragma once

#include <SFML/Graphics.hpp>

namespace GameObjects {
  namespace DisplayPixels {
    sf::Color ColorA{196, 207, 165},
              ColorB{57, 63, 44},
              ColorC{179, 191, 145};
    //
    const int PixelSize = 26,
              Thickness = 2;

    class Pixel : public sf::Drawable {
     public:
      int x, y;
      //
      int z = 0;

      Pixel(int x, int y) : x(x), y(y) {
      }
    };

    class Snake : public Pixel {
     public:
      Snake(int x, int y) : Pixel(x, y) {
        z = 2;
      }

     private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
          sf::RectangleShape inner,
                             outer;

          inner.setSize({PixelSize - 12, PixelSize - 12});
          inner.setFillColor(ColorB);
          //
          outer.setSize({PixelSize - 2 * Thickness, PixelSize - 2 * Thickness});
          outer.setOutlineColor(ColorB);
          outer.setOutlineThickness(2);

          inner.setPosition((float) x * (PixelSize - Thickness) + 4, (float) y * (PixelSize - Thickness) + 4);
          outer.setPosition((float) x * (PixelSize - Thickness)    , (float) y * (PixelSize - Thickness));

          target.draw(outer, states);
          target.draw(inner, states);
        }
    };

    class Wall : public Pixel {
     public:
      Wall(int x, int y) : Pixel(x, y) {
        z = 1;
      }

     private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
          sf::RectangleShape outer;

          outer.setSize({PixelSize - 2 * Thickness, PixelSize - 2 * Thickness});
          outer.setFillColor(ColorC);
          outer.setOutlineColor(ColorB);
          outer.setOutlineThickness(2);

          outer.setPosition((float) x * (PixelSize - Thickness), (float) y * (PixelSize - Thickness));

          target.draw(outer, states);
        }
    };

    class Empty : public Pixel {
     public:
      Empty(int x, int y) : Pixel(x, y) {
        z = 0;
      }

     private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
          sf::RectangleShape outer;

          outer.setSize({PixelSize - 2 * Thickness, PixelSize - 2 * Thickness});
          outer.setFillColor(ColorA);
          outer.setOutlineColor(ColorC);
          outer.setOutlineThickness(2);

          outer.setPosition((float) x * (PixelSize - Thickness), (float) y * (PixelSize - Thickness));

          target.draw(outer, states);
        }
    };
  }

  class Display {
   public:
    Plane &plane;
    Snake &snake;
    //
    vector<vector<DisplayPixels::Pixel *>> buffer{};
    //
    sf::RenderWindow *window;

   public:
    Display(Plane &plane, Snake &snake) : plane(plane), snake(snake) {
      // create buffer lines
      buffer.resize(plane.height);
      // initialize buffer
      for (auto &row : buffer)
        row.resize(plane.width);
      clearBuffer();
      //
      window = new sf::RenderWindow(
          sf::VideoMode(plane.width  * (DisplayPixels::PixelSize - DisplayPixels::Thickness) - DisplayPixels::Thickness,
                        plane.height * (DisplayPixels::PixelSize - DisplayPixels::Thickness)),
                        " Game window", sf::Style::Titlebar);
    }

    void renderObstacles() {
      for (auto const& obstacle : plane.obstacles) {
        int x = 0;
        auto &row = buffer[obstacle.y];
        //
        for (auto hole : obstacle.holes) {
          for (int i = x; i < hole.x; ++i)
            row[i] = new DisplayPixels::Wall(i, obstacle.y);
          // move to next hole
          x = hole.x + hole.size;
        }
        for (int i = x; i < plane.width; ++i)
          row[i] = new DisplayPixels::Wall(i, obstacle.y);
      }
    };

    void renderSnake(int sleep = 0) {
      for (auto const& pos : snake.movements) {
        buffer[pos.y][pos.x] = new DisplayPixels::Snake(pos.x, pos.y);
        // delay after print rendered result if needed
        if (sleep) {
          printWindow();
          sf::sleep(sf::milliseconds(sleep));
        }
      }
      printWindow();
    }

    void renderAll() {
      for (int z = 0; z < 3; ++z)
          for (auto const& row : buffer)
            for (auto &pixel : row)
              if (pixel and pixel->z == z)
                window->draw(*pixel);
    }

    void clearBuffer() {
      int x, y;
      //
      for (y = 0; y < plane.height; ++y)
        for (x = 0; x < plane.width; ++x)
          buffer[y][x] = new DisplayPixels::Empty(x, y);
    }

    void printWindow(bool all=true) {
      if (all)
        renderAll();
      window->display();
    }

    void printMessage(string const& message, int sleep) {
      sf::Text text;
      sf::Font font;
      //
      if(font.loadFromFile("LiberationMono-Bold.ttf")) {
        text.setString(message);
        text.setCharacterSize(40);
        text.setFont(font);
        text.setFillColor(DisplayPixels::ColorB);
        text.setOutlineColor(DisplayPixels::ColorA);
        text.setOutlineThickness(10);

        auto   textRect = text.getLocalBounds();
        auto windowRect = window->getSize();
        //
        text.setOrigin(textRect.left + textRect.width  / 2,
                       textRect.top  + textRect.height / 2);
        text.setPosition(sf::Vector2f((float) windowRect.x / 2,(float) windowRect.y / 2));

        renderAll();
        window->draw(text);
        printWindow(false);
        // sleep if needed
        sf::sleep(sf::milliseconds(sleep));
      }

    }

    bool isEmpty(int x, int y) const {
      return dynamic_cast<DisplayPixels::Empty*>(buffer[y][x]) != nullptr;
    }
  };
}