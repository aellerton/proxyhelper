# Proxy Helper

A small Windows console program that detects the current HTTP proxy (if any)
and display settings needed for various programs to work with that proxy.

If you're in a corporate environment and the browsers work fine but you can't
run curl, httpie, NPM, pip etc from a console, then this program should get you running.

In technical terms, this program uses the Windows API to process the proxy auto-config
(PAC) file on the internal network, then display settings on the console.

In a bash-like shell (e.g. cygwin, git-bash or msys2):

```
$ ./proxyhelper --help
usage: proxyhelper.exe [flags]

Options:

  -c --cmd    set env for CMD (DOS) shell
  -b --bash   set env for Bash shell (Cygwin, Git Bash, msys)
  -x --setx   setx style
  -n --npm    settings for NPM
```

In an environment with a proxy server, you'll get a result like:

```
$ ./proxyhelper.exe --bash
export http_proxy=10.20.30.40:8080
export https_proxy=10.20.30.40:8080
export ftp_proxy=10.20.30.40:8080
export rsync_proxy=10.20.30.40:8080
```

It is possible there are two or more proxies, separated by `';'`, e.g. "10.20.30.40:8080;22.33.44.55:9090".
If that happens, the current version just picks the first one.

In an environment with no proxy server, the command-line will _unset_ the proxies, like this:

```
$ ./proxyhelper --bash --npm
unset http_proxy
unset https_proxy
unset ftp_proxy
unset rsync_proxy
npm config delete proxy
npm config delete https-proxy
```

This is for people who are in a corporate environment but change between networks.
For example, when you're docked, the ethernet has a proxy, but on wifi you don't.

Run with `--cmd` to get settings appropriate for a CMD window:

```
C:\> proxyhelper --cmd
set http_proxy=
set https_proxy=
set ftp_proxy=
set rsync_proxy=
C:\>
```

### Implementation

This implementation uses the Windows API to retrieve and process the proxy.
Read about the [winhttp autoproxy API on MSDN].

The underlying implementation is a different beast. Basically, a corporate environment
hosts a JavaScript file on the network somewhere. The file contains a JavaScript function
that yields the proxy server address (or addresses) based on input like which site you want
to load.

Read about [proxy auto-config on Wikipedia][4]

I don't fully understand how the system knows where the file is, but [maybe DNS?][2]
In any case, you can see the result in Chrome by loading this URL: chrome://net-internals/#proxy

Parsing the PAC file without a browser or the built-in system API can be done by using
any JavaScript engine. @manugarg did it years ago using Mozilla's then JavaScript engine,
spider monkey: [pacparser][3].

### Other notes

For msys2, may also be relevant: https://stackoverflow.com/a/47335314

For NPM on windows, see: https://stackoverflow.com/a/21241320

[1]: https://docs.microsoft.com/en-us/windows/desktop/winhttp/winhttp-autoproxy-api
[2]: https://support.microsoft.com/en-au/help/900935/how-the-windows-update-client-determines-which-proxy-server-to-use-to
[3]: https://github.com/manugarg/pacparser
[4]: http://en.wikipedia.org/wiki/Proxy_auto-config


