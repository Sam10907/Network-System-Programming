#include "svmsg_file.h"

static int clientId;
static char path1[10] = "/MSG12";
static void
removeQueue(void)
{
    if (mq_unlink(path1) == -1)
        errExit("mq_unlink");
}

int
main(int argc, char *argv[])
{
    struct requestMsg req;
    struct responseMsg resp;
    int serverId, numMsgs;
    ssize_t msgLen, totBytes;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pathname\n", argv[0]);

    if (strlen(argv[1]) > sizeof(req.pathname) - 1)
        cmdLineErr("pathname too long (max: %ld bytes)\n",
                (long) sizeof(req.pathname) - 1);

    /* Get server's queue identifier; create queue for response */
    char path[10] = "/MSG11";
    /*serverId = msgget(SERVER_KEY, S_IWUSR);*/
    serverId = mq_open(path,O_WRONLY);
    if (serverId == -1)
        errExit("mq_open - server message queue");

    /*clientId = msgget(IPC_PRIVATE, S_IRUSR | S_IWUSR | S_IWGRP);*/
    struct mq_attr attr1;
    attr1.mq_msgsize = RESP_MSG_SIZE ;
    attr1.mq_maxmsg = 20;
    clientId = mq_open(path1,O_RDWR | O_CREAT,S_IREAD | S_IWRITE,&attr1);
    //printf("%d\n",clientId);
    if (clientId == -1)
        errExit("mq_open - client message queue");

    if (atexit(removeQueue) != 0)
        errExit("atexit");

    /* Send message asking for file named in argv[1] */

    req.mtype = 1;                      /* Any type will do */
    req.clientId = clientId;
    strncpy(req.pathname, argv[1], sizeof(req.pathname) - 1);
    req.pathname[sizeof(req.pathname) - 1] = '\0';
                                        /* Ensure string is terminated */
    //unsigned int prio = 1;
    int bytes;
    if ((bytes = mq_send(serverId,(char*)&req, REQ_MSG_SIZE, 0)) == -1)
        errExit("mq_send");
    /* Get first response, which may be failure notification */

    msgLen = mq_receive(clientId,(char*) &resp, RESP_MSG_SIZE, NULL);
    if (msgLen == -1)
        errExit("mq_receive");

    if (resp.mtype == RESP_MT_FAILURE) {
        printf("%s\n", resp.data);      /* Display msg from server */
        exit(EXIT_FAILURE);
    }

    /* File was opened successfully by server; process messages
       (including the one already received) containing file data */

    totBytes = msgLen;                  /* Count first message */
    for (numMsgs = 1; resp.mtype == RESP_MT_DATA; numMsgs++) {
        msgLen = mq_receive(clientId,(char*) &resp, RESP_MSG_SIZE, NULL);
        if (msgLen == -1)
            errExit("mq_receive");

        totBytes += msgLen;
    }

    printf("Received %ld bytes (%d messages)\n", (long) totBytes - 1, numMsgs);

    exit(EXIT_SUCCESS);
}