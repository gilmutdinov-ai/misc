#include "http_client.h"

namespace misc {

size_t crawl_function_pt(void *ptr, size_t size, size_t nmemb,
                         std::string *stream) {

  if (size * nmemb == 0)
    return 0;

  char bf[size * nmemb + 1];

  memcpy(bf, ptr, size * nmemb);
  bf[size * nmemb] = '\0';

  *stream += std::string(bf);

  return size * nmemb;
}

void _http_client_async_onFinished() {}

} // namespace misc
