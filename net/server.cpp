#include "server.hpp"

namespace net {
	const unsigned int server::receive_timeout = NET_SERVER_RECEIVE_TIMEOUT;
	const unsigned int server::send_timeout = NET_SERVER_SEND_TIMEOUT;
	
	server::server(const char* const endpoint)
			: endpoint{0}, isRunning(false), doExit(false), workerThreadCount(0),
			context(1), clients(context, ZMQ_ROUTER), workers(context, ZMQ_DEALER),
			control(context, ZMQ_PUB) {
		memcpy(this->endpoint, endpoint, strlen(endpoint));
	}
	
	server::~server() {
		stop();
	}
	
	void server::listen(const std::size_t workerCount) {
		if(workerCount > NET_SERVER_MAX_THREADS) {
			throw std::runtime_error(err_msg::_arybnds);
		}
		
		std::lock_guard<std::mutex> lock(stateChangeMutex);
		
		if(!isRunning) {
			// Update our internal state
			isRunning = true;
			
			// Front end where we listen for client requests
			clients.bind(endpoint);
			// Internal address worker threads will retrieve work from
			workers.bind(NET_SERVER_ZMQ_WORKER_LOCATION);
			// Control socket used to send commands to the steerable proxy,
			// the device that connects the front end and internal worker address
			control.bind(NET_SERVER_ZMQ_CONTROL_LOCATION);
			
			// Launch worker thread pool
			workerThreadCount = workerCount;
			for(std::size_t i = 0; i < workerThreadCount; i++) {
				workerThreads[i] = std::thread(&server::work, this);
			}
			
			// Connect work threads to client threads via a so called proxy.
			// Needs to be called in a thread because the call is blocking.
			proxyThread = std::thread(&server::proxy_work, this);
		}
	}
	
	void server::stop() {
		std::lock_guard<std::mutex> lock(stateChangeMutex);
		
		if(isRunning) {
			// Signal our proxy thread to shutdown
			control.send("TERMINATE", 9);
			
			// Signal our worker threads to shutdown
			doExit = true;
			
			for(std::size_t i = 0; i < workerThreadCount; i++) {
				workerThreads[i].join();
			}
			workerThreadCount = 0;
			
			control.disconnect(NET_SERVER_ZMQ_CONTROL_LOCATION);
			proxyThread.join();
			
			// Update our internal state
			doExit = false;
			isRunning = false;
		}
	}
	
	void server::proxy_work() {
		// We listen for control signals for our proxy on this socket
		// Reference to context is threadsafe
		::zmq::socket_t controlSub = ::zmq::socket_t(context, ZMQ_SUB);
		controlSub.setsockopt(ZMQ_SUBSCRIBE, "", 0);
		controlSub.connect(NET_SERVER_ZMQ_CONTROL_LOCATION);
		
		zmq::proxy_steerable((void*)clients, (void*)workers, 0, (void*)controlSub);
		
		controlSub.disconnect(NET_SERVER_ZMQ_CONTROL_LOCATION);
	}
	
	void server::work() {
		// Context is threadsafe
		zmq::socket_t socket(context, ZMQ_REP);
		socket.connect(NET_SERVER_ZMQ_WORKER_LOCATION);
		socket.setsockopt(ZMQ_RCVTIMEO, &receive_timeout, sizeof(receive_timeout));
		socket.setsockopt(ZMQ_SNDTIMEO, &send_timeout, sizeof(send_timeout));
		
		while(!doExit) {
			zmq::message_t requestMsg;
			// Wait to receive request from client
			if(socket.recv(&requestMsg)) {
				response* result = 0;
				// Perform work
				if(((char*)requestMsg.data())[requestMsg.size()-1] != '\0') {
					result = new response(err_msg::_mnltrmr, true);
				} else {
					// After this call to generate a request, the data within the
					// requestMsg object is NOT valid. We use insitu parsing to avoid
					// reallocation. If you call requestMsg.data() after this, do not
					// expect to get sensible results.
					result = route_request(request((char*)requestMsg.data()));
				}
				
				// Send reply
				socket.send(::zmq::message_t((void*)result->get_json(),
						result->get_json_size(),
						// This conforms to the requirement imposed by zmq::message_t
						// zero-copy idiom that passes a pointer to the data along
						// with a hint object. Because our data is within the hint
						// object, we just deallocate the hint object, which is our
						// case is a request object. The use of the idiom ensures we
						// do not copy the data of a request in zmq and rather we tell
						// zmq the buffer is safe to use until the message is sent.
						// This function is then called automatically to delete the
						// request object.
						[] (void* data, void* hint) {
							UNUSED(data);
							delete static_cast<response*>(hint);
						},
						result));
			}
		}
		
		socket.disconnect(NET_SERVER_ZMQ_WORKER_LOCATION);
	}
}
