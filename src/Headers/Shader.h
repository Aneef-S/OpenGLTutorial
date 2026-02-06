

class Shader
{
  private:
    unsigned int shaderProgramID;
  public:
  
    /// @brief 
    /// @param vertexShaderFilePath Path to the vertex shader file
    /// @param fragmentShaderFilePath Path to the fragment shader file
    Shader(const char* vertexShaderFilePath,const char* fragmentShaderFilePath);

    /// @brief Set a Boolean uniform in the shader
    /// @param boolName Name of the uniform
    /// @param value Value of the uniform
    void SetBool(const std::string &boolName,bool value) const;

    /// @brief Set a Integer uniform in the shader
    /// @param boolName Name of the uniform
    /// @param value Value of the uniform
    void SetInt(const std::string &intName,int value) const;

    /// @brief Set a Float uniform in the shader
    /// @param boolName Name of the uniform
    /// @param value Value of the uniform
    void SetFloat(const std::string &floatName,float value)const;

    /// @brief User the Shader
    void Use();
};