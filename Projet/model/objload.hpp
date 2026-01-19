#include <vector>
#include <glm/glm.hpp>
#include <iostream>
#include <map>
#include <string>

struct Material {
    glm::vec3 Ka; // Ambient
    glm::vec3 Kd; // Diffuse
    glm::vec3 Ks; // Specular
    float Ns;     // Shininess
    float d;      // Transparency
    std::string name;
};

bool loadMTL(
    const char* path,
    std::map<std::string, Material>& materials
);

bool loadOBJ(
	const char * path,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
);

bool loadOBJWithMaterials(
    const char* path,
    std::vector<glm::vec3>& out_vertices,
    std::vector<glm::vec2>& out_uvs,
    std::vector<glm::vec3>& out_normals,
    std::vector<std::string>& out_materialNames,
    std::map<std::string, Material>& out_materials
);
