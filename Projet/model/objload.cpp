#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <map>
#include <string>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <glm/glm.hpp>
#include "objload.hpp"

// Load OBJ with material support and QUAD handling
bool loadOBJWithMaterials(
    const char* path,
    std::vector<glm::vec3>& out_vertices,
    std::vector<glm::vec2>& out_uvs,
    std::vector<glm::vec3>& out_normals,
    std::vector<std::string>& out_materialNames,
    std::map<std::string, Material>& out_materials
) {
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;
    
    std::string currentMaterial = "None";
    FILE* file = fopen(path, "r");
    if (file == NULL) return false;

    while (true) {
        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF) break;

        if (strcmp(lineHeader, "v") == 0) {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);
        }
        else if (strcmp(lineHeader, "vt") == 0) {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            temp_uvs.push_back(uv);
        }
        else if (strcmp(lineHeader, "vn") == 0) {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_normals.push_back(normal);
        }
        else if (strcmp(lineHeader, "usemtl") == 0) {
            char matName[128];
            fscanf(file, "%s\n", matName);
            currentMaterial = matName;
        }
        else if (strcmp(lineHeader, "f") == 0) {
            unsigned int vIdx[4], uIdx[4], nIdx[4];
            char line[512];
            fgets(line, 512, file);

            // Try to match a QUAD (4 vertices)
            int matches = sscanf(line, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
                &vIdx[0], &uIdx[0], &nIdx[0],
                &vIdx[1], &uIdx[1], &nIdx[1],
                &vIdx[2], &uIdx[2], &nIdx[2],
                &vIdx[3], &uIdx[3], &nIdx[3]);

            if (matches == 12) { // It's a Quad! Split into two triangles
                // Triangle 1
                unsigned int tris[] = { 0, 1, 2, 0, 2, 3 };
                for (int i = 0; i < 6; i++) {
                    vertexIndices.push_back(vIdx[tris[i]]);
                    uvIndices.push_back(uIdx[tris[i]]);
                    normalIndices.push_back(nIdx[tris[i]]);
                    out_materialNames.push_back(currentMaterial);
                }
            } 
            else { 
                // Fallback to your original triangle logic
                matches = sscanf(line, "%d/%d/%d %d/%d/%d %d/%d/%d",
                    &vIdx[0], &uIdx[0], &nIdx[0],
                    &vIdx[1], &uIdx[1], &nIdx[1],
                    &vIdx[2], &uIdx[2], &nIdx[2]);
                
                if (matches == 9) {
                    for (int i = 0; i < 3; i++) {
                        vertexIndices.push_back(vIdx[i]);
                        uvIndices.push_back(uIdx[i]);
                        normalIndices.push_back(nIdx[i]);
                        out_materialNames.push_back(currentMaterial);
                    }
                }
            }
        }
    }
    fclose(file);

    // Final processing (Indexing)
    for (unsigned int i = 0; i < vertexIndices.size(); i++) {
        out_vertices.push_back(temp_vertices[vertexIndices[i] - 1]);
        
        if (uvIndices[i] > 0)
            out_uvs.push_back(temp_uvs[uvIndices[i] - 1]);
        else
            out_uvs.push_back(glm::vec2(0.0f));

        if (normalIndices[i] > 0)
            out_normals.push_back(temp_normals[normalIndices[i] - 1]);
        else
            out_normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    }
    return true;
}

// Original simple OBJ loader (kept for compatibility)

bool loadOBJ(
    const char * path,
    std::vector<glm::vec3> & out_vertices,
    std::vector<glm::vec2> & out_uvs,
    std::vector<glm::vec3> & out_normals
)
{
    printf("Loading OBJ file %s...\n", path);

    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;


    FILE * file = fopen(path, "r");
    if( file == NULL ){
        printf("Impossible to open the file !\n");
        getchar();
        return false;
    }

    while( 1 ){

        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF
        if ( strcmp( lineHeader, "v" ) == 0 ){
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            temp_vertices.push_back(vertex);
        }else if ( strcmp( lineHeader, "vt" ) == 0 ){
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y );
            uv.y = 1.0f - uv.y; // Changed from -uv.y
            temp_uvs.push_back(uv);
        }else if ( strcmp( lineHeader, "vn" ) == 0 ){
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
            temp_normals.push_back(normal);
        }else if ( strcmp( lineHeader, "f" ) == 0 ){
            char line[512];
            fgets(line, 512, file);
            
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = sscanf(line, "%d/%d/%d %d/%d/%d %d/%d/%d", 
                &vertexIndex[0], &uvIndex[0], &normalIndex[0], 
                &vertexIndex[1], &uvIndex[1], &normalIndex[1], 
                &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            
            if (matches == 9){
                // v/vt/vn format
                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[1]);
                vertexIndices.push_back(vertexIndex[2]);
                uvIndices.push_back(uvIndex[0]);
                uvIndices.push_back(uvIndex[1]);
                uvIndices.push_back(uvIndex[2]);
                normalIndices.push_back(normalIndex[0]);
                normalIndices.push_back(normalIndex[1]);
                normalIndices.push_back(normalIndex[2]);
            } else {
                // Try v/vt format (no normals)
                matches = sscanf(line, "%d/%d %d/%d %d/%d",
                    &vertexIndex[0], &uvIndex[0],
                    &vertexIndex[1], &uvIndex[1],
                    &vertexIndex[2], &uvIndex[2]);
                
                if (matches == 6) {
                    vertexIndices.push_back(vertexIndex[0]);
                    vertexIndices.push_back(vertexIndex[1]);
                    vertexIndices.push_back(vertexIndex[2]);
                    uvIndices.push_back(uvIndex[0]);
                    uvIndices.push_back(uvIndex[1]);
                    uvIndices.push_back(uvIndex[2]);
                    // Generate normals later
                    normalIndices.push_back(0);
                    normalIndices.push_back(0);
                    normalIndices.push_back(0);
                } else {
                    printf("Face format not supported in line: %s", line);
                    return false;
                }
            }
        }else{
            // Probably a comment
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }

    }

    std::cout<<vertexIndices.size()<<std::endl;

    for( unsigned int i=0; i<vertexIndices.size(); i++ ){
        // Get the indices of its attributes
        unsigned int vertexIndex = vertexIndices[i];
        unsigned int uvIndex = uvIndices[i];
        unsigned int normalIndex = normalIndices[i];

        // Get the attributes thanks to the index
        glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
        glm::vec2 uv = (uvIndex > 0 && uvIndex <= temp_uvs.size()) ? temp_uvs[uvIndex-1] : glm::vec2(0.0f, 0.0f);
        glm::vec3 normal;
        
        if (normalIndex > 0 && normalIndex <= temp_normals.size()) {
            normal = temp_normals[normalIndex-1];
        } else {
            // Generate flat normal from triangle
            if (i % 3 == 0 && i + 2 < vertexIndices.size()) {
                glm::vec3 v0 = temp_vertices[vertexIndices[i] - 1];
                glm::vec3 v1 = temp_vertices[vertexIndices[i + 1] - 1];
                glm::vec3 v2 = temp_vertices[vertexIndices[i + 2] - 1];
                normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
            } else {
                normal = glm::vec3(0.0f, 1.0f, 0.0f);
            }
        }

        // Put the attributes in buffers
        out_vertices.push_back(vertex);
        out_uvs     .push_back(uv); 
        out_normals .push_back(normal);
    }

    return true;
}