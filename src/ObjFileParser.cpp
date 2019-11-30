#include "ObjFileParser.h"
#include <fstream>
#include <algorithm>
#include <unordered_map>

namespace
{
    struct Key
    {
        Key(int fst, int snd, int trd) : first(fst), second(snd), third(trd){};
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
            return ((std::hash<int>()(k.first) ^
                     (std::hash<int>()(k.second) << 1)) >> 1) ^
                     (std::hash<int>()(k.third) << 1);
        }
    };
}

void ObjFileParser::Parse(std::vector<Vertex> *vertices, std::vector<int> *indices)

{
    int newIdx = 0;
    std::unordered_map<Key, int, KeyHasher> idxMap;
    std::string buffer;
    upFileToParse->GetContents(buffer);

    std::vector<glm::vec3> coordinates;
    std::vector<int> idxCoordinates;
    std::vector<glm::vec2> texels;
    std::vector<int> idxTexels;
    std::vector<glm::vec3> normals;
    std::vector<int> idxNormals;
    

    std::istringstream iss(buffer);

    std::string line;

    bool hasTexels = false;
    bool hasNormals = false;

    for (std::string line; std::getline(iss, line); )
    {
        std::vector<std::string> tokens;
        tokenize(line, ' ', tokens);


        if(tokens.at(0).compare("v") == 0 && tokens.size() == 4)
        {
            //Vertex
            coordinates.emplace_back(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));
        }
        else if (tokens.at(0).compare("vt") == 0 && tokens.size() == 3)
        {
            // texture coordinates
            texels.emplace_back(std::stof(tokens[1]), std::stof(tokens[2]));
            hasTexels = true;
        }
        else if (tokens.at(0).compare("vn") == 0 && tokens.size() == 4)
        {
            // normals
            normals.emplace_back(std::stof(tokens[1]), std::stof(tokens.at(2)), std::stof(tokens.at(3)));
            hasNormals = true;
        }
        else if (tokens.at(0).compare("f") == 0 && tokens.size() == 4 )
		{
			for (int i = 1; i < 4; ++i)
			{
				std::vector<std::string> subTokens;
				tokenize(tokens.at(i), '/', subTokens);

                // Indices start at 1 that is why we have to subtract 1
                idxCoordinates.push_back(std::stoi(subTokens[0]) - 1);
                idxTexels.push_back(std::stoi(subTokens[1]) - 1);
                idxNormals.push_back(std::stoi(subTokens[2]) - 1);

                Key idxKey(idxCoordinates.back(), idxTexels.back(), idxNormals.back());

                if(idxMap.count(idxKey))
                {
                    indices->push_back(idxMap[idxKey]);
                }
                else 
                {
                    idxMap[idxKey] = newIdx;
                    indices->push_back(newIdx);
                    ++newIdx;

                    if(hasTexels && !hasNormals)
                    {
                        vertices->emplace_back(coordinates[idxCoordinates.back()], 
                                           texels[idxTexels.back()]);

                    }
                    else if (!hasTexels && hasNormals)
                    {
                        vertices->emplace_back(coordinates[idxCoordinates.back()], 
                                           normals[idxNormals.back()]);
                    }
                    else if (!hasTexels && !hasNormals)
                    {
                        vertices->emplace_back(coordinates[idxCoordinates.back()]);
                    }
                    else
                    {
                        vertices->emplace_back(coordinates[idxCoordinates.back()], 
                                           texels[idxTexels.back()],
                                           normals[idxNormals.back()]);
                    }
                    
                }

			}
		}
	}

}


void ObjFileParser::tokenize(std::string &line, char delim, std::vector<std::string> &tokens)
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


