#include "UserCallback.h"

map<string, sdc::ChatClientCallbackIPrx> UserCallback::callbacks;
boost::mutex UserCallback::mutex;

sdc::ChatClientCallbackIPrx UserCallback::getCallback(string user) {
  boost::lock_guard<boost::mutex> lock(mutex);

  return callbacks[user];
}

void UserCallback::setCallback(string user, sdc::ChatClientCallbackIPrx callback) {
  boost::lock_guard<boost::mutex> lock(mutex);

  callbacks[user] = callback;
}
