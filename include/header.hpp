// Copyright 2018 Avsyankaa Avsyankaa@gmail.com

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_

#include <string>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <cstdlib>
#include <boost/aligned_storage.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/thread.hpp>
#include <mutex>
#include <vector>

extern boost::asio::io_service service;

struct talk_to_svr
{
	talk_to_svr(const std::string & username): sock_(service),
        started_(true), username_(username) {}
	void connect(boost::asio::ip::tcp::endpoint ep);
	void loop();
	std::string username() const { return username_; }

private:
	boost::asio::ip::tcp::socket sock_;
	enum { max_msg = 1024 };
	int already_read_;
	char buff_[1024];
	bool started_;
	std::string username_;
	void write_request()
{
	write("ping\n");
}
	void read_answer();
	void process_msg();
        void on_login() { do_ask_clients(); }
	void on_ping(const std::string & msg);
	void on_clients(const std::string & msg);
	void do_ask_clients();
	void write(const std::string & msg);
	size_t read_complete(const boost::system::error_code & err, size_t bytes);
};

#endif // INCLUDE_HEADER_HPP_
