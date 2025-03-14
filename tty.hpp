#ifndef EMU_UTILS_TTY_HPP
#define EMU_UTILS_TTY_HPP

#include <iostream>
#include "external_device.hpp"

// just uses a single byte for inputting characters
class TTY: public ExternalDevice<uint16_t>
{
  public:
    virtual uint8_t read(uint16_t position)
    {
      return 0;
    }

    virtual void write(uint16_t position, uint8_t value)
    {
      std::cout << (char)value << std::flush;
    }
  private:
    
};

#endif // EMU_UTILS_TTY_HPP
