#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "common.h"

void fatal(char *message)
{
    fprintf(stderr, "에러: %s\n", message);
    exit(1);
}

void syserr(char *message)
{
    fprintf(stderr, "에러: %s (%d)\n", message, errno);
    exit(1);
}

void waitfor(int pid)
{
    int wpid, status;

    while ((wpid = wait(&status)) != pid && wpid != ERROR);
}

void print_prompt()
{
    char *ps;
    char *index;

    if((ps = (char*)getenv("PS2")) != NULL)
    {
        while(*ps != '\0')
        {
            if(*ps == '\\')
            {
                ps++;

                if(*ps == 'u')
                {
                    printf("%s", getenv("USER"));
                }
                else if(*ps == 'h')
                {
                    printf("%s", getenv("HOSTNAME"));
                }
                else if(*ps == 'w')
                {
                    printf("%s", get_current_dir_name());
                }
            }
            else
            {
                printf("%c", *ps);
                ps++;
            }
        }
    }
    else
    {
        printf("명령어를 입력하세요 : ");
    }
}

BOOLEAN shellcmd(int ac, char *av[], int sourcefd, int destfd)
{
    char *path;

    if(!strcmp(av[0], "cd"))
    {
        cmd_cd(ac, av);
    }
    else if(!strcmp(av[0], "ls"))
    {
        cmd_ls(ac, av);
    }
    else if(!strcmp(av[0], "cp"))
    {
        cmd_cp(ac, av);
    }
    else if(!strcmp(av[0], "rm"))
    {
        cmd_rm(ac, av);
    }
    else if(!strcmp(av[0], "mv"))
    {
        cmd_mv(ac, av);
    }
    else if(!strcmp(av[0], "mkdir"))
    {
        cmd_mkdir(ac, av);
    }
    else if(!strcmp(av[0], "rmdir"))
    {
        cmd_rmdir(ac, av);
    }
    else if(!strcmp(av[0], "cat"))
    {
        cmd_cat(ac, av);
    }
    else if(!strcmp(av[0], "exit"))
    {
        cmd_exit();
    }
    else if(!strcmp(av[0], "pwd"))
    {
        cmd_pwd();
    }
    else if (!strcmp(av[0], "ln"))
    {
        cmd_ln(ac, av);
    }
    else
    {
        return FALSE;
    }

    if (sourcefd != 0 || destfd != 1)
    {
        fprintf(stderr, "잘못된 리디렉션 또는 파이프라인입니다.\n");
    }

    return TRUE;
}

