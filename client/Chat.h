#ifndef CHAT_H
#define CHAT_H

#include <boost/format.hpp>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <IceSSL/IceSSL.h>
#include "SecureDistributedChat.h"
#include "ChatClientCallbackImpl.h"

using namespace std;

// wrapper for Ice boilerplate
class Chat {
public:
  // constructor tries to set up connection to server, throws if it fails
  Chat(string server, string port, string cert_path) {
    boost::format connection_string = boost::format("Authentication:ssl -h %1% -p %2%");
    int argc = 1;
    char prog_name[] = "sdc_client";
    char *argv[] = { prog_name };
    Ice::PropertiesPtr props = Ice::createProperties(argc, argv);
    try {
      // enable the SSL plugin for secure connections
      props->setProperty("Ice.Plugin.IceSSL", "IceSSL:createIceSSL");
      props->setProperty("IceSSL.CertAuthFile", cert_path);
      props->setProperty("Ice.ACM.Client", "0");
      props->setProperty("Ice.ThreadPool.Client.Size", "3");
      props->setProperty("Ice.ThreadPool.Server.Size", "3");
      // necessary so invalid connections will time out
      props->setProperty("Ice.Override.ConnectTimeout", "1000");
      // enable warnings
      // props->setProperty("Ice.Warn.Connections", "1");
      // props->setProperty("Ice.Trace.Protocol", "1");
      // props->setProperty("Ice.Trace.Network", "2");

      // Initialize a communicator with these properties.
      Ice::InitializationData id;
      id.properties = props;
      ic = Ice::initialize(id);

      Ice::ObjectPrx base = ic->stringToProxy(
        (connection_string % server % port).str() );

      auth = sdc::AuthenticationIPrx::checkedCast(base);


      /* create callback interface */
      Ice::ObjectAdapterPtr adapter = ic->createObjectAdapter("");
      callback_ident.name = IceUtil::generateUUID();
      callback_ident.category = "";
      auto cb = new ChatClientCallbackImpl;
      adapter->add(cb, callback_ident);
      adapter->activate();
      base->ice_getConnection()->setAdapter(adapter);
    } catch (const Ice::Exception& e) {
      if (ic) ic->destroy();
      throw;
    }
  }

  // automatically close connection on destruction
  ~Chat() {
    if (ic) ic->destroy();
  }

  // wrapper for echo method
  string echo(string s) {
    return auth->echo(s);
  }

  void registerUser(const sdc::User &u, const string &pw) {
    auth->registerUser(u, pw);
  }

  sdc::SessionIPrx login(const sdc::User &u, const string &pw) {
    return auth->login(u, pw, callback_ident);
  }

private:
  Ice::CommunicatorPtr ic;
  sdc::AuthenticationIPrx auth;
  Ice::Identity callback_ident;
};

#endif
