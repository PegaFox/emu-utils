#ifndef EMU_UTILS_DISPLAY_15X8_1BIT_HPP
#define EMU_UTILS_DISPLAY_15X8_1BIT_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "external_device.hpp"

template <typename size_type>
class display_15x8_1bit : public ExternalDevice<size_type>
{
  public:

    display_15x8_1bit(uint8_t scale) : scale{scale}
    {
      SCREEN = new sf::RenderWindow(sf::VideoMode(sf::Vector2u(width * scale, 8 * scale), 1), "1-bit 15x8 display");
      pixel.setSize(sf::Vector2f(scale, scale));
    }

    void update()
    {
      SCREEN->clear();
      for (uint8_t pix = 0; pix < width * 8; pix++)
      {
        pixel.setPosition(sf::Vector2f((pix / 8) * scale, (pix % 8) * scale));
        uint8_t c = ((pixelBuffer[pix / 8] >> (pix%8)) & 1) * 255;
        pixel.setFillColor(sf::Color(c, c, c));
        SCREEN->draw(pixel);
      }
      SCREEN->display();
    }

    virtual uint8_t read(size_type position)
    {

    }

    virtual void write(size_type position, uint8_t value)
    {
      if (value == 0xF8) { // flush screen
        std::copy(pixels, pixels+sizeof(pixels), pixelBuffer);

      } else if (value >> 1 == 0x78) { // fill screen
        std::fill(pixels, pixels+sizeof(pixels), 0xFF * (value & 1));

      } else
      { // set pixel
        if (value & 1)
        {
          pixels[value >> 4] |= (1 << ((value >> 1) & 0b111));
        } else
        {
          pixels[value >> 4] &= ~(1 << ((value >> 1) & 0b111));
        }
      }

      while (SCREEN->pollEvent())
      {
          
      } 
    }

  private:
    static const uint8_t width = 15;

    uint8_t scale;
    sf::RenderWindow* SCREEN;
    sf::RectangleShape pixel;
    uint8_t pixels[width]; // real value of pixels
    uint8_t pixelBuffer[width]; // displayed value of pixels
};

#endif // EMU_UTILS_DISPLAY_15X8_1BIT_HPP
