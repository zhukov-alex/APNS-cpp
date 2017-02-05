#ifndef _APNS_NOTIFIER_H
#define _APNS_NOTIFIER_H

#include "global.hpp"
#include "connection.hpp"
#include <string>

typedef struct Notification
{
    int *priority;
    std::string *alert;
    int badge;
    std::string *sound;
} Notification;

typedef struct Device
{
    char const* _tokenHex;
    char _tokenBin[DEVICE_BIN_MAX_SIZE];

    Device(char const* tokenHex)
    {
        this->_tokenHex = tokenHex;
        unsigned int i = 0;
        int j = 0;
        int tmpi;
        char tmp[3];
        while(i < strlen(tokenHex))
        {
            if(tokenHex[i] == ' ') {
                i++;
            } else {
                tmp[0] = tokenHex[i];
                tmp[1] = tokenHex[i + 1];
                tmp[2] = '\0';

                sscanf(tmp, "%x", &tmpi);
                this->_tokenBin[j] = static_cast<char>(tmpi);
                i += 2;
                j++;
            }
        }
    }
} Device;

class Notifier {
public:
    explicit Notifier(Connection* conn);

    virtual ~Notifier();

    bool sendNotification(Notification* n, const Device* device);

private:
    Connection* _conn;

    bool packNotification(std::string* packed, const Notification* n, const Device* device);
};
#endif
