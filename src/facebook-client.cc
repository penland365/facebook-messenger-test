#include "facebook-client.h"
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/format.hpp>
#include "util.h"

using boost::asio::ip::tcp;

FacebookClient::FacebookClient(const std::string access_token, const std::string json_string) : kFacebookGraphVersion("v2.6") {
  this->access_token_ = access_token;
  this->json_string_ = json_string;
}

void FacebookClient::PostMessage() {
  namespace ssl = boost::asio::ssl;
  typedef ssl::stream<tcp::socket> ssl_socket;

  // Create context that uses the default paths for for finding CA certificates
  ssl::context ctx(ssl::context::sslv23);
  ctx.set_default_verify_paths();

  // Create io_service and resolve host
  boost::asio::io_service io_service;
  ssl_socket sock(io_service, ctx);
  tcp::resolver resolver(io_service);
  tcp::resolver::query query("graph.facebook.com", "https");
  boost::asio::connect(sock.lowest_layer(), resolver.resolve(query));
  sock.lowest_layer().set_option(tcp::no_delay(true));

  // Perform SSL handshake and ensure Facebook isn't being devious
  sock.set_verify_mode(ssl::verify_peer);
  sock.set_verify_callback(ssl::rfc2818_verification("graph.facebook.com"));
  sock.handshake(ssl_socket::client);

  boost::asio::streambuf request;
  build_request(request);

  // Send the request.
  boost::asio::write(sock, request);
  boost::asio::streambuf response;
  boost::asio::read_until(sock, response, "\r\n");
  std::istream response_stream(&response);
  std::string http_version;
  response_stream >> http_version;
  int status_code;
  response_stream >> status_code;
  std::string status_message;
  std::getline(response_stream, status_message);
  if (!response_stream || http_version.substr(0, 5) != "HTTP/")
    util::Die("Invalid response\n");
  if (status_code != 200)
    util::Die("Response returned with status code " + std::to_string(status_code) + "\n");


  // skip headers
  boost::asio::read_until(sock, response, "\r\n\r\n");
  boost::system::error_code error;
  while (boost::asio::read(sock, response, boost::asio::transfer_at_least(1), error))
      std::cout << &response;
    if (error != boost::asio::error::eof)
      throw boost::system::system_error(error);
}

void FacebookClient::build_request(boost::asio::streambuf &request) {
  std::ostream request_stream(&request);

  request_stream << build_status_line();
  request_stream << "User-Agent: facebook-messenger-tester\r\n";
  request_stream << "Host: graph.facebook.com\r\n";
  request_stream << "Content-Type: application/json\r\n";
  request_stream << "Content-Length: " << json_string_.length() << "\r\n";
  request_stream << "Connection: close\r\n\r\n";
  request_stream << json_string_;
}

inline std::string FacebookClient::build_status_line() {
  auto formatter = boost::format("POST /%1%/me/messages?access_token=%2% HTTP/1.1\r\n") % kFacebookGraphVersion % access_token_;
  return formatter.str();
}
