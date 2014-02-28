#include "platform.h"
#include <cstdlib>

char* OS::getHostName()
{
    return getenv("HOSTNAME");
}
char* OS::getUserName()
{
    return getenv("USER");
}
