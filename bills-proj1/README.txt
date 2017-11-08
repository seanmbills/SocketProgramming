README.txt
Sean M. Bills
For Socket Programming Assignment
CS 3251 -- Networking

To start up the program:
1) Open two terminals
2) cd into project folder p1 in both terminals
3) Make the files using the given Makefil by typing:
    make
in one terminal.
4) in one terminal start the server with the following command:
    ./server
NOTE: this will start up the server.c file and start running the server in
that terminal
5) Start the client in the other terminal with the following:
    ./client accountName IP_address Port_Num Command [Withdrawal_amount]
NOTE: all args after ./client are arguments that the user must input to the command line
    accountName is the account you wish to perform an action on: 
        can be any of the following -- {mySavings, myChecking, myRetirement, myCollege}
        NOTE: any other inputs will cause the server/client connection to shut down
    IP_address: the IP address for which you wish to run the client on
        NOTE: it shouldn't matter what is put in here as the server is built to accept any IP address connections
    Port_Num: the port number on which you wish to run the client
        NOTE: the server is currently hardcoded to run on the port *** 5000 *** so it is advised that the client
        is instructed to run on this port as well, unless the port number in the server is changed
    Command: the command you wish to perform on the specified account
        can be either *** WITHDRAW *** or *** BAL ***
        NOTE: failure to use one of these specified commands will cause nothing to happen in the server/client connection
    [Withdrawal_amount]: if using the WITHDRAW command, this is the integer amount specified that you wish to withdraw
        from the specified account
        NOTE: if a withdrawal amount is given while calling the BAL command, the client will warn you to input the
        correct number of args in the correct order and will shut itself down


Once you have both the server and client running in separate terminals, feel free to mess around with varying client
command line commands in the above stated form. Checking a balance on an account should return messages to the console
of the form:
accountName
Balance: accountBalance

Making withdrawals from an account should return messages to the console of the form:
accountName:
New Balance: newBalance
Successful withdrawal!

If you happen to attempt to make 4 withdrawals in under a minute (60 seconds), then the client terminal will simply print out
a message similar to the following:
ERROR: Server timed out!

Please keep in mind that every time a server timeout on an account occurs, all of the objects keeping track of the various access
times will "rotate" such that the oldest access time becomes what was previously the second oldest access time. This is to keep
the client from attempting to trick the server by continuously calling withdrawals until 60 seconds from the original access have
elapsed. In order to account for this, once an initial error message is printed, the client must wait 60 seconds from the second
oldest timestamp in order to be able to successfully withdraw money from the account again. 


With any questions, please feel free to contact sbills3 'at' gatech.edu