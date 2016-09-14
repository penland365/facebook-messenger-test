#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace util {

  void Die(std::string reason) {
    std::cout << reason << std::endl;
    exit(1);
  }

  std::string ReadAccessTokenOrDie() {
    auto access_token = std::getenv("FB_MESSENGER_TEST_ACCESS_TOKEN");
    if(access_token == nullptr)
      Die("Facebook Messenger Access Token not found");
    return std::string(access_token);
  }


  std::string ReadJsonFile(const std::string json_file) {
    std::ifstream input_stream(json_file);
    if(!input_stream)
      Die("Json File cannot be opened");

    std::stringstream sstream;
    std::string line;
    while(getline(input_stream, line))
      sstream << line << std::endl;

    return sstream.str();
  }

}
