#ifndef ICE_CHAT_H
#define ICE_CHAT_H

#include <Ice/Ice.h>
#include <string>
using namespace std;

class IceChat {
public:
  IceChat(string pub_key_path, string priv_key_path, string ca_path);
  ~IceChat();

  void run();
  void stop();
  Ice::ObjectPrx createAdapter(const Ice::ObjectPtr &o, const Ice::ConnectionPtr &con);
private:
  Ice::CommunicatorPtr ic;
};

#endif
