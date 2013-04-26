#include <iostream>
#include <Ice/Ice.h>

using namespace std;
using namespace IceUtil;

class ThreadHook : public virtual Ice::ThreadNotification {
public:
  void start() {
    cout << "start: id = " << ThreadControl().id() << endl;
  }

  void stop() {
    cout << "stop: id = " << ThreadControl().id() << endl;
  }
};
