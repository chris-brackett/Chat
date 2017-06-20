Chris Brackett
2/8/2017

Instructions:
1.) Unzip all files into a folder.
2.) Navigate to that folder in a terminal window.
3.) Type "chmod +x p1script" without the quotes and hit enter.
4.) Type "p1script" without the quotes and hit enter.
5.) Start chatserve by typing "./chatserve <port#>" without quotes and replacing <port#> with a valid port number (i.e. ./chatserve 12468), then hit enter.
6.) Open a new terminal window (can be on a different flip server) and navigate to the same folder.
7.) Start chatclient by typing "./chatclient <server-hostname> <port#>" without quotes and replacing <server-hostname> with the server name that chatserve is running on, and <port#> with the port number that chatserve is running on (i.e. ./chatclient flip2.engr.oregonstate.edu 12468), then hit enter.

To use the programs:
1.) Enter a handle when prompted. It must be 10 characters or less and not contain "\quit".
2.) Send a message from the client.
3.) Message will be received by the server, allowing you to send a message from the server.
4.) Messages must alternate back and forth between client and server.
5.) To quit, send a message of "\quit" from either the client or the server.
6.) To terminate chatserve, press ctrl+c.
