/******************************************************************************
*
* FILENAME:
*     ssc.h
*
* DESCRIPTION:
*     Define some structure for stream socket communication(TCP).
*
* REVISION(MM/DD/YYYY):
*     12/02/2013  Shengkui Leng (lengshengkui@outlook.com)
*     - Initial version 
*     03/17/2014  Shengkui Leng (lengshengkui@outlook.com)
*     - Define a common header for both request and response packets.
*
******************************************************************************/
#ifndef _SSC_H_
#define _SSC_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Version of the programm */
#define VERSION_MAJOR           1
#define VERSION_MINOR           0

/*--------------------------------------------------------------
 * Definition for both client and server
 *--------------------------------------------------------------*/
/* Default IP address and port number of server */
#define SERVER_IP               "127.0.0.1"
#define SERVER_PORT             5555

/* The read/write buffer size of socket */
#define SSC_BUF_SIZE            4096

/* The signature of the request/response packet */
#define SSC_SIGNATURE           0xDEADBEEF

/* Make a structure 1-byte aligned */
#define BYTE_ALIGNED            __attribute__((packed))


/* Request type */
enum ssc_request_type {
    CMD_GET_VERSION = 0x8001,   /* Get the version of server */
    CMD_GET_MESSAGE,            /* Receive a message from server */
    CMD_PUT_MESSAGE,            /* Send a message to server */

    CMD_UNKNOWN                 /* */
};

/* Status code, start from 160 to skip all system pre-defined error code */
enum ssc_status_code {
    STATUS_SUCCESS = 0,         /* Success */
    STATUS_ERROR = 160,         /* Generic error */
    STATUS_INIT_ERROR,          /* Server/client init error */
    STATUS_INVALID_COMMAND,     /* Unknown request type */

    STATUS_UNKNOWN              /* */
};

/* Common header of both request/response packets */
typedef struct ssc_command {
    uint32_t signature;         /* Signature, shall be SSC_SIGNATURE */
    union {
        uint32_t command;       /* Request type, refer ssc_request_type */
        uint32_t status;        /* Status code of response, refer ssc_status_code */
    };
    uint32_t data_len;          /* The data length of packet */

    uint16_t checksum;          /* The checksum of the packet */
} BYTE_ALIGNED ssc_command_t;


/* Response for CMD_GET_VERSION */
typedef struct ssc_response_version {
    ssc_command_t common;       /* Common header of response */
    uint8_t major;              /* Major version */
    uint8_t minor;              /* Minor version */
} BYTE_ALIGNED ssc_response_version_t;


/* Response for CMD_GET_MESSAGE */
#define SSC_GET_MSG_SIZE        256
typedef struct ssc_response_get_msg {
    ssc_command_t common;           /* Common header of response */
    char data[SSC_GET_MSG_SIZE];    /* Data from server to client */
} BYTE_ALIGNED ssc_response_get_msg_t;


/* Request for CMD_PUT_MESSAGE */
#define SSC_PUT_MSG_SIZE       256
typedef struct ssc_request_put_msg {
    ssc_command_t common;           /* Common header of request */
    char data[SSC_PUT_MSG_SIZE];    /* Data from client to server */
} BYTE_ALIGNED ssc_request_put_msg_t;


/*--------------------------------------------------------------
 * Definition for client only
 *--------------------------------------------------------------*/

/* Keep the information of client */
typedef struct ssc_client {
    int sockfd;         /* Socket fd of the client */
} ssc_client_t;


ssc_client_t *client_init(const char *server_ip, int server_port, int timeout);
ssc_command_t *client_send_request(ssc_client_t *c, ssc_command_t *req);
void client_close(ssc_client_t *s);



/*--------------------------------------------------------------
 * Definition for server only
 *--------------------------------------------------------------*/

/* The maximum length of the queue of pending connections */
#define SSC_MAX_BACKLOG     10

/* The maxium count of client connected */
#define SSC_MAX_CLIENT      10

typedef ssc_command_t * (*request_handler_t) (ssc_command_t *);

/* Keep the information of connection */
typedef struct ssc_connect {
    int inuse;                  /* 1: the connection structure is in-use; 0: free */
    int client_fd;              /* Socket fd of the connection */
    pthread_t thread_id;        /* The thread id of request handler */
    struct ssc_server *serv;    /* The pointer of ssc_server who own the connection */
} ssc_connect_t;

/* Keep the information of server */
typedef struct ssc_server {
    int sockfd;                         /* Socket fd of the server */
    ssc_connect_t conn[SSC_MAX_CLIENT]; /* Connections managed by server */
    request_handler_t request_handler;  /* Function pointer of the request handle */
} ssc_server_t;


ssc_server_t *server_init(request_handler_t req_handler, int port);
int server_accept_request(ssc_server_t *s);
void server_close(ssc_server_t *s);


#endif /* _SSC_H_ */
