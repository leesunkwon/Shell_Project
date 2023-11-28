#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include "common.h"

int check_arg(char *av[], const char *opt)
{
    int count = 0;

    while (*av != '\0')
    {
        if (!strcmp(av[count], opt))
        {
            return TRUE;
        }

        av++;
    }

    return FALSE;
}

void cmd_cd(int ac, char *av[])
{
    char *path;

    if (ac > 1)
    {
        path = av[1];
        fprintf(stderr, "디렉토리를 %s로 변경합니다.", path);
    }
    else if ((path = (char *)getenv("HOME")) == NULL)
    {
        path = ".";
    }

    if (chdir(path) == ERROR)
    {
        fprintf(stderr, "%s: 잘못된 디렉토리입니다.\n", path);
    }
}

void cmd_exit()
{
    exit(1);
}

void cmd_ls(int ac, char *av[])
{
    DIR *dp;
    struct dirent *entry;
    char *path;
    int count;
    int opt_a;
    int opt_l;

    if (ac < 2)
    {
        path = ".";
    }
    else
    {
        path = av[1];
    }

    if ((dp = opendir(path)) == NULL)
    {
        fprintf(stderr, "%s 디렉토리를 열 수 없습니다.", av[1]);
        return;
    }

    opt_a = check_arg(av, "-a");
    opt_l = check_arg(av, "-l");

    count = 0;

    while ((entry = readdir(dp)) != NULL)
    {
        if (!opt_a)
        {
            if (entry->d_name[0] == '.')
            {
                continue;
            }
        }

        printf("%s\t", entry->d_name);

        if (opt_l)
        {
            printf("\n");
        }
        else
        {
            if (count > 3)
            {
                printf("\n");
                count = 0;
            }
            else
            {
                count++;
            }
        }
    }

    closedir(dp);
    printf("\n");
}

void cmd_cp(int ac, char *av[])
{
    FILE *src;
    FILE *dst;
    char ch;

    if (ac < 3)
    {
        fprintf(stderr, "인수가 충분하지 않습니다.\n");
        return;
    }

    if ((src = fopen(av[1], "r")) == NULL)
    {
        fprintf(stderr, "%s: 파일을 열 수 없습니다.\n", av[1]);
        return;
    }

    if ((dst = fopen(av[2], "w")) == NULL)
    {
        fprintf(stderr, "%s: 파일을 열 수 없습니다.\n", av[2]);
        return;
    }

    while (!feof(src))
    {
        ch = (char)fgetc(src);

        if (ch != EOF)
        {
            fputc((int)ch, dst);
        }
    }

    if (check_arg(av, "-v"))
    {
        printf("cp %s %s\n", av[1], av[2]);
    }

    fclose(src);
    fclose(dst);
}

void cmd_rm(int ac, char *av[])
{
    if (ac < 2)
    {
        fprintf(stderr, "인수가 충분하지 않습니다.\n");
        return;
    }

    unlink(av[1]);

    if (check_arg(av, "-v"))
    {
        printf("rm %s\n", av[1]);
    }
}

void cmd_mv(int ac, char *av[])
{
    FILE *src;
    FILE *dst;
    char ch;

    if (ac < 3)
    {
        fprintf(stderr, "인수가 충분하지 않습니다.\n");
        return;
    }

    if ((src = fopen(av[1], "r")) == NULL)
    {
        fprintf(stderr, "%s: 파일을 열 수 없습니다.\n", av[1]);
        return;
    }

    if ((dst = fopen(av[2], "w")) == NULL)
    {
        fprintf(stderr, "%s: 파일을 열 수 없습니다.\n", av[2]);
        return;
    }

    while (!feof(src))
    {
        ch = (char)fgetc(src);

        if (ch != EOF)
        {
            fputc((int)ch, dst);
        }
    }

    fclose(src);
    fclose(dst);

    unlink(av[1]);

    if (check_arg(av, "-v"))
    {
        printf("mv %s %s\n", av[1], av[2]);
    }
}

void cmd_mkdir(int ac, char *av[])
{
    if (ac < 2)
    {
        fprintf(stderr, "인수가 충분하지 않습니다.\n");
        return;
    }

    if (mkdir(av[1], 0755))
    {
        fprintf(stderr, "디렉토리 생성에 실패했습니다.\n");
    }
}

void cmd_rmdir(int ac, char *av[])
{
    if (ac < 2)
    {
        fprintf(stderr, "인수가 충분하지 않습니다.\n");
        return;
    }

    if (rmdir(av[1]))
    {
        fprintf(stderr, "디렉토리 삭제에 실패했습니다.\n");
    }
}

void cmd_cat(int ac, char *av[])
{
    int ch;
    FILE *fp;

    if (ac < 2)
    {
        fprintf(stderr, "인수가 충분하지 않습니다.\n");
        return;
    }

    if ((fp = fopen(av[1], "r")) == NULL)
    {
        fprintf(stderr, "디렉토리에 해당 파일이 없습니다.\n");
        return;
    }

    while ((ch = getc(fp)) != EOF)
    {
        putchar(ch);
    }

    fclose(fp);
}

void cmd_pwd()
{
    char cwd[1024];

    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("%s\n", cwd);
    }
    else
    {
        perror("getcwd() 오류");
    }
}

void cmd_ln(int ac, char *av[])
{
    if (ac < 3)
    {
        fprintf(stderr, "인수가 충분하지 않습니다.\n");
        return;
    }

    if (link(av[1], av[2]) == -1)
    {
        fprintf(stderr, "하드 링크 생성 오류.\n");
        return;
    }

    if (check_arg(av, "-v"))
    {
        printf("ln %s %s\n", av[1], av[2]);
    }
}

