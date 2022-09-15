#include "svmsg_file.h"

static void             /* SIGCHLD handler */
grimReaper(int sig)
{
    int savedErrno;

    savedErrno = errno;                 /* waitpid() might change 'errno' */
    while (waitpid(-1, NULL, WNOHANG) > 0)
        continue;
    errno = savedErrno;
}
static char path[10] = "/MSG11";
static char path1[10] = "/MSG12";
static int clientId;
static void
removeQueue(void)
{
    if (mq_unlink(path) == -1)
        errExit("mq_unlink");
}

static void             /* Executed in child process: serve a single client */
serveRequest(const struct requestMsg *req)
{
    int fd;
    ssize_t numRead;
    struct responseMsg resp;
    clientId = mq_open(path1,O_WRONLY);
    //unsigned int prio = 1;
    fd = open(req->pathname, O_RDONLY);
    if (fd == -1) {                     /* Open failed: send error text */
        resp.mtype = RESP_MT_FAILURE;
        snprintf(resp.data, sizeof(resp.data), "%s", "Couldn't open");
        /*msgsnd(req->clientId, &resp, strlen(resp.data) + 1, 0);*/ 
        if(mq_send(clientId,(char*) &resp,strlen(resp.data) + 1 , 0) == -1){
            perror("mq_send");
        }
        exit(EXIT_FAILURE);             /* and terminate */
    }

    /* Transmit file contents in messages with type RESP_MT_DATA. We don't
       diagnose read() and msgsnd() errors since we can't notify client. */

    resp.mtype = RESP_MT_DATA;
    while ((numRead = read(fd, resp.data, RESP_MSG_SIZE)) > 0)
        if (mq_send(clientId,(char*) &resp, numRead, 0) == -1){
             perror("mq_send1");
            break;
        }

    /* Send a message of type RESP_MT_END to signify end-of-file */

    resp.mtype = RESP_MT_END;
    //msgsnd(req->clientId, &resp, 0, 0);        /* Zero-length mtext */
    if(mq_send(clientId,(char*) &resp,1,0) == -1) perror("mq_send2");
}

int
main(int argc, char *argv[])
{
    struct requestMsg req;
    pid_t pid;
    ssize_t msgLen;
    int serverId;
    struct sigaction sa, sa1;
    /* Create server message queue */

    /*serverId = msgget(SERVER_KEY, IPC_CREAT | IPC_EXCL |
                            S_IRUSR | S_IWUSR | S_IWGRP); */
    struct mq_attr attr;
    attr.mq_msgsize = REQ_MSG_SIZE;
    attr.mq_maxmsg = 20;
    serverId = mq_open(path,O_RDONLY | O_CREAT,S_IRWXU,&attr);
    if (serverId == -1)
        errExit("mq_open");
    if (atexit(removeQueue) != 0)
        errExit("atexit");
    /* Establish SIGCHLD handler to reap terminated children */

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = grimReaper;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
        errExit("sigaction");
    
    /* Read requests, handle each in a separate child process */

    for (;;) {
        /*msgLen = msgrcv(serverId, &req, REQ_MSG_SIZE, 0, 0);*/ 
        //unsigned int prio;
        msgLen = mq_receive(serverId,(char*)&req,sizeof(req),NULL);
        if (msgLen == -1) {
            if (errno == EINTR)         /* Interrupted by SIGCHLD handler? */
                continue;               /* ... then restart msgrcv() */
            errMsg("mq_receive");           /* Some other error */
            break;                      /* ... so terminate loop */
        }

        pid = fork();                   /* Create child process */
        if (pid == -1) {
            errMsg("fork");
            break;
        }

        if (pid == 0) {                 /* Child handles request */
            serveRequest(&req);
            _exit(EXIT_SUCCESS);
        }

        /* Parent loops to receive next client request */
    }

    /* If msgrcv() or fork() fails, remove server MQ and exit */

    if (mq_unlink(path) == -1) //
        errExit("mq_unlink");
    exit(EXIT_SUCCESS);
}