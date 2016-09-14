#include "boost/program_options.hpp"
#include "facebook-client.h"
#include "util.h"
#include <iostream>

namespace {
  const size_t kErrorInCommandLine = 1;
  const size_t kSuccess = 0;
  const size_t kErrorUnhandledException = 2;
  const std::string kVersionNumber = "0.0.1";
}

int ReadJsonAndPost(const std::string json_file, const std::string access_token);

int main(int argc, char** argv) {
  try {
    namespace po = boost::program_options;
    po::options_description desc("Options");
    desc.add_options()
      ("help,h", "Print help message")
      ("json_file,jf", po::value<std::string>()->required(), "Required JSON File to POST to Facebook Messenger")
      ("access_token,at", "Facebook Messenger Access Token. Can be provided by FB_MESSENGER_TEST_ACCESS_TOKEN ENV variable. Required.")
      ("version,v", "Print the version number");
    po::variables_map vm;
    try {
      po::store(po::parse_command_line(argc, argv, desc), vm);
      if(vm.count("help")) {
        std::cout << "fbmt - Facebook Messenger Test. A simple program for POSTing different send messages." << std::endl;
        std::cout << std::endl;
        std::cout << "See https://developers.facebook.com/docs/messenger-platform/product-overview/conversation#send_messages " << std::endl;
        std::cout << std::endl;
        std::cout << desc << std::endl;
        return kSuccess;
      } else if(vm.count("version")) {
        std::cout << "version " << kVersionNumber << std::endl;
        std::cout << "built " << __DATE__ << " " << __TIME__ << std::endl;
        return kSuccess;
      } else if(vm.count("json_file")) {
        auto access_token = util::ReadAccessTokenOrDie();
        std::string file_name = vm["json_file"].as<std::string>();
        return ReadJsonAndPost(file_name, access_token);
      } else if(vm.count("json_file") && vm.count("access_token")) {
        auto access_token = vm["access_token"].as<std::string>();
        std::string file_name = vm["json_file"].as<std::string>();
        return ReadJsonAndPost(file_name, access_token);
      }
      po::notify(vm);
    } catch(po::error& error) {
      std::cerr << "ERROR: " << error.what() << std::endl << std::endl;
      std::cerr << desc << std::endl;
      return kErrorInCommandLine;
    }
  } catch (std::exception& ex) {
    std::cerr << "Unhandled exception reached the top of main: "
              << ex.what() << ", application terminating" << std::endl;
    return kErrorUnhandledException;
  }

  return kSuccess;
}

int ReadJsonAndPost(const std::string json_file, const std::string access_token) {
  auto json_string = util::ReadJsonFile(json_file);
  std::unique_ptr<FacebookClient> facebook_client(new FacebookClient(access_token, json_string));
  facebook_client->PostMessage();
  return kSuccess;
}
