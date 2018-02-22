#include <cstdint>
#include <cassert>
#include <algorithm>
#include "context-helib.hpp"
#include "simple-circuits.hpp"
#include "circuit-test-util.hpp"

using namespace Sheep::HElib;
typedef ContextHElib<uint8_t>::Plaintext Plaintext;
typedef ContextHElib<uint8_t>::Ciphertext Ciphertext;

// Encrypt a value, decrypt the result, and check that we are
// left with the original value.
void test_single(ContextHElib<uint8_t>& context) {
  Plaintext pt_orig = 243;
  Plaintext pt_new = context.decrypt(context.encrypt(pt_orig));
  assert(pt_orig == pt_new);
}

int main(void) {
  ContextHElib<uint8_t> context(20,80);
  test_single(context);
}