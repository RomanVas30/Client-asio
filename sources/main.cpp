// Copyright 2018 Roman Vasyutin romanvas3008@gmail.com

#include <client.hpp>

boost::asio::ip::tcp::endpoint ep(
       boost::asio::ip::address::from_string("127.0.0.1"), 8001);

void run_client(const std::string& client_name) {
  talk_to_svr client(client_name);
  try {
    client.connect(ep);
    client.loop();
  }
  catch(boost::system::system_error& err) {
    boost::recursive_mutex::scoped_lock lk(mutex);
    std::cout << "client terminated " << std::endl;
  }
}

int main() {
  std::srand(std::time(0));
  boost::thread_group threads;
  threads.create_thread(boost::bind(run_client, "Romka"));
  threads.create_thread(boost::bind(run_client, "Asik"));
  threads.create_thread(boost::bind(run_client, "Anton"));
  threads.create_thread(boost::bind(run_client, "Robert"));
  threads.join_all();
  return 0;
}
