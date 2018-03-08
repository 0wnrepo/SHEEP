#include <memory>

#include <cstdint>
#include <cassert>
#include <algorithm>
#include "context-helib.hpp"
#include "circuit-repo.hpp"
#include "circuit-test-util.hpp"

typedef std::chrono::duration<double, std::micro> DurationT;

int main(void) {
	using namespace Sheep::HElib;

    //// instantiate the Circuit Repository
	CircuitRepo cr;

	Circuit circ = cr.create_circuit(Gate::Multiply, 1);
	std::cout << circ;
	std::vector<DurationT> durations;
	ContextHElib<bool> ctx;

	/// test two 1s
	std::list<bool> inputs = {1, 1};
	std::list<bool> result = ctx.eval_with_plaintexts(circ, inputs, durations);
	assert(result.front() == 1);
	std::cout<<"  1 * 1 = "<<std::to_string(result.front())<<std::endl;	
	/// test one of each
	inputs = {0, 1};
	result = ctx.eval_with_plaintexts(circ, inputs, durations);
	assert(result.front() == 0);
	std::cout<<" 0 * 1 = "<<std::to_string(result.front())<<std::endl;
	/// test both zeros
	inputs = {0, 0};
	result = ctx.eval_with_plaintexts(circ, inputs, durations);
	std::cout<<" 0 * 0 = "<<std::to_string(result.front())<<std::endl;
	assert(result.front() == 0);

}