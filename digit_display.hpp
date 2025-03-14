#ifndef EMU_UTILS_DIGIT_DISPLAY_HPP
#define EMU_UTILS_DIGIT_DISPLAY_HPP

#include <cstdint>
#include <iostream>
#include "external_device.hpp"

template <typename size_type>
class DigitDisplay: public ExternalDevice<size_type>
{
  public:

    DigitDisplay()
    {

    }

    void update()
    {

    }

    virtual uint8_t read(size_type position)
    {
      return 0;
    }

    virtual void write(size_type position, uint8_t value)
    {
      std::cout << "DigitDisplay: " << (int)value << '\n';
    }

  private:
};

#endif // EMU_UTILS_DIGIT_DISPLAY_HPP
