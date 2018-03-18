#include "filehandler.h"


Config::Config(const std::string path)
{
    v[0] = v[1] = v[2] = 0;
    canX[0] = canX[1] = canX[2] = -1;
    std::string line;
    std::ifstream confile (path);
    if (confile.is_open())
    {

      while ( std::getline(confile,line) )
      {
        std::cout << line << '\n';

        if(line.find("vcan0") != std::string::npos) v[0] = true;
        if(line.find("vcan1") != std::string::npos) v[1] = true;
        if(line.find("vcan2") != std::string::npos) v[2] = true;

        //Name translate
        if(line.find("can0") != std::string::npos)
        {
            if(line.find("spi0.0") != std::string::npos) canX[0] = 0;
            if(line.find("spi0.1") != std::string::npos) canX[0] = 1;
            if(line.find("spi1.0") != std::string::npos) canX[0] = 2;
        }
        if(line.find("can1") != std::string::npos)
        {
            if(line.find("spi0.0")!= std::string::npos) canX[1] = 0;
            if(line.find("spi0.1")!= std::string::npos) canX[1] = 1;
            if(line.find("spi1.0")!= std::string::npos) canX[1] = 2;
        }
        if(line.find("can2") != std::string::npos)
        {
            if(line.find("spi0.0")!= std::string::npos) canX[2] = 0;
            if(line.find("spi0.1")!= std::string::npos) canX[2] = 1;
            if(line.find("spi1.0")!= std::string::npos) canX[2] = 2;
        }



      }
      confile.close();
      loaded == true;
    }

    else std::cout << "Unable to open file";
}




const char* Config::NameComfort()
{
    return Name(0).c_str();
}
const char* Config::NameDiagnose()
{
    return Name(1).c_str();
}
const char * Config::NameEngine()
{
    return Name(2).c_str();
}

std::string Config::Name(int type)
{
    std::cout<<"\nwtf";
    std::string retV;
    for(int i = 0; i < 3; i++)
    {
        if(canX[i] == type)
        {
            if(v[i]){
                retV = "vcan";
                retV += std::to_string(i);
                std::cout<<"\n"<<retV;
                return retV;
            }
            else
            {
                retV="can";
                retV += std::to_string(i);
                std::cout<<"\n"<<retV;
                return retV;
            }
        }
    }
    return "unknown";
}

