#include "junk/algo/simd/common/simd_utils.h"

#include <immintrin.h>
#include <iostream>

int main() {
  std::cout << "=== SIMD Header Introduction ===" << std::endl;
  std::cout << "This example demonstrates basic SIMD vector operations."
            << std::endl;
  std::cout << std::endl;

  // Example 1: Basic vector addition with AVX2
  std::cout << "Example 1: Vector Addition" << std::endl;

  // Initialize two SIMD vectors with 8 float values each
  __m256 a = _mm256_set_ps(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
  __m256 b = _mm256_set_ps(8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f);

  // Add the vectors element-wise
  __m256 c = _mm256_add_ps(a, b);

  // Print the vectors using our utility function
  print_m256(a, "Vector A");
  print_m256(b, "Vector B");
  print_m256(c, "A + B");

  // Example 2: Storing SIMD results back to memory
  std::cout << std::endl;
  std::cout << "Example 2: Storing SIMD Results" << std::endl;

  // Allocate aligned memory for results
  float *result = aligned_alloc<float>(8);

  // Store the SIMD vector to memory
  _mm256_store_ps(result, c);

  // Print the results from memory
  std::cout << "Result array: [";
  for (int i = 0; i < 7; i++) {
    std::cout << result[i] << ", ";
  }
  std::cout << result[7] << "]" << std::endl;

  // Example 3: Different data types
  std::cout << std::endl;
  std::cout << "Example 3: Different Data Types" << std::endl;

  // Integer SIMD operations
  __m256i int_a = _mm256_set_epi32(1, 2, 3, 4, 5, 6, 7, 8);
  __m256i int_b = _mm256_set_epi32(8, 7, 6, 5, 4, 3, 2, 1);
  __m256i int_sum = _mm256_add_epi32(int_a, int_b);

  print_m256i(int_a, "Integer Vector A");
  print_m256i(int_b, "Integer Vector B");
  print_m256i(int_sum, "A + B (Integer)");

  // Double precision SIMD operations (4 doubles in a 256-bit register)
  __m256d double_a = _mm256_set_pd(1.0, 2.0, 3.0, 4.0);
  __m256d double_b = _mm256_set_pd(4.0, 3.0, 2.0, 1.0);
  __m256d double_sum = _mm256_add_pd(double_a, double_b);

  print_m256d(double_a, "Double Vector A");
  print_m256d(double_b, "Double Vector B");
  print_m256d(double_sum, "A + B (Double)");

  // Clean up
  free(result);

  return 0;
}
