#include <signal.h>
#include <time.h>
#include "fifo_seqnum.h"
int
main(int argc, char *argv[])
{
    int serverFd, dummyFd, clientFd;
    char clientFifo[CLIENT_FIFO_NAME_LEN];
    struct request req;
    struct response resp;
    int seqNum = 0; /* This is our "service" */
    umask(0); /* So we get the permissions we want */
    if (mkfifo(SERVER_FIFO, S_IRUSR | S_IWUSR | S_IWGRP) == -1
    && errno != EEXIST)
        errExit("mkfifo %s", SERVER_FIFO);
    serverFd = open(SERVER_FIFO, O_RDONLY | O_NONBLOCK);
    if (serverFd == -1)
        errExit("open %s", SERVER_FIFO);
 /* Open an extra write descriptor, so that we never see EOF */
    dummyFd = open(SERVER_FIFO, O_WRONLY | O_NONBLOCK);
    if (dummyFd == -1)
        errExit("open %s", SERVER_FIFO);
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
        errExit("signal");
    int flags;
    flags = fcntl(serverFd, F_GETFL); /* Fetch open files status flags */
    flags |= O_NONBLOCK; /* Enable O_NONBLOCK bit */
    fcntl(serverFd, F_SETFL, flags);
    for (;;) { /* Read requests and send responses */
        if (read(serverFd, &req, sizeof(struct request))
        != sizeof(struct request)) {
            //fprintf(stderr, "Error reading request; discarding\n");
            perror("read error");
            continue; /* Either partial read or error */
        }
        /* Open client FIFO (previously created by client) */
        snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE,
        (long) req.pid);
        clientFd = open(clientFifo, O_WRONLY | O_NONBLOCK); //必須改為nonblock
        if (clientFd == -1) { /* Open failed, give up on client */
            errMsg("open %s", clientFifo);
        }
        int flags;
        flags = fcntl(clientFd, F_GETFL); /* Fetch open files status flags */
        flags |= O_NONBLOCK; /* Enable O_NONBLOCK bit */
        fcntl(clientFd, F_SETFL, flags);
        /* Send response and close FIFO */
        resp.seqNum = seqNum;
        if (write(clientFd, &resp, sizeof(struct response))
        != sizeof(struct response))
            //fprintf(stderr, "Error writing to FIFO %s\n", clientFifo);
            perror("write error");
        if (close(clientFd) == -1)
            errMsg("close");
        seqNum += req.seqLen; /* Update our sequence number */
    }
}