#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>

#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>

#include <fpm/fixed.hpp>

#include "networking.hpp"
#include "collider.hpp"
#include "graphics.hpp"
#include "physics.hpp"

using f16 = fpm::fixed_16_16;

std::string HOST = "127.0.0.1";
uint PORT = 8080;


std::shared_ptr<Graphics> graphics (new Graphics());
std::shared_ptr<Physics> physics (new Physics());
std::shared_ptr<Peer> network;

int main(int argc, char** argv) {

  //-------------------------------
  // COMMAND LINE PARSING
  //-------------------------------
  if (argc < 2){
      std::clog << "BROKE\n";
USAGE:
    std::cout << "usage: game -host [host_address] -server\n";
    std::cout << "options:\n";
    std::cout << "  -host: Host must be valid IP address\n";
    std::cout << "  -port: Port cannot be 0\n";
    std::cout << "  -server: makes instance server\n";
    return 0;
  }

  bool is_server = false;
  bool shall_pass = false;
  for (int i = 1; i < argc; i++) {
    std::string str (argv[i]);
    std::clog << str << '\n';
    std::transform(str.begin(), str.end(),str.begin(), ::toupper);

    if (str.compare("-SERVER") == 0) {
      shall_pass = true;
      is_server = true;
      std::clog << "SERVER\n";
      continue;
    }

    if (str.compare("-PORT") == 0) {
      PORT = atoi(argv[i+1]);
      continue;
    }

    if (str.compare("-HOST") == 0) {
      HOST = argv[i+1];
      shall_pass = true;
      continue;
    }
  }

  if (!shall_pass) { goto USAGE; }

  if (is_server) {
    network = std::shared_ptr<Peer>(new Server(PORT));
  } else {
    network = std::shared_ptr<Peer>(new Client(HOST.c_str(), PORT));
  }

  network->start();
  physics->run(network);
  graphics->update(physics);
  physics->abort();
}
