#include <common.hpp>
#include "net/server.hpp"
#include <csignal>
#include <atomic>
#include <string>
#include <iostream>
#include <boost/program_options.hpp>

#define DEFAULT_THREAD_COUNT 1

static std::sig_atomic_t signal_code(0);

/**
 * \brief Handler for process signals.
 */
void term_handler(int code) {
	if(signal_code == 0) {
		signal_code = code;
	}
}

/**
 * \brief The main entry point of the program.
 */
int main(int argc, char *argv[]) {
	// Program parameters
	std::string endpoint;
	std::uint_fast64_t threadCount(DEFAULT_THREAD_COUNT);
	
	try {
		namespace po = boost::program_options;
		
		po::options_description desc("Options");
		desc.add_options()
			("help,h", "Print help")
			("endpoint,e", po::value<std::string>(&endpoint)->required(), "Endpoint")
			("thread,t", po::value<std::uint_fast64_t>(&threadCount), "Thread Count");
		
		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		
		if(vm.count("help")) {
			std::cout << desc << std::endl;
			exit(EXIT_SUCCESS);
		}
		
		po::notify(vm);
		
	} catch(boost::program_options::required_option &e) {
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
	} catch(boost::program_options::error &e) {
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	
	net::server server(endpoint.c_str());
	server.listen(threadCount);
	
	// Handle term signal
	struct sigaction sigIntHandler;
	sigIntHandler.sa_handler = term_handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, 0);
	sigaction(SIGTERM, &sigIntHandler, 0);
	
	// Wait for signal
	pause();
	std::cout << "Caught signal " << signal_code << "." << std::endl;
	
	// Graceful stop and cleanup
	server.stop();
	
	return 0;
}
