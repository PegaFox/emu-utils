#ifndef EMU_UTILS_VGA_SIM_HPP
#define EMU_UTILS_VGA_SIM_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <cstdint>
#include <pegafox/utils.hpp>

class VGAsim
{
  public:
    VGAsim(uint16_t width, uint16_t height, uint16_t depth, uint16_t clockSpeed): width(width), height(height), depth(depth), clockSpeed(clockSpeed)
    {
      buffer = sf::Image(sf::Vector2u(width, height), sf::Color::Transparent);

      sprite.setScale(sf::Vector2f(3.0f, 3.0f));
    }

    void write(float red, float green, float blue, bool hSync, bool vSync)
    {
      scanClock.get_fps(false);

      uint16_t xScanPos = (scanClock.deltaTime - lastHSync) * clockSpeed;

      if (xScanPos < width)
      {
        buffer.setPixel(sf::Vector2u((scanClock.deltaTime - lastHSync) * clockSpeed, yScanPos), sf::Color(red * 255.0f, green * 255.0f, blue * 255.0f));
      }

      if (!hSync)
      {
        lastHSync = scanClock.deltaTime;
        yScanPos++;
      }
      if (!vSync)
      {
        yScanPos = 0;
      }
    }

    void update()
    {
      while (SCREEN.pollEvent())
      {

      }

      texture.loadFromImage(buffer);
      sprite.setTexture(texture);
      SCREEN.draw(sprite);

      SCREEN.display();
    }
  private:
    pf::FPS scanClock;
    uint16_t clockSpeed;

    float lastHSync = 0.0f;

    uint16_t yScanPos;

    uint16_t width;
    uint16_t height;
    uint16_t depth;

    sf::Image buffer;
    sf::Texture texture;
    sf::Sprite sprite = sf::Sprite(texture);

    sf::RenderWindow SCREEN = sf::RenderWindow(sf::VideoMode(sf::Vector2u(width*3, height*3), depth), "VGA output");
};

#endif // EMU_UTILS_VGA_SIM_HPP
