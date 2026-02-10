#ifndef GLAD_D_H
#define GLAD_D_H

#include <glad/glad.h>


/** 
 * glad with documentation.
 * Simple header files with added description for each glad functions.(since the pure glad is hell a hard for understand)
 * All of the definitions are REFERED from https://learnopengl.com/. (Check out the page guys, its free)
 * I have build this since I have a good IDE and enough time to waste. You can use this if you have a good IDE/Code editor and don't have enough.
 
  **/

/// @brief OpenGL's glGenBuffers function generates one or multiple buffer object(s). Buffer objects are instantiated behind the scenes and reference ID(s) are returned to the user to access those buffer objects. The type of the returned IDs is GLuint.
/// @param size Defines how many buffer objects you would like to generate. gIGenBuffers Will return this many buffer IDs to the target buffers array.
/// @param buffers Buffer is a GLuint array where glGenBuffers will store its resulting buffer reference IDs
void glGenBuffers_d(GLsizei size,GLuint *buffers)
{
  glGenBuffers(size,buffers);
}


/// @brief The function glBindBuffer binds a buffer object to the current buffer type target. Only a single buffer can be bound for each buffer type. Binding e as a buffer resets the currently bound buffer to a NULL-Iike state.
/// @param target target: Specifies the target buffer object of which the most common are GL_ARRAY_BUFFER and GL_ELEMENT_ARRAY_BUFFER.
/// @param buffer The buffer object's reference ID of the buffer you'd like to bind.
void glBindBuffer_d(GLenum target,GLuint buffer)
{
  glBindBuffer(target,buffer);
}

/// @brief The function gIBufferData allocates memory and stores data within the initialized memory in the currently bound buffer object. One of many functions to add/edit the buffer of a buffer object
/// @param mode Specifies the target buffer object of which the most common are GL_ARRAY_BUFFER and GL_ELEMENT_ARRAY_BUFFER.
/// @param size Specifies the size in bytes of the buffer object's new data.
/// @param data Specifies a pointer to data that will be copied into the buffer, or NULL if no data is to be copied (leaves allocted memory empty)
/// @param usage usage: Specifies the expected usage pattern of the data. Most common are GL_STATIC_DRAW,GL_DYNAMIC_DRAW and GL_STREAM_DRAW.
void glBufferData_d(GLenum mode,GLsizeiptr size,const GLvoid* data,GLenum usage)
{
  glBufferData(mode,size,data,usage);
}


#endif