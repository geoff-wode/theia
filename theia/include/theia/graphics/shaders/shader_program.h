/// Declare a GPU shader program object.

#if ! defined(__THEIA_GFX_SHADER_PROGRAM__)
#define __THEIA_GFX_SHADER_PROGRAM__

namespace theia
{
  namespace graphics
  {
    class ShaderProgram
    {
    public:
      ~ShaderProgram();

      /// Create a shader program containing a vertex and fragment stage.
      ///
      /// @param[in] vertexSrc    Pointer to the nul-terminated source code of the vertex shader stage source code.
      /// @param[in] fragmentSrc  Pointer to the nul-terminated source code of the fragment shader stage source code.
      ///
      /// @return A pointer to a newly created shader program on success or NULL otherwise.
      static ShaderProgram* Create(const char* vertexSrc, const char* fragmentSrc);

      /// Create a shader program containing vertex, geometry and fragment stages.
      ///
      /// @param[in] vertexSrc    Pointer to the nul-terminated source code of the vertex shader source code.
      /// @param[in] geometrySrc  Pointer to the nul-terminated source code of the geometry shader source code.
      /// @param[in] fragmentSrc  Pointer to the nul-terminated source code of the fragment shader source code.
      ///
      /// @return A pointer to a newly created shader program on success or NULL otherwise.
      static ShaderProgram* Create(const char* vertexSrc, const char* geometrySrc, const char* fragmentSrc);

    private:
      ShaderProgram();
      ShaderProgram(const ShaderProgram&);
      ShaderProgram& operator=(const ShaderProgram&);

      class Impl;
      Impl* impl;
    };
  }
}

#endif // __THEIA_GFX_SHADER_PROGRAM__
