#ifndef _FACEBOOK_CLIENT_HEADER_H_ 
#define _FACEBOOK_CLIENT_HEADER_H_ 

#include <boost/asio.hpp>
#include <string>

class FacebookClient {
  public:
    FacebookClient(const std::string access_token, const std::string json_string);
    void PostMessage();
  private:
    const std::string kFacebookGraphVersion;
    std::string access_token_;
    std::string json_string_;
    inline std::string build_status_line();
    void build_request(boost::asio::streambuf &request);
};

#endif
