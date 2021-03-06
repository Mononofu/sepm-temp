#include <iostream>
#include <boost/program_options.hpp>
#include "ExitHandler.h"
#include "zmqpp/zmqpp.hpp"
#include "SecureDistributedChat.h"
#include "Ice.h"
#include "SocketHandler.h"

#include "auth/Auth.h"

namespace po = boost::program_options;
using namespace std;

int main(int argc, char** argv) {
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "produce help message")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if(vm.count("help")) {
    cout << desc << "\n";
    return 1;
  }

  cout << "Hello from server" << endl;

  zmqpp::context context;
  Auth auth(&context);
  SocketHandler::context = &context;

  IceChat chat("server.crt", "server.key", "ca.crt");


  string wait;
  cin >> wait;
  cout << "Exiting" << endl;

  chat.stop();

  return 0;
}
