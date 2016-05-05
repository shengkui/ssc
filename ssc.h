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


/*--------------------------------------------------------------
 * Definition for both client and server
 *--------------------------------------------------------------*/

/* The read/write buffer size of socket */
#define SSC_BUF_SIZE            4096

/* The signature of the request/response packet */
#define SSC_SIGNATURE           0xDEADBEEF

/* Make a structure 1-byte aligned */
#define BYTE_ALIGNED            __attribute__((packed))


/* Status code, the values used in struct ssc_command_t.status */
#define STATUS_SUCCESS          0   /* Success */
#define STATUS_ERROR            1   /* Generic error */


/* Common header of both request/response packets */
typedef struct ssc_command {
    uint32_t signature;         /* Signature, shall be SSC_SIGNATURE */
    union {
        uint32_t command;       /* Request type */
        uint32_t status;        /* Status code of response, refer ssc_status_code */
    };
    uint32_t data_len;          /* The data length of packet */

    uint16_t checksum;          /* The checksum of the packet */
} BYTE_ALIGNED ssc_command_t;


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
