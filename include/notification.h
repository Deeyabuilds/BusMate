#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include "structures.h"

int send_notification(const char *message,
                     const char *recipient_id);

#endif