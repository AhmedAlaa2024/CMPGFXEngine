#include "forward-renderer.hpp"
#include "../mesh/mesh-utils.hpp"
#include "../texture/texture-utils.hpp"

namespace our
{

    void ForwardRenderer::initialize(glm::ivec2 windowSize, const nlohmann::json &config)
    {
        // First, we store the window size for later use
        this->windowSize = windowSize;

        // Then we check if there is a sky texture in the configuration
        if (config.contains("sky"))
        {
            // First, we create a sphere which will be used to draw the sky
            this->skySphere = mesh_utils::sphere(glm::ivec2(16, 16));

            // We can draw the sky using the same shader used to draw textured objects
            ShaderProgram *skyShader = new ShaderProgram();
            skyShader->attach("assets/shaders/textured.vert", GL_VERTEX_SHADER);
            skyShader->attach("assets/shaders/textured.frag", GL_FRAGMENT_SHADER);
            skyShader->link();

            // TODO: (Req 10) Pick the correct pipeline state to draw the sky
            //  Hints: the sky will be draw after the opaque objects so we would need depth testing but which depth funtion should we pick?
            //  We will draw the sphere from the inside, so what options should we pick for the face culling.
            PipelineState skyPipelineState{};
            skyPipelineState.depthTesting.enabled = true;
            skyPipelineState.depthTesting.function = GL_LEQUAL;
            skyPipelineState.blending.enabled = true;
            skyPipelineState.faceCulling.enabled = true;
            skyPipelineState.faceCulling.culledFace = GL_FRONT;
            skyPipelineState.setup();

            // Load the sky texture (note that we don't need mipmaps since we want to avoid any unnecessary blurring while rendering the sky)
            std::string skyTextureFile = config.value<std::string>("sky", "");
            Texture2D *skyTexture = texture_utils::loadImage(skyTextureFile, false);

            // Setup a sampler for the sky
            Sampler *skySampler = new Sampler();
            skySampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            skySampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            skySampler->set(GL_TEXTURE_WRAP_S, GL_REPEAT);
            skySampler->set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Combine all the aforementioned objects (except the mesh) into a material
            this->skyMaterial = new TexturedMaterial();
            this->skyMaterial->shader = skyShader;
            this->skyMaterial->texture = skyTexture;
            this->skyMaterial->sampler = skySampler;
            this->skyMaterial->pipelineState = skyPipelineState;
            this->skyMaterial->tint = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            this->skyMaterial->alphaThreshold = 1.0f;
            this->skyMaterial->transparent = false;
        }

        // Then we check if there is a postprocessing shader in the configuration
        if (config.contains("postprocess"))
        {
            // TODO: (Req 11) Create a framebuffer
            glGenFramebuffers(1, &postprocessFrameBuffer);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postprocessFrameBuffer);

            // TODO: (Req 11) Create a color and a depth texture and attach them to the framebuffer
            //  Hints: The color format can be (Red, Green, Blue and Alpha components with 8 bits for each channel).
            //  The depth format can be (Depth component with 24 bits).

            colorTarget = new our::Texture2D();
            colorTarget->bind();
            GLuint rt_levels = (GLuint)glm::floor(glm::log2(glm::max<float>((float)windowSize.x, (float)windowSize.y))) + 1;
            // we allocate memory for the texture
            glTexStorage2D(GL_TEXTURE_2D, rt_levels, GL_RGBA8, windowSize.x, windowSize.y);
            // glFramebufferTexture2D attaches a texture to the framebuffer. The texture can be a color or depth texture.
            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTarget->getOpenGLName(), 0);

