#include "gauge972.h"
#include <iostream>

int parse_arg(int argc, char *argv[], int *dev, std::string *cmd);
void help(void);


int main(int argc, char *argv[])
{

}

int parse_arg(int argc, char *argv[], int *dev, std::string *cmd)
{
    *dev=0;
    if (argc%2==0 || argc==1)
    {
        return -1;
    }
    for (int i=1;i<argc;i++)
    {
        if (std::string(argv[i]).compare("--dev")==0)
        {
            *dev = std::atoi(argv[i+1]);
        }
        if (std::string(argv[i]).compare("--cmd")==0)
        {
            *cmd = std::string(argv[i]);
        }
        if (std::string(argv[i]).compare("--help")==0)
        {
            help();
            return 1;
        }

    }
    return 0;
}
void help(void)
{
    std::cout<<"Usage: Gauge972 --dev <device number> --cmd <MKS 972 command>"<<std::endl;

}


