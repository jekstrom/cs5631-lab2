/* This file is Copyright 2000-2009 Meyer Sound Laboratories Inc.  See the included LICENSE.txt file for details. */  

#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <time.h>

#include "dataio/UDPSocketDataIO.h"
#include "iogateway/MessageIOGateway.h"
#include "reflector/StorageReflectConstants.h"
#include "util/NetworkUtilityFunctions.h"
#include "system/SetupSystem.h"

using namespace muscle;

#define TEST(x) if ((x) != B_NO_ERROR) printf("Test failed, line %i\n",__LINE__)

// This is a text based UDP test client.
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

   ConstSocketRef s = CreateUDPSocket();
   if (s() == NULL)
   {
      printf("Error creating UDP Socket!\n");
      return 10;
   }
   
        if (port == 0) port = 2960;
   else if (port < 0)
   {
      port = -port;
      if ((BindUDPSocket(s, port) == B_NO_ERROR)&&(SetSocketBlockingEnabled(s, true) == B_NO_ERROR))
      {
         printf("Listening for incoming UDP packets on port %i\n", port);
         char buf[4096];
         while(true)
         {
            ip_address fromIP;
            uint16 fromPort;
            int32 received = ReceiveDataUDP(s, buf, sizeof(buf), true, &fromIP, &fromPort);
            if (received >= 0) 
            {
               char ipBuf[64]; Inet_NtoA(fromIP, ipBuf);
               printf("Received "INT32_FORMAT_SPEC" bytes of UDP data on port %i (data was from %s, %u)\n", received, port, ipBuf, fromPort);
               printf("  [");
               for (int32 i=0; i<received; i++) printf("%02x ", buf[i]);
               printf("]\n");
            }
            else
            {
               printf("Error receiving UDP data, aborting!\n");
               return 10;
            }
         }
      }
      else 
      {
         printf("Error binding UDP socket to port %i\n", port);
      }
   }

   if (SetUDPSocketTarget(s, hostName, (uint16)port, false) != B_NO_ERROR)
   {
      printf("Error setting UDP socket target to [%s]:%i\n", hostName, port);
      return 10;
   }

   MessageIOGateway gw;
   gw.SetDataIO(DataIORef(new UDPSocketDataIO(s, false)));
   char text[1000] = "";
   fd_set readSet, writeSet;
   QueueGatewayMessageReceiver inQueue;
   printf("UDP Event loop starting...\n");
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
      // BeOS can't select() on stdin (yet), so just make the user press enter or whatever
      printf("Enter something => "); 
      fflush(stdout);
      if (fgets(text, sizeof(text), stdin) == NULL) text[0] = '\0';
      char * ret = strchr(text, '\n'); if (ret) *ret = '\0';
      timeout = &poll;  // prohibit blocking in select()
#else
      if (STDIN_FILENO > maxfd) maxfd = STDIN_FILENO;
      FD_SET(STDIN_FILENO, &readSet);
#endif

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
            printf("You typed: [%s]\n",text);
            bool send = true;
            MessageRef ref = GetMessageFromPool();

            switch(text[0])
            {
               case 'm':
                  ref()->what = MAKETYPE("umsg");
                  ref()->AddString(PR_NAME_KEYS, &text[2]);
                  ref()->AddString("info", "This is a user message");
               break;

               case 's':
                  ref()->what = PR_COMMAND_SETDATA;
                  ref()->AddMessage(&text[2], Message(MAKETYPE("HELO"))); 
               break;
   
               case 'k':
                  ref()->what = PR_COMMAND_KICK;
                  ref()->AddString(PR_NAME_KEYS, &text[2]);
               break;

               case 'b':
                  ref()->what = PR_COMMAND_ADDBANS;
                  ref()->AddString(PR_NAME_KEYS, &text[2]);
               break;

               case 'B':
                  ref()->what = PR_COMMAND_REMOVEBANS;
                  ref()->AddString(PR_NAME_KEYS, &text[2]);
               break;

               case 'g':
                  ref()->what = PR_COMMAND_GETDATA;
                  ref()->AddString(PR_NAME_KEYS, &text[2]);
               break;
   
               case 'G':
                  ref()->what = PR_COMMAND_GETDATATREES;
                  ref()->AddString(PR_NAME_KEYS, &text[2]);
                  ref()->AddString(PR_NAME_TREE_REQUEST_ID, "Tree ID!");
               break;

               case 'q':
                  send = false;
                  s.Reset();
               break;
   
               case 'p':
                  ref()->what = PR_COMMAND_SETPARAMETERS;
                  ref()->AddString(&text[2], "");
               break;
   
               case 'P':
                  ref()->what = PR_COMMAND_GETPARAMETERS;
               break;
   
               case 'd':
                  ref()->what = PR_COMMAND_REMOVEDATA;
                  ref()->AddString(PR_NAME_KEYS, &text[2]);
               break;
   
               case 'D':
                  ref()->what = PR_COMMAND_REMOVEPARAMETERS;
                  ref()->AddString(PR_NAME_KEYS, &text[2]);
               break;
   
               case 't':
               {
                  // test all data types
                  ref()->what = 1234;
                  ref()->AddString("String", "this is a string");
                  ref()->AddInt8("Int8", 123);
                  ref()->AddInt8("-Int8", -123);
                  ref()->AddInt16("Int16", 1234);
                  ref()->AddInt16("-Int16", -1234);
                  ref()->AddInt32("Int32", 12345);
                  ref()->AddInt32("-Int32", -12345);
                  ref()->AddInt64("Int64", 123456789);
                  ref()->AddInt64("-Int64", -123456789);
                  ref()->AddBool("Bool", true);
                  ref()->AddBool("-Bool", false);
                  ref()->AddFloat("Float", 1234.56789f);
                  ref()->AddFloat("-Float", -1234.56789f);
                  ref()->AddDouble("Double", 1234.56789);
                  ref()->AddDouble("-Double", -1234.56789);
                  ref()->AddPointer("Pointer", ref());
                  ref()->AddFlat("Flat", *ref());
                  char data[] = "This is some data";
                  ref()->AddData("Flat", B_RAW_TYPE, data, sizeof(data));
               }
               break;

               break;

               default:
                  printf("Sorry, wot?\n");
                  send = false;
               break;
            }
   
            if (send) 
            {
               printf("Sending message...\n");
//             ref()->PrintToStream();
               gw.AddOutgoingMessage(ref);
            }
            text[0] = '\0';
         }
   
         bool reading = FD_ISSET(fd, &readSet);
         bool writing = FD_ISSET(fd, &writeSet);
         bool writeError = ((writing)&&(gw.DoOutput() < 0));
         bool readError  = ((reading)&&(gw.DoInput(inQueue) < 0));
         if ((readError)||(writeError))
         {
            printf("%s:  Connection closed, exiting.\n", readError?"Read Error":"Write Error");
            s.Reset();
         }

         MessageRef incoming;
         while(inQueue.RemoveHead(incoming) == B_NO_ERROR)
         {
            printf("Heard message from server:-----------------------------------\n");
            incoming()->PrintToStream();
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
