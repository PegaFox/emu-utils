#ifndef MENU_UTILS_EXTERNAL_DEVICE_HPP
#define MENU_UTILS_EXTERNAL_DEVICE_HPP

template <typename size_type>
class ExternalDevice
{
  public:
    virtual void update() = 0;

    virtual uint8_t read(size_type position) = 0;

    virtual void write(size_type position, uint8_t value) = 0;

  private:


};

#endif // MENU_UTILS_EXTERNAL_DEVICE_HPP