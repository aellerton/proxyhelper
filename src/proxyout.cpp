#include "pch.h"
#include <sstream>
#include "proxyout.h"

std::wstring get_bash(const ProxyInfoResult& result, bool verbose) {
    std::wstringstream buf;
    if (verbose) buf << "# bash\n";
    if (result.hasProxy) {
        buf << "export http_proxy=" << result.proxy1 << std::endl
            << "export https_proxy=" << result.proxy1 << std::endl
            << "export ftp_proxy=" << result.proxy1 << std::endl
            << "export rsync_proxy=" << result.proxy1 << std::endl
            ;
    }
    else {
        buf << "unset http_proxy" << std::endl
            << "unset https_proxy" << std::endl
            << "unset ftp_proxy" << std::endl
            << "unset rsync_proxy" << std::endl
            ;
    }
    return buf.str();
}

std::wstring get_setx(const ProxyInfoResult& result, bool verbose) {
    std::wstringstream buf;
    buf << (verbose ? "# setx style:\n" : "")
        << "setx http_proxy = \"" << result.proxy1 << "\"" << std::endl
        << "setx https_proxy = \"" << result.proxy1 << "\"" << std::endl
        << "setx ftp_proxy = \"" << result.proxy1 << "\"" << std::endl
        << "setx rsync_proxy = \"" << result.proxy1 << "\"" << std::endl
        ;
    return buf.str();
}

std::wstring get_cmd(const ProxyInfoResult& result, bool verbose) {
    std::wstringstream buf;
    buf << (verbose ? "rem CMD style:\n" : "")
        << "set http_proxy=" << result.proxy1 << "" << std::endl
        << "set https_proxy=" << result.proxy1 << "" << std::endl
        << "set ftp_proxy=" << result.proxy1 << "" << std::endl
        << "set rsync_proxy=" << result.proxy1 << "" << std::endl
        ;
    return buf.str();
}

std::wstring get_npm(const ProxyInfoResult& result, bool verbose) {
    std::wstringstream buf;

    buf << (verbose ? "# NPM configuration:\n" : "");
    if (result.hasProxy) {
        buf << "npm config set proxy " << result.proxy1 << std::endl
            << "npm config set https-proxy " << result.proxy1 << std::endl
            ;
    }
    else {
        buf << "npm config delete proxy" << std::endl
            << "npm config delete https-proxy" << std::endl
            ;
    }
    return buf.str();
}

std::wstring get_general(const ProxyInfoResult& result, bool verbose) {
    std::wstringstream buf;
    buf << "status: " << (result.hasProxy ? "DETECTED a proxy" : "NO proxy detected") << std::endl;
    buf << "proxy: [" << result.proxy << "]" << std::endl;
    buf << "proxy bypass: [" << result.proxyBypass << "]" << std::endl;
    return buf.str();
}
