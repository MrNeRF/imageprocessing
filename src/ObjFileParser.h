#ifndef OBJECT_FILE_PARSER
#define OBJECT_FILE_PARSER

#include "File.h"
#include <Eigen/Dense>
#include <memory>
#include <string>
#include <vector>

class Mesh3D;

class ObjFileParser
{
public:
    std::unique_ptr<Mesh3D> Parse(std::unique_ptr<File> spObjFile);

private:
    void tokenize(std::string& line, char delim, std::vector<std::string>& tokens);
    void createOutputMatrices(std::vector<Eigen::Vector3f>& vertexData,
                              std::vector<Eigen::Vector2f>& TextureCoordinatesData,
                              std::vector<Eigen::Vector3f>& normalData,
                              Mesh3D&                       mesh);

private:
    bool                    hasTextureCoordinates = false;
    bool                    hasNormals = false;
};

#endif
