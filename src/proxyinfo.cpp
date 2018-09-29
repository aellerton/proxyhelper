#include "pch.h"
#include "proxyinfo.h"
#include <stdio.h>

/**
 * Attempt to detect the proxy, if any. Mutate struct parameter
 * with results and return an enum code indicating success or failure.
 *
 * See see https://docs.microsoft.com/en-us/windows/desktop/winhttp/winhttp-autoproxy-api
 */
ProxyInfoResultCode get_proxy_info(ProxyInfoResult * pResult) {
    HINTERNET hHttpSession = NULL;
    HINTERNET hConnect = NULL;
    HINTERNET hRequest = NULL;

    WINHTTP_AUTOPROXY_OPTIONS  AutoProxyOptions;
    WINHTTP_PROXY_INFO         ProxyInfo;
    DWORD                      cbProxyInfoSize = sizeof(ProxyInfo);

    ZeroMemory(&AutoProxyOptions, sizeof(AutoProxyOptions));
    ZeroMemory(&ProxyInfo, sizeof(ProxyInfo));

    //
    // Create the WinHTTP session.
    //
    hHttpSession = WinHttpOpen(L"ProxyHelper/1.0",
        WINHTTP_ACCESS_TYPE_NO_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS,
        0);

    // Exit if WinHttpOpen failed.
    if (!hHttpSession) {
        return E_FAIL_WIN_HTTP_OPEN;
    }

    //
    // Create the WinHTTP connect handle.
    //
    hConnect = WinHttpConnect(hHttpSession,
        pResult->hostname.c_str(), //e.g.: L"www.microsoft.com",
        INTERNET_DEFAULT_HTTP_PORT,
        0);

    // Exit if WinHttpConnect failed.
    if (!hConnect) {
        return E_FAIL_WIN_HTTP_CONNECT;
    }

    //
    // Create the HTTP request handle.
    //
    hRequest = WinHttpOpenRequest(hConnect,
        L"GET",
        pResult->path.c_str(), // e.g. L"/ms.htm",
        L"HTTP/1.1",
        WINHTTP_NO_REFERER,
        WINHTTP_DEFAULT_ACCEPT_TYPES,
        0);

    // Exit if WinHttpOpenRequest failed.
    if (!hRequest) {
        return E_FAIL_WIN_HTTP_OPEN_REQUEST;
    }

    //
    // Set up the autoproxy call.
    //

    // Use auto-detection because the Proxy 
    // Auto-Config URL is not known.
    AutoProxyOptions.dwFlags = WINHTTP_AUTOPROXY_AUTO_DETECT;

    // Use DHCP and DNS-based auto-detection.
    AutoProxyOptions.dwAutoDetectFlags =
        WINHTTP_AUTO_DETECT_TYPE_DHCP |
        WINHTTP_AUTO_DETECT_TYPE_DNS_A;

    // If obtaining the PAC script requires NTLM/Negotiate
    // authentication, then automatically supply the client
    // domain credentials.
    AutoProxyOptions.fAutoLogonIfChallenged = TRUE;

    //
    // Call WinHttpGetProxyForUrl with our target URL. If 
    // auto-proxy succeeds, then set the proxy info on the 
    // request handle. If auto-proxy fails, ignore the error 
    // and attempt to send the HTTP request directly to the 
    // target server (using the default WINHTTP_ACCESS_TYPE_NO_PROXY 
    // configuration, which the requesthandle will inherit 
    // from the session).
    //
    std::wstring href = pResult->href();
    if (WinHttpGetProxyForUrl(hHttpSession,
        href.c_str(), //e.g.: L"http://www.microsoft.com/ms.htm"
        &AutoProxyOptions,
        &ProxyInfo))
    {
        // A proxy configuration was found, set it on the
        // request handle.
        pResult->hasProxy = true;

        if (!WinHttpSetOption(hRequest,
            WINHTTP_OPTION_PROXY,
            &ProxyInfo,
            cbProxyInfoSize))
        {
            // Exit if setting the proxy info failed.
            //goto Exit;
            return E_FAIL_WIN_HTTP_SET_OPTION;
        }
    }

    //
    // Send the request.
    //
    if (!WinHttpSendRequest(hRequest,
        WINHTTP_NO_ADDITIONAL_HEADERS,
        0,
        WINHTTP_NO_REQUEST_DATA,
        0,
        0,
        NULL))
    {
        return E_FAIL_WIN_HTTP_SEND_REQUEST;
    }

    //
    // Wait for the response.
    //
    if (!WinHttpReceiveResponse(hRequest, NULL))
    {
        return E_FAIL_WIN_HTTP_RECEIVE_RESPONSE;
    }

    //
    // A response has been received, then process it.
    //
    pResult->proxy = ProxyInfo.lpszProxy == nullptr ? L"" : ProxyInfo.lpszProxy;
    pResult->proxyBypass = ProxyInfo.lpszProxyBypass == nullptr ? L"" : ProxyInfo.lpszProxyBypass;

    //
    // Clean up the WINHTTP_PROXY_INFO structure.
    //
    if (ProxyInfo.lpszProxy != NULL) {
        GlobalFree(ProxyInfo.lpszProxy);
    }

    if (ProxyInfo.lpszProxyBypass != NULL) {
        GlobalFree(ProxyInfo.lpszProxyBypass);
    }

    //
    // Close the WinHTTP handles.
    //
    if (hRequest != NULL) {
        WinHttpCloseHandle(hRequest);
    }

    if (hConnect != NULL) {
        WinHttpCloseHandle(hConnect);
    }

    if (hHttpSession != NULL) {
        WinHttpCloseHandle(hHttpSession);
    }

    return OK_PROXY_INFO;
}

std::string get_result_code_text(ProxyInfoResultCode code) {
    switch (code) {
    case OK_PROXY_INFO:
        return "OK";
    case E_FAIL_WIN_HTTP_OPEN:
        return "FAIL_WIN_HTTP_OPEN";
    case E_FAIL_WIN_HTTP_CONNECT:
        return "FAIL_WIN_HTTP_CONNECT";
    case E_FAIL_WIN_HTTP_OPEN_REQUEST:
        return "FAIL_WIN_HTTP_OPEN_REQUEST";
    case E_FAIL_WIN_HTTP_SET_OPTION:
        return "FAIL_WIN_HTTP_SET_OPTION";
    case E_FAIL_WIN_HTTP_SEND_REQUEST:
        return "FAIL_WIN_HTTP_SEND_REQUEST";
    case E_FAIL_WIN_HTTP_RECEIVE_RESPONSE:
        return "FAIL_WIN_HTTP_RECEIVE_RESPONSE";
    default:
        return "(Unknown error)";
    }
}
