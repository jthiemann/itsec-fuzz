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

        if(line.find("vcan0")) v[0] = true;
        if(line.find("vcan1")) v[0] = true;
        if(line.find("vcan2")) v[0] = true;

        //Name translate
        if(line.find("can0"))
        {
            if(line.find("spi0.0")) canX[0] = 0;
            if(line.find("spi0.1")) canX[0] = 1;
            if(line.find("spi1.0")) canX[0] = 2;
        }
        if(line.find("can1"))
        {
            if(line.find("spi0.0")) canX[1] = 0;
            if(line.find("spi0.1")) canX[1] = 1;
            if(line.find("spi1.0")) canX[1] = 2;
        }
        if(line.find("can2"))
        {
            if(line.find("spi0.0")) canX[2] = 0;
            if(line.find("spi0.1")) canX[2] = 1;
            if(line.find("spi1.0")) canX[2] = 2;
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
    for(int i = 0; i < 3; i++)
    {
        if(canX[i] == type)
        {

        }
    }
    if(type == 0)
    {
        if(v[0]) return "vcan0";
        else return xcan0;
    }
    if(type == 1)
    {
        if(v1) return "vcan1";
        else return xcan1;
    }
    if(type == 2)
    {
        if(v2) return "vcan2";
        else return xcan2;
    }
    return "unknown";
}

class Config
{
public:

    Config(const std::string path);
    ~Config();
    std::string tr(std::string bustype);

private:
    bool loaded;
    std::string xcan0,xcan1,xcan2;
    bool v1,v2,v3;
    std::ofstream File;
};
