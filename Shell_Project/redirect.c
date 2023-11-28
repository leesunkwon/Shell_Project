#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "common.h"

void redirect(int sourcefd, char *sourcefile, int destfd, char *destfile, BOOLEAN append, BOOLEAN backgrnd)
{
    int flags, fd;

    if (sourcefd == 0 && backgrnd)
    {
        strcpy(sourcefile, "/dev/null");
        sourcefd = BADFD;
    }

    if (sourcefd != 0)
    {
        if (close(0) == ERROR)
        {
            syserr("close");
        }

        if (sourcefd > 0)
        {
            if (dup(sourcefd) != 0)
            {
                fatal("dup");
            }
        }
        else if (open(sourcefile, O_RDONLY, 0) == ERROR)
        {
            fprintf(stderr, "%s를 열 수 없습니다.\n", sourcefile);
            exit(0);
        }
    }

    if (destfd != 1)
    {
        if (close(1) == ERROR)
        {
            syserr("close");
        }

        if (destfd > 1)
        {
            if (dup(destfd) != 1)
            {
                fatal("dup");
            }
        }
        else
        {
            flags = O_WRONLY | O_CREAT;

            if (!append)
            {
                flags |= O_TRUNC;
            }

            if (open(destfile, flags, 0666) == ERROR)
            {
                fprintf(stderr, "%s를 생성할 수 없습니다.\n", destfile);
                exit(0);
            }

            if (append)
            {
                if (lseek(1, 0L, 2) == ERROR)
                    syserr("lseek");
            }
        }
    }

    for (fd = 3; fd < MAXFD; fd++)
    {
        close(fd);
    }

    return;
}

