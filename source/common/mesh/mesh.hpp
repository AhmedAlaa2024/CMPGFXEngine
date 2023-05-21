#pragma once

#include <glad/gl.h>
#include "vertex.hpp"
#include <iostream>

namespace our
{

#define ATTRIB_LOC_POSITION 0
#define ATTRIB_LOC_COLOR 1
#define ATTRIB_LOC_TEXCOORD 2
#define ATTRIB_LOC_NORMAL 3

    class Mesh
    {
        // Here, we store the object names of the 3 main components of a mesh:
        // A vertex array object, A vertex buffer and an element buffer
        unsigned int VBO, EBO;
        unsigned int VAO;
        // We need to remember the number of elements that will be draw by glDrawElements
        GLsizei elementCount;

        glm::vec3 center;
        float width;
        glm::vec3 xmin, xmax, ymin, ymax, zmin, zmax;
        float radius;

    public:
        // The constructor takes two vectors:
        // - vertices which contain the vertex data.
        // - elements which contain the indices of the vertices out of which each rectangle will be constructed.
        // The mesh class does not keep a these data on the RAM. Instead, it should create
        // a vertex buffer to store the vertex data on the VRAM,
        // an element buffer to store the element data on the VRAM,
        // a vertex array object to define how to read the vertex & element buffer during rendering
        Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &elements)
        {
            // TODO: (Req 2) Write this function
            //  remember to store the number of elements in "elementCount" since you will need it for drawing
            //  For the attribute locations, use the constants defined above: ATTRIB_LOC_POSITION, ATTRIB_LOC_COLOR, etc

            elementCount = elements.size();
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
            // void glBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage)
            // vertices is a std:::vector but glBufferData expects a pointer.
            // convert it to a pointer by calling "vertices.data()"
            glBufferData(GL_ARRAY_BUFFER, (vertices.size()) * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
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
            glVertexAttribPointer(ATTRIB_LOC_COLOR, 4, GL_UNSIGNED_BYTE, true, sizeof(Vertex), (void *)offsetof(Vertex, color));
            // enable sending data to attribute location
            glEnableVertexAttribArray(ATTRIB_LOC_TEXCOORD);
            // Syntax: void glVertexAttribPointer( attribute location, element size(2 for xy), type, normalized or not, stride/jump size after reading 1 element, start);
            glVertexAttribPointer(ATTRIB_LOC_TEXCOORD, 2, GL_FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, tex_coord));
            // enable sending data to attribute location
            glEnableVertexAttribArray(ATTRIB_LOC_NORMAL);
            // Syntax: void glVertexAttribPointer( attribute location, element size(2 for xyz), type, normalized or not, stride/jump size after reading 1 element, start);
            glVertexAttribPointer(ATTRIB_LOC_NORMAL, 3, GL_FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, normal));

            // generates 1 empty(no space of memory is given to it) buffer, give it pointer to VBO to put name of generated buffer in it.
            glGenBuffers(1, &EBO);
            // bind EBO to enable dealing with it in the current context
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            // store elements data in buffer
            // void glBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
            // vertices is a std:::vector but glBufferData expects a pointer.
            // convert it to a pointer by calling "vertices.data()"
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, (elements.size()) * sizeof(unsigned int), elements.data(), GL_STATIC_DRAW);
            // unbind the currently bound vertex array object (VAO)
            // Syntax: void glBindVertexArray(GLuint array);
            glBindVertexArray(0);

            // Get the max and min vertex positions in x, y, and z
            // Sort all the verteces from the minimum to the maximum according to their x, y, and z coordinates
            // The first element will be the minimum and the last element will be the maximum
            this->xmin = vertices.begin()->position;
            this->xmax = vertices.rbegin()->position;

            this->ymin = vertices.begin()->position;
            this->ymax = vertices.rbegin()->position;

            this->zmin = vertices.begin()->position;
            this->zmax = vertices.rbegin()->position;

            for (const auto &vertex : vertices)
            {
                if (vertex.position.x < xmin.x)
                    xmin = vertex.position;

                if (vertex.position.x > xmax.x)
                    xmax = vertex.position;

                if (vertex.position.y < ymin.y)
                    ymin = vertex.position;

                if (vertex.position.y > ymax.y)
                    ymax = vertex.position;

                if (vertex.position.z < zmin.z)
                    zmin = vertex.position;

                if (vertex.position.z > zmax.z)
                    zmax = vertex.position;
            }

            this->center.x = (xmin.x + xmax.x) / 2.0f;
            this->center.y = (ymin.y + ymax.y) / 2.0f;
            this->center.z = (zmin.z + zmax.z) / 2.0f;

            // Calculate the maximum distance from the center to any vertex
            this->radius = 0.0f;
            float distance = 0.0f;
            for (const auto &vertex : vertices)
            {
                distance = glm::distance(this->center, vertex.position);
                radius = std::max(this->radius, distance);
            }

        }

        // this function should render the mesh
        void draw()
        {
            // TODO: (Req 2) Write this function
            //  Explaination: This function binds the vertex array object (VAO)
            //  that has been defined in the mesh class to the OpenGL context.
            glBindVertexArray(this->VAO);

            // Explaination: The glDrawElements function specifies the primitive type (in this case, GL_TRIANGLES)
            // and the number of elements to render (in this case, this->elementCount).
            // It also specifies the data type of the indices (in this case, GL_UNSIGNED_INT)
            //  and the pointer to the index data (in this case, (void*)0).
            //  This function call uses the current bound VAO to specify the vertex attributes for the mesh.
            glDrawElements(GL_TRIANGLES, this->elementCount, GL_UNSIGNED_INT, (void *)0);
        }

        glm::vec3 getCenter() const
        {
            return this->center;
        }

        float getRadius() const
        {
            return this->radius;
        }

        // this function should return the minimum vertex position in x, y, and z
        glm::vec3 getXMin() const
        {
            return this->xmin;
        }

        glm::vec3 getXMax() const
        {
            return this->xmax;
        }

        glm::vec3 getYMin() const
        {
            return this->ymin;
        }

        glm::vec3 getYMax() const
        {
            return this->ymax;
        }

        glm::vec3 getZMin() const
        {
            return this->zmin;
        }

        glm::vec3 getZMax() const
        {
            return this->zmax;
        }

        // this function should delete the vertex & element buffers and the vertex array object
        ~Mesh()
        {
            // TODO: (Req 2) Write this function
            //  The glDeleteBuffers() function is used to delete buffer objects
            //  Syntax: void glDeleteBuffers(GLsizei n, const GLuint* buffers)
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