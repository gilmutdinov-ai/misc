#pragma once

#include <curl/curl.h>
#include <sys/select.h>

#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <string>

namespace misc {

// not thread safe
template <class ODataT> class HttpClientA {
public:
  class JobInfo {
  public:
    JobInfo(ODataT _userdata, const std::string &_postdata);

    ODataT userdata;

    std::string resp;
    bool success;
    std::string postdata;
    std::string curlerror;
  };

  struct HttpSettings {

    std::string user_agent;
    long request_timeout_secs;
    long select_interval_usecs;
  };

private:
  inline static const std::string s_curlopt_encoding{"UTF-8"};

  HttpSettings m_settings;

  std::map<CURL *, JobInfo> m_e_curls;

  CURLM *m_curl;

  std::function<void(HttpClientA::JobInfo _ji)> m_onCalled;

  bool m_iskicking;

  void performTransfers();

public:
  HttpClientA(std::function<void(HttpClientA::JobInfo _ji)> _onCalled,
              HttpSettings _settings);
  ~HttpClientA();

  int getRequestsCount();
  void call(ODataT userdata, const std::string &_url);
  void callPost(ODataT userdata, const std::string &_url,
                const std::string &_postdata);
  void kick();
};

size_t crawl_function_pt(void *ptr, size_t size, size_t nmemb,
                         std::string *stream);
void _http_client_async_onFinished();

#include "http_client.impl"

} // namespace misc
