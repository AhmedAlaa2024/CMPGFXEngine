#pragma once

#include <glad/gl.h>
#include "vertex.hpp"
#include <iostream>

namespace our {

    #define ATTRIB_LOC_POSITION 0
    #define ATTRIB_LOC_COLOR    1
    #define ATTRIB_LOC_TEXCOORD 2
    #define ATTRIB_LOC_NORMAL   3

    class Mesh {
        // Here, we store the object names of the 3 main components of a mesh:
        // A vertex array object, A vertex buffer and an element buffer
        unsigned int VBO, EBO;
        unsigned int VAO;
        // We need to remember the number of elements that will be draw by glDrawElements 
        GLsizei elementCount;
    public:

        // The constructor takes two vectors:
        // - vertices which contain the vertex data.
        // - elements which contain the indices of the vertices out of which each rectangle will be constructed.
        // The mesh class does not keep a these data on the RAM. Instead, it should create
        // a vertex buffer to store the vertex data on the VRAM,
        // an element buffer to store the element data on the VRAM,
        // a vertex array object to define how to read the vertex & element buffer during rendering 
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& elements)
        {
            //TODO: (Req 2) Write this function
            // remember to store the number of elements in "elementCount" since you will need it for drawing
            // For the attribute locations, use the constants defined above: ATTRIB_LOC_POSITION, ATTRIB_LOC_COLOR, etc

            elementCount = elements.size();

            // bind EBO to GL_ELEMENT_ARRAY_BUFFER to enable dealing with it in the current context
            // store vertices data in buffers by passing data, its size, and usage 
            // usage GL_STATIC_DRAW : We won't change data and will use it in drawing
            //////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////
            // enable sending data to attribute location
            // use vertex array to tell OpenGL where to get the data for attributes
            // attribute location, element size(3 for xyz, 4 for rgba), type, normalized or not, stride/jump size after reading 1 element, start 

            // generates 1 vertex array, give it pointer to VAO to put name of generated vertex array in it.
            // Syntax: void glGenVertexArrays(GLsizei n, GLuint* arrays);
            glGenVertexArrays(1, &VAO);
            // bind VAO to enable dealing with it in the current context
            // Syntax: void glBindVertexArray(GLuint array);
            glBindVertexArray(VAO);

            // generates 1 empty(no space of memory is given to it) buffer, give it pointer to VBO to put name of generated buffer in it. 
            // Syntax: void glGenBuffers(GLsizei n, GLuint* buffers);
            glGenBuffers(1, &VBO);
            // bind VBO to GL_ARRAY_BUFFER to enable dealing with it in the current context
            // Syntax: void glBindBuffer(GLuint buffer)
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
             // store vertices data in buffers
             // void glBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
            glBufferData(GL_ARRAY_BUFFER, (vertices.size())*sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
            // enable sending data to attribute location
            // Syntax: void glEnableVertexAttribArray(GLuint index);
            glEnableVertexAttribArray(ATTRIB_LOC_POSITION);
            // use vertex array to tell OpenGL where to get the data for attributes
            // Syntax: void glVertexAttribPointer( attribute location, element size(3 for xyz), type, normalized or not, stride/jump size after reading 1 element, start);
            glVertexAttribPointer(ATTRIB_LOC_POSITION, 3, GL_FLOAT, false, sizeof(Vertex), 0);
            // enable sending data to attribute location
            glEnableVertexAttribArray(ATTRIB_LOC_COLOR);
            // use vertex array to tell OpenGL where to get the data for attributes
            // Syntax: void glVertexAttribPointer( attribute location, element size(4 for rgba), type, normalized or not, stride/jump size after reading 1 element, start);
            glVertexAttribPointer(ATTRIB_LOC_COLOR, 4, GL_UNSIGNED_BYTE, true, sizeof(Vertex), (void*)offsetof(Vertex, color));
            // enable sending data to attribute location
            glEnableVertexAttribArray(ATTRIB_LOC_TEXCOORD);
            // Syntax: void glVertexAttribPointer( attribute location, element size(2 for xy), type, normalized or not, stride/jump size after reading 1 element, start);
            glVertexAttribPointer(ATTRIB_LOC_TEXCOORD, 2, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));
            // enable sending data to attribute location           
            glEnableVertexAttribArray(ATTRIB_LOC_NORMAL);
            // Syntax: void glVertexAttribPointer( attribute location, element size(2 for xyz), type, normalized or not, stride/jump size after reading 1 element, start);
            glVertexAttribPointer(ATTRIB_LOC_NORMAL, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, normal));

            // generates 1 empty(no space of memory is given to it) buffer, give it pointer to VBO to put name of generated buffer in it. 
            glGenBuffers(1, &EBO);
            // bind EBO to enable dealing with it in the current context
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            // store elements data in buffer
            // void glBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, (elements.size())*sizeof(unsigned int), elements.data(), GL_STATIC_DRAW);
            // unbind the currently bound vertex array object (VAO)
            // Syntax: void glBindVertexArray(GLuint array);
            glBindVertexArray(0);

        }

        // this function should render the mesh
        void draw() 
        {
            //TODO: (Req 2) Write this function
            glBindVertexArray(this->VAO);
            
            glDrawElements(GL_TRIANGLES, this->elementCount, GL_UNSIGNED_INT, (void*)0);
        }

        // this function should delete the vertex & element buffers and the vertex array object
        ~Mesh(){
            //TODO: (Req 2) Write this function
            // The glDeleteBuffers() function is used to delete buffer objects
            // Syntax: void glDeleteBuffers(GLsizei n, const GLuint* buffers)
            glDeleteBuffers(1, &VBO);
            // The glDeleteBuffers() function is used to delete buffer objects
            // Syntax: void glDeleteBuffers(GLsizei n, const GLuint* buffers)
            glDeleteBuffers(1, &EBO);
            // The glDeleteVertexArrays() function is used to delete vertex array object (VAO)
            // Syntax: void glDeleteVertexArrays(GLsizei n, const GLuint* arrays)
            glDeleteVertexArrays(1, &VAO);
        }

        Mesh(Mesh const &) = delete;
        Mesh &operator=(Mesh const &) = delete;
    };

}