#include "pch.h"
#include "proxyinfo.h"
#include <iostream>


bool hasFlag(const char *flag, int argc, const char **argv) {
    for (int i = 1; i < argc; i++) {
        if (0 == strcmp(flag, argv[i])) {
            return true;
        }
    }
    return false;
}


int main(int argc, const char **argv)
{
    // poor man's arg parsing, because I'm too lazy to find an argparse that works on Windows
    bool isVerbose = hasFlag("-v", argc, argv) || hasFlag("--verbose", argc, argv);
    bool wantHelp = hasFlag("-h", argc, argv) || hasFlag("--help", argc, argv);
    bool wantCmd = hasFlag("-c", argc, argv) || hasFlag("--cmd", argc, argv);
    bool wantBash = hasFlag("-b", argc, argv) || hasFlag("--bash", argc, argv);
    bool wantSetx = hasFlag("-x", argc, argv) || hasFlag("--setx", argc, argv);
    bool wantNpm = hasFlag("-n", argc, argv) || hasFlag("--npm", argc, argv);
    bool anyShell = wantCmd || wantBash || wantSetx || wantNpm;

    if (wantHelp) {

        std::cout << "usage: "
            << PathFindFileNameA(argv[0])
            << " [flags]\n\n"
            << "Options:\n\n"
            << "  -c --cmd    set env for CMD (DOS) shell \n"
            << "  -b --bash   set env for Bash shell (Cygwin, Git Bash, msys)\n"
            << "  -x --setx   setx style\n"
            << "  -n --npm    settings for NPM\n"
            << std::endl;
        return 0;
    }

    ProxyInfoResult result;
    ProxyInfoResultCode code = GetProxyInfo(&result);

    if (isVerbose) {
        std::cout << "proxy query result:" << code << std::endl;
    }

    if (wantBash) {
        if (isVerbose) std::wcout << "# bash\n";
        if (result.hasProxy) {
            std::wcout
                << "export http_proxy=" << result.proxy << std::endl
                << "export https_proxy=" << result.proxy << std::endl
                << "export ftp_proxy=" << result.proxy << std::endl
                << "export rsync_proxy=" << result.proxy << std::endl
                ;
        } else {
            std::wcout
                << "unset http_proxy" << std::endl
                << "unset https_proxy" << std::endl
                << "unset ftp_proxy" << std::endl
                << "unset rsync_proxy" << std::endl
                ;
        }
    }

    if (wantSetx) {
        std::wcout
            << (isVerbose ? "# setx style:\n" : "")
            << "setx http_proxy = \"" << result.proxy << "\"" << std::endl
            << "setx https_proxy = \"" << result.proxy << "\"" << std::endl
            << "setx ftp_proxy = \"" << result.proxy << "\"" << std::endl
            << "setx rsync_proxy = \"" << result.proxy << "\"" << std::endl
            ;
    }

    if (wantCmd) {
        std::wcout
            << (isVerbose ? "rem CMD style:\n" : "")
            << "set http_proxy=" << result.proxy << "" << std::endl
            << "set https_proxy=" << result.proxy << "" << std::endl
            << "set ftp_proxy=" << result.proxy << "" << std::endl
            << "set rsync_proxy=" << result.proxy << "" << std::endl
            ;
    }

    if (wantNpm) {
        std::wcout << (isVerbose ? "# NPM configuration:\n" : "");
        if (result.hasProxy) {
            std::wcout
                << "npm config set proxy " << result.proxy << std::endl
                << "npm config set https-proxy " << result.proxy << std::endl
                ;
        }
        else {
            std::wcout
                << "npm config delete proxy" << std::endl
                << "npm config delete https-proxy" << std::endl
                ;
        }
    }

    if (!anyShell) {
        std::wcout << "status: " << (result.hasProxy ? "DETECTED a proxy" : "NO proxy detected") << std::endl;
        std::wcout << "proxy: [" << result.proxy << "]" << std::endl;
        std::wcout << "proxy bypass: [" << result.proxyBypass << "]" << std::endl;
    }
    return code;
}

