#include <stdio.h>
#include "common.h"


int main()
{
    int pid, fd;
    SYMBOL term;

    
    print_prompt();

   
    while (TRUE)
    {
       
        term = parse(&pid, FALSE, NULL);

      
     
        if (term != S_AMP && pid != 0)
        {
          
            waitfor(pid);
        }

 
        if (term == S_NL)
        {
            print_prompt();
        }

    
        for (fd=3; fd<MAXFD; fd++)
        {
            close(fd);
        }
    }
}
