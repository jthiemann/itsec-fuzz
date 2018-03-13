#include <iostream>
#include <fstream>
#include "filepolicy.hpp"

FilePolicy::FilePolicy(const std::string &path)
{
    this->outFile.open(path, std::ofstream::out);
    //this->outFile.open(path, std::ofstream::out | std::ofstream::app);
}

FilePolicy::~FilePolicy()
{
    this->outFile.close();
}


void FilePolicy::write( const std::string& message )
{
    if(this->outFile.is_open())
    {
       this->outFile << message << std::endl;
    }
    else std::cerr << "Unable to open file";

    //std::cerr << message << std::endl;
}
