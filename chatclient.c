//chatclient
//Chris Brackett
//2/8/2017
//Note: code adapted from client.c example from OSUCS344 lecture materials

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

//makeContact
//This function intitiates contact with the server and returns the socket file descriptor for use later in the program.
int makeContact(const char * argv[])
{
    int socketFD, portNumber;
    struct sockaddr_in serverAddress;
    struct hostent* serverHostInfo;

    // Set up the server address struct - this block of code is mostly from the CS344 lectures
    memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
    portNumber = atoi(argv[2]); // Get the port number, convert to an integer from a string
    serverAddress.sin_family = AF_INET; // Create a network-capable socket
    serverAddress.sin_port = htons(portNumber); // Store the port number
    serverHostInfo = gethostbyname(argv[1]); // Convert the machine name into a special form of address
    if (serverHostInfo == NULL) { fprintf(stderr, "CLIENT: ERROR, no such host\n"); exit(0); }
    memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); // Copy in the address

    // Set up the socket
    socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
    if (socketFD < 0) printf("CLIENT: ERROR opening socket\n");

    // Connect to server
    if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to address
    {
        printf("CLIENT: ERROR connecting to server\n");
        exit(0);
    }
    else
    {
        printf("Connected to server.\n");
    }

    return socketFD;
}


//getHandle
//This function gets the handle of the user and verifies it is valid.
void getHandle(char* handle)
{
    char* quitCommand = "\\quit\0"; //quit command to compare to handle
    memset(handle, '\0', 50); //zero out handle

    printf("What is your handle? "); //get handle from user
    fflush(stdout);
    scanf("%s", handle);

    int handleLength = strlen(handle); //check if handle is more than 10 characters or contains \quit
    while (handleLength > 10 || strstr(handle, quitCommand) != NULL)
    {
        printf("Your handle must be 10 characters or less and not contain \\quit.\n");
        printf("What is your handle? ");
        fflush(stdout);
        memset(handle, '\0', 50); //zero out handle
        scanf("%s", handle);
        handleLength = strlen(handle);
    }

    handle[handleLength] = '>'; //add > to end of handle
    handle[handleLength+1] = ' ';

    char c;
    while ((c = getchar()) != '\n' && c != EOF) { } //clear stdin of newlines - http://stackoverflow.com/questions/7898215/how-to-clear-input-buffer-in-c
}


//sendMessage
//This function gets input from the user, sends it to the server, and checks whether to quit or not.
void sendMessage(char* handle, char* buffer, int socketFD)
{
    memset(buffer, '\0', 512); // zero out buffer
    int i;
    int handleLength = strlen(handle);
    for (i = 0; i < handleLength; i++)
    {
        buffer[i] = handle[i];
    }

    printf("%s", handle);

    fgets(buffer+handleLength, 512-handleLength, stdin);
    buffer[strcspn(buffer, "\n")] = '\0'; //strip newline off end

    send(socketFD, buffer, 512, 0); //send message

    char* quitCommand = "\\quit\0"; //quit command to compare to message
    if(strstr(buffer, quitCommand) != NULL) //if the quit command was entered by the client
    {
        close(socketFD); //close the socket and exit
        printf("Connection closed.\n");
        exit(0);
    }
}


//receiveMessage
//This function receives the message from the server, prints it to the screen, and checks whether to quit or not.
void receiveMessage(char* buffer, int socketFD)
{
    memset(buffer, '\0', 512); //zero out buffer

    recv(socketFD, buffer, 512, 0); //get message
    printf("%s\n", buffer); //print the message

    char* quitCommand = "\\quit\0"; //quit command to compare to message
    if(strstr(buffer, quitCommand) != NULL) //if the quit command was entered by the server
    {
        close(socketFD); //close the socket and exit
        printf("Connection closed.\n");
        exit(0);
    }
}


int main(int argc, const char * argv[]) {
    if (argc != 3) { fprintf(stderr,"USAGE: %s chatclient <server-hostname> <port#>\n", argv[0]); exit(1); } // Check usage & args

    int socketFD = makeContact(argv); //make contact, get socket file descriptor

    char buffer[512]; //buffer for holding messages being sent and received

    char handle[50]; //holds handle of user
    getHandle(handle); //get handle from user

    while(1)
    {
        sendMessage(handle, buffer, socketFD); //send message
        receiveMessage(buffer, socketFD); //receieve message
    }

    return 0;
}
