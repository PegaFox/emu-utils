#ifndef MENU_UTILS_HDD_HPP
#define MENU_UTILS_HDD_HPP

#include <string>
#include <array>
#include <vector>
#include <fstream>
#include "external_device.hpp"

// uses seven bytes
// read/write bytes 1-4: access the selected block.
// read/write byte 5: access the selected word.
// read/write bytes 6-7: access the word at the specified offset in the selected block.
class HDD: public ExternalDevice<uint16_t>
{
  public:
    HDD(const std::string& filename, uint32_t minBlockCount = 0): filename(filename)
    {
      std::filebuf drive;
      drive.open(filename, std::ios::in | std::ios::binary);

      if (drive.is_open())
      {
        data.resize(std::max(drive.in_avail() >> 9, (std::streamsize)minBlockCount));

        drive.sgetn((char*)data.data(), data.size() << 9);
        drive.close();
      } else
      {
        data.resize(minBlockCount);
      }
    }

    ~HDD()
    {
      std::filebuf drive;
      drive.open(filename, std::ios::out | std::ios::binary);

      if (drive.is_open())
      {
        drive.sputn((char*)data.data(), data.size() << 9);

        drive.close();
      }
    }

    virtual uint8_t read(uint16_t position)
    {
      switch (position)
      {
        case 0:
          return selectedBlock & 0xFF;
        case 1:
          return (selectedBlock >> 8) & 0xFF;
        case 2:
          return (selectedBlock >> 16) & 0xFF;
        case 3:
          return selectedBlock >> 24;
        case 4:
          return wordOffset;
        case 5:
          return data[selectedBlock][(uint16_t)wordOffset << 1];
        case 6:
          return data[selectedBlock][((uint16_t)wordOffset << 1) + 1];
      }
      return 0;
    }

    virtual void write(uint16_t position, uint8_t value)
    {
      switch (position)
      {
        case 0:
          selectedBlock = (selectedBlock & 0xFFFFFF00) | value;
          break;
        case 1:
          selectedBlock = (selectedBlock & 0xFFFF00FF) | (value << 8);
          break;
        case 2:
          selectedBlock = (selectedBlock & 0xFF00FFFF) | (value << 16);
          break;
        case 3:
          selectedBlock = (selectedBlock & 0x00FFFFFF) | (value << 24);
          break;
        case 4:
          wordOffset = value;
          break;
        case 5:
          data[selectedBlock][(uint16_t)wordOffset << 1] = value;
          //std::cout << "write " << (int)value << " to block " << selectedBlock << ", offset " << ((uint16_t)wordOffset << 1) << "\n";
          break;
        case 6:
          data[selectedBlock][((uint16_t)wordOffset << 1) + 1] = value;
          //std::cout << "write " << (int)value << " to block " << selectedBlock << ", offset " << (((uint16_t)wordOffset << 1) + 1) << "\n";
          break;
      }
    }
  private:
    uint32_t selectedBlock = 0;
    uint8_t wordOffset = 0;

    std::string filename;

    std::vector<std::array<uint8_t, 512>> data;
};

#endif // MENU_UTILS_HDD_HPP
