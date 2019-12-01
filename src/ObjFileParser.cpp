#include "ObjFileParser.h"
#include <algorithm>
#include <fstream>

std::unique_ptr<Mesh3D> ObjFileParser::Parse(std::unique_ptr<File> spObjFile)
{
    std::string buffer;
    spObjFile->GetContents(buffer);

    std::vector<Eigen::Vector3f> vertexData;
    std::vector<Eigen::Vector2f> texelData;
    std::vector<Eigen::Vector3f> normalData;

    std::istringstream iss(buffer);

    std::string line;


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
            texelData.emplace_back(Eigen::Vector2f{std::stof(tokens[1]), std::stof(tokens[2])});
            hasTexels = true;
        }
        else if (tokens.at(0).compare("vn") == 0 && tokens.size() == 4)
        {
            // normalData
            normalData.emplace_back(Eigen::Vector3f{std::stof(tokens[1]), std::stof(tokens.at(2)), std::stof(tokens.at(3))});
            hasNormals = true;
        }
        else if (tokens.at(0).compare("f") == 0 && tokens.size() == 4)
        {
            // start of face index enumeration.
            for (int i = 1; i < 4; ++i)
            {
                std::vector<std::string> subTokens;
                tokenize(tokens.at(i), '/', subTokens);

                // Indices start at 1 that is why we have to subtract 1
                // format is: vertex, texel, normal
                vertexIndices.push_back(std::stoi(subTokens[0]) - 1);
                texelIndices.push_back(std::stoi(subTokens[1]) - 1);
                normalsIndices.push_back(std::stoi(subTokens[2]) - 1);
            }
        }
    }

    createOutputMatrices(vertexData, texelData, normalData);
    return std::move(spMesh3D);
}

void ObjFileParser::createOutputMatrices(std::vector<Eigen::Vector3f>& vertexData,
                                         std::vector<Eigen::Vector2f>& texelData,
                                         std::vector<Eigen::Vector3f>& normalData)

{
    spMesh3D->vertices.resize(vertexData.size(), 3);

    int row = 0;
    for (const auto& v : vertexData)
    {
        spMesh3D->vertices(row, 0) = v(0);
        spMesh3D->vertices(row, 1) = v(1);
        spMesh3D->vertices(row, 2) = v(2);
        ++row;
    }

    spMesh3D->indices  = vertexIndices;

    // @TODO Weitere Attribute noch einbauen.
    if (hasTexels)
    {
        Eigen::MatrixX2f texels(texelData.size(), 2);
        row = 0;
        for (const auto& t : texelData)
        {
            texels(row, 0) = t(0);
            texels(row, 1) = t(1);
            ++row;
        }
    }

    if (hasNormals)
    {
        Eigen::MatrixX3f normals(normalData.size(), 3);
        row = 0;
        for (const auto& n : normalData)
        {
            normals(row, 0) = n(0);
            normals(row, 1) = n(1);
            normals(row, 2) = n(2);
            row++;
        }
    }
}

void ObjFileParser::tokenize(std::string& line, char delim, std::vector<std::string>& tokens)
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

