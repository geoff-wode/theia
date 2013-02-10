/// Declare the interface which must be defined by users to enable a Theia-based application to be started.


#if ! defined(__THEIA_PROGRAM__)
#define __THEIA_PROGRAM__

namespace theia
{
  namespace Program
  {
    void Start();
  };
}

#endif // __THEIA_PROGRAM__
