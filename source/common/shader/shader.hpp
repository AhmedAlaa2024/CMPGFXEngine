#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <iostream>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace our
{

    class ShaderProgram
    {

    private:
        // Shader Program Handle (OpenGL object name)
        GLuint program;

    public:
        ShaderProgram()
        {
            // TODO: (Req 1) Create A shader program
            program = glCreateProgram();
        }
        ~ShaderProgram()
        {
            // TODO: (Req 1) Delete a shader program
            glDeleteProgram(program);
        }

        bool attach(const std::string &filename, GLenum type) const;

        bool link() const;

        void use()
        {
            glUseProgram(program);
        }

        GLuint getUniformLocation(const std::string &name)
        {
            // TODO: (Req 1) Return the location of the uniform with the given name
            GLuint loction = glGetUniformLocation(program, name.c_str());
            return loction;
        }

        void set(const std::string &uniform, GLfloat value)
        {
            // TODO: (Req 1) Send the given float value to the given uniform
            GLuint uniform_loc = getUniformLocation(uniform);
            if (uniform_loc == -1)
                std::cout << "Error in finding loction of <float> Uniform [" << uniform.c_str() << "]" << std::endl;
            glUniform1f(uniform_loc, value);
        }

        void set(const std::string &uniform, GLuint value)
        {
            // TODO: (Req 1) Send the given unsigned integer value to the given uniform
            GLuint uniform_loc = getUniformLocation(uniform);
            if (uniform_loc == -1)
                std::cout << "Error in finding loction of <unsigned int> Uniform [" << uniform.c_str() << "]" << std::endl;
            glUniform1ui(uniform_loc, value);
        }

        void set(const std::string &uniform, GLint value)
        {
            // TODO: (Req 1) Send the given integer value to the given uniform
            GLuint uniform_loc = getUniformLocation(uniform);
            if (uniform_loc == -1)
                std::cout << "Error in finding loction of <integer> Uniform [" << uniform.c_str() << "]" << std::endl;
            glUniform1i(uniform_loc, value);
        }

        void set(const std::string &uniform, glm::vec2 value)
        {
            // TODO: (Req 1) Send the given 2D vector value to the given uniform
            GLuint uniform_loc = getUniformLocation(uniform);
            if (uniform_loc == -1)
                std::cout << "Error in finding loction of <vec2> Uniform [" << uniform.c_str() << "]" << std::endl;
            glUniform2fv(uniform_loc, 1, &value[0]);
        }

        void set(const std::string &uniform, glm::vec3 value)
        {
            // TODO: (Req 1) Send the given 3D vector value to the given uniform
            GLuint uniform_loc = getUniformLocation(uniform);
            if (uniform_loc == -1)
                std::cout << "Error in finding loction of <vec3> Uniform [" << uniform.c_str() << "]" << std::endl;
            glUniform3fv(uniform_loc, 1, &value[0]);
        }

        void set(const std::string &uniform, glm::vec4 value)
        {
            // TODO: (Req 1) Send the given 4D vector value to the given uniform
            GLuint uniform_loc = getUniformLocation(uniform);
            if (uniform_loc == -1)
                std::cout << "Error in finding loction of <vec4> Uniform [" << uniform.c_str() << "]" << std::endl;
            glUniform4fv(uniform_loc, 1, &value[0]);
        }

        void set(const std::string &uniform, glm::mat4 matrix)
        {
            // TODO: (Req 1) Send the given matrix 4x4 value to the given uniform
            GLuint uniform_loc = getUniformLocation(uniform);
            if (uniform_loc == -1)
                std::cout << "Error in finding loction of <mat4> Uniform [" << uniform.c_str() << "]" << std::endl;
            glUniformMatrix4fv(uniform_loc, 1, false, &matrix[0][0]);
        }

        // TODO: (Req 1) Delete the copy constructor and assignment operator.
        ShaderProgram &operator=(const ShaderProgram &) = delete;
        ShaderProgram (const ShaderProgram &) = delete;
        // Question 2: copy constructor ()==>default
        // Question: Why do we delete the copy constructor and assignment operator?
        // to prevent deleting program from one and used by the second
    };

}

#endif