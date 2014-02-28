#include "os.h"
#include <cstdlib>
const char* OS::getHostName()const
{
    return getenv("HOSTNAME");
}
const char* OS::getUserName()const
{
    return getenv("USERNAME");
}
