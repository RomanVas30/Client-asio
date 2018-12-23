// Copyright 2018 Roman Vasyutin romanvas3008@gmail.com

#include <client.hpp>

boost::asio::io_service service;
boost::recursive_mutex mutex;

void talk_to_svr::connect(boost::asio::ip::tcp::endpoint ep) {
  sock_.connect(ep);
}

void talk_to_svr::loop() {
  write("login " + username_ + "\n");
  read_answer();
  while (started_) {
    write_request();
    read_answer();
    boost::this_thread::sleep(boost::posix_time::millisec
                             (std::rand() % 7000));
  }
}

void talk_to_svr::read_answer() {
  for (unsigned i = 0; i < max_msg; ++i)
    buff_[i] = ' ';
  read(sock_, boost::asio::buffer(buff_),
       boost::bind(&talk_to_svr::read_complete, this, _1));
  process_msg();
}

void talk_to_svr::process_msg() {
  std::string msg = "";
  unsigned pos = 0;
  for (int i = max_msg; i > -1; --i)
    if (buff_[i] == '\n') {
        pos = i;
        break;
    }
  for (unsigned i = 0; i < pos; ++i) {
    msg += buff_[i];
  }
  if ( msg.find("login ") == 0) on_login(msg);
  else if ( msg.find("ping") == 0) on_ping(msg);
  else if ( msg.find("clients ") == 0) on_clients(msg);
  else
  {
    boost::recursive_mutex::scoped_lock lk(mutex);
    std::cerr << "invalid msg " << msg << std::endl;
  }
}

void talk_to_svr::on_login(const std::string& msg) {
  boost::recursive_mutex::scoped_lock lk(mutex);
  std::cout << msg << std::endl;
  do_ask_clients();
}

void talk_to_svr::on_ping(const std::string& msg) {
  std::string curr_msg = msg;
  std::istringstream in(msg);
  std::string answer;
  in >> answer >> answer;
  if ( answer == "client_list_changed")
    do_ask_clients();
  else
  {
    boost::recursive_mutex::scoped_lock lk(mutex);
    std::cout << curr_msg << std::endl;
  }
}

void talk_to_svr::on_clients(const std::string& msg) {
  std::string clients = msg.substr(8);
  boost::recursive_mutex::scoped_lock lk(mutex);
  std::cout << username_ << ", new client list:"
            << clients << std::endl;
}

void talk_to_svr::do_ask_clients() {
  write("ask_clients\n");
  read_answer();
}

void talk_to_svr::write(const std::string& msg) {
  boost::recursive_mutex::scoped_lock lk(mutex);
  sock_.write_some(boost::asio::buffer(msg));
}

size_t talk_to_svr::read_complete(const
       boost::system::error_code& err) {
  if (err) return 0;
  bool found = false;
  for (unsigned i = 0; i < max_msg; ++i)
    if (buff_[i] == '\n') {
      found = true;
      break;
    }
  return found ? 0 : 1;
}



