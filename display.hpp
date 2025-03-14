#ifndef EMU_UTILS_DISPLAY_HPP
#define EMU_UTILS_DISPLAY_HPP

#include <cstdint>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "external_device.hpp"

class Display: public ExternalDevice<uint16_t>
{
  public:
    Display(uint16_t width, uint16_t height): width(width), height(height)
    {
      buffer = sf::Image(sf::Vector2u(width, height), sf::Color::Transparent);
    }

    virtual uint8_t read(uint16_t position)
    {
      return 0;
    }

    virtual void write(uint16_t position, uint8_t value)
    {
      ((uint8_t*)(&operation))[position] = value;

      switch (operation.opcode)
      {
        case Fill:
          buffer = sf::Image(sf::Vector2u(width, height), sf::Color(value & 0xE0, (value & 0x1C) << 3, (value & 0x03) << 6));
          operation.opcode = None;
          break;
        case SetPixel:
          buffer.setPixel(sf::Vector2u(operation.x, operation.y), sf::Color(value & 0xE0, (value & 0x1C) << 3, (value & 0x03) << 6));
          operation.opcode = None;
          break;
        case DrawRect:
          for (uint16_t x = operation.x; x < operation.x + operation.width; x++)
          {
            for (uint16_t y = operation.y; y < operation.y + operation.height; y++)
            {
              buffer.setPixel(sf::Vector2u(x, y), sf::Color(value & 0xE0, (value & 0x1C) << 3, (value & 0x03) << 6));
            }
          }
          operation.opcode = None;
          break;
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

      SCREEN.clear();

      SCREEN.display();
    }
  private:
    enum Command
    {
      None,
      Fill,
      SetPixel,
      DrawRect
    };

    struct Operation
    {
      Command opcode;
      uint8_t color;
      float x = 0.0f;
      float y = 0.0f;
      float width = 0.0f;
      float height = 0.0f;
    } operation;

    uint16_t width;
    uint16_t height;

    sf::Image buffer;
    sf::Texture texture;
    sf::Sprite sprite = sf::Sprite(texture);

    sf::RenderWindow SCREEN = sf::RenderWindow(sf::VideoMode(sf::Vector2u(width, height)), "video output", sf::Style::Titlebar);
};

#endif // EMU_UTILS_DISPLAY_HPP
