#ifndef EMU_UTILS_DISPLAY_15X8_1BIT_HPP
#define EMU_UTILS_DISPLAY_15X8_1BIT_HPP


template <typename size_type>
class display_15x8_1bit : public ExternalDevice<size_type>
{
  public:

    display_15x8_1bit(uint8_t pin, uint8_t scale) : pin{pin}, scale{scale}
    {
      SCREEN = new sf::RenderWindow(sf::VideoMode(width * scale, 8 * scale, 1), "1-bit 15x8 display");
      pixel.setSize(sf::Vector2f(scale, scale));
    }

    virtual void update()
    {
      if (clock60hz.deltaTime < 1.0f/60.0f)
      {
        return;
      }

      SCREEN->clear();
      for (uint8_t pix = 0; pix < width * 8; pix++)
      {
        pixel.setPosition((pix / 8) * scale, (pix % 8) * scale);
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
      if (outputPins[pin] == 0xF8) { // flush screen
        std::copy(pixels, pixels+sizeof(pixels), pixelBuffer);

      } else if (outputPins[pin] >> 1 == 0x78) { // fill screen
        std::fill(pixels, pixels+sizeof(pixels), 0xFF * (outputPins[pin] & 1));

      } else
      { // set pixel
        if (outputPins[pin] & 1)
        {
          pixels[outputPins[pin] >> 4] |= (1 << ((outputPins[pin] >> 1) & 0b111));
        } else
        {
          pixels[outputPins[pin] >> 4] &= ~(1 << ((outputPins[pin] >> 1) & 0b111));
        }
      }

      {
        sf::Event event;
        while (SCREEN->pollEvent(event))
        {
          
        }
      }
    }

  private:
    static const uint8_t width = 15;

    uint8_t pin;
    uint8_t scale;
    sf::RenderWindow* SCREEN;
    sf::RectangleShape pixel;
    uint8_t pixels[width]; // real value of pixels
    uint8_t pixelBuffer[width]; // displayed value of pixels
};

#endif // EMU_UTILS_DISPLAY_15X8_1BIT_HPP