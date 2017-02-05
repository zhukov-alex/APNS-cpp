#include "../inc/notifier.hpp"

Notifier::Notifier(Connection *conn)
{
    this->_conn = conn;
}

Notifier::~Notifier() {}

bool Notifier::sendNotification(Notification* n, const Device* device)
{
    return true;
}

bool Notifier::packNotification(std::string* packed, const Notification* n, const Device* device)
{
    return true;
}
