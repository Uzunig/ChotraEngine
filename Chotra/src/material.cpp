#include "material.h"
#include "stb_image.h"

namespace Chotra {

    Material::Material(std::string mtl_path) {

        std::string directory = mtl_path.substr(0, mtl_path.find_last_of('/')) + '/';
        std::ifstream mtl_file(mtl_path);
        if (!mtl_file) {
            std::cout << "The MTL file could not open for writing!" << std::endl;

        }
        else {

            while (mtl_file) {
                std::string s;
                mtl_file >> s;
                std::string texture_filename;
                Texture texture;

                if (s == "map_Kd") {
                    mtl_file >> texture_filename;
                    texture.type = "albedoMap";
                    texture.path = directory + texture_filename;
                    texture.id = LoadTexture(texture.path);
                    textures.push_back(texture);

                }
                else if (s == "map_Bump") {
                    mtl_file >> texture_filename;
                    texture.type = "normalMap";
                    texture.path = directory + texture_filename;
                    texture.id = LoadTexture(texture.path);
                    textures.push_back(texture);

                }
                else if (s == "map_Pm") {
                    mtl_file >> texture_filename;
                    texture.type = "metallicMap";
                    texture.path = directory + texture_filename;
                    texture.id = LoadTexture(texture.path);
                    textures.push_back(texture);

                }
                else if (s == "map_Pr") {
                    mtl_file >> texture_filename;
                    texture.type = "roughnessMap";
                    texture.path = directory + texture_filename;
                    texture.id = LoadTexture(texture.path);
                    textures.push_back(texture);

                }
                else if (s == "map_AO") {
                    mtl_file >> texture_filename;
                    texture.type = "aoMap";
                    texture.path = directory + texture_filename;
                    texture.id = LoadTexture(texture.path);
                    textures.push_back(texture);
                }
            }
        }
    }

    void Material::SetupTextures() {

        Texture texture;

        texture.type = "albedoMap";
        texture.path = "models/textures/earth/albedo.png";
        texture.id = LoadTexture(texture.path);
        textures.push_back(texture);

        texture.type = "normalMap";
        texture.path = "models/textures/earth/normal.png";
        texture.id = LoadTexture(texture.path);
        textures.push_back(texture);

        texture.type = "metallicMap";
        texture.path = "models/textures/earth/metallic.png";
        texture.id = LoadTexture(texture.path);
        textures.push_back(texture);


        texture.type = "roughnessMap";
        texture.path = "models/textures/earth/roughness.png";
        texture.id = LoadTexture(texture.path);
        textures.push_back(texture);

        texture.type = "aoMap";
        texture.path = "models/textures/earth/ao.png";
        texture.id = LoadTexture(texture.path);
        textures.push_back(texture);
    }


     unsigned int Material::LoadTexture(std::string& path) {

        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);

        if (data) {
            GLenum format;

            if (nrComponents == 1)
                format = GL_RED;

            else if (nrComponents == 3)
                format = GL_RGB;

            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);

        }
        else {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
        }

        return textureID;
    }

} // namspace Chotra
