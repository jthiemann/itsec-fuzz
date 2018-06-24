#include "filehandler.h"
#include "Logger.h"

using namespace FuzzLogging;

Dmesg* Dmesg::m_Instance =0;


Dmesg* Dmesg::getInstance()
{
    if(m_Instance == 0)
    {
        m_Instance = new Dmesg();
    }
    return m_Instance;
}

Dmesg::Dmesg()
{
    loaded = false;
    vcan = false;
    readDmesg();
}
int Dmesg::upSPIAll()
{
    int sum = 0;
    sum += upSPI(0);
    sum += upSPI(1);
    sum += upSPI(2);
    return sum;
}


void Dmesg::setVCAN(bool b)
{
    vcan = b;
}

std::string Dmesg::nameSPI(int spi)
{
    std::string cmd;
    if(vcan) {
        cmd = "vcan";
        cmd += std::to_string(spi);
        return cmd;
    }
    else cmd ="can";
    int canx = getCANXfromSPIX(spi);
    if(canx >= 0) return (cmd+=std::to_string(spi));
    else return "device_not_found";
}

int Dmesg::downSPIAll()
{
    int sum = 0;
    sum += downSPI(0);
    sum += downSPI(1);
    sum += downSPI(2);
    return sum;
}

int Dmesg::downSPI(int spi)
{
    LOG_INFO("downSPI", debugfile);
    LOG_INFO(std::to_string(spi), debugfile);
    std::string cmd = "../downCANX can";
    int canx = getCANXfromSPIX(spi);
    if(canx < 0)
    {
        LOG_ERROR("downSPI failed", debugfile);
        return 0;
    }
    cmd += std::to_string(canx);
    LOG_INFO(cmd, debugfile);
    return system(cmd.c_str());
}

int Dmesg::upSPI(int spi)
{
    LOG_INFO("upSPI", debugfile);
    LOG_INFO(std::to_string(spi), debugfile);
    std::string cmd = "../upCANX can";
    int canx = getCANXfromSPIX(spi);
    if(canx < 0)
    {
        LOG_ERROR("upSPI failed", debugfile);
        return 0;
    }
    cmd += std::to_string(canx);
    cmd += " ";
    if(spi == 2) cmd+= std::to_string(500000); // SPI 1.0
    else cmd+= std::to_string(100000); // SPI 0.0 oder 0.1
    LOG_INFO(cmd, debugfile);
    return system(cmd.c_str());
}

int Dmesg::getSPIXfromCANX(int iface)
{
    if(!loaded) return -1;
    if(iface == 0) return canX[0];
    if(iface == 1) return canX[1];
    if(iface == 2) return canX[2];
    return -2;
}

int Dmesg::getCANXfromSPIX(int spi)
{
    if(!loaded) return -1;
    for(int i = 0; i < 3; i++)
    {
        if(canX[i] == spi) return i;
    }
    return -2;
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
      loaded = true;

      std::ostringstream ss;
      ss << "\ncan0 is on spi 0." << canX[0]<<std::endl;
      ss << "can1 is on spi 0." << canX[1]<<std::endl;
      ss << "can2 is on spi 0." << canX[2]<<std::endl;

      LOG_INFO(ss, debugfile);
    }
    else LOG_ERROR("Unable to Dmesg outputfile open file", debugfile);
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
    else LOG_ERROR("Unable to open file", debugfile);
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
                //std::cout<<"\n"<<retV;
                LOG_INFO(retV, debugfile);
                return retV;
            }
            else
            {
                retV="can";
                retV += std::to_string(i);
                //std::cout<<"\n"<<retV;
                LOG_INFO(retV, debugfile);
                return retV;
            }
        }
    }
    return "unknown";
}

