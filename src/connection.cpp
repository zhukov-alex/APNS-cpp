#include "../inc/connection.hpp"
#include "../inc/global.hpp"

Connection::Connection (
            const std::string& host,
            const int port,
            const std::string& path,
            const std::string& cert,
            const std::string& key,
            const std::string& pass
) {
    this->_host = host;
    this->_port = port;
    this->_path = path;
    this->_cert = cert;
    this->_key = key;
    this->_pass = pass;

    this->_ssl = NULL;
    this->_ctx = NULL;
    this->_socket = -1;
}

Connection::~Connection()
{
    this->closeConn();
}

bool Connection::isOpened(void) const
{
    return this->_ssl != NULL && this->_ctx && this->_socket != -1;
}

void Connection::openConn(void)
{
    if (this->isOpened())
        return;

    struct hostent *he;
    struct sockaddr_in sa;

    this->_ctx = SSL_CTX_new(TLSv1_method());

    if (NULL == this->_ctx)
    {
        std::cout << "SSL_CTX_new err" << std::endl;
        exit(1);
    }

    if (SSL_CTX_load_verify_locations(this->_ctx, NULL, ".") <= 0) {
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    if (SSL_CTX_use_certificate_file(this->_ctx, this->_cert.c_str(), SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    if (!this->_pass.empty())
        SSL_CTX_set_default_passwd_cb_userdata(this->_ctx, (void*) this->_pass.c_str());
    if (SSL_CTX_use_PrivateKey_file(this->_ctx, this->_key.c_str(), SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    if (SSL_CTX_use_PrivateKey_file(this->_ctx, this->_cert.c_str(), SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    if (SSL_CTX_check_private_key(this->_ctx) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    this->_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (this->_socket == -1) {
        std::cout << "Socket err" << std::endl;
        exit(1);
    }

    he = gethostbyname(APNS_HOST);
    if (!he) {
        close(this->_socket);
        exit(1);
    }

    sa.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr *) he->h_addr_list[0])));
    sa.sin_port = htons(APNS_PORT);

    if (connect(this->_socket, (struct sockaddr *) &sa, sizeof(sa)) == -1) {
        close(this->_socket);
        exit(1);
    }

    this->_ssl = SSL_new(this->_ctx);
    SSL_set_fd(this->_ssl, this->_socket);
    if (SSL_connect(this->_ssl) == -1) {
        shutdown(this->_socket, 2);
        close(this->_socket);
        exit(1);
    }
}

void Connection::closeConn(void)
{
    if (!this->isOpened())
        return;

    SSL_shutdown(this->_ssl);

    SSL_free(this->_ssl);
    this->_ssl = NULL;

    close(this->_socket);
    this->_socket = -1;

    SSL_CTX_free(this->_ctx);
    this->_ctx = NULL;
}

bool Connection::send(const unsigned char* data, const size_t len)
{
    if (!this->isOpened())
        this->openConn();

    int res = SSL_write(this->_ssl, data, len);
    if (res <= 0) {
        int errorCode = SSL_get_error(this->_ssl, res);
        std::cout << "SSL_write err, code:" << errorCode << std::endl;
    }
    return true;
}
