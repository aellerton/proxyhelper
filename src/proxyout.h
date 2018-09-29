#pragma once
#include "proxyinfo.h"

std::wstring get_bash(const ProxyInfoResult& result, bool verbose);
std::wstring get_setx(const ProxyInfoResult& result, bool verbose);
std::wstring get_cmd(const ProxyInfoResult& result, bool verbose);
std::wstring get_npm(const ProxyInfoResult& result, bool verbose);
std::wstring get_general(const ProxyInfoResult& result, bool verbose);
