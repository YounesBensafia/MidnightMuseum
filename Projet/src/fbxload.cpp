#include "../model/fbxload.hpp"
#include "../include/stb_image.h"

FBXModel loadFBXModel(const char* filepath) {
    FBXModel model;
    model.name = filepath;
    model.vertexCount = 0;
    model.indexCount = 0;
    model.textureID = 0;
    model.baseColor = glm::vec3(0.8f, 0.8f, 0.8f);  // Default gray
    
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filepath, 
        aiProcess_Triangulate | 
        aiProcess_GenNormals |
        aiProcess_JoinIdenticalVertices);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return model;
    }
    
    std::vector<FBXVertex> vertices;
    std::vector<unsigned int> indices;
    
    // Process all meshes
    for (unsigned int m = 0; m < scene->mNumMeshes; m++) {
        aiMesh* mesh = scene->mMeshes[m];
        
        unsigned int baseVertex = vertices.size();
        
        // Process vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            FBXVertex vertex;
            
            // Position
            vertex.position.x = mesh->mVertices[i].x;
            vertex.position.y = mesh->mVertices[i].y;
            vertex.position.z = mesh->mVertices[i].z;
            
            // Normal
            if (mesh->HasNormals()) {
                vertex.normal.x = mesh->mNormals[i].x;
                vertex.normal.y = mesh->mNormals[i].y;
                vertex.normal.z = mesh->mNormals[i].z;
            } else {
                vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);
            }
            
            // Texture coordinates
            if (mesh->mTextureCoords[0]) {
                vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
                vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
            } else {
                vertex.texCoords = glm::vec2(0.0f, 0.0f);
            }
            
            vertices.push_back(vertex);
        }
        
        // Process indices (faces)
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                indices.push_back(baseVertex + face.mIndices[j]);
            }
        }
    }
    
    model.vertexCount = vertices.size();
    model.indexCount = indices.size();
    
    if (vertices.empty()) {
        std::cerr << "No vertices loaded from " << filepath << std::endl;
        return model;
    }
    
    // Load embedded texture if available
    if (scene->HasMaterials() && scene->mNumMaterials > 0) {
        // Try all materials until we find a texture
        for (unsigned int matIdx = 0; matIdx < scene->mNumMaterials && model.textureID == 0; matIdx++) {
            aiMaterial* material = scene->mMaterials[matIdx];
            
            // Try to get base color first
            aiColor3D color(0.8f, 0.8f, 0.8f);
            if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
                model.baseColor = glm::vec3(color.r, color.g, color.b);
                std::cout << "Material " << matIdx << " base color: (" << color.r << ", " << color.g << ", " << color.b << ")" << std::endl;
            }
            
            // Check for embedded texture - try multiple texture types
            aiTextureType textureTypes[] = {
                aiTextureType_DIFFUSE,
                aiTextureType_BASE_COLOR,
                aiTextureType_EMISSIVE
            };
            
            for (int texTypeIdx = 0; texTypeIdx < 3 && model.textureID == 0; texTypeIdx++) {
                aiTextureType texType = textureTypes[texTypeIdx];
                
                if (material->GetTextureCount(texType) > 0) {
                    aiString texPath;
                    material->GetTexture(texType, 0, &texPath);

                    std::cout << "Found texture of type " << texTypeIdx << ": " << texPath.C_Str() << std::endl;

                    // Check if texture is embedded (starts with *)
                    if (texPath.C_Str()[0] == '*') {
                        int texIndex = atoi(&texPath.C_Str()[1]);
                        if (texIndex >= 0 && texIndex < (int)scene->mNumTextures) {
                            aiTexture* embeddedTex = scene->mTextures[texIndex];

                            if (!embeddedTex || !embeddedTex->pcData) {
                                std::cerr << "ERROR: Embedded texture data is NULL" << std::endl;
                                continue;
                            }

                            std::cout << "Processing embedded texture index " << texIndex << ", format: " << embeddedTex->achFormatHint << std::endl;

                            glGenTextures(1, &model.textureID);
                            glBindTexture(GL_TEXTURE_2D, model.textureID);

                            if (embeddedTex->mHeight == 0) {
                                // Compressed texture (PNG, JPG, etc.)
                                std::cout << "Compressed texture, size: " << embeddedTex->mWidth << " bytes" << std::endl;

                                // Safety check - texture should be at least 1KB
                                if (embeddedTex->mWidth < 100) {
                                    std::cerr << "WARNING: Texture size too small (" << embeddedTex->mWidth << " bytes), likely corrupted. Skipping." << std::endl;
                                    continue;
                                }

                                int width, height, channels;
                                unsigned char* data = stbi_load_from_memory(
                                    reinterpret_cast<unsigned char*>(embeddedTex->pcData),
                                    embeddedTex->mWidth,
                                    &width, &height, &channels, 0
                                );

                                if (data) {
                                    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
                                    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                                    glGenerateMipmap(GL_TEXTURE_2D);
                                    stbi_image_free(data);
                                    std::cout << "Loaded embedded compressed texture from GLB (" << width << "x" << height << ", " << channels << " channels)" << std::endl;
                                } else {
                                    std::cerr << "Failed to decode embedded texture: " << stbi_failure_reason() << std::endl;
                                    glDeleteTextures(1, &model.textureID);
                                    model.textureID = 0;
                                }
                            } else {
                                // Uncompressed RGBA texture
                                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, embeddedTex->mWidth, embeddedTex->mHeight, 0, 
                                             GL_RGBA, GL_UNSIGNED_BYTE, embeddedTex->pcData);
                                glGenerateMipmap(GL_TEXTURE_2D);
                                std::cout << "Loaded uncompressed embedded texture from GLB (" << embeddedTex->mWidth << "x" << embeddedTex->mHeight << ")" << std::endl;
                            }

                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                        }
                    } else {
                        // Texture path is external/relative. Attempt to load from disk.
                        std::string texFile = texPath.C_Str();
                        // Build full path relative to model file
                        std::string modelPath = filepath;
                        size_t pos = modelPath.find_last_of("/\\");
                        std::string dir = (pos == std::string::npos) ? std::string(".") : modelPath.substr(0, pos);
                        std::string fullPath = dir + "/" + texFile;

                        int width, height, channels;
                        unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, &channels, 0);
                        if (data) {
                            glGenTextures(1, &model.textureID);
                            glBindTexture(GL_TEXTURE_2D, model.textureID);
                            GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
                            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                            glGenerateMipmap(GL_TEXTURE_2D);
                            stbi_image_free(data);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                        }
                    }
                }
            }
        }
    }

    // Create VAO, VBO, and EBO (always, after texture loading)
    glGenVertexArrays(1, &model.VAO);
    glGenBuffers(1, &model.VBO);
    glGenBuffers(1, &model.EBO);

    glBindVertexArray(model.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, model.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(FBXVertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)offsetof(FBXVertex, normal));
    glEnableVertexAttribArray(1);

    // Texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)offsetof(FBXVertex, texCoords));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    std::cout << "Loaded FBX model: " << filepath << " (" << model.vertexCount << " vertices, " << model.indexCount << " indices)" << std::endl;

    return model;
}

// Load model with clamped texture (no repeat) - for paintings/artwork
FBXModel loadFBXModelClamped(const char* filepath) {
    FBXModel model = loadFBXModel(filepath);
    
    // Re-bind texture and change wrapping to clamp
    if (model.textureID > 0) {
        glBindTexture(GL_TEXTURE_2D, model.textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    
    return model;
}
