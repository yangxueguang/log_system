#include "logger.h"

using namespace std;

int main()
{
    initLogger("./info", "./warn","./err");
    LOG(INFO)<<"this is info\n";
    LOG(WARNING) << "this is warning\n";
    LOG(ERROR) << "this is error\n";
    return 0;
}