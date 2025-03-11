#ifndef MENU_UTILS_GAMEPAD_HPP
#define MENU_UTILS_GAMEPAD_HPP

class Gamepad : public ExternalDevice {
  public:

    Gamepad(uint8_t pin) : pin{pin} {

    }

    virtual void update() {}

    virtual void input(uint8_t* outputPins) {}

    virtual void output(uint8_t* inputPins) {
      inputPins[pin] = 0;

      inputPins[pin] |= (int)sf::Keyboard::isKeyPressed(sf::Keyboard::W) * 0x20;
      inputPins[pin] |= (int)sf::Keyboard::isKeyPressed(sf::Keyboard::S) * 0x10;
      inputPins[pin] |= (int)sf::Keyboard::isKeyPressed(sf::Keyboard::A) * 0x80;
      inputPins[pin] |= (int)sf::Keyboard::isKeyPressed(sf::Keyboard::D) * 0x40;

      inputPins[pin] |= (int)sf::Keyboard::isKeyPressed(sf::Keyboard::Up) * 0x20;
      inputPins[pin] |= (int)sf::Keyboard::isKeyPressed(sf::Keyboard::Down) * 0x10;
      inputPins[pin] |= (int)sf::Keyboard::isKeyPressed(sf::Keyboard::Left) * 0x80;
      inputPins[pin] |= (int)sf::Keyboard::isKeyPressed(sf::Keyboard::Right) * 0x40;

      inputPins[pin] |= (int)sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) * 0x08;
      inputPins[pin] |= (int)sf::Keyboard::isKeyPressed(sf::Keyboard::Space) * 0x04;

      inputPins[pin] |= (int)sf::Keyboard::isKeyPressed(sf::Keyboard::J) * 0x02;
      inputPins[pin] |= (int)sf::Keyboard::isKeyPressed(sf::Keyboard::K) * 0x01;


      sf::Joystick::update();
      for (uint8_t j = 0; j < sf::Joystick::Count; j++) {
        if (!sf::Joystick::isConnected(j)) break;

        if (sf::Joystick::hasAxis(j, sf::Joystick::X) && sf::Joystick::hasAxis(j, sf::Joystick::Y)) {

          inputPins[pin] += int((sf::Joystick::getAxisPosition(j, sf::Joystick::X) * 0.01) * 0x70);
          inputPins[pin] += int((sf::Joystick::getAxisPosition(j, sf::Joystick::Y) * 0.01) * 0x7);

        }
      }
      
    }

  private:
    uint8_t pin;
};

#endif // MENU_UTILS_GAMEPAD_HPP