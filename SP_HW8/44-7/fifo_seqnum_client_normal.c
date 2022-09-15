#include "fifo_seqnum.h"
static char clientFifo[CLIENT_FIFO_NAME_LEN];
static void /* Invoked on exit to delete client FIFO */
removeFifo(void)
{
 unlink(clientFifo);
}
int main(int argc, char *argv[]){
    int serverFd, clientFd;
    struct request req;
    struct response resp;
    if (argc > 1 && strcmp(argv[1], "--help") == 0)
        usageErr("%s [seq-len...]\n", argv[0]);
 /* Create our FIFO (before sending request, to avoid a race) */
    umask(0); /* So we get the permissions we want */
    snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE,
    (long) getpid());
    if (mkfifo(clientFifo, S_IRUSR | S_IWUSR | S_IWGRP) == -1
    && errno != EEXIST)
        errExit("mkfifo %s", clientFifo);
    if (atexit(removeFifo) != 0)
        errExit("atexit");
 /* Construct request message, open server FIFO, and send request */
    req.pid = getpid();
    req.seqLen = (argc > 1) ? getInt(argv[1], GN_GT_0, "seq-len") : 1;
    serverFd = open(SERVER_FIFO, O_WRONLY | O_NONBLOCK);
    if (serverFd == -1)
        errExit("open %s", SERVER_FIFO);
    int flags;
    flags = fcntl(serverFd, F_GETFL); /* Fetch open files status flags */
    flags |= O_NONBLOCK; /* Enable O_NONBLOCK bit */
    fcntl(serverFd, F_SETFL, flags);
    if (write(serverFd, &req, sizeof(struct request)) !=
    sizeof(struct request))
        fatal("Can't write to server");
 /* Open our FIFO, read and display response */
    clientFd = open(clientFifo, O_RDONLY | O_NONBLOCK);
    if (clientFd == -1)
        errExit("open %s", clientFifo);
    int flags1;
    flags1 = fcntl(clientFd, F_GETFL); /* Fetch open files status flags */
    flags1 |= O_NONBLOCK; /* Enable O_NONBLOCK bit */
    fcntl(clientFd, F_SETFL, flags1);
    while(read(clientFd, &resp, sizeof(struct response))
    != sizeof(struct response)){
        //fatal("Can't read response from server");
        perror("read error");
    }
    printf("%d\n", resp.seqNum);
    exit(EXIT_SUCCESS);
}