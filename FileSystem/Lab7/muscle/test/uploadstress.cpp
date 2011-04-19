/* This file is Copyright 2000-2009 Meyer Sound Laboratories Inc.  See the included LICENSE.txt file for details. */  

#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <time.h>

#include "dataio/TCPSocketDataIO.h"
#include "iogateway/MessageIOGateway.h"
#include "reflector/StorageReflectConstants.h"
#include "util/NetworkUtilityFunctions.h"
#include "system/SetupSystem.h"

using namespace muscle;

#define TEST(x) if ((x) != B_NO_ERROR) printf("Test failed, line %i\n",__LINE__)

// This client just uploads a bunch of stuff to the server, trying to batter it down.
int main(int argc, char ** argv)
{
   CompleteSetupSystem css;

   const char * hostName = "localhost";
   int port = 0;
   if (argc > 1) hostName = argv[1];
   if (argc > 2) port = atoi(argv[2]);
   if (port <= 0) port = 2960;

   QueueGatewayMessageReceiver inQueue;
   while(true)
   {
      uint32 bufCount=0;
      ConstSocketRef s = Connect(hostName, (uint16)port, "uploadstress");
      if (s() == NULL) return 10;

      MessageIOGateway gw;
      gw.SetDataIO(DataIORef(new TCPSocketDataIO(s, false)));
      fd_set readSet, writeSet;
      while(true)
      {
         int fd = s.GetFileDescriptor();
         int maxfd = fd;
         FD_ZERO(&readSet);
         FD_ZERO(&writeSet);
         FD_SET(fd, &readSet);
         FD_SET(fd, &writeSet);

         if (select(maxfd+1, &readSet, &writeSet, NULL, NULL) < 0) printf("uploadstress: select() failed!\n");

         bool reading = FD_ISSET(fd, &readSet);
         bool writing = FD_ISSET(fd, &writeSet);

         if (gw.HasBytesToOutput() == false)
         {
            char buf[128];
            sprintf(buf, UINT32_FORMAT_SPEC, bufCount++);
            printf("Adding message [%s]\n", buf);

            MessageRef smsg = GetMessageFromPool(PR_COMMAND_SETDATA);
            Message data(1234);
            data.AddString("nerf", "boy!");
            smsg()->AddMessage(buf, data);
            gw.AddOutgoingMessage(smsg);
         }

         bool writeError = ((writing)&&(gw.DoOutput() < 0));
         bool readError  = ((reading)&&(gw.DoInput(inQueue) < 0));
         if ((readError)||(writeError))
         {
            printf("Connection closed, exiting.\n");
            break;
         }

         MessageRef incoming;
         while(inQueue.RemoveHead(incoming) == B_NO_ERROR)
         {
            printf("Heard message from server:-----------------------------------\n");
            incoming()->PrintToStream();
            printf("-------------------------------------------------------------\n");
         }
      }
   }

   printf("\n\nBye!\n");
   return 0;
}