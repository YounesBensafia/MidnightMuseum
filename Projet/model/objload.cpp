#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <map>
#include <string>
#include <stdio.h>
#include <cstring>

#include <glm/glm.hpp>

#include "objload.hpp"

// Load MTL file
bool loadMTL(const char* path, std::map<std::string, Material>& materials) {
    printf("Loading MTL file %s...\n", path);
    
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        printf("Cannot open MTL file!\n");
        return false;
    }
    
    Material currentMaterial;
    std::string currentMaterialName = "";
    
    while (true) {
        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF) break;
        
        if (strcmp(lineHeader, "newmtl") == 0) {
            // Save previous material if exists
            if (!currentMaterialName.empty()) {
                materials[currentMaterialName] = currentMaterial;
            }
            // Start new material
            char name[128];
            fscanf(file, "%s\n", name);
            currentMaterialName = name;
            currentMaterial.name = name;
            // Set defaults
            currentMaterial.Ka = glm::vec3(0.2f);
            currentMaterial.Kd = glm::vec3(0.8f);
            currentMaterial.Ks = glm::vec3(0.5f);
            currentMaterial.Ns = 32.0f;
            currentMaterial.d = 1.0f;
        }
        else if (strcmp(lineHeader, "Ka") == 0) {
            fscanf(file, "%f %f %f\n", &currentMaterial.Ka.x, &currentMaterial.Ka.y, &currentMaterial.Ka.z);
        }
        else if (strcmp(lineHeader, "Kd") == 0) {
            fscanf(file, "%f %f %f\n", &currentMaterial.Kd.x, &currentMaterial.Kd.y, &currentMaterial.Kd.z);
        }
        else if (strcmp(lineHeader, "Ks") == 0) {
            fscanf(file, "%f %f %f\n", &currentMaterial.Ks.x, &currentMaterial.Ks.y, &currentMaterial.Ks.z);
        }
        else if (strcmp(lineHeader, "Ns") == 0) {
            fscanf(file, "%f\n", &currentMaterial.Ns);
        }
        else if (strcmp(lineHeader, "d") == 0) {
            fscanf(file, "%f\n", &currentMaterial.d);
        }
        else {
            char buffer[1000];
            fgets(buffer, 1000, file);
        }
    }
    
    // Save last material
    if (!currentMaterialName.empty()) {
        materials[currentMaterialName] = currentMaterial;
    }
    
    fclose(file);
    printf("Loaded %zu materials\n", materials.size());
    return true;
}

