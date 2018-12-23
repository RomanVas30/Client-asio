#include <header.hpp>

boost::asio::ip::tcp::endpoint ep( boost::asio::ip::address::from_string("127.0.0.1"), 8001);
void run_client(const std::string & client_name)
{
	talk_to_svr client(client_name);
	try
	{
		client.connect(ep);
		client.loop();
	}
	catch(boost::system::system_error & err)
	{
		std::cout << "client terminated " << std::endl;
	}
}

int main() {
	run_client("Asya");
	return 0;
}
