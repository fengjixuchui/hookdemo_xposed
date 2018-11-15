#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <android/log.h>

#include "hook.h"



uint32_t hookNative::get_module_base(pid_t pid, const char *module_path) 
{
    FILE *fp = NULL;
    char *pch = NULL;
    char filename[32];
    char line[512];
    uint32_t addr = 0;

    if ( pid < 0 ) 
        snprintf(filename, sizeof(filename), "/proc/self/maps");
    else 
        snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);

    if ( (fp = fopen(filename, "r")) == NULL ) 
    {
        LOGE("open %s failed!", filename);
        return 0;
    }

    while ( fgets(line, sizeof(line), fp) ) 
    {
        if ( strstr(line, module_path) ) 
        {
            pch = strtok(line, "-");
            addr = strtoul(pch, NULL, 16);
            break;
        }
    }

    fclose(fp);

    return addr;
}

const char* hookNative::get_process_name(pid_t pid) {
    static char buffer[255];
    FILE* f;
    char path[255];

    snprintf(path, sizeof(path), "/proc/%d/cmdline", pid);
    LOGE("open %s ", path);
    if ((f = fopen(path, "r")) == NULL) {
        return NULL;
    }

    if (fgets(buffer, sizeof(buffer), f) == NULL) {
        return NULL;
    }

    LOGE("open %s ", buffer);
    fclose(f);
    return buffer;
}