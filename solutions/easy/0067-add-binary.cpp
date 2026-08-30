// 67. Add Binary (Easy)
// https://leetcode.com/problems/add-binary/

#include <iostream>
#include <string>

class Solution {
 public:
  std::string addBinary(std::string a, std::string b) {
    // Trim leading zeros
    a = trimLeadingZero(a);
    b = trimLeadingZero(b);

    int n = a.size();
    int m = b.size();

    // swap the strings if a is smaller length
    if (n < m) {
      return addBinary(b, a);
    }

    int j = m - 1;
    int carry = 0;

    // Traverse both strings from the end
    for (int i = n - 1; i >= 0; i--) {
      // Current bit of a
      int bit1 = a[i] - '0';
      int sum = bit1 + carry;

      // If there are remaining bits in b, add them to the sum
      if (j >= 0) {
        // Current bit of b
        int bit2 = b[j] - '0';
        sum += bit2;
        j--;
      }

      // Calculate the result bit and update carry
      int bit = sum % 2;
      carry = sum / 2;

      // Update the current bit in a
      a[i] = (char)(bit + '0');
    }

    // If there's any carry left, update a
    if (carry > 0) {
      a = '1' + a;
    }

    return a;
  }

 private:
  // Function to trim leading zeros from a binary string
  std::string trimLeadingZero(const std::string& s) {
    // Find the position of the first '1'
    size_t firstOne = s.find('1');

    return (firstOne == std::string::npos) ? "0" : s.substr(firstOne);
  }
};

void check(std::string a, std::string b, std::string expected) {
  Solution sol;
  std::string got = sol.addBinary(a, b);

  std::cout << (got == expected ? "PASS" : "FAIL") << "  a=" << a << "  b=" << b
            << "  expected=" << expected << "  got=" << got << "\n"
            << std::string(60, '-') << "\n";
}

int main() {
  // --- the LeetCode examples ---
  check("11", "1", "100");
  check("1010", "1011", "10101");

  // --- zero handling ---
  check("0", "0", "0");
  check("0", "1", "1");
  check("1", "0", "1");

  // --- single bits, with and without carry ---
  check("1", "1", "10");

  // --- carry propagating all the way through ---
  check("111", "1", "1000");
  check("1111", "1111", "11110");
  check("11111111", "1", "100000000");

  // --- no carry at all ---
  check("1010", "0101", "1111");
  check("100", "011", "111");

  // --- carry in the middle only ---
  check("1101", "10", "1111");
  check("1011", "100", "1111");

  // --- very different lengths, both orders (exercises the swap) ---
  check("1", "1111111111", "10000000000");
  check("1111111111", "1", "10000000000");
  check("1", "10000000000", "10000000001");
  check("101010101010", "11", "101010101101");

  // --- leading zeros in the input, which trimLeadingZero must strip ---
  check("0011", "0001", "100");
  check("000", "000", "0");
  check("0000101", "0", "101");
  check("0", "0000101", "101");
  check("0001", "0001", "10");

  // --- longer operands ---
  check("110010101101", "101101010011", "1100000000000");
  check("10000000000000000000", "1", "10000000000000000001");

  // --- 100-bit operands, past what a 64-bit integer could hold ---
  check(std::string(100, '1'), "1", "1" + std::string(100, '0'));
  check(std::string(100, '1'), std::string(100, '1'),
        std::string(100, '1') + "0");

  return 0;
}
