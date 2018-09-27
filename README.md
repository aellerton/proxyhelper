# Proxy Helpers

Small Windows console program to auto-detect the current HTTP proxy (if any)
and display on the console.

Useful for generating settings for command-line programs like curl, npm, pip, etc.

In cygwin, git-bash or msys2:

```bash
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
