/* minimalist client code */
                                                                                
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/in.h> /* needed by Linux but not Sun */

main()
{
        int fd, tmp;
        struct sockaddr_in s;
        fd = socket(PF_INET, SOCK_STREAM, 0);
        printf("fd = %d\n", fd);
        if (fd == -1)
                exit(1);
        s.sin_family = AF_INET;
        s.sin_port = htons(21);
        *((int *) &s.sin_addr) = inet_addr("172.16.51.103");
        tmp = connect(fd, (struct sockaddr *) &s, sizeof s);
        printf("connect() returned %d\n", tmp);
        if (tmp == -1)
                exit(1);
        while (1)
                ;
}
