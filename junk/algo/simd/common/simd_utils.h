/**
 * simd_utils.h - Utility functions and macros for SIMD programming
 *
 * This header provides common utilities for SIMD programming, including:
 * - Type definitions for SIMD vectors
 * - Helper macros for alignment
 * - Utility functions for printing SIMD vectors
 * - Performance measurement utilities
 */

#ifndef SIMD_UTILS_H
#define SIMD_UTILS_H

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <immintrin.h> // AVX2, 256-bit operations
#include <iomanip>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>

// Alignment macros
#define SIMD_ALIGN_32 alignas(32)
#define SIMD_ALIGN_64 alignas(64)

// Helper union for accessing SIMD vector elements
union float8 {
  __m256 v;
  float a[8];

  float8(__m256 _v) : v(_v) {}
  float8() : v(_mm256_setzero_ps()) {}
};

union double4 {
  __m256d v;
  double a[4];

  double4(__m256d _v) : v(_v) {}
  double4() : v(_mm256_setzero_pd()) {}
};

union int8 {
  __m256i v;
  int a[8];

  int8(__m256i _v) : v(_v) {}
  int8() : v(_mm256_setzero_si256()) {}
};

// Print utilities
inline void print_m256(const __m256 &v, const std::string &label = "") {
  float8 tmp(v);
  if (!label.empty()) {
    std::cout << label << ": ";
  }
  std::cout << "[";
  for (int i = 0; i < 7; i++) {
    std::cout << tmp.a[i] << ", ";
  }
  std::cout << tmp.a[7] << "]" << std::endl;
}

inline void print_m256d(const __m256d &v, const std::string &label = "") {
  double4 tmp(v);
  if (!label.empty()) {
    std::cout << label << ": ";
  }
  std::cout << "[";
  for (int i = 0; i < 3; i++) {
    std::cout << tmp.a[i] << ", ";
  }
  std::cout << tmp.a[3] << "]" << std::endl;
}

inline void print_m256i(const __m256i &v, const std::string &label = "") {
  int8 tmp(v);
  if (!label.empty()) {
    std::cout << label << ": ";
  }
  std::cout << "[";
  for (int i = 0; i < 7; i++) {
    std::cout << tmp.a[i] << ", ";
  }
  std::cout << tmp.a[7] << "]" << std::endl;
}

// Performance measurement utilities
class Timer {
private:
  std::chrono::high_resolution_clock::time_point start_time;
  std::string label;

public:
  Timer(const std::string &_label = "Operation") : label(_label) {
    start_time = std::chrono::high_resolution_clock::now();
  }

  ~Timer() {
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        end_time - start_time);
    std::cout << label << " took " << duration.count() << " microseconds"
              << std::endl;
  }
};

namespace simd_bench_detail {

inline std::string &suite_label() {
  static std::string label = "unspecified_suite";
  return label;
}

inline std::string &csv_path_store() {
  static std::string path;
  return path;
}

inline std::mutex &csv_mutex() {
  static std::mutex m;
  return m;
}

inline std::string csv_escape(const std::string &input) {
  std::string out;
  out.reserve(input.size() + 2);
  out.push_back('"');
  for (char ch : input) {
    if (ch == '"') {
      out.push_back('"');
      out.push_back('"');
    } else {
      out.push_back(ch);
    }
  }
  out.push_back('"');
  return out;
}

inline std::string effective_csv_path() {
  const char *env_path = std::getenv("SIMD_BENCHMARK_CSV");
  if (env_path && *env_path) {
    return std::string(env_path);
  }
  return csv_path_store();
}

} // namespace simd_bench_detail

inline void set_benchmark_suite(const std::string &suite) {
  simd_bench_detail::suite_label() = suite;
}

inline void set_benchmark_csv_path(const std::string &path) {
  simd_bench_detail::csv_path_store() = path;
}

// Benchmark function to compare scalar vs SIMD implementations
template <typename ScalarFunc, typename SimdFunc>
void benchmark_comparison(const std::string &label, ScalarFunc scalar_func,
                          SimdFunc simd_func, int iterations = 1000) {
  // Warm-up
  scalar_func();
  simd_func();

  // Benchmark scalar implementation
  auto scalar_start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < iterations; i++) {
    scalar_func();
  }
  auto scalar_end = std::chrono::high_resolution_clock::now();
  auto scalar_duration = std::chrono::duration_cast<std::chrono::microseconds>(
      scalar_end - scalar_start);

  // Benchmark SIMD implementation
  auto simd_start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < iterations; i++) {
    simd_func();
  }
  auto simd_end = std::chrono::high_resolution_clock::now();
  auto simd_duration = std::chrono::duration_cast<std::chrono::microseconds>(
      simd_end - simd_start);

  // Print results
  std::cout << "===== " << label << " Benchmark =====" << std::endl;
  std::cout << "Scalar implementation: " << scalar_duration.count()
            << " microseconds" << std::endl;
  std::cout << "SIMD implementation: " << simd_duration.count()
            << " microseconds" << std::endl;

  double speedup =
      static_cast<double>(scalar_duration.count()) / simd_duration.count();
  std::cout << "Speedup: " << std::fixed << std::setprecision(2) << speedup
            << "x" << std::endl;
  std::cout << "===============================" << std::endl;

  const std::string csv_path = simd_bench_detail::effective_csv_path();
  if (!csv_path.empty()) {
    std::lock_guard<std::mutex> lock(simd_bench_detail::csv_mutex());
    bool need_header = false;
    {
      std::ifstream existing(csv_path);
      if (!existing.good() ||
          existing.peek() == std::ifstream::traits_type::eof()) {
        need_header = true;
      }
    }

    std::ofstream csv(csv_path, std::ios::app);
    if (csv) {
      if (need_header) {
        csv << "suite,label,iterations,scalar_us,simd_us,speedup" << '\n';
      }
      csv << simd_bench_detail::csv_escape(simd_bench_detail::suite_label())
          << ',' << simd_bench_detail::csv_escape(label) << ',' << iterations
          << ',' << scalar_duration.count() << ',' << simd_duration.count()
          << ',' << std::setprecision(6) << speedup << '\n';
    } else {
      std::cerr << "Failed to write benchmark CSV at " << csv_path << std::endl;
    }
  }
}

// Allocate aligned memory
template <typename T> T *aligned_alloc(size_t size, size_t alignment = 32) {
  void *ptr = nullptr;
  if (posix_memalign(&ptr, alignment, size * sizeof(T)) != 0) {
    throw std::bad_alloc();
  }
  return static_cast<T *>(ptr);
}

#endif // SIMD_UTILS_H