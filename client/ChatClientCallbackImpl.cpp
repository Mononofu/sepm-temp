#include "ChatClientCallbackImpl.h"
#include <iostream>
#include <string>

using namespace std;

void ChatClientCallbackImpl::appendMessageToChat(const sdc::ByteSeq &msg, const string &chat,
  const sdc::User &sender, const Ice::Current&) {

  std::string msg_str(msg.begin(), msg.end());
  cout << "[" << chat << "] " << sender.ID << ": " << msg_str << endl;
}
