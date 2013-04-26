#include "SecureDistributedChat.h"
using namespace std;

class AuthenticationImpl : public virtual sdc::AuthenticationI {
public:
  string echo(const string &s, const Ice::Current&) { return s; }
  void registerUser(const sdc::User&, const string&, const Ice::Current&) {}
  sdc::SessionIPrx login(const sdc::User&, const string&, const Ice::Identity&,
    const Ice::Current&) { return NULL; }
};
