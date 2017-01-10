#ifndef _STATE_CHP_STATE_HPP
#define _STATE_CHP_STATE_HPP

#include <common.hpp>
#include "quantum_state.hpp"
#include "../universe.hpp"

#define INITIALIZE	0 // The index in ../language/dialect/chpext.cc
#define IDENTITY	1 // "
#define HADAMARD	2 // "
#define PHASE		3 // "
#define MEASURE		4 // "
#define CNOT		5 // "
#define PAULIX		6 // "
#define PAULIY		7 // "
#define PAULIZ		8 // "

namespace state {
	/**
	 * \brief The underlying state object derived from the CHP source.
	 * 
	 * Based on CHP: CNOT-Hadamard-Phase by Scott Aaronson.
	 * Using modified CHP from Megan Lilly
	 */
	struct _chp_state {
		/**
		 * \brief Constructor.
		 */
		_chp_state() : n(0), x(0), z(0), r(0), over32(0) {
		}
		
		/**
		* \brief Copy constructor is disabled.
		*/
		_chp_state(const _chp_state&) = delete;
		
		/**
		 * \brief Move constructor.
		 */
		_chp_state(_chp_state&& old) {
			n = old.n;
			x = old.x;
			old.x = 0;
			z = old.z;
			old.z = 0;
			r = old.r;
			old.r = 0;
			over32 = old.over32;
		}
		
		/**
		 * \brief Copy assignment operator is disabled.
		 */
		_chp_state& operator=(const _chp_state&) = delete;
		
		/**
		 * \brief Move assignment operator.
		 */
		_chp_state& operator=(_chp_state& old) {
			n = old.n;
			x = old.x;
			old.x = 0;
			z = old.z;
			old.z = 0;
			r = old.r;
			old.r = 0;
			over32 = old.over32;
			return *this;
		}
		
		/**
		 * \brief Destructor.
		 */
		~_chp_state() {
			if(x != 0) {
				if(x[0] != 0) {
					// Because of how we allocated memory initially, this deletes all the
					// entries of x.
					delete[] x[0];
				}
				
				delete[] x;
			}
			if(z != 0) {
				if(z[0] != 0) {
					// Because of how we allocated memory initially, this deletes all the
					// entries of x.
					delete[] z[0];
				}
				
				delete[] z;
			}
			if(r != 0) {
				delete[] r;
			}
		}
		
		// Number of qubits
		std::size_t n;
		
		// (2n+1)*n matrix for stabilizer/destabilizer x bits
		// There's one row at the bottom that's "scratch space"
		std::int_fast64_t **x;
		
		// (2n+1)*n matrix for z bits
		std::int_fast64_t **z;
		
		// Phase bits: 0 for +1, 1 for i, 2 for -1, 3 for -i
		// Normally either 0 or 2.
		std::int_fast64_t *r;
		
		// floor(n/8)+1
		std::uint_fast64_t over32;
	};
	
	/**
	 * \brief A state based on CHP.
	 * 
	 * Based on CHP: CNOT-Hadamard-Phase by Scott Aaronson.
	 * Using modified CHP from Megan Lilly
	 */
	class chp_state : public quantum_state<_chp_state> {
	 public:
		chp_state();
		~chp_state();
		
	 protected:
		_chp_state encode_state(const ::language::program& program);
		bool modify_state(_chp_state& state, const ::language::program& program);
		bool measure_state(_chp_state& state, const ::language::program& program, string_buffer<>& buffer);
		bool compute_result(const ::language::program& program, string_buffer<>& buffer);
	
	 private:
		static const char* name;
		static const state_type_register<chp_state> child_register;
		const char* instructionLookup[9];
		
		/**
		 * \brief Helper function to calculate 2^i.
		 * 
		 * Reduces code clutter and reduces the load on the memory because 2^i is only
		 * a bitshift that can be executed by the modern CPU very quickly in a very small
		 * number of clock cycles. Originally this was an array but that approach requires
		 * access to memory or in the best case access to the CPU cache.
		 */
		inline std::uint_fast64_t pt(const std::uint_fast64_t i) const {
			return 1 << i;
		}
				