            depthTarget = new our::Texture2D();
            depthTarget->bind();
            glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, windowSize.x, windowSize.y);

            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTarget->getOpenGLName(), 0);

            // TODO: (Req 11) Unbind the framebuffer just to be safe
            // We need to unbind the framebuffer so that we can render to the default framebuffer.
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

            // Create a vertex array to use for drawing the texture
            glGenVertexArrays(1, &postProcessVertexArray);

            // Create a sampler to use for sampling the scene texture in the post processing shader
            Sampler *postprocessSampler = new Sampler();
            postprocessSampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            postprocessSampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            postprocessSampler->set(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            postprocessSampler->set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Create the post processing shader
            ShaderProgram *postprocessShader = new ShaderProgram();
            postprocessShader->attach("assets/shaders/fullscreen.vert", GL_VERTEX_SHADER);
            postprocessShader->attach(config.value<std::string>("postprocess", ""), GL_FRAGMENT_SHADER);
            postprocessShader->link();

            if (config.contains("addedTex"))
            {
                additionalTexture = texture_utils::loadImage(config.value<std::string>("addedTex", ""));
            }
            effect_power = config.value("effect_power", effect_power);
            exposure = config.value("exposure", exposure);
            fog_power = config.value("fog_power", fog_power);
            fog_distance = config.value("fog_distance", fog_distance);

            // Create a post processing material
            postprocessMaterial = new TexturedMaterial();
            postprocessMaterial->shader = postprocessShader;
            postprocessMaterial->texture = colorTarget;
            postprocessMaterial->sampler = postprocessSampler;
            // The default options are fine but we don't need to interact with the depth buffer
            // so it is more performant to disable the depth mask
            postprocessMaterial->pipelineState.depthMask = false;
        }
    }

    void ForwardRenderer::destroy()
    {
        // Delete all objects related to the sky
        if (skyMaterial)
        {
            delete skySphere;
            delete skyMaterial->shader;
            delete skyMaterial->texture;
            delete skyMaterial->sampler;
            delete skyMaterial;
        }
        // Delete all objects related to post processing
        if (postprocessMaterial && apply)
        {
            glDeleteFramebuffers(1, &postprocessFrameBuffer);
            glDeleteVertexArrays(1, &postProcessVertexArray);
            delete colorTarget;
            delete depthTarget;
            delete postprocessMaterial->sampler;
            delete postprocessMaterial->shader;
            delete postprocessMaterial;
        }
    }

    void ForwardRenderer::render(World *world)
    {
        // First of all, we search for a camera and for all the mesh renderers
        CameraComponent *camera = nullptr;
        opaqueCommands.clear();
        transparentCommands.clear();
        // remove all elements from the lightSources vector and resetting its size to 0, lightSources is a vector of LightComponent pointers
        lightSources.clear();
        for (auto entity : world->getEntities())
        {
            // If we hadn't found a camera yet, we look for a camera in this entity
            if (!camera)
                camera = entity->getComponent<CameraComponent>();
            // If this entity has a mesh renderer component
            if (auto meshRenderer = entity->getComponent<MeshRendererComponent>(); meshRenderer)
            {
                // We construct a command from it
                RenderCommand command;
                command.localToWorld = meshRenderer->getOwner()->getLocalToWorldMatrix();
                command.center = glm::vec3(command.localToWorld * glm::vec4(0, 0, 0, 1));
                command.mesh = meshRenderer->mesh;
                command.material = meshRenderer->material;
                // if it is transparent, we add it to the transparent commands list
                if (command.material->transparent)
                {
                    transparentCommands.push_back(command);
                }
                else
                {
                    // Otherwise, we add it to the opaque command list
                    opaqueCommands.push_back(command);
                }
            }
            // if this entity has a light component store it in lightSources vector
            if (auto lightComp = entity->getComponent<LightComponent>(); lightComp)
            {
                lightSources.push_back(lightComp);
            }
        }

        // If there is no camera, we return (we cannot render without a camera)
        if (camera == nullptr)
            return;

        // TODO: (Req 9) Modify the following line such that "cameraForward" contains a vector pointing the camera forward direction
        //  HINT: See how you wrote the CameraComponent::getViewMatrix, it should help you solve this one
        // Explaination: The following line computes the forward direction of the camera in world space.
        // It does this by transforming the negative z-axis direction of the camera's local coordinate system
        // into world space using the camera's owner's local-to-world matrix.
        glm::vec3 cameraForward = camera->getOwner()->getLocalToWorldMatrix() * glm::vec4(0, 0, -1.0f, 1.0f);

        // Explaination: The following line computes the position of the camera in world space.
        // It does this by inverting the camera's view matrix and then taking the fourth column of the resulting matrix,
        // which represents the translation component of the matrix.
        glm::vec3 cameraPosition = glm::inverse(camera->getViewMatrix())[3];

        // Explaination: The following line sorts the transparent commands by distance from the camera's position along the camera's forward direction.
        // It does this by computing the dot product between the camera's forward direction and
        // the vector from the camera to the center of each command's bounding box.
        // Commands with smaller dot products (i.e., closer to the camera along the forward direction) will be drawn first.
        std::sort(transparentCommands.begin(), transparentCommands.end(), [cameraForward](const RenderCommand &first, const RenderCommand &second)
                  {
            //TODO: (Req 9) Finish this function
            // HINT: the following return should return true "first" should be drawn before "second".
            return glm::dot(first.center, cameraForward) < glm::dot(second.center, cameraForward); });

        // TODO: (Req 9) Get the camera ViewProjection matrix and store it in VP
        // Explaination: The following line computes the camera's view-projection matrix by multiplying its projection matrix and its view matrix.
        // It does this using the camera's getProjectionMatrix() and getViewMatrix() methods.
        glm::mat4 VP = camera->getProjectionMatrix(this->windowSize) * camera->getViewMatrix();

        // TODO: (Req 9) Set the OpenGL viewport using viewportStart and viewportSize
        // Explaination: The following line sets the viewport for the OpenGL context.
        // It does this by calling the glViewport() function with the origin (0,0) and the dimensions of the window.
        glViewport(0, 0, this->windowSize.x, this->windowSize.y);

        // TODO: (Req 9) Set the clear color to black and the clear depth to 1
        // Explaination: The following line clears the color and depth buffers for the current framebuffer.
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // TODO: (Req 9) Set the color mask to true and the depth mask to true (to ensure the glClear will affect the framebuffer)
        // Explaination: These lines enable writing to the color and depth buffers, respectively.
        // They do this by calling the glColorMask() and glDepthMask() functions with GL_TRUE arguments.
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDepthMask(GL_TRUE);

        // If there is a postprocess material, bind the framebuffer
        if (postprocessMaterial && apply)
        // command.material->setup();
        {
            // TODO: (Req 11) bind the framebuffer
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postprocessFrameBuffer);
        }

        // TODO: (Req 9) Clear the color and depth buffers
        // Explaination: The following line clears the color and depth buffers for the current framebuffer again,
        // to ensure they are cleared before drawing.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // TODO: (Req 9) Draw all the opaque commands
        // Don't forget to set the "transform" uniform to be equal the model-view-projection matrix for each render command
        // Explaination: The following loop draws all of the opaque commands. For each command, it sets up the material using the setup() method,
        // sets the transform uniform in the shader to the model-view-projection matrix
        // (computed as the product of the view-projection matrix and the command's local-to-world matrix), and calls the draw() method of the mesh.
        for (const auto &command : opaqueCommands)
        {
            // check if the command.material is of type LitMaterial by using a dynamic_cast
            // If the cast is successful, the resulting pointer is assigned to light_material, and the code inside the if statement is executed
            if (auto light_material = dynamic_cast<LitMaterial *>(command.material); light_material)
            {   // If we get here, the light_material pointer is valid, so we call the setup() function on it
                light_material->setup();
                // The shader uniforms for the view-projection matrix, local-to-world matrix, eye position, and inverse transpose of the local-to-world matrix are set using the set() function of the shader
                light_material->shader->set("VP", VP);
                light_material->shader->set("M", command.localToWorld);
                light_material->shader->set("eye", command.localToWorld * glm::vec4(0, 0, 0, 1));
                light_material->shader->set("M_IT", glm::transpose(glm::inverse(command.localToWorld)));
                // The number of light sources is set as an integer uniform in the shader
                light_material->shader->set("light_count", (int)lightSources.size());
                // The sky color values are set as vector uniforms in the shader
                light_material->shader->set("sky.top", glm::vec3(0.7, 0.3, 0.8));
                light_material->shader->set("sky.horizon", glm::vec3(0.7, 0.3, 0.8));
                light_material->shader->set("sky.bottom", glm::vec3(0.7, 0.3, 0.8));
                // This is a loop that iterates over all the LightComponent objects in lightSources
                for (int i = 0; i < (int)lightSources.size(); i++)
                {
                    // For each light, the position and direction of the light source in world space are calculated by multiplying the light's local-to-world matrix with the relative direction and position of light to its owner entity
                    glm::vec3 position = lightSources[i]->getOwner()->getLocalToWorldMatrix() * glm::vec4(0, 0, 0, 1);
                    glm::vec3 direction = lightSources[i]->getOwner()->getLocalToWorldMatrix() * glm::vec4(0, 0, -1, 0);
                    // the relevant light properties (type, color, attenuation, etc.) are set as uniform values in the shader using the set() function
                    light_material->shader->set("lights[" + std::to_string(i) + "].type", lightSources[i]->type);
                    light_material->shader->set("lights[" + std::to_string(i) + "].color", lightSources[i]->color);
                    light_material->shader->set("lights[" + std::to_string(i) + "].attenuation", lightSources[i]->attenuation);

                    int t = lightSources[i]->type;
                    // The switch statement is used to handle the different types of lights (directional, point, or spotlight) and set the appropriate shader uniforms
                    switch (t)
                    {
                    case 0:
                        light_material->shader->set("lights[" + std::to_string(i) + "].direction", glm::normalize(direction));
                        break;
                    case 1:
                        light_material->shader->set("lights[" + std::to_string(i) + "].position", position);
                        break;
                    case 2:
                        light_material->shader->set("lights[" + std::to_string(i) + "].position", position);
                        light_material->shader->set("lights[" + std::to_string(i) + "].direction", glm::normalize(direction));
                        light_material->shader->set("lights[" + std::to_string(i) + "].cone_angles", lightSources[i]->cone_angles);
                        break;
                    }
                }
            }
            else
            {
                command.material->setup();
                glUniformMatrix4fv(command.material->shader->getUniformLocation("transform"), 1, GL_FALSE, glm::value_ptr(VP * command.localToWorld));
            }
            command.mesh->draw();
        }

        // If there is a sky material, draw the sky
        if (this->skyMaterial)
        {
            // TODO: (Req 10) setup the sky material
            this->skyMaterial->setup();

            // TODO: (Req 10) Get the camera position
            // Explaination: gets the camera's position by inverting its view matrix and accessing the fourth column vector
            // (which corresponds to the camera position in homogeneous coordinates).
            cameraPosition = glm::inverse(camera->getViewMatrix())[3];

            // TODO: (Req 10) Create a model matrix for the sky such that it always follows the camera (sky sphere center = camera position)
            // Explaination: creates a model matrix for the sky such that it always follows the camera's position.
            // The glm::translate() function creates a translation matrix from the identy to the camera's position.
            glm::mat4 skyModelMatrix = glm::translate(glm::mat4(1.0f), cameraPosition);

            // TODO: (Req 10) We want the sky to be drawn behind everything (in NDC space, z=1)
            //  We can acheive the is by multiplying by an extra matrix after the projection but what values should we put in it?
            // Explaination: creates a matrix that will move the sky behind everything by setting its z-coordinate to 1 in homogeneous coordinates.
            // This is done by setting the (3,3) element to 0, and the (3,2) element to 1.
            glm::mat4 alwaysBehindTransform = glm::mat4(
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 1.0f);

            // TODO: (Req 10) set the "transform" uniform
            glm::mat4 transform = alwaysBehindTransform * camera->getProjectionMatrix(this->windowSize) * camera->getViewMatrix() * skyModelMatrix;

            // sets the transform uniform in the shader to the model-view-projection matrix
            // (computed as the product of the view-projection matrix and the command's local-to-world matrix)
            glUniformMatrix4fv(this->skyMaterial->shader->getUniformLocation("transform"), 1, GL_FALSE, glm::value_ptr(transform));

            // TODO: (Req 10) draw the sky sphere
            this->skySphere->draw();
        }

        // TODO: (Req 9) Draw all the transparent commands
        //  Don't forget to set the "transform" uniform to be equal the model-view-projection matrix for each render command
        // Explaination: The following loop draws all of the transparent commands. For each command, it sets up the material using the setup() method,
        // sets the transform uniform in the shader to the model-view-projection matrix
        // (computed as the product of the view-projection matrix and the command's local-to-world matrix), and calls the draw() method of the mesh.
        for (const auto &command : transparentCommands)
        {

            // command.material->setup();
            if (auto light_material = dynamic_cast<LitMaterial *>(command.material); light_material)
            {   // If we get here, the light_material pointer is valid, so we call the setup() function on it
                light_material->setup();
                // The shader uniforms for the view-projection matrix, local-to-world matrix, eye position, and inverse transpose of the local-to-world matrix are set using the set() function of the shader
                light_material->shader->set("VP", VP);
                light_material->shader->set("M", command.localToWorld);
                light_material->shader->set("eye", command.localToWorld * glm::vec4(0, 0, 0, 1));
                light_material->shader->set("M_IT", glm::transpose(glm::inverse(command.localToWorld)));
                // The number of light sources is set as an integer uniform in the shader
                light_material->shader->set("light_count", (int)lightSources.size());
                // // The sky color values are set as vector uniforms in the shader
                light_material->shader->set("sky.top", glm::vec3(0.0f, 0.1f, 0.5f));
                light_material->shader->set("sky.horizon", glm::vec3(0.3f, 0.3f, 0.3f));
                light_material->shader->set("sky.bottom", glm::vec3(0.1f, 0.1f, 0.1f));
                // This is a loop that iterates over all the LightComponent objects in lightSources
                for (int i = 0; i < (int)lightSources.size(); i++)
                {
                    // For each light, the position and direction of the light source in world space are calculated by multiplying the light's local-to-world matrix with the relative direction and position of light to its owner entity
                    glm::vec3 position = lightSources[i]->getOwner()->getLocalToWorldMatrix() * glm::vec4(0, 0, 0, 1);
                    glm::vec3 direction = lightSources[i]->getOwner()->getLocalToWorldMatrix() * glm::vec4(0, 0, -1, 0);
                    // the relevant light properties (type, color, attenuation, etc.) are set as uniform values in the shader using the set() function
                    light_material->shader->set("lights[" + std::to_string(i) + "].type", lightSources[i]->type);
                    light_material->shader->set("lights[" + std::to_string(i) + "].color", lightSources[i]->color);
                    light_material->shader->set("lights[" + std::to_string(i) + "].attenuation", lightSources[i]->attenuation);

                    int t = lightSources[i]->type;
                    // The switch statement is used to handle the different types of lights (directional, point, or spotlight) and set the appropriate shader uniforms
                    switch (t)
                    {
                    case 0:
                        light_material->shader->set("lights[" + std::to_string(i) + "].direction", glm::normalize(direction));
                        break;
                    case 1:
                        light_material->shader->set("lights[" + std::to_string(i) + "].position", position);
                        break;
                    case 2:
                        light_material->shader->set("lights[" + std::to_string(i) + "].position", position);
                        light_material->shader->set("lights[" + std::to_string(i) + "].direction", glm::normalize(direction));
                        light_material->shader->set("lights[" + std::to_string(i) + "].cone_angles", lightSources[i]->cone_angles);
                        break;
                    }
                }
            }
            else
            {
                command.material->setup();
                glUniformMatrix4fv(command.material->shader->getUniformLocation("transform"), 1, GL_FALSE, glm::value_ptr(VP * command.localToWorld));
            }
            command.mesh->draw();
        }

        // If there is a postprocess material, apply postprocessing
        if (postprocessMaterial && apply)
        { // TODO: (Req 11) Return to the default framebuffer
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            // TODO: (Req 11) Setup the postprocess material and draw the fullscreen triangle
            postprocessMaterial->setup();
            //////////// FOR FOG///////////////////////////////////////////
            //Activate texture 1 to bind depth texture 
            glActiveTexture(GL_TEXTURE1);
            // binds the depth texture to the current texture unit
            depthTarget->bind();
            postprocessMaterial->sampler->bind(1);
            postprocessMaterial->shader->set("depth_sampler", 1);
            postprocessMaterial->shader->set("inverse_projection", glm::inverse(camera->getProjectionMatrix(windowSize)));
            postprocessMaterial->shader->set("fog_power", fog_power);
            postprocessMaterial->shader->set("fog_exponent", fog_distance);
            // glActiveTexture(GL_TEXTURE0);
            //////////// FOR FOG///////////////////////////////////////////

            glBindVertexArray(postProcessVertexArray);
            // ////////// FOR Distortion ///////////////////////////////////////////
            // if (additionalTexture)
            // {
            //     glActiveTexture(GL_TEXTURE1);
            //     additionalTexture->bind();
            //     postprocessMaterial->sampler->bind(1);
            //     postprocessMaterial->shader->set("additional_sampler", 1);
            //     postprocessMaterial->shader->set("effect_power", effect_power); // TODO
            // }
            // ////////// FOR Distortion ///////////////////////////////////////////

            //////////// FOR Tone ///////////////////////////////////////////

            //     postprocessMaterial->shader->set("exposure", exposure);

            //////////// FOR Tone ///////////////////////////////////////////
            // Specifies the starting index in the enabled arrays.
            // Specifies the number of indices to be rendered.
            glDrawArrays(GL_TRIANGLES, GLint(0), 4);
        }
    }
}