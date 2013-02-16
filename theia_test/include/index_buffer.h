#if ! defined(__INDEX_BUFFER__)
#define __INDEX_BUFFER__

#include <gl_loader.h>
#include <vertex_decl.h>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

class IndexBuffer;
typedef boost::shared_ptr<IndexBuffer> IndexBufferPtr;

namespace IndexBufferDataType
{
  enum Enum
  {
    UnsignedShort,
    UnsignedInt
  };
}

class IndexBuffer
{
public:
  struct Factory
  {
    static IndexBufferPtr New();
  };

  ~IndexBuffer();

  void Configure(size_t numIndices, IndexBufferDataType::Enum dataType);

  size_t GetNumIndices() const;
  IndexBufferDataType::Enum GetDataType() const;
  void SetData(size_t dataSize, size_t offset, const void* data);

  void Apply() const;

private:
  IndexBuffer();
  struct Impl;
  boost::scoped_ptr<Impl> impl;
};

#endif // __INDEX_BUFFER__