		/**
		 * \brief Apply a CNOT gate to two qubits.
		 * 
		 * \note Adapted from CHP.
		 */
		inline void cnot(_chp_state& state, const std::size_t control, const std::size_t target) {
			for(std::size_t i = 0; i < 2*state.n; i++) {
				if(state.x[i][control>>5]&pt(control&0x1F)) {
					 state.x[i][target>>5] ^= pt(target&0x1F);
				}
				if(state.z[i][target>>5]&pt(target&0x1F)) {
					state.z[i][control>>5] ^= pt(control&0x1F);
				}
				if((state.x[i][control>>5]&pt(control&0x1F)) && (state.z[i][target>>5]&pt(target&0x1F))
						&& (state.x[i][target>>5]&pt(target&0x1F)) && (state.z[i][control>>5]&pt(control&0x1F))) {
					state.r[i] = (state.r[i]+2)%4;
				}
				if ((state.x[i][control>>5]&pt(control&0x1F)) && (state.z[i][target>>5]&pt(target&0x1F)) &&
						!(state.x[i][target>>5]&pt(target&0x1F)) && !(state.z[i][control>>5]&pt(control&0x1F))) {
					state.r[i] = (state.r[i]+2)%4;
				}
			}
		}
		
		/**
		 * \brief Apply a hadamard gate a qubit.
		 * 
		 * \note Adapted from CHP.
		 */
		inline void hadamard(_chp_state& state, const std::size_t qubit) {
			for(std::size_t i = 0; i < 2*state.n; i++) {
				auto tmp(state.x[i][qubit>>5]);
				state.x[i][qubit>>5] ^= (state.x[i][qubit>>5] ^ state.z[i][qubit>>5]) & pt(qubit&0x1F);
				state.z[i][qubit>>5] ^= (state.z[i][qubit>>5] ^ tmp) & pt(qubit&0x1F);
				if((state.x[i][qubit>>5]&pt(qubit&0x1F)) && (state.z[i][qubit>>5]&pt(qubit&0x1F))) {
					state.r[i] = (state.r[i]+2)%4;
				}
			}
		}
		
		/**
		 * \brief Apply a phase gate to a qubit.
		 * 
		 * \note Adapted from CHP.
		 */
		inline void phase(_chp_state& state, const std::size_t qubit) {
			for(std::size_t i = 0; i < 2*state.n; i++) {
				if((state.x[i][qubit>>5]&pt(qubit&0x1F)) && (state.z[i][qubit>>5]&pt(qubit&0x1F))) {
					state.r[i] = (state.r[i]+2)%4;
				}
				state.z[i][qubit>>5] ^= state.x[i][qubit>>5]&pt(qubit&0x1F);
			}
		}
		
		/**
		 * \brief Copy one row into another.
		 * 
		 * \note Adapted from CHP.
		 */
		inline void rowcopy(_chp_state& state, const std::size_t to, const std::size_t from) {
			for(std::size_t i = 0; i < state.over32; i++) {
				state.x[to][i] = state.x[from][i];
				state.z[to][i] = state.z[from][i];
			}
			state.r[to] = state.r[from];
		}
		
		/**
		 * \brief Swap two rows.
		 * 
		 * \note Adapted from CHP.
		 */
		inline void rowswap(_chp_state& state, const std::size_t a, const std::size_t b) {
			rowcopy(state, 2*state.n, b);
			rowcopy(state, b, a);
			rowcopy(state, a, 2*state.n);
		}
		
		/**
		 * \brief Sets a row equal to the given observable (X_1,...X_n,Z_1,...,Z_n).
		 * 
		 * \note Adapted from CHP.
		 */
		inline void rowset(_chp_state& state, const std::size_t row, const std::size_t observable) {
			for(std::size_t j = 0; j < state.over32; j++) {
				state.x[row][j] = 0;
				state.z[row][j] = 0;
			}
			state.r[row] = 0;
			if(observable < state.n) {
				state.x[row][observable>>5] = pt(observable&0x1F);
			} else {
				state.z[row][(observable - state.n)>>5] = pt((observable - state.n)&0x1F);
			}
		}
		
