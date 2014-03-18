#include "platform.h"
#include <cstdlib>

namespace Platform {

QString getHostName()
{
    return getenv("HOSTNAME");
}
QString getUserName()
{
    return getenv("USER");
}

}
