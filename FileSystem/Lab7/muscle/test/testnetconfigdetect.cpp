#include "system/DetectNetworkConfigChangesSession.h"
#include "reflector/ReflectServer.h"
#include "system/SetupSystem.h"

using namespace muscle;

class TestSession : public DetectNetworkConfigChangesSession
{
public:
   TestSession() {/* empty */}

   virtual void NetworkInterfacesChanged()
   {
      LogTime(MUSCLE_LOG_INFO, "Network configuration change detected!\n");
   }
};

int main(int argc, char ** argv) 
{
   CompleteSetupSystem css;  // set up our environment

   ReflectServer server;
   TestSession session;

   if (server.AddNewSession(AbstractReflectSessionRef(&session, false)) == B_NO_ERROR)
   {
      LogTime(MUSCLE_LOG_INFO, "Beginning Network-Configuration-Change-Detector test... try changing your network config, or plugging/unplugging an Ethernet cable.\n");
      if (server.ServerProcessLoop() == B_NO_ERROR) LogTime(MUSCLE_LOG_INFO, "testnetconfigdetect event loop exiting.\n");
                                               else LogTime(MUSCLE_LOG_CRITICALERROR, "testnetconfigdetect event loop exiting with an error condition.\n");
   }
   server.Cleanup();

   return 0;
}
