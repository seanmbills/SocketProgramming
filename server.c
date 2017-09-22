/*///////////////////////////////////////////////////////////
*
* FILE:		server.c
* AUTHOR:	Sean M Bills
* PROJECT:	CS 3251 Project 1 - Professor Ellen Zegura 
* DESCRIPTION:	Network Server Code
* CREDIT:	Adapted from Professor Traynor
*
*////////////////////////////////////////////////////////////

/*Included libraries*/

#include <stdio.h>	  /* for printf() and fprintf() */
#include <sys/socket.h>	  /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>	  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>	  /* supports all sorts of functionality */
#include <unistd.h>	  /* for close() */
#include <string.h>	  /* support any string ops */
#include <time.h>       /* support the time functions to keep track of accesses to the server */

#define RCVBUFSIZE 512		/* The receive buffer size */
#define SNDBUFSIZE 512		/* The send buffer size */
#define BUFSIZE 40		/* Your name can be as many as 40 chars*/

/* The main function */
int main(int argc, char *argv[])
{

    int serverSock;				/* Server Socket */
    int clientSock;				/* Client Socket */
    struct sockaddr_in changeServAddr;		/* Local address */
    struct sockaddr_in changeClntAddr;		/* Client address */
    unsigned short changeServPort;		/* Server port */
    unsigned int clntLen;			/* Length of address data struct */

    char nameBuf[BUFSIZE];			/* Buff to store account name from client */
    int  balance;				/* Place to record account balance result */
    char recBuf[RCVBUFSIZE];

    int mySavings = 12000;              /* integer to store the value of the user's savings account */
    int myChecking = 500;               /* integer to store the value of the user's checking account */
    int myRetirement = 60000;           /* integer to store value of user's retirement fund */
    int myCollege = 0;                  /* integer to store value of user's college fund... $0 because I'm poor 
                                            and can't actually afford college lol */

    // set up all of the time_t to keep track of the three accesses
    // to the savings account
    int savingsAccessCount = 0;
    time_t savingsTimeOne;
    time_t savingsTimeTwo;
    time_t savingsTimeThree;
    int withdrawFromSavings = 1;

    // set up all time_t to keep track of checking accesses
    int checkingAccessCount = 0;
    time_t checkingTimeOne;
    time_t checkingTimeTwo;
    time_t checkingTimeThree;
    int withdrawFromChecking = 1;

    // set up all time_t to keep track of retirement accesses
    int retirementAccessCount = 0;
    time_t retirementTimeOne;
    time_t retirementTimeTwo;
    time_t retirementTimeThree;
    int withdrawFromRetirement = 1;

    // set up all time_t to keep track of college accesses
    int collegeAccessCount = 0;
    time_t collegeTimeOne;
    time_t collegeTimeTwo;
    time_t collegeimeThree;
    int withdrawFromCollege = 1;

    // buffer to keep track of the account name...not actually used currently
    char accountName[BUFSIZE];
    // integer to keep track of the amount the client wishes to withdraw from
    // their account
    int withdrawAmount;

    // a buffer to return the new balance to the client after a withdrawal
    char balBuf[16];
    
    // flush the stdout buffer to prevent print issues
    fflush(stdout);

    // create a way of verifying how many bytes were sent/received
    // as a method of error checking
    ssize_t numBytes;

    /* Create new TCP Socket for incoming requests*/
    /*	    FILL IN	*/
    serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSock < 0) {
        printf("Failed to create the socket...\n");
        close(serverSock);
        exit(1);
    }


    /* Construct local address structure*/
    /*	    FILL IN	*/
    memset(&changeServAddr, 0, sizeof(changeServAddr));
    changeServAddr.sin_family = AF_INET;
    changeServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    changeServAddr.sin_port = htons(5000);
    
    /* Bind to local address structure */
    /*	    FILL IN	*/
    if (bind(serverSock, (struct sockaddr *) &changeServAddr, sizeof(changeServAddr)) < 0) {
        printf("Binding of the local address structure failed...\n");
        close(serverSock);
        exit(1);
    }

    /* Listen for incoming connections */
    /*	    FILL IN	*/
    if (listen(serverSock, 5) < 0) {
        printf("Listening to the socket failed...\n");
        close(serverSock);
        exit(1);
    }

    /* Loop server forever*/
    while(1)
    {
	   /* Accept incoming connection */
	   /*	FILL IN	    */
        clntLen = sizeof(changeClntAddr);
        clientSock = accept(serverSock, (struct sockaddr *) &changeClntAddr, &clntLen);
        if (clientSock < 0) {
            printf("Accepting the incoming connection failed...\n");
            close(clientSock);
            close(serverSock);
            exit(1);
        }

	   
	   /* Look up account balance, store in balance */
	   /*	FILL IN	    */
        // extract the command type from the socket: either WITHDRAWAL or BAL
        memset(&recBuf, 0, RCVBUFSIZE);
        numBytes = recv(clientSock, recBuf, RCVBUFSIZE, 0);
        if (numBytes < 0) {
            printf("Receiving the name from the packet failed...\n");
            close(clientSock);
            close(serverSock);
            exit(1);
        } else if (numBytes == 0) {
            printf("The receive connection closed prematurely...\n");
            
            close(clientSock);
            close(serverSock);
            exit(1);
        }
        // check whether the client wishes to make a balance inquiry or
        // make a withdrawal from their account
        if (strncmp(recBuf, "BAL", sizeof("BAL")) == 0) {
            /* Return account balance to client */
            /*  FILL IN     */
            fflush(stdout);
            
            // clear the recBuf that the server receives its data in
            memset(&recBuf, 0, RCVBUFSIZE);
            // receive the account name the client wants the balance of
            numBytes = recv(clientSock, recBuf, RCVBUFSIZE, 0);
            if (numBytes < 0) {
                printf("Receiving the name from the packet failed...\n");
                close(clientSock);
                close(serverSock);
                exit(1);
            } else if (numBytes == 0) {
                printf("The receive connection closed prematurely...\n");
                
                close(clientSock);
                close(serverSock);
                exit(1);
            }
            // check which account the user wishes to make a balance inquiry on
            if (!(strncmp(recBuf, "mySavings", sizeof("mySavings")))) {
                balance = mySavings;
                
                memset(&balBuf, 0, 16);
                sprintf(balBuf, "%i", balance);
                // send the balance of the account back to the client
                send(clientSock, balBuf, 16, 0);
            } else if (!(strncmp(recBuf, "myChecking", sizeof("myChecking")))) {
                balance = myChecking;
                
                memset(&balBuf, 0, 16);
                sprintf(balBuf, "%i", balance);
                // send the balance of the account back to the client
                send(clientSock, balBuf, 16, 0);
            } else if (!(strncmp(recBuf, "myRetirement", sizeof("myRetirement")))) {
                balance = myRetirement;
                
                memset(&balBuf, 0, 16);
                sprintf(balBuf, "%i", balance);
                // send the balance of the account back to the client
                send(clientSock, balBuf, 16, 0);
            } else if (!(strncmp(recBuf, "myCollege", sizeof("myCollege")))) {
                balance = myCollege;
                
                memset(&balBuf, 0, 16);
                sprintf(balBuf, "%i", balance);
                // send the balance of the account back to the client
                send(clientSock, balBuf, 16, 0);
            } else {
                // if user enters an account option that isn't valid, notify them
                // and shut down the connection
                printf("The account option entered was invalid. Exiting...\n");
                
                close(clientSock);
                close(serverSock);
                exit(1);
            }
            // check whether the user wishes to make a withdrawal
        } else if (strncmp(recBuf, "WITHDRAW", sizeof("WITHDRAW")) == 0) {
            memset(&recBuf, 0, RCVBUFSIZE);
            // receive the name of the account which the user wishes to withdraw from
            numBytes = recv(clientSock, recBuf, RCVBUFSIZE, 0);
            if (numBytes < 0) {
                printf("Receiving the name from the packet failed...\n");
                close(clientSock);
                close(serverSock);
                exit(1);
            } else if (numBytes == 0) {
                printf("The receive connection closed prematurely...\n");
                close(clientSock);
                close(serverSock);
                exit(1);
            }
            // copy the account name into the temp accountName buffer
            // so that you can clear recBuf for another reception
            strcpy(accountName, recBuf);
            

            fflush(stdout);

            memset(&recBuf, 0, RCVBUFSIZE);
            // receive the withdrawal amount for the account
            numBytes = recv(clientSock, recBuf, RCVBUFSIZE, 0);
            if (numBytes < 0) {
                printf("Receiving the withdrawal amount from the packet failed...\n");
                close(clientSock);
                close(serverSock);
                exit(1);
            } else if (numBytes == 0) {
                printf("The receive connection closed prematurely...\n");
                close(clientSock);
                close(serverSock);
                exit(1);
            }
            
            fflush(stdout);

            // extract the withdrawal amount from the recBuf
            withdrawAmount = atoi(recBuf);
            // printf("amount %d\n", withdrawAmount);
            // printf("Account: %s\n", accountName);

            // check which account the client wishes to withdraw from
            if (!(strncmp(accountName, "mySavings", sizeof("mySavings")))) {

                if (withdrawAmount > mySavings) {
                    send(clientSock, "Not enough funds!", SNDBUFSIZE, 0); 
                } else {
                    // logic to check how many times in the past minute the client
                    // has withdrawn money
                    if (savingsAccessCount == 0) {
                        savingsAccessCount = savingsAccessCount + 1;
                        savingsTimeOne = time(NULL);
                        //printf("%ld\n", savingsTimeOne);
                        withdrawFromSavings = 1;
                        // if successful, alert the client that there is no
                        // account timeout so that it performs properly
                        send(clientSock, "No timeout", SNDBUFSIZE, 0);
                    } else if (savingsAccessCount == 1) {
                        savingsAccessCount++;
                        savingsTimeTwo = time(NULL);
                        //printf("%ld\n", savingsTimeTwo);
                        withdrawFromSavings = 1;
                        send(clientSock, "No timeout", SNDBUFSIZE, 0);
                    } else if (savingsAccessCount == 2) {
                        savingsAccessCount++;
                        savingsTimeThree = time(NULL);
                        //printf("%ld\n", savingsTimeThree);
                        withdrawFromSavings = 1;
                        send(clientSock, "No timeout", SNDBUFSIZE, 0);
                    } else if (savingsAccessCount == 3) {
                        if ((time(NULL) - savingsTimeOne) < 60) {
                            withdrawFromSavings = 0;
                            send(clientSock, "Timeout error", SNDBUFSIZE, 0);
                        } else {
                            savingsTimeOne = time(NULL);
                            withdrawFromSavings = 1;
                            savingsAccessCount = 1;
                            send(clientSock, "No timeout", SNDBUFSIZE, 0);
                        }
                    }
                    if (withdrawFromSavings) {
                        // this logic basically handles cases where the server has "timed out"
                        // and thus this logic will only be deployed in situations where the
                        // server is not timed out
                        mySavings = mySavings - withdrawAmount;
                        balance = mySavings;
                        //printf("Balance: %i", balance);
                        memset(&balBuf, 0, 16);
                        sprintf(balBuf, "%i", balance);
                        send(clientSock, balBuf, SNDBUFSIZE, 0);
                        send(clientSock, "Successful withdrawal!", SNDBUFSIZE, 0);
                    }
                }
            } else if (!(strncmp(accountName, "myChecking", sizeof("myChecking")))) {
                if (withdrawAmount > myChecking) {
                    send(clientSock, "Not enough funds!", SNDBUFSIZE, 0); 
                } else {
                    // logic to check how many times in the past minute the client
                    // has withdrawn money
                    if (checkingAccessCount == 0) {
                        checkingAccessCount = checkingAccessCount + 1;
                        checkingTimeOne = time(NULL);
                        //printf("%ld\n", savingsTimeOne);
                        withdrawFromChecking = 1;
                        // if successful, alert the client that there is no
                        // account timeout so that it performs properly
                        send(clientSock, "No timeout", SNDBUFSIZE, 0);
                    } else if (checkingAccessCount == 1) {
                        checkingAccessCount++;
                        checkingTimeTwo = time(NULL);
                        //printf("%ld\n", savingsTimeTwo);
                        withdrawFromChecking = 1;
                        send(clientSock, "No timeout", SNDBUFSIZE, 0);
                    } else if (checkingAccessCount == 2) {
                        checkingAccessCount++;
                        checkingTimeThree = time(NULL);
                        //printf("%ld\n", savingsTimeThree);
                        withdrawFromChecking = 1;
                        send(clientSock, "No timeout", SNDBUFSIZE, 0);
                    } else if (checkingAccessCount == 3) {
                        if ((time(NULL) - checkingTimeOne) < 60) {
                            withdrawFromChecking = 0;
                            send(clientSock, "Timeout error", SNDBUFSIZE, 0);
                        } else {
                            checkingTimeOne = time(NULL);
                            withdrawFromChecking = 1;
                            checkingAccessCount = 1;
                            send(clientSock, "No timeout", SNDBUFSIZE, 0);
                        }
                    }
                    if (withdrawFromChecking) {
                        myChecking = myChecking - withdrawAmount;
                        balance = myChecking;
                        //printf("Balance: %i", balance);
                        memset(&balBuf, 0, 16);
                        sprintf(balBuf, "%i", balance);
                        send(clientSock, balBuf, SNDBUFSIZE, 0);
                        send(clientSock, "Successful withdrawal!", SNDBUFSIZE, 0);
                    }
                }
            } else if (!(strncmp(accountName, "myRetirement", sizeof("myRetirement")))) {
                if (withdrawAmount > myRetirement) {
                    send(clientSock, "Not enough funds!", SNDBUFSIZE, 0); 
                } else {
                    // logic to check how many times in the past minute the client
                    // has withdrawn money
                    if (retirementAccessCount == 0) {
                        retirementAccessCount = retirementAccessCount + 1;
                        retirementTimeOne = time(NULL);
                        //printf("%ld\n", savingsTimeOne);
                        withdrawFromRetirement = 1;
                        // if successful, alert the client that there is no
                        // account timeout so that it performs properly
                        send(clientSock, "No timeout", SNDBUFSIZE, 0);
                    } else if (retirementAccessCount == 1) {
                        retirementAccessCount++;
                        retirementTimeTwo = time(NULL);
                        //printf("%ld\n", savingsTimeTwo);
                        withdrawFromRetirement = 1;
                        send(clientSock, "No timeout", SNDBUFSIZE, 0);
                    } else if (retirementAccessCount == 2) {
                        retirementAccessCount++;
                        retirementTimeThree = time(NULL);
                        //printf("%ld\n", savingsTimeThree);
                        withdrawFromRetirement = 1;
                        send(clientSock, "No timeout", SNDBUFSIZE, 0);
                    } else if (retirementAccessCount == 3) {
                        if ((time(NULL) - retirementTimeOne) < 60) {
                            withdrawFromRetirement = 0;
                            send(clientSock, "Timeout error", SNDBUFSIZE, 0);
                        } else {
                            retirementTimeOne = time(NULL);
                            withdrawFromRetirement = 1;
                            retirementAccessCount = 1;
                            send(clientSock, "No timeout", SNDBUFSIZE, 0);
                        }
                    }
                    if (withdrawFromRetirement) {
                        myRetirement = myRetirement - withdrawAmount;
                        balance = myRetirement;
                        //printf("Balance: %i", balance);
                        memset(&balBuf, 0, 16);
                        sprintf(balBuf, "%i", balance);
                        send(clientSock, balBuf, SNDBUFSIZE, 0);
                        send(clientSock, "Successful withdrawal!", SNDBUFSIZE, 0);
                    }
                }
            } else if (!(strncmp(accountName, "myCollege", sizeof("myCollege")))) {
                if (withdrawAmount > myCollege) {
                    send(clientSock, "Not enough funds!", SNDBUFSIZE, 0); 
                } else {
                    // logic to check how many times in the past minute the client
                    // has withdrawn money
                    if (collegeAccessCount == 0) {
                        collegeAccessCount = collegeAccessCount + 1;
                        collegeTimeOne = time(NULL);
                        //printf("%ld\n", savingsTimeOne);
                        withdrawFromCollege = 1;
                        // if successful, alert the client that there is no
                        // account timeout so that it performs properly
                        send(clientSock, "No timeout", SNDBUFSIZE, 0);
                    } else if (collegeAccessCount == 1) {
                        collegeAccessCount++;
                        collegeTimeTwo = time(NULL);
                        //printf("%ld\n", savingsTimeTwo);
                        withdrawFromCollege = 1;
                        send(clientSock, "No timeout", SNDBUFSIZE, 0);
                    } else if (collegeAccessCount == 2) {
                        collegeAccessCount++;
                        collegeimeThree = time(NULL);
                        //printf("%ld\n", savingsTimeThree);
                        withdrawFromCollege = 1;
                        send(clientSock, "No timeout", SNDBUFSIZE, 0);
                    } else if (collegeAccessCount == 3) {
                        if ((time(NULL) - collegeTimeOne) < 60) {
                            withdrawFromCollege = 0;
                            send(clientSock, "Timeout error", SNDBUFSIZE, 0);
                        } else {
                            collegeTimeOne = time(NULL);
                            withdrawFromCollege = 1;
                            collegeAccessCount = 1;
                            send(clientSock, "No timeout", SNDBUFSIZE, 0);
                        }
                    }
                    if (withdrawFromCollege) {
                        myCollege = myCollege - withdrawAmount;
                        balance = myCollege;
                        //printf("Balance: %i", balance);
                        memset(&balBuf, 0, 16);
                        sprintf(balBuf, "%i", balance);
                        send(clientSock, balBuf, SNDBUFSIZE, 0);
                        send(clientSock, "Successful withdrawal!", SNDBUFSIZE, 0);
                    }
                }
            } else {
                send(clientSock, "The account option entered was invalid. Exiting...", SNDBUFSIZE, 0);
                exit(1);
            }
        }
    }
    close(clientSock);
    close(serverSock);

}