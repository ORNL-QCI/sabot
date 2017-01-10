#ifndef _NET_SERVER_HPP
#define _NET_SERVER_HPP

#include <common.hpp>
#include "../actions.hpp"
#include "request.hpp"
#include "response.hpp"
#include <atomic>
#include <mutex>
#include <thread>
#include <cppzmq/zmq.hpp>

/**
 * \brief Maximum number of milliseconds to block while waiting to receive a message.
 */
#define NET_SERVER_RECEIVE_TIMEOUT 1000

/**
 * \brief Maximum number of milliseconds to block while waiting to send a message.
 */
#define NET_SERVER_SEND_TIMEOUT 250

/**
 * \brief Maximum number of worker threads that can be launched.
 */
#define NET_SERVER_MAX_THREADS 16

/**
 * \brief The maxmimum length of the endpoint to bind to (including null terminator).
 */
#define NET_SERVER_MAX_LENGTH_ENDPOINT 128

/**
 * \brief Internally used zmq socket used for communication between frontend listener and
 * backend workers.
 */
#define NET_SERVER_ZMQ_WORKER_LOCATION "inproc://workers"

/**
 * \brief Internally used zmq socket used to signal zmq proxy that binds the frontend
 * listener and the backend workers.
 */
#define NET_SERVER_ZMQ_CONTROL_LOCATION "inproc://control"

namespace net {
	/**
	 * \brief An RPC server for interfacing with the universe.
	 */
	class server {
	 public:
		/**
		 * \brief Constructor takes an endpoint to bind to.
		 * 
		 * The format should be transport://address where transport is the transport
		 * protocol to be used and address is specific to the transport protocol. For
		 * example a TCP connection on localhost could be tcp://127.0.0.1:12345. When
		 * using network devices under *nix, you may use either the IP address of the
		 * device or the device name, e.g. eth0, lo, etc.
		 */
		server(const char* const endpoint);
		
		/**
		 * \brief Copy constructor is disabled.
		 */
		server(const server&) = delete;
		
		 /**
		 * \brief Move constructor is disabled.
		 */
		server(server&&) = delete;
		
		 /**
		 * \brief Assignment operator is disabled.
		 */
		server& operator=(const server&) = delete; 
		
		 /**
		 * \brief Move assignment operator is disabled.
		 */
		server& operator=(server&&) = delete;
		
		/**
		 * \brief Destructor will shut down server gracefully if it is still up.
		 */
		~server();
		
		/**
		 * \brief Start the server listening with a particular number of listen threads in
		 * the pool.
		 * 
		 * \note Threadsafe
		 */
		void listen(const std::size_t workerCount);
		
		/**
		 * \brief Stop the server listening and processing requests.
		 * 
		 * \note Threadsafe
		 */
		void stop();
		
		/**
		 * \brief Return whether or not the server is running.
		 * 
		 * \note Threadsafe.
		 */
		inline bool is_running() const {
			return isRunning;
		}
	
	 private:
		/**
		 * \brief Maximum number of milliseconds to block with zmq's recv().
		 * 
		 * \note See the appropriate #define directive in this header file to change this
		 * value. We cannot initialize the value here, since we are required to pass it by
		 * address to zmq. Instead, the value is initialized from that directive within
		 * the server source file.
		 */
	 	static const unsigned int receive_timeout;
	 	
		/**
		 * \brief Maximum number of milliseconds to block with zmq's send().
		 * 
		 * \note See the appropriate #define directive in this header file to change this
		 * value. We cannot initialize the value here, since we are required to pass it by
		 * address to zmq. Instead, the value is initialized from that directive within
		 * the server source file.
		 */
		static const unsigned int send_timeout;
		
		/**
		 * \brief The endpoint we are listening on.
		 * 
		 * See the server constructor for format details.
		 */
		char endpoint[NET_SERVER_MAX_LENGTH_ENDPOINT];
		
		/**
		 * \brief Mutex to protect startup and shutdown.
		 */
		std::mutex stateChangeMutex;
		
		/**
		 * \brief Whether or not we are currently listening.
		 */
		std::atomic_bool isRunning;
		
		/**
		 * \brief Whether or not we are signaling to the worker threads to exit.
		 */
		std::atomic_bool doExit;
		
		/**
		 * \brief The thread that runs a zmq proxy that connects our front end listener to
		 * our backend workers.
		 * 
		 * A zmq control socket is used in stop() that signals this thread to exit.
		 */
		std::thread proxyThread;
		
		/**
		 * \brief An array of thread objects where each thread is a worker that is
		 * processing requests.
		 */
		std::thread workerThreads[NET_SERVER_MAX_THREADS];
		
		/**
		 * \brief The number of worker threads.
		 */
		std::size_t workerThreadCount;
		
		/**
		 * \brief The zmq context object.
		 * 
		 * See zmq documentation for more details.
		 */
		zmq::context_t context;
		
		/**
		 * \brief The front end listener that actually binds to our endpoint. All requests
		 * are funnelled through this socket.
		 */
		zmq::socket_t clients;
		
		/**
		 * \brief An internal socket where requests accepted by the frontend socket are
		 * passes to the worker threads that are listening.
		 */
		zmq::socket_t workers;
		
		/**
		 * \brief The zmq control socket that allows us to shutdown our zmq proxy thread.
		 */
		zmq::socket_t control;
		
		/**
		 * \brief Function to start the zmq proxy_steerable.
		 * 
		 * Because zmq::proxy_steerable is blocking, we need to launch it on a seperate
		 * thread.
		 * 
		 * \note Threadsafe
		 */
		void proxy_work();
		
		/**
		 * \brief Given a zmq context, wait for a client request, process the request, and
		 * send the client a reply.
		 * 
		 * In our case, first this function decodes the request into a format we can
		 * better process. Second it routes the RPC request to the proper function and
		 * invokes it. Third, it encodes the request of this function into the same format
		 * the client sent, and sends the reply back to the client.
		 * 
		 * \note Threadsafe
		 */
		void work();
		
		/**
		 * \brief Route a request to the proper method and return the result.
		 * 
		 * \note Threadsafe
		 */
		inline response* route_request(const request& request) const {
			try {
				return ::actions::find_call(request);
			} catch (const std::runtime_error& e) {
				return new response(e.what(), true);
			}
		}
	};
}

#endif
