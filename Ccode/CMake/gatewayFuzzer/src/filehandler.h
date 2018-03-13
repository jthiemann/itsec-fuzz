#include <fstream>


class FileReader
{
public:

    FileReader(const std::string& path);
    ~FileReader();
    std::string read();

private:
    std::ofstream File;
};

class FileWriter
{
public:

   FileWriter(const std::string& path);
    ~FileWriter();
    std::string read();

private:
    std::ofstream File;
};
