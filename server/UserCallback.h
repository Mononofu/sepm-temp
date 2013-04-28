#include <map>
#include "AuthenticationImpl.h"
#include <boost/thread.hpp>

using namespace std;

class UserCallback {
public:
  static sdc::ChatClientCallbackIPrx getCallback(string user);
  static void setCallback(string user, sdc::ChatClientCallbackIPrx callback);
private:
  static map<string, sdc::ChatClientCallbackIPrx> callbacks;
  static boost::mutex mutex;
};
