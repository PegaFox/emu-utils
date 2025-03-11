#ifndef EMU_UTILS_BUS_HPP
#define EMU_UTILS_BUS_HPP

#include <cstdint>
#include <vector>
#include "external_device.hpp"

template <typename size_type>
class Bus
{
  public:
    bool connect(ExternalDevice<size_type>* device, size_type begin, size_type end)
    {
      typename std::vector<BusElement>::iterator pos = find(begin);

      if (pos == devices.end() || pos->end < begin)
      {
        devices.emplace(pos, device, begin, end);
        return true;
      }

      return false;
    }

    bool disconnect(size_type address)
    {
      typename std::vector<BusElement>::iterator pos = find(address);

      if (pos != devices.end() && pos->end >= address)
      {
        devices.erase(pos);
        return true;
      }
      return false;
    }

    uint8_t read(size_type address) const
    {
      typename std::vector<BusElement>::const_iterator pos = find(address);

      if (pos != devices.end())
      {
        return pos->device->read(address - pos->begin);
      } else
      {
        return 0;
      }
    }

    bool write(size_type address, uint8_t value)
    {
      typename std::vector<BusElement>::iterator pos = find(address);
      if (pos != devices.end() && pos->end >= address)
      {
        pos->device->write(address - pos->begin, value);
        return true;
      }
      return false;
    }
  private:
    // unlike STL iterators, end is the last element, not one past the end
    struct BusElement
    {
      BusElement(ExternalDevice<size_type>* device, size_type begin, size_type end): device(device), begin(begin), end(end)
      {
        
      }

      ExternalDevice<size_type>* device;
      size_type begin;
      size_type end;
    };

    typename std::vector<BusElement>::iterator find(size_type address)
    {
      typename std::vector<BusElement>::iterator searchBegin = devices.begin();
      typename std::vector<BusElement>::iterator searchEnd = devices.end();

      while (searchBegin != searchEnd)
      {
        typename std::vector<BusElement>::iterator testSubject = (searchBegin + (searchEnd - searchBegin) / 2);
        if (testSubject->end < address)
        {
          searchBegin = testSubject+1;
        } else if (testSubject->begin > address)
        {
          searchEnd = testSubject;
        } else
        {
          searchBegin = testSubject;
          break;
        }
      }

      return searchBegin;
    }

    typename std::vector<BusElement>::const_iterator find(size_type address) const
    {
      typename std::vector<BusElement>::const_iterator searchBegin = devices.cbegin();
      typename std::vector<BusElement>::const_iterator searchEnd = devices.cend();

      while (searchBegin != searchEnd)
      {
        typename std::vector<BusElement>::const_iterator testSubject = (searchBegin + (searchEnd - searchBegin) / 2);
        if (testSubject->end < address)
        {
          searchBegin = testSubject+1;
        } else if (testSubject->begin > address)
        {
          searchEnd = testSubject;
        } else
        {
          searchBegin = testSubject;
          break;
        }
      }

      return searchBegin;
    }

    std::vector<BusElement> devices;
};

#endif // EMU_UTILS_BUS_HPP