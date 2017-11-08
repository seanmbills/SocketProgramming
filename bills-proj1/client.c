/*///////////////////////////////////////////////////////////
*
* FILE:		client.c
* AUTHOR:	Sean M Bills 
* PROJECT:	CS 3251 Project 1 - Professor Ellen Zegura 
* DESCRIPTION:	Network Client Code
* CREDIT:	Adapted from Professor Traynor
*
*////////////////////////////////////////////////////////////

/* Included libraries */

#include <stdio.h>		    /* for printf() and fprintf() */
#include <sys/socket.h>		    /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>		    /* for sockaddr_in and inet_addr() */
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Constants */
#define RCVBUFSIZE 512		    /* The receive buffer size */
#define SNDBUFSIZE 512		    /* The send buffer size */
#define REPLYLEN 32

/* The main function */
int main(int argc, char *argv[])
{

    int clientSock;		    /* socket descriptor */
    struct sockaddr_in serv_addr;   /* server address structure */

    char *accountName;		    /* Account Name  */
    char *servIP;		    /* Server IP address  */
    unsigned short servPort;	    /* Server Port number */
    

    char sndBuf[SNDBUFSIZE];	    /* Send Buffer */
    char rcvBuf[RCVBUFSIZE];	    /* Receive Buffer */
    
    int balance;		    /* Account balance */

    char *commandName;              // command name (either BAL or WITHDRAW)
    char* withdrawalAmount;
    char tempBuffer[RCVBUFSIZE];

    /* Get the Account Name from the command line */
    if (argc < 5 || argc > 6) {
    	printf("Incorrect number of arguments. The correct format is:\n\taccountName serverIP serverPort Command Amount\n");
    	exit(1);
    }



    accountName = argv[1];
    // printf("%s", accountName);
    memset(&sndBuf, 0, SNDBUFSIZE);
    memset(&rcvBuf, 0, RCVBUFSIZE);

    ssize_t numBytes;

    /* Get the addditional parameters from the command line */
    /*	    FILL IN	*/

    servIP = argv[2];
    unsigned short port_num = atoi(argv[3]);
    commandName = argv[4];

    if (strcmp(commandName, "BAL") == 0) {
        if (argc != 5) {
            printf("Incorrect number of arguments. The correct format is:\n\taccountName serverIP serverPort Command\n");
            exit(1);
        }
    } else if (strcmp(commandName, "WITHDRAW") == 0) {
        if (argc != 6) {
            printf("Incorrect number of arguments. The correct format is:\n\taccountName serverIP serverPort Command Amount\n");
            exit(1);
        }
        withdrawalAmount = argv[5];
        //printf("Withdrawal amount: %s", withdrawalAmount);
    }

    /* Create a new TCP socket*/
    /*	    FILL IN	*/
    //puts("creating client socket...");
    clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSock < 0) {
        puts("Could not create socket...");
        close(clientSock);
        exit(1);
    }

    /* Construct the server address structure */
    /*	    FILL IN	 */
    //puts("constructing server addr struct...");
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(servIP);
    serv_addr.sin_port = htons(port_num);

    /* Establish connecction to the server */
    /*	    FILL IN	 */
    //puts("establishing connection...");
    if (connect(clientSock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("Could not connect to server...\n");
        close(clientSock);
        exit(1);
    }
    
    /* Send the string to the server */
    /*	    FILL IN	 */
    if (strcmp(commandName, "BAL") == 0) {
        // load the account name into the send buffer
        strcpy(sndBuf, accountName);

        // alert the server of the BAL inquiry
        numBytes = send(clientSock, "BAL", SNDBUFSIZE, 0);
        if (numBytes < 0) {
            puts("Sending the command type failed...");
            close(clientSock);
            exit(1);
        }

        // send the name of the account that you're inquiring about
        numBytes = send(clientSock, sndBuf, SNDBUFSIZE, 0);
        if (numBytes < 0) {
            puts("Sending the account name failed...");
            close(clientSock);
            exit(1);
        }
        
        /* Receive and print response from the server */
        /*	    FILL IN	 */
        memset(&rcvBuf, 0, RCVBUFSIZE);

        // receive back the response regarding the balance of the account
        numBytes = recv(clientSock, rcvBuf, RCVBUFSIZE, 0);
        if (numBytes < 0) {
            puts("Receiving of the balance failed...");
            close(clientSock);
            exit(1);
        } else if (numBytes == 0) {
            puts("Receiving connection closed prematurely...");
            close(clientSock);
            exit(1);
        }

        // move the balance amount from rcvBuf into the balance holder
        sscanf(rcvBuf, "%d", &balance);

        // display the account name and the balance of it
        printf("%s\n", accountName);
        printf("Balance is: %i\n", balance);
    } else if (strcmp(commandName, "WITHDRAW") == 0) {
        // load the account name into the send buffer
        strcpy(sndBuf, accountName);

        // alert the server of the withdrawal request
        numBytes = send(clientSock, "WITHDRAW", SNDBUFSIZE, 0);
        if (numBytes < 0) {
            puts("Sending the command type failed...");
            close(clientSock);
            exit(1);
        }

        // send the name of the account you wish to withdraw from
        numBytes = send(clientSock, sndBuf, SNDBUFSIZE, 0);
        if (numBytes < 0) {
            puts("Sending the account name failed...");
            close(clientSock);
            exit(1);
        }


        // clear the send buffer so you can send the withdrawal amount
        memset(&sndBuf, 0, SNDBUFSIZE);
        // copy the withdrawal amount into the send buffer
        strcpy(sndBuf, withdrawalAmount);
        // send the amount for which you wish to make a withdrawal
        //printf("%s", sndBuf);
        numBytes = send(clientSock, sndBuf, SNDBUFSIZE, 0);
        //printf("numBytes %zd\n", numBytes);
        if (numBytes < 0) {
            puts("Sending the withdrawal amount failed...");
            close(clientSock);
            exit(1);
        } else if (numBytes == 0) {
            puts("The sending socket closed prematurely...");
            close(clientSock);
            exit(1);
        }
        // receive a response alerting as to whether the withdrawal caused a timeout to occur
        numBytes = recv(clientSock, rcvBuf, RCVBUFSIZE, 0);
        if (numBytes < 0) {
            puts("Receiving response regarding account timeout failed...");
            close(clientSock);
            exit(1);
        }
        // if you receive a "no timeout" response then you know the withdrawal was not
        // a fourth attempt in under a minute
        if (strcmp(rcvBuf, "No timeout") == 0) {
            /* Receive and print response from the server */
            /*      FILL IN  */
            memset(&rcvBuf, 0, RCVBUFSIZE);
            // thus, look to receive the response as to whether there are enough funds
            numBytes = recv(clientSock, rcvBuf, RCVBUFSIZE, 0);
            if (numBytes < 0) {
                puts("Receiving of the packet failed...");
                close(clientSock);
                exit(1);
            } else if (numBytes == 0) {
                puts("Receiving connection closed prematurely...");
                close(clientSock);
                exit(1);
            }
            // if there are not enough funds to complete the withdrawal, alert the client
            // of that fact
            if (strcmp(rcvBuf, "Not enough funds!") == 0) {
                puts(rcvBuf);
                // otherwise, go ahead and print out the updated balance sent back
                // by the server
            } else {
                // load the balance into balance holder
                sscanf(rcvBuf, "%d", &balance);
                // print out the account name and balance
                printf("%s\n", accountName);
                printf("New Balance is: %i\n", balance);

                memset(&rcvBuf, 0, RCVBUFSIZE);
                // receive the message to alert the user the withdrawal was successful
                numBytes = recv(clientSock, rcvBuf, RCVBUFSIZE, 0);
                // print the success message
                puts(rcvBuf);
            }
            // otherwise, if the response is that there aren't enough funds, let
            // the client know that and close the socket
        } else if (strcmp(rcvBuf, "Not enough funds!") == 0) {
            puts(rcvBuf);
            close(clientSock);
            exit(1);
            // if neitehr is the case, then it means the server timed out
            // and the client needs to be informed of that
        } else {
            puts("ERROR: Account timeout occurred!");
            close(clientSock);
            exit(1);
        } 
    }

    close(clientSock);

    return 0;
}

