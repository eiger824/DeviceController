/* 
 * tcpserver.c - A simple TCP echo server 
 * usage: tcpserver <port>
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 1024

int main(int argc, char **argv) {
    int parentfd; /* parent socket */
    int childfd; /* child socket */
    int portno; /* port to listen on */
    int slaveID; /* slave ID to use*/
    int clientlen; /* byte size of client's address */
    struct sockaddr_in serveraddr; /* server's addr */
    struct sockaddr_in clientaddr; /* client addr */
    struct hostent *hostp; /* client host info */
    char buf[BUFSIZE]; /* message buffer */
    char *hostaddrp; /* dotted decimal host addr string */
    int optval; /* flag value for setsockopt */
    int n; /* message byte size */

    /* 
     * check command line arguments 
     */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <slave ID>\n", argv[0]);
        exit(1);
    }
    slaveID = atoi(argv[1]);
    portno = 3422;

    /* 
     * socket: create the parent socket 
     */
    parentfd = socket(AF_INET, SOCK_STREAM, 0);
    if (parentfd < 0) 
        perror("ERROR opening socket");

    /* setsockopt: Handy debugging trick that lets 
     * us rerun the server immediately after we kill it; 
     * otherwise we have to wait about 20 secs. 
     * Eliminates "ERROR on binding: Address already in use" error. 
     */
    optval = 1;
    setsockopt(parentfd, SOL_SOCKET, SO_REUSEADDR, 
            (const void *)&optval , sizeof(int));

    /*
     * build the server's Internet address
     */
    bzero((char *) &serveraddr, sizeof(serveraddr));

    /* this is an Internet address */
    serveraddr.sin_family = AF_INET;

    /* let the system figure out our IP address */
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* this is the port we will listen on */
    serveraddr.sin_port = htons((unsigned short)portno);

    /* 
     * bind: associate the parent socket with a port 
     */
    if (bind(parentfd, (struct sockaddr *) &serveraddr, 
                sizeof(serveraddr)) < 0) 
        perror("ERROR on binding");

    /* 
     * listen: make this socket ready to accept connection requests 
     */
    if (listen(parentfd, 5) < 0) /* allow 5 requests to queue up */ 
        perror("ERROR on listen");

    /* 
     * main loop: wait for a connection request, echo input line, 
     * then close connection.
     */
    clientlen = sizeof(clientaddr);
    while (1)
    {

        /* 
         * accept: wait for a connection request 
         */
        childfd = accept(parentfd, (struct sockaddr *) &clientaddr, &clientlen);
        if (childfd < 0) 
            perror("ERROR on accept");

        /* 
         * gethostbyaddr: determine who sent the message 
         */
        hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
                sizeof(clientaddr.sin_addr.s_addr), AF_INET);
        if (hostp == NULL)
            perror("ERROR on gethostbyaddr");
        hostaddrp = inet_ntoa(clientaddr.sin_addr);
        if (hostaddrp == NULL)
            perror("ERROR on inet_ntoa\n");
        printf("server established connection with %s (%s)\n", 
                hostp->h_name, hostaddrp);

        /* 
         * read: read input string from the client
         */
        bzero(buf, BUFSIZE);
        n = read(childfd, buf, BUFSIZE);
        if (n < 0) 
            perror("ERROR reading from socket");

        /* Analyze received buffer */
        if (n != 3 && n != 0) fprintf(stderr, "Wrong data format (byte count = %d)\n", n);
        else
        {
            if (n != 0) //Just process stuff when reading something
            {
                printf("Parsed sequence: [");
                for (unsigned i=0; i < n; ++i)
                {
                    printf("%#x%s", buf[i], (i==n-1?"":" "));
                }
                printf("]\n");
                if (buf[0] !=  slaveID) fprintf(stderr, "Wrong dest ID (%#x != %d)\n",
                        (int)buf[0], slaveID);
                else
                {
                    if (buf[1] != 0x00) fprintf(stderr, "Wrong source ID (%#x != 0x00)\n",
                            (int)buf[1]);
                    else
                    {
                        if (buf[2] != 0x1a) fprintf(stderr,
                                "Wrong command (%#x != PING(0x1a))\n",
                                (int) buf[2]);
                        else
                        {
                            char rsp[4];
                            rsp[0] = 0x00; //Dest: HP master
                            rsp[1] = slaveID; //Source: myself
                            rsp[2] = 0x1b; //Pong comand
                            rsp[3] = 0x00; //Terminate string
                            n = write(childfd, rsp, 3);
                            if (n < 0) 
                                perror("ERROR writing to socket");
                        }
                    }
                }
            }
        }
        //Close the child socket
        close(childfd);
    }
}
