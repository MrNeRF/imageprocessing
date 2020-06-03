#include "ObjFileParser.h"
#include "Logger.h"
#include "Macros.h"
#include "Mesh3D.h"
#include "VertexColorAttribute.h"
#include "VertexNormalAttribute.h"
#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <unordered_map>

// See Stackoverflow for description of this custom hash function
// https://stackoverflow.com/a/17017281/6304078

namespace
{
struct Key
{
    Key(int fst, int snd, int trd)
        : first(fst)
        , second(snd)
        , third(trd){};
    int first;
    int second;
    int third;

    bool operator==(const Key& other) const
    {
        return (first == other.first && second == other.second && third == other.third);
    }
};

struct KeyHasher
{
    std::size_t operator()(const Key& k) const
    {
        std::size_t res = 17;

        res = res * 31 + std::hash<int>()(k.first);
        res = res * 31 + std::hash<int>()(k.second);
        res = res * 31 + std::hash<int>()(k.third);
        return res;
    }
};

} // namespace

static void tokenize(std::string& line, char delim, std::vector<std::string>& tokens)
{
    auto start = find(cbegin(line), cend(line), delim);
    tokens.push_back(std::string(cbegin(line), start));

    while (start != cend(line))
    {
        const auto finish = find(++start, cend(line), delim);

        tokens.push_back(std::string(start, finish));
        start = finish;
    }
}

std::shared_ptr<Mesh3D> ObjFileParser::GetMesh(std::unique_ptr<File> spObjFile)
{
    std::string buffer;
    spObjFile->GetContents(buffer);

    /* bool bHasTexels = false; */
    bool bHasNormals = false;

    std::vector<Eigen::Vector3f> vertexData;
    std::vector<Eigen::Vector2f> textureCoordinatesData;
    std::vector<Eigen::Vector3f> normalData;

    std::vector<uint32_t> vertexIndices;
    std::vector<uint32_t> textureCoordinatesIndices;
    std::vector<uint32_t> normalsIndices;

    /* std::vector<Eigen::Vector3f> remappedVertexData; */
    /* std::vector<Eigen::Vector2f> remappedTextureCoordinatesData; */
    /* std::vector<Eigen::Vector3f> remappedNormalData; */
    /* // Vector of new Indices */
    /* std::vector<uint32_t> remappedIndices; */
    // New Index Number
    /* uint32_t newIndex = 0; */
    // The map with the key value tuples
    /* std::unordered_map<Key, int, KeyHasher> indexMap; */

    std::istringstream iss(buffer);
    std::string        line;

    for (std::string line; std::getline(iss, line);)
    {
        std::vector<std::string> tokens;
        tokenize(line, ' ', tokens);

        if (tokens.at(0).compare("v") == 0 && tokens.size() == 4)
        {
            //Vertex
            vertexData.emplace_back(Eigen::Vector3f{std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3])});
        }
        else if (tokens.at(0).compare("vt") == 0 && tokens.size() == 3)
        {
            // texture vertexData
            textureCoordinatesData.emplace_back(Eigen::Vector2f{std::stof(tokens[1]), std::stof(tokens[2])});
            /* bHasTexels = true; */
        }
        else if (tokens.at(0).compare("vn") == 0 && tokens.size() == 4)
        {
            // normalData
            normalData.emplace_back(Eigen::Vector3f{std::stof(tokens[1]), std::stof(tokens.at(2)), std::stof(tokens.at(3))});
            bHasNormals = true;
        }
        else if (tokens.at(0).compare("f") == 0 && tokens.size() == 4)
        {
            // start of face index enumeration.
            for (int i = 1; i < 4; ++i)
            {
                std::vector<std::string> subTokens;
                tokenize(tokens.at(i), '/', subTokens);

                // Indices start at 1 that is why we have to subtract 1
                // format is: vertex, TextureCoordinates, normal
                vertexIndices.push_back(std::stoi(subTokens[0]) - 1);
                textureCoordinatesIndices.push_back(std::stoi(subTokens[1]) - 1);
                normalsIndices.push_back(std::stoi(subTokens[2]) - 1);

                /* Key key(vertexIndices.back(), textureCoordinatesIndices.back(), normalsIndices.back()); */

                /* if (indexMap.count(key)) */
                /* { */
                /*     // the key already exists. We insert its index; */
                /*     remappedIndices.push_back(indexMap[key]); */
                /* } */
                /* else */
                /* { */
                /*     // we create new index for this new key */
                /*     indexMap[key] = newIndex; */
                /*     remappedIndices.push_back(newIndex); */
                /*     ++newIndex; */
                /*     remappedVertexData.push_back(vertexData[vertexIndices.back()]); */
                /*     if (hasTextureCoordinates) */
                /*     { */
                /*         remappedTextureCoordinatesData.push_back(textureCoordinatesData[textureCoordinatesIndices.back()]); */
                /*     } */
                /*     if (hasNormals) */
                /*     { */
                /*         remappedNormalData.push_back(normalData[normalsIndices.back()]); */
                /*     } */
                /* } */
            }
        }
    }

    std::shared_ptr<Mesh3D> spMesh = std::make_unique<Mesh3D>(vertexData, vertexIndices, spObjFile->filename);

    if (bHasNormals)
    {
        VertexNormalAttribute& rNormal = dynamic_cast<VertexNormalAttribute&>(spMesh->AddVertexAttribute(Mesh3D::EVertexAttribute::Normal));
        rNormal.SetVertexNormals(normalData, normalsIndices);
    }
    /* ASSERT(vertexIndices.size() == remappedIndices.size()); */
    /* if (hasNormals) */
    /* { */
    /*     ASSERT(remappedNormalData.size() == remappedVertexData.size()); */
    /* } */
    /* int i = 0; */
    /* for (const auto& idx : vertexIndices) */
    /* { */
    /*     assert(vertexData[idx] == remappedVertexData[remappedIndices[i++]]); */
    /* } */
    /* Logger::GetInstance().GetLogger().info("{}: Number of Vertice {}, number of indices {}", spObjFile->filename, remappedVertexData.size(), remappedIndices.size()); */
    return spMesh;
    /* return createMeshObject(remappedVertexData, remappedTextureCoordinatesData, remappedNormalData, remappedIndices, spObjFile->filename); */
}
