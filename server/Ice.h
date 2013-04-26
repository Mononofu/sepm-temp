#include <Ice/Ice.h>
#include <string>
using namespace std;

class IceChat {
public:
  IceChat(string pub_key_path, string priv_key_path, string ca_path);
  ~IceChat();
private:
  Ice::CommunicatorPtr ic;
};
