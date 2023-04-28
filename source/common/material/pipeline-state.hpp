#pragma once

#include <glad/gl.h>
#include <glm/vec4.hpp>
#include <json/json.hpp>

namespace our {
    // There are some options in the render pipeline that we cannot control via shaders
    // such as blending, depth testing and so on
    // Since each material could require different options (e.g. transparent materials usually use blending),
    // we will encapsulate all these options into a single structure that will also be responsible for configuring OpenGL's pipeline
    struct PipelineState {
        // This set of pipeline options specifies whether face culling will be used or not and how it will be configured
        struct {
            bool enabled = false;
            GLenum culledFace = GL_BACK;
            GLenum frontFace = GL_CCW;
        } faceCulling;

        // This set of pipeline options specifies whether depth testing will be used or not and how it will be configured
        struct {
            bool enabled = false;
            GLenum function = GL_LEQUAL; //Passes if the incoming depth value is less than or equal to the stored depth value.
        } depthTesting;

        // This set of pipeline options specifies whether blending will be used or not and how it will be configured
        struct {
            bool enabled = false;
            GLenum equation = GL_FUNC_ADD;
            GLenum sourceFactor = GL_SRC_ALPHA;
            GLenum destinationFactor = GL_ONE_MINUS_SRC_ALPHA;
            glm::vec4 constantColor = {0, 0, 0, 0};
        } blending;

        // These options specify the color and depth mask which can be used to
        // prevent the rendering/clearing from modifying certain channels of certain targets in the framebuffer
        glm::bvec4 colorMask = {true, true, true, true}; // To know how to use it, check glColorMask
        bool depthMask = true; // To know how to use it, check glDepthMask


        // This function should set the OpenGL options to the values specified by this structure
        // For example, if faceCulling.enabled is true, you should call glEnable(GL_CULL_FACE), otherwise, you should call glDisable(GL_CULL_FACE)
        void setup() const {
            //TODO: (Req 4) Write this function
             if(faceCulling.enabled){
                //Syntax : glEnable(GLenum cap)
                glEnable(GL_CULL_FACE);
                //tell which face to ignore 
                //Syntax: glCullFace(GLenum mode);
                glCullFace(faceCulling.culledFace);
                //direction of the face that matters to me
                //Syntax: glFrontFace(GLenum mode);
                glFrontFace(faceCulling.frontFace);
            } else{
                //Syntax: glDisable(GLenum cap);
                glDisable(GL_CULL_FACE);
            }
            if(depthTesting.enabled){
                //ennabled to know which object should be infront of which object according to the depth
                //Syntax : glEnable(GLenum cap)
                glEnable(GL_DEPTH_TEST);
                //Syntax: glDepthFunc(GLenum func);
                glDepthFunc(depthTesting.function);
            }else{
                //Syntax: glDisable(GLenum cap);
                glDisable(GL_DEPTH_TEST);
            }
            if(blending.enabled){
                //Syntax : glEnable(GLenum cap)
                glEnable(GL_BLEND);
                //blending = (source) (equation) (destination)
                //Syntax: glBlendEquation(	GLenum mode)
                glBlendEquation(blending.equation);
                //Syntax: glBlendFunc(GLenum sfactor,GLenum dfactor)
                glBlendFunc(blending.sourceFactor,blending.destinationFactor);
                //Syntax: glBlendColor(	GLclampf red,GLclampf green,GLclampf blue,GLclampf alpha);
                glBlendColor(blending.constantColor.r, blending.constantColor.g, blending.constantColor.b, blending.constantColor.a);
                
            }else{
                //Syntax: glDisable(GLenum cap);
                glDisable(GL_BLEND);
            }
            //specify whether the individual color components in the frame buffer can or cannot be written
            //Syntax:glColorMask(GLboolean red,GLboolean green,GLboolean blue,GLboolean alpha);
            glColorMask(colorMask.r,colorMask.g,colorMask.b,colorMask.a);
            //enable or disable writing into the depth buffer
            //Syntax:glDepthMask(GLboolean flag);
            glDepthMask(depthMask);
        }

        // Given a json object, this function deserializes a PipelineState structure
        void deserialize(const nlohmann::json& data);
    };

}