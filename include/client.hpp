// Copyright 2018 Roman Vasyutin romanvas3008@gmail.com

#ifndef INCLUDE_CLIENT_HPP_
#define INCLUDE_CLIENT_HPP_

#include <boost/aligned_storage.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

extern boost::asio::io_service service;
extern boost::recursive_mutex mutex;

struct talk_to_svr {
  explicit talk_to_svr(const std::string& username):
              sock_(service),
              started_(true), username_(username) {}
  void connect(boost::asio::ip::tcp::endpoint ep);
  void loop();
  std::string username() const {
    return username_;
  }

private:
  boost::asio::ip::tcp::socket sock_;
  enum { max_msg = 1024 };
  char buff_[1024];
  bool started_;
  std::string username_;
  void write_request() {
    write("ping\n");
  }
  void read_answer();
  void process_msg();
  void on_login(const std::string& msg);
  void on_ping(const std::string& msg);
  void on_clients(const std::string& msg);
  void do_ask_clients();
  void write(const std::string& msg);
  size_t read_complete(const boost::system::error_code& err);
};

#endif // INCLUDE_CLIENT_HPP_
