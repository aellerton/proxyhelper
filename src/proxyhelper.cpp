#include "pch.h"
#include "proxyinfo.h"
#include "proxyout.h"
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
    ProxyInfoResultCode code = get_proxy_info(&result);

    if (code != OK_PROXY_INFO) {
        std::cout << "Failed: " << get_result_code_text(code) << " (" << code << ")" << std::endl;
        return code; // returns non-zero to caller
    }

    if (wantBash) {
        std::wcout << get_bash(result, isVerbose);
    }

    if (wantSetx) {
        std::wcout << get_setx(result, isVerbose);
    }

    if (wantCmd) {
        std::wcout << get_cmd(result, isVerbose);
    }

    if (wantNpm) {
        std::wcout << get_npm(result, isVerbose);
    }

    if (!anyShell) {
        std::wcout << get_general(result, isVerbose);
    }
    return 0;
}
