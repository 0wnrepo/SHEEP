#include <memory>

#include <cstdint>
#include <cassert>
#include <algorithm>
#include "context-helib.hpp"
#include "circuit-repo.hpp"
#include "circuit-test-util.hpp"


int main(void) {

  using namespace Sheep::HElib;
  
	ContextHElib_F2<int8_t> ctx;

	/// test small postitive numbers
	ContextHElib_F2<int8_t>::Plaintext pt_input = 15;
	ContextHElib_F2<int8_t>::Ciphertext ct = ctx.encrypt(pt_input);
	long const_val = 22;
	ContextHElib_F2<int8_t>::Ciphertext ct_out = ctx.AddConstant(ct, const_val);
	ContextHElib_F2<int8_t>::Plaintext pt_out = ctx.decrypt(ct_out);
	std::cout<<"D( E(15) + 22) = "<<std::to_string(pt_out)<<std::endl;
	assert(pt_out == 37);
	/// small negative numbers
	pt_input = 5;
	ct = ctx.encrypt(pt_input);
	const_val = -42;
	ct_out = ctx.AddConstant(ct, const_val);
	pt_out = ctx.decrypt(ct_out);
	std::cout<<"D( E(5) - 42) = "<<std::to_string(pt_out)<<std::endl;
	assert(pt_out == -37);
	/// out-of-range negative
	pt_input = -125;
	ct = ctx.encrypt(pt_input);
	const_val = -121;
	ct_out = ctx.AddConstant(ct, const_val);
	pt_out = ctx.decrypt(ct_out);
	std::cout<<"D( E(-125) - 121) = "<<std::to_string(pt_out)<<std::endl;
	assert(pt_out == 10);
	/// out-of-range positive
	pt_input = 120;
	ct = ctx.encrypt(pt_input);
	const_val = 121;
	ct_out = ctx.AddConstant(ct, const_val);
	pt_out = ctx.decrypt(ct_out);
	std::cout<<"D( E(120) + 121) = "<<std::to_string(pt_out)<<std::endl;
	assert(pt_out == -15);

	
	

}
