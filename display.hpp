#ifndef EMU_UTILS_DISPLAY_HPP
#define EMU_UTILS_DISPLAY_HPP

class Display: public ExternalDevice<uint16_t>
{
  public:
    Display(uint16_t width, uint16_t height): width(width), height(height)
    {
      buffer.create(width, height, sf::Color::Transparent);
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
          buffer.create(width, height, sf::Color(value & 0xE0, (value & 0x1C) << 3, (value & 0x03) << 6));
          operation.opcode = None;
          break;
        case SetPixel:
          buffer.setPixel(operation.x, operation.y, sf::Color(value & 0xE0, (value & 0x1C) << 3, (value & 0x03) << 6));
          operation.opcode = None;
          break;
        case DrawRect:
          for (uint16_t x = operation.x; x < operation.x + operation.width; x++)
          {
            for (uint16_t y = operation.y; y < operation.y + operation.height; y++)
            {
              buffer.setPixel(x, y, sf::Color(value & 0xE0, (value & 0x1C) << 3, (value & 0x03) << 6));
            }
          }
          operation.opcode = None;
          break;
      }
    }

    void update()
    {
      clock60hz.get_fps(false);
      if (clock60hz.deltaTime < 1.0f/60.0f)
      {
        return;
      }

      sf::Event event;
      while (SCREEN.pollEvent(event))
      {

      }

      texture.loadFromImage(buffer);
      sprite.setTexture(texture);
      SCREEN.draw(sprite);

      SCREEN.clear(sf::Color(clock60hz.frameCount << 8));
      std::cout << clock60hz.frameCount << '\n';

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
      Fhalf x = Fhalf(0.0f);
      Fhalf y = Fhalf(0.0f);
      Fhalf width = Fhalf(0.0f);
      Fhalf height = Fhalf(0.0f);
    } operation;

    uint16_t width;
    uint16_t height;

    sf::Image buffer;
    sf::Texture texture;
    sf::Sprite sprite;

    sf::RenderWindow SCREEN = sf::RenderWindow(sf::VideoMode(width, height), "video output", sf::Style::Titlebar);
};

#endif // EMU_UTILS_DISPLAY_HPP