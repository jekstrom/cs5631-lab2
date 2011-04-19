/* This file is Copyright 2000-2009 Meyer Sound Laboratories Inc.  See the included LICENSE.txt file for details. */  

#include "dataio/TCPSocketDataIO.h"
#include "iogateway/PlainTextMessageIOGateway.h"
#include "reflector/StorageReflectConstants.h"
#include "util/NetworkUtilityFunctions.h"
#include "system/SetupSystem.h"

using namespace muscle;

#define TEST(x) if ((x) != B_NO_ERROR) printf("Test failed, line %i\n",__LINE__)

// This is a text based test client to test the PlainTextMessageIOGateway class.
// It should be able to communicate with any server that sends and receives
// lines of ASCII text (e.g. Web servers, XML, eCommerce, etc).
int main(int argc, char ** argv)
{
   CompleteSetupSystem css;

#ifdef SELECT_ON_FILE_DESCRIPTORS_NOT_AVAILABLE
   printf("Warning:  This program does not run very well on this OS, because the OS can not select() on stdin.  You'll need to press return a lot.\n");
#endif

   const char * hostName = "localhost";
   int port = 0;
   if (argc > 1) hostName = argv[1];
   if (argc > 2) port = atoi(argv[2]);
   if (port <= 0) port = 80;

   ConstSocketRef s = Connect(hostName, (uint16)port, "portableplaintextclient", false);
   if (s() == NULL) return 10;

   PlainTextMessageIOGateway gw;
   gw.SetDataIO(DataIORef(new TCPSocketDataIO(s, false)));
   char text[1000] = "";
   fd_set readSet, writeSet;
   while(s())
   {
      FD_ZERO(&readSet);
      FD_ZERO(&writeSet);

      int fd = s.GetFileDescriptor();
      int maxfd = fd;
      FD_SET(fd, &readSet);
      struct timeval * timeout = NULL;
      struct timeval poll = {0, 0};

      if (gw.HasBytesToOutput()) FD_SET(fd, &writeSet);

#ifdef SELECT_ON_FILE_DESCRIPTORS_NOT_AVAILABLE
      // BeOS/Win32 can't select() on stdin (yet), so just make the user press enter or whatever
      printf("Enter something => "); 
      fflush(stdout);
      if (fgets(text, sizeof(text), stdin) == NULL) text[0] = '\0';
      char * ret = strchr(text, '\n'); if (ret) *ret = '\0';
      timeout = &poll;  // prohibit blocking in select()
#else
      if (STDIN_FILENO > maxfd) maxfd = STDIN_FILENO;
      FD_SET(STDIN_FILENO, &readSet);
#endif

      QueueGatewayMessageReceiver inQueue;
      while(s()) 
      {
         if (select(maxfd+1, &readSet, &writeSet, NULL, timeout) < 0) printf("portablereflectclient: select() failed!\n");

         timeout = &poll;  // only block the first time (Linux) or not at all (BeOS)

#ifndef SELECT_ON_FILE_DESCRIPTORS_NOT_AVAILABLE
         if (FD_ISSET(STDIN_FILENO, &readSet)) 
         {
            if (fgets(text, sizeof(text), stdin) == NULL) text[0] = '\0';
            char * ret = strchr(text, '\n'); if (ret) *ret = '\0';
         }
#endif

         if (text[0])
         {
            printf("Sending: [%s]\n",text);
            MessageRef msg = GetMessageFromPool();
            msg()->AddString(PR_NAME_TEXT_LINE, text);
            gw.AddOutgoingMessage(msg);
            text[0] = '\0';
         }
   
         bool reading = FD_ISSET(fd, &readSet);
         bool writing = FD_ISSET(fd, &writeSet);
         bool writeError = ((writing)&&(gw.DoOutput() < 0));
         bool readError  = ((reading)&&(gw.DoInput(inQueue) < 0));
         if ((readError)||(writeError))
         {
            printf("Connection closed, exiting.\n");
            s.Reset();
         }

         MessageRef incoming;
         while(inQueue.RemoveHead(incoming) == B_NO_ERROR)
         {
            printf("Heard message from server:-----------------------------------\n");
            const char * inStr;
            for (int i=0; (incoming()->FindString(PR_NAME_TEXT_LINE, i, &inStr) == B_NO_ERROR); i++) printf("Line %i: [%s]\n", i, inStr);
           
            printf("-------------------------------------------------------------\n");
         }

         if ((reading == false)&&(writing == false)) break;

         FD_ZERO(&readSet);
         FD_ZERO(&writeSet);
         FD_SET(fd, &readSet);
         if (gw.HasBytesToOutput()) FD_SET(fd, &writeSet);
      }
   }

   if (gw.HasBytesToOutput())
   {
      printf("Waiting for all pending messages to be sent...\n");
      while((gw.HasBytesToOutput())&&(gw.DoOutput() >= 0)) {printf ("."); fflush(stdout);}
   }
   printf("\n\nBye!\n");
   return 0;
}
