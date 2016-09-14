#ifndef _UTIL_HEADER_H_ 
#define _UTIL_HEADER_H_ 

namespace util {
  void Die(std::string reason);
  std::string ReadAccessTokenOrDie();
  std::string ReadJsonFile(const std::string json_file);
}

#endif
