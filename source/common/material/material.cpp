#include "material.hpp"

#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"

namespace our {

    // This function should setup the pipeline state and set the shader to be used
    void Material::setup() const {
        //TODO: (Req 7) Write this function
        // setup the pipeline state
        pipelineState.setup();
        // set the shader to be used
        shader->use();
    }

    // This function read the material data from a json object
    void Material::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;

        if(data.contains("pipelineState")){
            pipelineState.deserialize(data["pipelineState"]);
        }
        shader = AssetLoader<ShaderProgram>::get(data["shader"].get<std::string>());
        transparent = data.value("transparent", false);
    }

    // This function should call the setup of its parent and
    // set the "tint" uniform to the value in the member variable tint 
    void TintedMaterial::setup() const {
        //TODO: (Req 7) Write this function
        // call the setup of its parent (Material class)
        Material::setup();
        // set the "tint" uniform to the value in the member variable tint set(uniform name, value)
        shader->set("tint",tint);
    }

    // This function read the material data from a json object
    void TintedMaterial::deserialize(const nlohmann::json& data){
        Material::deserialize(data);
        if(!data.is_object()) return;
        tint = data.value("tint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // This function should call the setup of its parent and
    // set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
    // Then it should bind the texture and sampler to a texture unit and send the unit number to the uniform variable "tex" 
    void TexturedMaterial::setup() const {
        //TODO: (Req 7) Write this function
        // should call the setup of its parent (TintedMaterial)
        TintedMaterial::setup();
        // set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
        shader->set("alphaThreshold",alphaThreshold);
        // activate texture unit 0
        glActiveTexture(GL_TEXTURE0);
        // check if texture and sampler aren't null
        if (this->texture && this->sampler) {
        // bind texture to the activated texture unit
        texture->bind();
        // bind sampler to texture unit 0 which it will sample from, bind(texture unit index to be bound to)
        sampler->bind(0);
        }
        // send the unit number to the uniform variable "tex"
        shader->set("tex",0);
    }

    // This function read the material data from a json object
    void TexturedMaterial::deserialize(const nlohmann::json& data){
        TintedMaterial::deserialize(data);
        if(!data.is_object()) return;
        alphaThreshold = data.value("alphaThreshold", 0.0f);
        texture = AssetLoader<Texture2D>::get(data.value("texture", ""));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }

    void LitMaterial::setup() const {
        // should call the setup of its parent (TintedMaterial)
        Material::setup();
        // activate texture unit 0
        glActiveTexture(GL_TEXTURE0);
        // check if texture and sampler aren't null
        if (this->albedo && this->sampler) {
            // bind texture to the activated texture unit
            albedo->bind();
            // bind sampler to texture unit 0 which it will sample from, bind(texture unit index to be bound to)
            sampler->bind(0);
        }
        // send the unit number to the uniform variable "material.albedo"
        shader->set("material.albedo",0);

        // activate texture unit 1
        glActiveTexture(GL_TEXTURE1);
        // check if texture and sampler aren't null
        if (this->specular && this->sampler) {
            // bind texture to the activated texture unit
            specular->bind();
            // bind sampler to texture unit 1 which it will sample from, bind(texture unit index to be bound to)
            sampler->bind(1);
        }
        // send the unit number to the uniform variable "material.specular"
        shader->set("material.specular",1);

        // activate texture unit 2
        glActiveTexture(GL_TEXTURE2);
        // check if texture and sampler aren't null
        if (this->roughness && this->sampler) {
            // bind texture to the activated texture unit
            roughness->bind();
            // bind sampler to texture unit 2 which it will sample from, bind(texture unit index to be bound to)
            sampler->bind(2);
        }
        // send the unit number to the uniform variable "material.roughness"
        shader->set("material.roughness",2);

        // activate texture unit 3
        glActiveTexture(GL_TEXTURE3);
        // check if texture and sampler aren't null
        if (this->ao && this->sampler) {
            // bind texture to the activated texture unit
            ao->bind();
            // bind sampler to texture unit 3 which it will sample from, bind(texture unit index to be bound to)
            sampler->bind(3);
        }
        // send the unit number to the uniform variable "material.ambient_occlusion"
        shader->set("material.ambient_occlusion",3);

        // activate texture unit 4
        glActiveTexture(GL_TEXTURE4);
        // check if texture and sampler aren't null
        if (this->emissive && this->sampler) {
            // bind texture to the activated texture unit
            emissive->bind();
            // bind sampler to texture unit 3 which it will sample from, bind(texture unit index to be bound to)
            sampler->bind(4);
        }
        // send the unit number to the uniform variable "material.ambient_occlusion"
        shader->set("material.emissive",4);
    }

    // This function read the material data from a json object
    void LitMaterial::deserialize(const nlohmann::json& data){
        LitMaterial::deserialize(data);
        if(!data.is_object()) return;
        albedo = AssetLoader<Texture2D>::get(data.value("albedo", ""));
        specular = AssetLoader<Texture2D>::get(data.value("specular", ""));
        roughness = AssetLoader<Texture2D>::get(data.value("roughness", ""));
        ao = AssetLoader<Texture2D>::get(data.value("ao", ""));
        emissive = AssetLoader<Texture2D>::get(data.value("emissive", ""));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }


}