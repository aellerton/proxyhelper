# Proxy Helper

A small Windows console program to auto-detect the current HTTP proxy (if any)
and display settings needed for various programs.

This should be useful developers behind corporate proxies that need to use curl,
httpie, NPM, pip, etc.

In a bash-like shell (e.g. cygwin, git-bash or msys2):

```
$ ./proxyhelper --help
usage: proxyhelper.exe [flags]

Options:

  -c --cmd    set env for CMD (DOS) shell
  -b --bash   set env for Bash shell (Cygwin, Git Bash, msys)
  -x --setx   setx style
  -n --npm    settings for NPM

$ ./proxyhelper --bash --npm
unset http_proxy
unset https_proxy
unset ftp_proxy
unset rsync_proxy
npm config delete proxy
npm config delete https-proxy
```

In a CMD window:

```
C:\> proxyhelper --cmd
set http_proxy=
set https_proxy=
set ftp_proxy=
set rsync_proxy=
C:\>
```

### Other notes

For msys2, may also be relevant: https://stackoverflow.com/a/47335314

For NPM on windows, see: https://stackoverflow.com/a/21241320
