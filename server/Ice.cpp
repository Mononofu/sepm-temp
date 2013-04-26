#include "Ice.h"
#include <IceSSL/IceSSL.h>
#include "SecureDistributedChat.h"
#include "ThreadHook.h"
#include "AuthenticationImpl.h"

IceChat::IceChat(string pub_key_path, string priv_key_path, string ca_path) {
  int argc = 1;
  char prog_name[] = "sdc_client";
  char *argv[] = { prog_name };
  Ice::PropertiesPtr props = Ice::createProperties(argc, argv);
  try {
    // enable the SSL plugin for secure connections
    props->setProperty("Ice.Plugin.IceSSL", "IceSSL:createIceSSL");
    props->setProperty("IceSSL.CertFile", pub_key_path);
    props->setProperty("IceSSL.KeyFile", priv_key_path);
    props->setProperty("IceSSL.CertAuthFile", ca_path);
    props->setProperty("IceSSL.Trace.Security", "2");
    // necessary so invalid connections will time out
    props->setProperty("Ice.Override.ConnectTimeout", "1000");

    // Initialize a communicator with these properties.
    Ice::InitializationData id;
    id.properties = props;
    id.threadHook = new ThreadHook();
    ic = Ice::initialize(id);

    Ice::ObjectAdapterPtr oa =
      ic->createObjectAdapterWithEndpoints("AuthenticationEndpoint", "ssl -p 1337");
    oa->add(new AuthenticationImpl(), ic->stringToIdentity("Authentication"));
    oa->activate();
    ic->waitForShutdown();

  } catch (const Ice::Exception& e) {
    if (ic) ic->destroy();
    throw;
  }
}


IceChat::~IceChat() {
  if (ic) ic->destroy();
}