// Load OBJ with material support
bool loadOBJWithMaterials(
    const char* path,
    std::vector<glm::vec3>& out_vertices,
    std::vector<glm::vec2>& out_uvs,
    std::vector<glm::vec3>& out_normals,
    std::vector<std::string>& out_materialNames,
    std::map<std::string, Material>& out_materials
) {
    printf("Loading OBJ file %s...\n", path);
    
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;
    
    std::string currentMaterial = "";
    std::string mtlFile = "";
    
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        printf("Impossible to open the file!\n");
        return false;
    }
    
    while (true) {
        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF) break;
        
        if (strcmp(lineHeader, "mtllib") == 0) {
            char mtlPath[256];
            fscanf(file, "%s\n", mtlPath);
            // Build full path (assume MTL is in same directory as OBJ)
            std::string objPath(path);
            size_t lastSlash = objPath.find_last_of("/\\");
            std::string directory = (lastSlash != std::string::npos) ? objPath.substr(0, lastSlash + 1) : "";
            mtlFile = directory + mtlPath;
            loadMTL(mtlFile.c_str(), out_materials);
        }
        else if (strcmp(lineHeader, "usemtl") == 0) {
            char matName[128];
            fscanf(file, "%s\n", matName);
            currentMaterial = matName;
        }
        else if (strcmp(lineHeader, "v") == 0) {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);
        }
        else if (strcmp(lineHeader, "vt") == 0) {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            uv.y = -uv.y;
            temp_uvs.push_back(uv);
        }
        else if (strcmp(lineHeader, "vn") == 0) {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_normals.push_back(normal);
        }
        else if (strcmp(lineHeader, "f") == 0) {
            char line[512];
            fgets(line, 512, file);
            
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            
            // Try format: v/vt/vn
            int matches = sscanf(line, "%d/%d/%d %d/%d/%d %d/%d/%d",
                &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            
            if (matches == 9) {
                // Has vertex/texture/normal
                for (int i = 0; i < 3; i++) {
                    vertexIndices.push_back(vertexIndex[i]);
                    uvIndices.push_back(uvIndex[i]);
                    normalIndices.push_back(normalIndex[i]);
                    out_materialNames.push_back(currentMaterial);
                }
            }
            else {
                // Try format: v/vt (without normals)
                matches = sscanf(line, "%d/%d %d/%d %d/%d",
                    &vertexIndex[0], &uvIndex[0],
                    &vertexIndex[1], &uvIndex[1],
                    &vertexIndex[2], &uvIndex[2]);
                
                if (matches == 6) {
                    // Has vertex/texture, will generate normals later
                    for (int i = 0; i < 3; i++) {
                        vertexIndices.push_back(vertexIndex[i]);
                        uvIndices.push_back(uvIndex[i]);
                        normalIndices.push_back(0); // Mark as no normal
                        out_materialNames.push_back(currentMaterial);
                    }
                }
                else {
                    // Try format: v//vn (vertex and normal, no texture)
                    matches = sscanf(line, "%d//%d %d//%d %d//%d",
                        &vertexIndex[0], &normalIndex[0],
                        &vertexIndex[1], &normalIndex[1],
                        &vertexIndex[2], &normalIndex[2]);
                    
                    if (matches == 6) {
                        for (int i = 0; i < 3; i++) {
                            vertexIndices.push_back(vertexIndex[i]);
                            uvIndices.push_back(0); // No UV
                            normalIndices.push_back(normalIndex[i]);
                            out_materialNames.push_back(currentMaterial);
                        }
                    }
                    else {
                        // Try format: v (vertex only)
                        matches = sscanf(line, "%d %d %d",
                            &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
                        
                        if (matches == 3) {
                            for (int i = 0; i < 3; i++) {
                                vertexIndices.push_back(vertexIndex[i]);
                                uvIndices.push_back(0);
                                normalIndices.push_back(0);
                                out_materialNames.push_back(currentMaterial);
                            }
                        }
                        else {
                            printf("Face format not supported: %s", line);
                            fclose(file);
                            return false;
                        }
                    }
                }
            }
        }
        else {
            char buffer[1000];
            fgets(buffer, 1000, file);
        }
    }
    
    fclose(file);
    
    // Process indices
    for (unsigned int i = 0; i < vertexIndices.size(); i++) {
        unsigned int vertexIndex = vertexIndices[i];
        unsigned int uvIndex = uvIndices[i];
        unsigned int normalIndex = normalIndices[i];
        
        glm::vec3 vertex = temp_vertices[vertexIndex - 1];
        glm::vec2 uv = (uvIndex > 0 && uvIndex <= temp_uvs.size()) ? temp_uvs[uvIndex - 1] : glm::vec2(0.0f);
        glm::vec3 normal;
        
        if (normalIndex > 0 && normalIndex <= temp_normals.size()) {
            normal = temp_normals[normalIndex - 1];
        }
        else {
            // Generate flat normal from triangle
            if (i % 3 == 0 && i + 2 < vertexIndices.size()) {
                glm::vec3 v0 = temp_vertices[vertexIndices[i] - 1];
                glm::vec3 v1 = temp_vertices[vertexIndices[i + 1] - 1];
                glm::vec3 v2 = temp_vertices[vertexIndices[i + 2] - 1];
                normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
            }
            else {
                normal = glm::vec3(0.0f, 1.0f, 0.0f);
            }
        }
        
        out_vertices.push_back(vertex);
        out_uvs.push_back(uv);
        out_normals.push_back(normal);
    }
    
    printf("Loaded %zu vertices with materials\n", out_vertices.size());
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
			uv.y = -uv.y;
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
		out_vertices.push_back(vertex);  //std::cout<<vertex.x<< " " << vertex.y<< " " << vertex.z << "       ";

		out_uvs     .push_back(uv); //std::cout<< uv.x<<" " << uv.y << "       ";

		out_normals .push_back(normal);//std::cout<<normal.x<<" " <<normal.y<<" "<<normal.z<<std::endl;


	}

	return true;
}
