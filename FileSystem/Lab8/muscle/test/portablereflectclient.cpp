/* This file is Copyright 2000-2009 Meyer Sound Laboratories Inc.  See the included LICENSE.txt file for details. */  

#include "dataio/StdinDataIO.h"
#include "dataio/TCPSocketDataIO.h"
#include "iogateway/MessageIOGateway.h"
#include "iogateway/PlainTextMessageIOGateway.h"
#include "reflector/StorageReflectConstants.h"
#include "util/MiscUtilityFunctions.h"
#include "util/NetworkUtilityFunctions.h"
#include "system/SetupSystem.h"

using namespace muscle;

#define TEST(x) if ((x) != B_NO_ERROR) printf("Test failed, line %i\n",__LINE__)

// This is a text based test client for the muscled server.  It is useful for testing
// the server, and could possibly be useful for other things, I don't know.
int main(int argc, char ** argv)
{
   CompleteSetupSystem css;

   String hostName;
   uint16 port = 2960;
   if (argc > 1) ParseConnectArg(argv[1], hostName, port, false);
   ConstSocketRef sock = Connect(hostName(), port, "portablereflectclient", false);
   if (sock() == NULL) return 10;

   // We'll receive plain text over stdin
   StdinDataIO stdinIO(false);
   PlainTextMessageIOGateway stdinGateway; 
   stdinGateway.SetDataIO(DataIORef(&stdinIO, false));

   // And send and receive flattened Message objects over our TCP socket
   TCPSocketDataIO tcpIO(sock, false);
   MessageIOGateway tcpGateway;
   tcpGateway.SetDataIO(DataIORef(&tcpIO, false));

   QueueGatewayMessageReceiver stdinInQueue, tcpInQueue;
   bool keepGoing = true;
   while(keepGoing)
   {
      fd_set readSet;  FD_ZERO(&readSet);
      fd_set writeSet; FD_ZERO(&writeSet);

      int stdinFD       = stdinIO.GetReadSelectSocket().GetFileDescriptor();
      int socketReadFD  = tcpIO.GetReadSelectSocket().GetFileDescriptor();
      int socketWriteFD = tcpIO.GetWriteSelectSocket().GetFileDescriptor();

      FD_SET(stdinFD,      &readSet);
      FD_SET(socketReadFD, &readSet);
      int maxFD = muscleMax(socketReadFD, stdinFD);
      if (tcpGateway.HasBytesToOutput())
      {
         FD_SET(socketWriteFD, &writeSet);
         maxFD = muscleMax(maxFD, socketWriteFD);
      }

      if (select(maxFD+1, &readSet, &writeSet, NULL, NULL) < 0) printf("portablereflectclient: select() failed!\n");

      // Receive data from stdin
      if (FD_ISSET(stdinFD, &readSet))
      {
         while(1)
         {
            int32 bytesRead = stdinGateway.DoInput(stdinInQueue);
            if (bytesRead < 0)
            {
               printf("Stdin closed, exiting!\n");
               keepGoing = false;
               break;
            }
            else if (bytesRead == 0) break;  // no more to read
         }
      }

      // Handle any input lines that were received from stdin
      MessageRef msgFromStdin;
      while(stdinInQueue.RemoveHead(msgFromStdin) == B_NO_ERROR)
      {
         const String * st;
         for (int32 i=0; msgFromStdin()->FindString(PR_NAME_TEXT_LINE, i, &st) == B_NO_ERROR; i++)
         {
            const char * text = st->Cstr();

            printf("You typed: [%s]\n", text);
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
                  keepGoing = send = false;
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

               default:
                  printf("Sorry, wot?\n");
                  send = false;
               break;
            }

            if (send) 
            {
               printf("Sending message...\n");
               ref()->PrintToStream();
               tcpGateway.AddOutgoingMessage(ref);
            }
         }
      }

      // Handle input and output on the TCP socket
      bool reading = (FD_ISSET(socketReadFD,  &readSet)  != 0);
      bool writing = (FD_ISSET(socketWriteFD, &writeSet) != 0);
      bool writeError = ((writing)&&(tcpGateway.DoOutput() < 0));
      bool readError  = ((reading)&&(tcpGateway.DoInput(tcpInQueue) < 0));
      if ((readError)||(writeError))
      {
         printf("Connection closed (%s), exiting.\n", writeError?"Write Error":"Read Error");
         keepGoing = false;
      }

      MessageRef msgFromTCP;
      while(tcpInQueue.RemoveHead(msgFromTCP) == B_NO_ERROR)
      {
         printf("Heard message from server:-----------------------------------\n");
         msgFromTCP()->PrintToStream();
         printf("-------------------------------------------------------------\n");
      }
   }

   if (tcpGateway.HasBytesToOutput())
   {
      printf("Waiting for all pending messages to be sent...\n");
      while((tcpGateway.HasBytesToOutput())&&(tcpGateway.DoOutput() >= 0)) {printf ("."); fflush(stdout);}
   }
   printf("\n\nBye!\n");

   return 0;
}