		/**
		 * \brief Return the phase (0,1,2,3) when row i is LEFT-multiplied by row k.
		 * 
		 * \note Adapted from CHP.
		 */
		inline int clifford(_chp_state& state, const std::int_fast64_t a, const std::int_fast64_t b) {
			// Power to which i is raised
			std::int_fast64_t e(0);
			
			for(std::size_t i = 0; i < state.over32; i++) {
				for(std::size_t j = 0; j < 32; j++) {
					if((state.x[b][i]&pt(j)) && (!(state.z[b][i]&pt(j)))) {
						// X
						if((state.x[a][i]&pt(j)) && (state.z[a][i]&pt(j))) {
							// XY=iZ
							e++;
						}
						if((!(state.x[a][i]&pt(j))) && (state.z[a][i]&pt(j))) {
							// XZ=-iY
							e--;
						}
					}
					if((state.x[b][i]&pt(j)) && (state.z[b][i]&pt(j))) {
						// Y
						if((!(state.x[a][i]&pt(j))) && (state.z[a][i]&pt(j))) {
							// YZ=iX
							e++;
						}
						if((state.x[a][i]&pt(j)) && (!(state.z[a][i]&pt(j)))) {
							// YX=-iZ
							e--;
						}
					}
					if((!(state.x[b][i]&pt(j))) && (state.z[b][i]&pt(j))) {
						// Z
						if((state.x[a][i]&pt(j)) && (!(state.z[a][i]&pt(j)))) {
							// ZX=iY
							e++;
						}
						if((state.x[a][i]&pt(j)) && (state.z[a][i]&pt(j))) {
							// ZY=-iX
							e--;
						}
					}
				}
			}
			
			e = (e+state.r[a]+state.r[b])%4;
			if(e >= 0) {
				return e;
			} else {
				return e+4;
			}
		}
		
		/**
		 * \brief Left-multiply row i by row j.
		 * 
		 * \note Adapted from CHP.
		 */
		inline void rowmult(_chp_state& state, const std::size_t a, const std::size_t b) {
			state.r[a] = clifford(state, a, b);
			for(std::size_t i = 0; i < state.over32; i++) {
				state.x[a][i] ^= state.x[b][i];
				state.z[a][i] ^= state.z[b][i];
			}
		}
		
		/**
		 * \brief Do Gaussian elimination.
		 * 
		 * Do Gaussian elimination to put the stabilizer generators in the following form:
		 * At the top, a minimal set of generators containing X's and Y's, in "quasi-upper-triangular" form.
		 * (Return value = number of such generators = log_2 of number of nonzero basis states)
		 * At the bottom, generators containing Z's only in quasi-upper-triangular form.
		 * 
		 * \note Adapted from CHP.
		 */
		inline std::int_fast64_t gaussian(_chp_state& state) {
			std::int_fast64_t i(state.n);
			
			for(std::size_t j = 0; j < state.n; j++) {
				// Find a generator containing X in jth column
				std::size_t k(i);
				while(k < 2*state.n) {
					if(state.x[k][j>>5]&pt(j&0x1F)) {
						break;
					}
					k++;
				}
				if(k < 2*state.n) {
					rowswap(state, i, k);
					rowswap(state, i-state.n, k-state.n);
					for(std::size_t k2 = i+1; k2 < 2*state.n; k2++) {
							if(state.x[k2][j>>5]&pt(j&0x1F)) {
								// Gaussian elimination step
								rowmult(state, k2, i);
								rowmult(state, i-state.n, k2-state.n);
							}
					}
					i++;
				}
			}
			// Return value
			std::int_fast64_t g(i - state.n);
			
			for(std::size_t j = 0; j < state.n; j++) {
				// Find a generator containing Z in jth column
				std::size_t k(i);
				while(k < 2*state.n) {
					if(state.z[k][j>>5]&pt(j&0x1F)) {
						break;
					}
					k++;
				}
				if(k < 2*state.n) {
					rowswap(state, i, k);
					rowswap(state, i-state.n, k-state.n);
					for(std::size_t k2 = i+1; k2 < 2*state.n; k2++) {
						if(state.z[k2][j>>5]&pt(j&0x1F)) {
							rowmult(state, k2, i);
							rowmult(state, i-state.n, k2-state.n);
						}
					}
					i++;
				}
			}
			
			return g;
		}
		
