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
    std::shared_ptr<Mesh3D> GetMesh(std::unique_ptr<File> spObjFile);
};

#endif
