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
    std::shared_ptr<Mesh3D> Parse(std::unique_ptr<File> spObjFile);

private:
    void tokenize(std::string& line, char delim, std::vector<std::string>& tokens);
    std::shared_ptr<Mesh3D> createMeshObject(std::vector<Eigen::Vector3f>& vertices,
                                             std::vector<Eigen::Vector2f>& textureCoordinates,
                                             std::vector<Eigen::Vector3f>& normal,
                                             std::vector<uint32_t>&        indices,
                                             const std::string& name);

private:
    bool                    hasTextureCoordinates = false;
    bool                    hasNormals = false;
};

#endif
