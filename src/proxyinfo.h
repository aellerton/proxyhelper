#pragma once
#include <string>

constexpr auto PROXY_INFO_DEFAULT_HOSTNAME = (L"www.microsoft.com");
constexpr auto PROXY_INFO_DEFAULT_PATH = (L"/ms.htm");

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
    std::wstring proxy; // result for the proxy, if any (or empty string if not).
    std::wstring proxyBypass; // result for proxy bypass if any (or empty string).
    bool hasProxy;

    ProxyInfoResult()
        : hostname(PROXY_INFO_DEFAULT_HOSTNAME)
        , path(PROXY_INFO_DEFAULT_PATH)
        , hasProxy(false)
    {}

    ProxyInfoResult(const LPWSTR pHostname, const LPWSTR pPath)
        : hostname(pHostname == nullptr ? PROXY_INFO_DEFAULT_HOSTNAME : pHostname)
        , path(pPath == nullptr ? PROXY_INFO_DEFAULT_PATH : pPath)
        , hasProxy(false)
    {}

    ProxyInfoResult(const std::wstring &hostname, const std::wstring &path)
        : hostname(hostname)
        , path(path)
        , hasProxy(false)
    {}

    // Return full href like "https://www.google.com/foo/bar/"
    std::wstring href() const {
        const std::wstring scheme = L"http://"; // not sure if this needs to be configurable
        return scheme + hostname + path;
    }
};

ProxyInfoResultCode get_proxy_info(ProxyInfoResult * pResult);
std::string get_result_code_text(ProxyInfoResultCode code);

