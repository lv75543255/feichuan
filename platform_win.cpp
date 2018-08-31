#include "platform.h"
#include <cstdlib>
const char* OS::getHostName()
{
    return getenv("HOSTNAME");
}
const char* OS::getUserName()
{
    return getenv("USERNAME");
}