		/**
		 * \brief Finds a Pauli operator P such that the basis state P|0...0>
		 * occurs with nonzero amplitude in q, and writes P to the scratch space of q.
		 * 
		 * For this to work, Gaussian elimination must already have been
		 * performed on q.  g is the return value from gaussian(q).
		 * 
		 * \note Adapted from CHP.
		 */
		inline void seed(_chp_state& state, const std::int_fast64_t g) {
			std::int_fast64_t min(0);
			state.r[2*state.n] = 0;
			for(std::size_t j = 0; j < state.over32; j++) {
				// Wipe the scratch space clean
				state.x[2*state.n][j] = 0;
				state.z[2*state.n][j] = 0;
			}
			for(std::size_t i = 2*state.n - 1; i >= state.n + g; i--) {
				std::int_fast64_t f(state.r[i]);
				for(int j = state.n - 1; j >= 0; j--) {
					if(state.z[i][j>>5]&pt(j&0x1F)) {
						min = j;
						if(state.x[2*state.n][j>>5]&pt(j&0x1F)) {
							f = (f+2)%4;
						}
					}
				}
				if(f == 2) {
					// Make the seed consistent with the ith equation
					state.x[2*state.n][min>>5] ^= pt(min&0x1F);
				}
			}
		}
		
		/**
		 * \brief Measure qubit.
		 * 
		 * \note Adapted from CHP.
		 */
		inline char measure(_chp_state& state, const std::size_t qubit) {
			bool isRandom(false);
			// Pivot row in stabilizer
			std::size_t p(0);
			
			// loop over stabilizer generators
			while(p < state.n) {
				// if a Zbar does NOT commute with Z_b (the operator being measured)
				// then outcome is random
				if(state.x[p+state.n][qubit>>5]&pt(qubit&0x1F)) {
					isRandom = true;
					break;
				}
				p++;
			}
			if(isRandom) {
				// Set Xbar_p := Zbar_p
				rowcopy(state, p, p + state.n);
				// Set Zbar_p := Z_b
				rowset(state, p + state.n, qubit + state.n);
				// Moment of quantum randomness
				state.r[p + state.n] = 2*(::universe::here().random_generator().get_uniform_integer(0, 1));
				// Update the Xbar's and Zbar's that don't commute with Z_b
				for(std::size_t i = 0; i < 2*state.n; i++) {
					if((i!=p) && (state.x[i][qubit>>5]&pt(qubit&0x1F))) {
						rowmult(state, i, p);
					}
				}
				if(state.r[p + state.n]) {
					return 1;
				} else {
					return 0;
				}
			} else {
				// Pivot row in destabilizer
				std::size_t m(0);
				// Before we were checking if stabilizer generators commute
				// with Z_b; now we're checking destabilizer generators
				while(m < state.n) {
					if(state.x[m][qubit>>5]&pt(qubit&0x1F)) {
						break;
					}
					m++;
				}
				rowcopy(state, 2*state.n, m + state.n);
				for(std::size_t i = m+1; i < state.n; i++) {
					if(state.x[i][qubit>>5]&pt(qubit&0x1F)) {
						rowmult(state, 2*state.n, i + state.n);
					}
				}
				if(state.r[2*state.n]) {
					return 1;
				} else {
					return 0;
				}
			}
		}
		
