// Declare a C#-style property with a Get and Set method.

#if ! defined(__PROPERTY_DEFINED__)
#define __PROPERTY_DEFINED__

template <class T> class Property
{
public:
  Property() { }
  Property(const T& value) : data(value) { }

  operator T() const { return data; }
  Property& operator=(const T& value) { data = value; return *this; }

private:
  T data;
};

#endif // __PROPERTY_DEFINED__