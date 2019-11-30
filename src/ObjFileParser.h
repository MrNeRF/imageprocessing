#ifndef OBJECT_FILE_PARSER
#define OBJECT_FILE_PARSER

#include "File.h"
#include "Vertex.h" 
#include <string>
#include <memory>
#include <vector>

#define GLM_FORCE_CXX14
#include <glm/glm.hpp>

class ObjFileParser
{
    public:
	    explicit ObjFileParser(std::unique_ptr<File> up_fileToParse) : upFileToParse(std::move(up_fileToParse)){};
        void Parse(std::vector<Vertex> *vertices, std::vector<int> *indices);
    private:
        void tokenize(std::string &line, char delim, std::vector<std::string> &tokens);

    private:
        //
       std::unique_ptr<File>  upFileToParse;
};

#endif
