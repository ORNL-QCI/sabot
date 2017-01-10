# sabot

## Introduction
Sabot is a quantum state simulator with a client-server architecture. This makes possible distributed simulations with many clients accessing a single instance of the simulator. This design is useful when more than one client wishes to access one quantum state. This occurs during quantum communication, for example. It is also possible to have users running on personal machines with the simulator running on a much more powerful machine.

At a glance,
* It's multi-threaded, increasing simulation throughput in most situations. 
* Simulation requests are formatted in human-readable [JSON](http://json.org), making it easy to use.
* The default server implements [ZeroMQ](http://zeromq.org), enabling connections over UDP, TCP, IPC, inproc, TIPC, PGM, NORM, and SOCKS5.
* State simulations are driven by an assembly language inspired by [QASM](https://www.media.mit.edu/quanta/qasm2circ/) and support macros.
* Provides a fast and good quality PRNG to clients for use in Monte Carlo routines.

## Installation

Sabot has the following dependencies:
* **C++ Compiler** with feature-complete C++11 support (GCC recommended)
* **CMake** with C++11 awareness (v2.8 or above)
* **ZeroMQ** with steerable proxy (v4.1 or above)
* **CPPZMQ** corresponding to ZeroMQ version
* **RapidJSON** stable versions only (v1 or above)
* **Doxygen** for code documentation (optional)

If you do not have binaries available, each major dependency is available on Github:
* [CMake](https://github.com/Kitware/CMake)
* [ZeroMQ](https://github.com/zeromq/libzmq)
* [Doxygen](https://github.com/doxygen/doxygen)

If you plan to go this route, be sure to [checkout](https://git-scm.com/docs/git-checkout) an official release and not use the default HEAD. This will place you on a development branch and may be unstable. Some of the dependencies are implemented as [Git submodules](https://git-scm.com/docs/git-submodule), which allows particular versions to be tagged as a dependecy:
* RapidJSON
* cppzmq

For each submodule, it will be necessary to clone the codebase into the sabot root directory:
```shell
$ git submodule update --init
```

A typical initial installation would look something like:
```shell
$ git submodule update --init
$ mkdir build
$ cd build
$ cmake ../
$ make
$ make test
$ sudo make install
```

Various build options are available when running *cmake ../*

* -DBUILD_RELEASE={ **ON** / OFF }
	
	Specify if you wish to build for release. If *OFF*, then debug is selected and no compiler optimizations are made while assert statements are not stripped from the resulting binary. If *ON*, then release is selected and maximum compiler optimizations are made while assert statements are stripped from the resulting binary. It is important to note that once you run _cmake -BUILD_TYPE ../_ in a directory, CMake will cache the value even if you change it later. The easiest way around this is having two build directories, one for debug and one for release.

* -DUSE_STATIC={ ON / **OFF** }
	
	Specify if you wish to perform static linking to external libraries.

* -DBUILD_TEST={ **ON** / OFF }
	
	Specify if you wish to build unit tests. It is recommended that you leave this *ON* and perform unit tests after building. If *OFF* then the _make check_ command will result in an error.

## Running

Statup via the shell is easy, as only 1 argument is required.

parameter | name | type | required | default
--- | --- | --- | --- | ---
-e | *server endpoint* | string | yes | *none*
-t | *thread count* | Uint | no | 1

The server endpoint should be of the form described [here](http://api.zeromq.org/4-1:zmq-connect).

### Creating Large States

You may find that allocations for large states result in the termination of Sabot while allocations for even larger states result in Sabot returning an exception. This is because of the way the Linux kernel handles virtual memory, in particular how [virtual memory may be overcomitted above the amount of physical RAM + swap available](https://kernel.org/doc/Documentation/vm/overcommit-accounting). The kernel supports several overcommitting modes, each of which may be useful for a particular situation. If you want more stability, disable overcommiting because then Sabot will handle the bad_alloc exception that is thrown. If overcommiting is allowed, then there's a possiblity that Sabot will be aborted without sending it a bad_alloc exception.

## Examples

Examples of using sabot can be found in examples/.

## Code Documentation

To generate code documentation, execute the following in the project root directory:
```shell
$ doxygen docs.doxy
```
This will generate and save documentation in docs/ which may be viewed by pointing your web browser to docs/html/index.html.

## Request Interface
Each request should have the following elements:

name | type | description | note
--- | --- | --- | ---
method | string | referring to the method name to invoke. | Must be lowercase
parameters | array(any) | Parameters for the invoked method. | *none*

Each response will consist of the following elements:

name | type | description | note
--- | --- | --- | ---
result | any | todo | *none*
error | boolean | Whether or not the request we are responding to resulted in an error. | If true, the data contained within result may or may not be correct.

### Extended Description

All requests shall be made with a JSON compliant object with 2 fields. The first field will have a key of "method" and a string value of the function to call. The second field will have a key of "parameters" and an array value. The array elements should not have keys and types should coreespond to the requerements of the function being called, in addition to the number of array elements. The order of the elements must not be altered from the specification as [RFC 7159 &sect;1, &sect;5](https://www.rfc-editor.org/rfc/rfc7159.txt) guarantees element order is preserved during transmission.

All responses to a request will be a JSON compliant object with at least 1 field. The first field will have a key of "result" and a value of type determined by the function invoked in the request. The second field is optional and will only appear to indicate an error occured in processing the request. The key will be "error" and the type will be bool. If an error has occured, the result field may contain an error message, no message, or other. There is no guarantee the result will be valid in the context of the request if the error field is present, even if the result appears valid.

All requests shall be a zero terminated string.

### Methods

#### Random Numbers
method | parameters | description
--- | --- | ---
"get_uniform_integer" | [count(uint), lower(int), upper(int)] | Return 'count' integers between lower and upper inclusive with a uniform distribution.
"get_uniform_real" | [count(uint), lower(int), upper(int)] | Return 'count' doubles between lower and upper inclusive with a uniform distribution.
"get_weighted_integer" | [count(uint), [weights(double)]] | Return 'count' integers between 0 and length([weights]) inclusive with a weighted distribution as described by [weights].

#### Quantum System Control
method | parameters | description
--- | --- | ---
"create_system" | [stateType(str)] | Create a quantum system using 'stateType' as the state storage model and return a uinteger Id of the system.
"delete_system" | [systemId(uint)] | Delete a quantum system with an id of 'systemId' returned by the original call to "create_system." Returns a bool indicating success.

#### Quantum State Control
method | parameters | description
--- | --- | ---
"create_state" | [systemId(uint), dialect(str), program(str), lineDelimiter(uint*)] | Create a state within the quantum system of id 'systemId' using a program to dictate initialization operators. Returns a uinteger Id of the state.
"delete_state" | [systemId(uint), stateId(uint)] | Delete a state with an id of 'stateId' within a system of id 'systemId' both originally returned by the original create functions. Returns a bool indicating success.
"modify_state" | [systemId(uint), stateId(uint) dialect(str), program(str), lineDelimiter(uint*)] | Modify a state with an id of 'stateId' within a quantum system of id 'systemId' using program to dictate modification operators. Returns a bool indicating success.
"measure_state" | [systemId(uint), stateId(uint), dialect(str), program(str), lineDelimiter(uint*)] | Measure a state with an id of 'stateId' within a quantum system of id 'systemId' using program to dictate measurement operators. Returns the measurement result in a format determined by the stateType of the system.

#### Computation
method | parameters | description
--- | --- | ---
"compute_result" | [systemId(uint), dialect(str), program(str), lineDelimiter(uint*)] | Create and perform measurement on a quantum state within the quantum system of id 'systemId' using a program to dictate initialization and measurement operators. This state is NOT stored but is discarded after return. Returns the measurement result in a format determined by the stateType of the system.

*Internally, this number is used as an ASCII character.

## Quantum Circuit Descriptions
To create, modify, and measure quantum states, we use a language based on [QASM](https://www.media.mit.edu/quanta/qasm2circ/). Currently, not all the features of QASM are supported, and it's really up to the state model in use to implement individual instructions.

### Lexical Structure

Statements are made of a single opcode mnemonic along with one or more operands. The opcode is seperated from the operands with a space or tab, while each operand is seperated with a comma. Comments are denoted with a semicolon and all text after a semicolon will be ignored.

Below is the gist of this structure.
```assembly
; Comment

opcode operand ; Comment
opcode operand, operand	; Comment
```

### Dialects

A dialect is a particular version of the language that describes valid operations and parameters. For CHP, the default used is *chpext*. Since CHP is the only state type currently supported, the following is valid for CHP.

#### chpext

The original operations present in CHP are included with some modifications, along with new ones to help describe more complex circuits in less operations.

mnemonic | operand count | description
--- | --- | ---
init | 1 | initialize n qubits
i | 1 | Apply identity gate to qubit *x*
h | 1 | Apply hadamard gate to qubit *x*
p | 1 | Apply phase gate to qubit *x*
c | 2 | Apply cnot gate to qubit *y* with control qubit *x* 
x | 1 | Apply x gate to qubit *x*
y | 1 | Apply y gate to qubit *x*
z | 1 | Apply z gate to qubit *x*
m | 1 | Measure qubit *x* in computational basis and return result

##### A Note About Initialization
A circuit should only initialize qubits once and should be the first statement in a circuit description.

##### A Note About Measurement
You may have noticed that there are 3 circumstances where a circuit may be supplied to alter a state: *create_state*, *modify_state*, *measure_state*. While measurement is valid in all three cases, you may only retrieve measurement results when calling *measure_state* although the effect of measurement is applied in each case.

Measurements will be returned in a buffer with each measurement result as a character in the order they are executed. For example, a call to *measure_state* with program
```assembly
x 0
m 0
m 1
```

with an initial state of |00> will return a string "10" in the buffer. Notice the buffer fills left to right.

##### Circuit Macros
Todo

##### Circuit Description Examples
Prepare 1 qubit in a superposition and perform a measurement:
```assembly
init 1
h 0
m 0
```

Prepare 2 qubits in the Bell basis:
```assembly
init 2
h 0
c 0, 1
```

A demonstration of [superdense coding](https://journals.aps.org/prl/abstract/10.1103/PhysRevLett.69.2881):
```assembly
init 2
h 0
c 0, 1
x 0; Encode the classical bits 10 (big-endian)
c 0, 1
h 0
m 0
m 1
```

Using the CHP state model allows us to simulate quantum states of enormous sizes. The following initializes 50000 qubits and will use roughly 2.5GB of RAM, so this example should run on most modern laptops without difficulty:
```assembly
init 50000
; Operations on those qubits here...
```

## Programming

### Style
Variables use camelCase while everything else use underscore_seperated_words.

### State Model

#### Adding a State Model
State models make clever use of statics and templates to self-register. A child of state_type should self register with something like
```cpp
const state_type_register<class> class::child_register(name);
```

### Server

### Pseudo-Random Number Generator
Almost all generators within boost::random are available for use ([here is a list of them](https://www.boost.org/doc/libs/1_60_0/doc/html/boost_random/reference.html#boost_random.reference.generators)). The default is boost::mt11213b, which is a specialization of the Mersenne Twister and is described as being a good uniform distribution in up to 350 dimensions in addition to being one of the fastest generators within boost. Changing the generator in use is fairly simple, take a look at generator.h. You'll see gen_type containing all the possible generators and also see where to change which of these the generator class uses.
