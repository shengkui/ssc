/******************************************************************************
*
* FILENAME:
*     common.h
*
* DESCRIPTION:
*     Define some common structure for both client and server.
*
* REVISION(MM/DD/YYYY):
*     05/05/2016  Shengkui Leng (lengshengkui@outlook.com)
*     - Initial version 
*
******************************************************************************/
#ifndef _COMMON_H_
#define _COMMON_H_
#include "ssc.h"

/* Version of the programm */
#define VERSION_MAJOR           1
#define VERSION_MINOR           0

/*--------------------------------------------------------------
 * Definition for both client and server
 *--------------------------------------------------------------*/
/* Default IP address and port number of server */
#define SERVER_IP               "127.0.0.1"
#define SERVER_PORT             5555

/* Extra status code, refer STATUS_ERROR defined in uds.h,
 * the values used in struct ssc_command_t.status */
#define STATUS_INIT_ERROR       (STATUS_ERROR+1)    /* Server/client init error */
#define STATUS_INVALID_COMMAND  (STATUS_ERROR+2)    /* Unkown request type */


/* Request type, the values used in struct ssc_command_t.command */
enum ssc_request_type {
    CMD_GET_VERSION = 0x8001,   /* Get the version of server */
    CMD_GET_MESSAGE,            /* Receive a message from server */
    CMD_PUT_MESSAGE,            /* Send a message to server */

    CMD_UNKNOWN                 /* */
};


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


#endif /* _COMMON_H_ */
