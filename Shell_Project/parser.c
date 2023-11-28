#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

SYMBOL parse(int *waitpid, BOOLEAN makepipe, int *pipefdp)
{
    SYMBOL symbol, term;
    int argc, sourcefd, destfd;
    int pid, pipefd[2];
    char *argv[MAXARG + 1], sourcefile[MAXFNAME];
    char destfile[MAXFNAME];
    char word[MAXWORD];
    BOOLEAN append;

    argc = 0;
    sourcefd = 0;
    destfd = 1;

    while (TRUE)
    {
        switch (symbol = getsymbol(word))
        {
            case S_WORD :
                if (argc == MAXARG)
                {
                    fprintf(stderr, "너무 많은 인수입니다.\n");
                    break;
                }

                argv[argc] = (char *)malloc(strlen(word) + 1);

                if (argv[argc] == NULL)
                {
                    fprintf(stderr, "인수 메모리 부족입니다.\n");
                    break;
                }

                strcpy(argv[argc], word);
                argc++;
                continue;

            case S_LT :
                if (makepipe)
                {
                    fprintf(stderr, "여분의 <입니다.\n");
                    break;
                }

                if (getsymbol(sourcefile) != S_WORD)
                {
                    fprintf(stderr, "잘못된 <입니다.\n");
                    break;
                }

                sourcefd = BADFD;
                continue;

            case S_GT :
            case S_GTGT :

                if (destfd != 1)
                {
                    fprintf(stderr, "> 또는 >>가 너무 많습니다.\n");
                    break;
                }

                if (getsymbol(destfile) != S_WORD)
                {
                    fprintf(stderr, "> 또는 >>가 잘못되었습니다.\n");
                    break;
                }

                destfd = BADFD;
                append = (symbol == S_GTGT);
                continue;

            case S_BAR :
            case S_AMP :
            case S_SEMI :
            case S_NL :

                argv[argc] = NULL;

                if (symbol == S_BAR)
                {
                    if (destfd != 1)
                    {
                        fprintf(stderr, "> 또는 >>가 |와 충돌합니다.\n");
                        break;
                    }

                    term = parse(waitpid, TRUE, &destfd);
                }
                else
                {
                    term = symbol;
                }

                if (makepipe)
                {
                    if (pipe(pipefd) == ERROR)
                    {
                        syserr("pipe");
                    }

                    *pipefdp = pipefd[1];
                    sourcefd = pipefd[0];
                }

                pid = execute(argc, argv, sourcefd, sourcefile,
                                destfd, destfile, append, term == S_AMP);

                if (symbol != S_BAR)
                {
                    *waitpid = pid;
                }

                if (argc == 0 && (symbol != S_NL || sourcefd > 1))
                {
                    fprintf(stderr, "명령이 없습니다.\n");
                }

                while (--argc >= 0)
                {
                    free(argv[argc]);
                }

                return term;

            case S_EOF :
                exit(0);
        }
    }
}

