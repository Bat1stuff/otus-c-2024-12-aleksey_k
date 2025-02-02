**Сборка curl c поддержкой только протоколов http, https и telnet (IPFS и IPNS идут вместе с http).**

Сборка с использованием autotools:

```
./configure --disable-dict --disable-file --disable-ftp --disable-gopher --disable-imap --disable-mqtt --disable-pop3 --disable-rtsp --disable-smb --disable-smtp --disable-tftp --disable-proxy --disable-ldap --disable-ldaps --without-librtmp --disable-websockets --enable-http --enable-telnet --with-openssl --prefix=/home/$USER/curl_build
```

проверяем:

```
/home/$USER/curl_build/bin/curl --version
```