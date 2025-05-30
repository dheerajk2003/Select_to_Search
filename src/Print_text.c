#include <libnotify/notification.h>
#include<libnotify/notify.h>
#include<libgen.h>
#include<string.h>

void Notify_Res(char* message)
{
    NotifyNotification *notification;
    notify_init("StS");
    notification = notify_notification_new("StS", message, NULL);
    notify_notification_set_timeout(notification, NOTIFY_EXPIRES_NEVER);
    notify_notification_show(notification, NULL);
}
