// Declare a C#-style property that allows getting and setting of a value.

#if ! defined(__THEIA_CORE_PROPERTY__)
#define __THEIA_CORE_PROPERTY__

namespace theia
{
  namespace core
  {
    template <typename T> class Property
    {
    public:
      Property() { }
      Property(const T& value) : value(value) { }

      virtual operator T() const { return value; }

      virtual void operator=(const T& value) { this->value = value; }

    protected:
      T value;
    };
  }
}

#endif // __THEIA_CORE_PROPERTY__
