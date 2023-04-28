#include "mesh-renderer.hpp"
#include "../asset-loader.hpp"

namespace our {
    // Receives the mesh & material from the AssetLoader by the names given in the json object
    void MeshRendererComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        // Notice how we just get a string from the json file and pass it to the AssetLoader to get us the actual asset
        //TODO: (Req 8) Get the material and the mesh from the AssetLoader by their names
        // which are defined with the keys "mesh" and "material" in data.
        // Hint: To get a value of type T from a json object "data" where the key corresponding to the value is "key",
        // you can use write: data["key"].get<T>().
        // Look at "source/common/asset-loader.hpp" to know how to use the static class AssetLoader.
        
        // to get a value of type string from a json object "data" where the key corresponding to the value is "mesh"
        // data["mesh"].get<std::string>() now we have mesh data as string value 
        // to get the actual asset
        // pass the string to get function from assetLoader of type Mesh class
        mesh = AssetLoader<Mesh>::get(data["mesh"].get<std::string>());
        // to get a value of type string from a json object "data" where the key corresponding to the value is "material"
        // data["material"].get<std::string>() now we have material data as string value 
        // to get the actual asset
        // pass the string to get function from assetLoader of type Material class
        material = AssetLoader<Material>::get(data["material"].get<std::string>());
    }
}