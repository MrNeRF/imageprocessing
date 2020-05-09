#include "File.h"
#include "fstream"
#include "iostream"

File::File(const std::string& filePath)
    : path{filePath}
{
    if (path.empty())
    {
        std::cerr << "Errror! No filename provided!" << std::endl;
        return;
    }
    // Test, if there is no slash, i.e. file is found in the current directory.
    std::string::size_type pos = path.rfind("/");
    if (std::string::npos == pos)
    {
        filename = path;
    }
    else
    {
        filename  = path.substr(pos + 1);
        directory = path.substr(0, pos);
    }

    // Strip file ending
    pos = filename.rfind(".");
    if (std::string::npos != pos)
    {
        fileending = filename.substr(pos);
        filename   = filename.substr(0, pos);
    }
}
void File::GetContents(std::string &buffer)
{
    std::ifstream filereader;
    filereader.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        filereader.open(path.c_str());
        std::stringstream ss;
        ss << filereader.rdbuf();
        filereader.close();
        buffer = ss.str();
    }
    catch(const std::ifstream::failure& e)
    {
        std::cerr << "ERROR: File "
                  << filename + fileending
                  << " not successfully read" << std::endl;
    }
}


