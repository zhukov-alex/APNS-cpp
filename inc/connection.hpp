#ifndef _APNS_CONNECTION_H
#define _APNS_CONNECTION_H
#include <cstddef>
#include <string>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <openssl/crypto.h>
#include <openssl/err.h>

class Connection {
public:
    Connection(
            const std::string& host,
            const int port,
            const std::string& path,
            const std::string& cert,
            const std::string& key,
            const std::string& pass
    );

    virtual ~Connection();

    bool isOpened() const;

    void openConn(void);

    void closeConn(void);

    bool send(const unsigned char* data, const size_t len);

private:
    std::string _host;
    unsigned short _port;
    std::string _path;
    std::string _cert;
    std::string _key;
    std::string _pass;

    SSL* _ssl;
    SSL_CTX* _ctx;
    int _socket;
};

#endif
