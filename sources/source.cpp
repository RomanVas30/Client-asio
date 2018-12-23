// Copyright 2018 Your Name <your_email>

#include <header.hpp>

boost::asio::io_service service;

void talk_to_svr::connect(boost::asio::ip::tcp::endpoint ep){
sock_.connect(ep);
}

void talk_to_svr::loop(){
 write("login " + username_ + "\n");
                read_answer();
                while ( started_)
                {
                        write_request();
                        read_answer();
                        boost::this_thread::sleep(boost::posix_time::millisec(rand() % 7000));
                }
}

void talk_to_svr::read_answer(){
already_read_ = 0;
	read(sock_, boost::asio::buffer(buff_),
	boost::bind(&talk_to_svr::read_complete, this, _1, _2));
	process_msg();
}

void talk_to_svr::process_msg()
{
	std::string msg(buff_, already_read_);
	if ( msg.find("login ") == 0) on_login();
	else if ( msg.find("ping") == 0) on_ping(msg);
	else if ( msg.find("clients ") == 0) on_clients(msg);
	else std::cerr << "invalid msg " << msg << std::endl;
}

void talk_to_svr::on_ping(const std::string & msg)
{
	std::istringstream in(msg);
	std::string answer;
	in >> answer >> answer;
	if ( answer == "client_list_changed")
	do_ask_clients();
}

void talk_to_svr::on_clients(const std::string & msg)
{
	std::string clients = msg.substr(8);
	std::cout << username_ << ", new client list:" << clients;
}

void talk_to_svr::do_ask_clients()
{
	write("ask_clients\n");
	read_answer();
}

void talk_to_svr::write(const std::string & msg) {
 sock_.write_some(boost::asio::buffer(msg)); }

size_t talk_to_svr::read_complete(
const boost::system::error_code & err, size_t bytes) {
if (err) return 0;
	bool found = std::find(buff_, buff_ + bytes, '\n') < buff_ + bytes;
	// we read one-by-one until we get to enter, no buffering
	return found ? 0 : 1;
}



