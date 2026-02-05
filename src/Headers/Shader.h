

class Shader
{
  private:
    unsigned int shaderProgram;
  public:
    Shader(const char* vertexShaderFilePath,const char* fragmentShaderFilePath);
    void Use();
};