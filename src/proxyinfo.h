#pragma once
#include <string>

constexpr auto PROXY_INFO_DEFAULT_HOSTNAME = (L"www.microsoft.com");
constexpr auto PROXY_INFO_DEFAULT_PATH = (L"/ms.htm");
constexpr auto PROXY_WHEN_NO_PROXY = (L"");

enum ProxyInfoResultCode {
    OK_PROXY_INFO = 0,
    E_FAIL_WIN_HTTP_OPEN,
    E_FAIL_WIN_HTTP_CONNECT,
    E_FAIL_WIN_HTTP_OPEN_REQUEST,
    E_FAIL_WIN_HTTP_SET_OPTION,
    E_FAIL_WIN_HTTP_SEND_REQUEST,
    E_FAIL_WIN_HTTP_RECEIVE_RESPONSE,
};

struct ProxyInfoResult {
    std::wstring hostname; // look up this hostname - just domain name, like "www.microsoft.com"
    std::wstring path; // retrieve this path - just the path, like "/". I think always start with "/" for the href() method.
    std::wstring proxy; // result for the proxy if any, or empty string if not.
    std::wstring proxy1; // the first proxy host if there are more than one in `proxy`.
    std::wstring proxyBypass; // result for proxy bypass if any (or empty string).
    bool hasProxy;

    ProxyInfoResult();

    ProxyInfoResult(const LPWSTR pHostname, const LPWSTR pPath);

    ProxyInfoResult(const std::wstring &hostname, const std::wstring &path);

    // Return full href like "https://www.google.com/foo/bar/"
    std::wstring href() const;
};

ProxyInfoResultCode get_proxy_info(ProxyInfoResult * pResult);
std::string get_result_code_text(ProxyInfoResultCode code);

