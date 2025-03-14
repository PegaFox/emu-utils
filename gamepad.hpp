#ifndef MENU_UTILS_GAMEPAD_HPP
#define MENU_UTILS_GAMEPAD_HPP

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Joystick.hpp>
#include "external_device.hpp"

template <typename size_type>
class Gamepad : public ExternalDevice<size_type> {
  public:

    Gamepad() {

    }

    void update() {}

    virtual void write(size_type position, uint8_t value) {}

    virtual uint8_t read(size_type position) {
      uint8_t value = 0;

      value |= (int)sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) * 0x20;
      value |= (int)sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) * 0x10;
      value |= (int)sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) * 0x80;
      value |= (int)sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) * 0x40;

      value |= (int)sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) * 0x20;
      value |= (int)sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) * 0x10;
      value |= (int)sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) * 0x80;
      value |= (int)sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) * 0x40;

      value |= (int)sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) * 0x08;
      value |= (int)sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) * 0x04;

      value |= (int)sf::Keyboard::isKeyPressed(sf::Keyboard::Key::J) * 0x02;
      value |= (int)sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K) * 0x01;


      sf::Joystick::update();
      for (uint8_t j = 0; j < sf::Joystick::Count; j++) {
        if (!sf::Joystick::isConnected(j)) break;

        if (sf::Joystick::hasAxis(j, sf::Joystick::Axis::X) && sf::Joystick::hasAxis(j, sf::Joystick::Axis::Y)) {

          value += int((sf::Joystick::getAxisPosition(j, sf::Joystick::Axis::X) * 0.01) * 0x70);
          value += int((sf::Joystick::getAxisPosition(j, sf::Joystick::Axis::Y) * 0.01) * 0x7);

        }
      }
      
      return value;
    }

  private:
};

#endif // MENU_UTILS_GAMEPAD_HPP
