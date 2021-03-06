#ifndef _PARSER_H
#define _PARSER_H

#include <string>
#include <vector>
#include <glm/glm.hpp>

using namespace std;

struct ppm_file
{
    int width;
    int height;
    int max;
    int bytecount;
    char* texture;
};

void parse_vtk_file(char* d, vector<glm::vec3> &points, vector<glm::vec3> &normals, vector<glm::vec3> &polygons, vector<glm::vec2> &textures);
ppm_file* parse_ppm_file(char* s);
string read_file(char* fname);

#endif