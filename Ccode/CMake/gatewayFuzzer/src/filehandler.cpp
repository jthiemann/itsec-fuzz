#include "filehandler.h"

Dmesg::Dmesg(const std::string path)
{

}

void Dmesg::readDmesg()
{
    util::updateDMESG();
    std::string line;
    std::ifstream confile ("dmesgout.spi");
    if (confile.is_open())
    {

      while ( std::getline(confile,line) )
      {

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

      std::cout << "can0 is on spi 0." << canX[0]<<std::endl;
      std::cout << "can0 is on spi 0." << canX[1]<<std::endl;
      std::cout << "can0 is on spi 0." << canX[2]<<std::endl;

    }

    else std::cout << "Unable to Dmesg outputfile open file";
}

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
        //std::cout << line << '\n';
        if(line.find("#") != std::string::npos) continue;
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
    std::string x = Name(0).c_str();
    char *y = new char[x.length() + 1];
    strcpy(y, x.c_str());
    return y;
}
const char* Config::NameDiagnose()
{
    std::string x = Name(1).c_str();
    char *y = new char[x.length() + 1];
    strcpy(y, x.c_str());
    return y;
}
const char * Config::NameEngine()
{
    std::string x = Name(2).c_str();
    char *y = new char[x.length() + 1];
    strcpy(y, x.c_str());
    return y;
}

std::string Config::Name(int type)
{

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

