#ifndef EMU_UTILS_ROM_HPP
#define EMU_UTILS_ROM_HPP

#include "external_device.hpp"

template <uint32_t size>
class ROM: public ExternalDevice<uint16_t>
{
  public:
    uint8_t memory[size];

    virtual uint8_t read(uint16_t position)
    {
      return memory[position];
    }

    virtual void write(uint16_t position, uint8_t value)
    {
      
    }
  private:
    
};

#endif // EMU_UTILS_ROM_HPP
