#ifndef OBJECT_FILE_PARSER
#define OBJECT_FILE_PARSER

#include "File.h"
#include "Mesh3D.h"
#include <Eigen/Dense>
#include <memory>
#include <string>
#include <vector>

class ObjFileParser
{
public:
    std::unique_ptr<Mesh3D> Parse(std::unique_ptr<File> spObjFile);

    std::vector<int> vertexIndices;
    std::vector<int> TextureCoordinatesIndices;
    std::vector<int> normalsIndices;

private:
    void tokenize(std::string& line, char delim, std::vector<std::string>& tokens);
    void createOutputMatrices(std::vector<Eigen::Vector3f>& vertexData,
                              std::vector<Eigen::Vector2f>& TextureCoordinatesData,
                              std::vector<Eigen::Vector3f>& normalData);

private:
    bool                    hasTextureCoordinates = false;
    bool                    hasNormals = false;
    std::unique_ptr<Mesh3D> spMesh3D   = std::make_unique<Mesh3D>();
};

#endif
