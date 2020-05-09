#ifndef FILE_OBJECT_H
#define FILE_OBJECT_H

#include <fstream>
#include <sstream>
#include <string>

class File
{
public:
    explicit File(const std::string& filePath);
    void GetContents(std::string& buffer);

    std::string filename;
    std::string directory;
    std::string fileending;
    std::string path;
};

#endif