		/**
		 * \brief Initialize state q to have n qubits.
		 * 
		 * \note Adapted from CHP.
		 */
		inline void init_state(_chp_state& state, const std::size_t n) {
			std::int_fast64_t* xchunk = 0;
			std::int_fast64_t* zchunk = 0;
			try {
				state.n = n;
				state.over32 = (state.n>>5) + 1;
				state.x = new std::int_fast64_t*[2*state.n + 1];
				state.z = new std::int_fast64_t*[2*state.n + 1];
				state.r = new std::int_fast64_t[2*state.n + 1];
				
				// For large n, allocating all of the memory at once instead of many smaller
				// allocations becomes a little better.
				// The effect of this logic is that state.{x,z}[0] = {x,z}chunk. Thus we
				// only need to call delete[] state.{x,z}[0] to deallocate the entire array.
				xchunk = new std::int_fast64_t[(2*state.n + 1)*state.over32];
				zchunk = new std::int_fast64_t[(2*state.n + 1)*state.over32];
				memset(xchunk, 0, (2*state.n + 1)*state.over32*sizeof(std::int_fast64_t));
				memset(zchunk, 0, (2*state.n + 1)*state.over32*sizeof(std::int_fast64_t));
				
				memset(state.r, 0, 2*state.n*sizeof(std::int_fast64_t));
				
				for(std::size_t i = 0; i < 2*state.n + 1; i++) {
					state.x[i] = &xchunk[i*state.over32];
					state.z[i] = &zchunk[i*state.over32];
					if(i < state.n) {
						state.x[i][i>>5] = pt(i&0x1F);
					} else {
						state.z[i][(i-state.n)>>5] = pt((i-state.n)&0x1F);
					}
				}
			} catch(const std::bad_alloc& e) {
				if(state.x != 0) {
					delete[] state.x;
				}
				if(state.z != 0) {
					delete[] state.z;
				}
				if(state.r != 0) {
					delete[] state.r;
				}
				if(xchunk != 0) {
					delete[] xchunk;
				}
				if(zchunk != 0) {
					delete[] zchunk;
				}
				throw std::bad_alloc();
			}
		}
		
		/**
		 * \brief Run a program on a state with a given program and write any results to a buffer.
		 */
		inline bool run_program(_chp_state& state, const ::language::program& program, string_buffer<>& buffer) {
			for(std::size_t t = 0; t < program.size(); t++) {
				const auto& instruction = program.instruction(t);
				
				switch(instruction.bytecode) {
				 case ::machine::operation::macro:
				 {
					auto& p = ::universe::here().get_macro(instruction.operands[0], instruction.operands[1]);
					run_program(state, p, buffer);
					break;
				 }
				 case ::machine::operation::initialize:
				 {
					try {
						init_state(state, instruction.operands[0]);
					} catch(const std::bad_alloc& e) {
						std::cerr << "state initialize failed";
						return false;
					}
					break;
				 }
				 case ::machine::operation::identity:
				 {
					break;
				 }
				 case ::machine::operation::hadamard:
				 {
					hadamard(state, instruction.operands[0]);
					break;
				 }
				 case ::machine::operation::phase:
				 {
					phase(state, instruction.operands[0]);
					break;
				 }
				 case ::machine::operation::measure:
				 {
					char tempMeasurement = measure(state, instruction.operands[0])+'0';
					buffer.add(&tempMeasurement, 1);
					break;
				 }
				 case ::machine::operation::cnot:
				 {
					cnot(state, instruction.operands[0], instruction.operands[1]);
					break;
				 }
				 case ::machine::operation::paulix:
				 {
					hadamard(state, instruction.operands[0]);
					phase(state, instruction.operands[0]);
					phase(state, instruction.operands[0]);
					hadamard(state, instruction.operands[0]);
					break;
				 }
				 case ::machine::operation::pauliy:
				 {
					phase(state, instruction.operands[0]);
					hadamard(state, instruction.operands[0]);
					phase(state, instruction.operands[0]);
					phase(state, instruction.operands[0]);
					hadamard(state, instruction.operands[0]);
					phase(state, instruction.operands[0]);
					phase(state, instruction.operands[0]);
					phase(state, instruction.operands[0]);
					break;
				 }
				 case ::machine::operation::pauliz:
				 {
					phase(state, instruction.operands[0]);
					phase(state, instruction.operands[0]);
					break;
				 }
				}
			}
			return true;
		}
		
		/**
		 * \brief Run a program on a state with a given program.
		 * 
		 * \note Overload simply calls original function with a null buffer.
		 */
		inline bool run_program(_chp_state& state, const ::language::program& program) {
			string_buffer<> empty;
			return run_program(state, program, empty);
		}
	};
}

#endif
