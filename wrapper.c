#include "wrapper.h"

#include <sys/stat.h>
#include <sys/types.h>

//Funtion to detect the OS
int create_dir(const char *path) {
  #ifdef _WIN32
    return mkdir(path);
  #else 
    return mkdir(path, 0755);
  #endif /* ifdef _WIN32 */
}
