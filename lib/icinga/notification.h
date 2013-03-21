/******************************************************************************
 * Icinga 2                                                                   *
 * Copyright (C) 2012 Icinga Development Team (http://www.icinga.org/)        *
 *                                                                            *
 * This program is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU General Public License                *
 * as published by the Free Software Foundation; either version 2             *
 * of the License, or (at your option) any later version.                     *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program; if not, write to the Free Software Foundation     *
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ******************************************************************************/

#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include "icinga/i2-icinga.h"
#include "icinga/user.h"
#include "icinga/usergroup.h"
#include "icinga/timeperiod.h"
#include "base/array.h"

namespace icinga
{

/**
 * The notification type.
 *
 * @ingroup icinga
 */
enum NotificationType
{
	NotificationDowntimeStart,
	NotificationDowntimeEnd,
	NotificationDowntimeRemoved,
	NotificationCustom,
	NotificationAcknowledgement,
	NotificationProblem,
	NotificationRecovery
};

class Service;

/**
 * An Icinga notification specification.
 *
 * @ingroup icinga
 */
class I2_ICINGA_API Notification : public DynamicObject
{
public:
	typedef shared_ptr<Notification> Ptr;
	typedef weak_ptr<Notification> WeakPtr;

	explicit Notification(const Dictionary::Ptr& serializedUpdate);
	~Notification(void);

	static Notification::Ptr GetByName(const String& name);

	shared_ptr<Service> GetService(void) const;
	Value GetNotificationCommand(void) const;
	double GetNotificationInterval(void) const;
	TimePeriod::Ptr GetNotificationPeriod(void) const;
	Dictionary::Ptr GetMacros(void) const;
	std::set<User::Ptr> GetUsers(void) const;
	std::set<UserGroup::Ptr> GetGroups(void) const;

	double GetLastNotification(void) const;
	void SetLastNotification(double time);

	double GetNextNotification(void) const;
	void SetNextNotification(double time);

	void BeginExecuteNotification(NotificationType type, const Dictionary::Ptr& cr, bool ignore_timeperiod);

	static String NotificationTypeToString(NotificationType type);

protected:
	void OnAttributeChanged(const String& name);

private:
	Attribute<Value> m_NotificationCommand;
	Attribute<double> m_NotificationInterval;
	Attribute<String> m_NotificationPeriod;
	Attribute<double> m_LastNotification;
	Attribute<double> m_NextNotification;
	Attribute<Dictionary::Ptr> m_Macros;
	Attribute<Array::Ptr> m_Users;
	Attribute<Array::Ptr> m_Groups;
	Attribute<String> m_HostName;
	Attribute<String> m_Service;

	std::set<ScriptTask::Ptr> m_Tasks;

	void NotificationCompletedHandler(const ScriptTask::Ptr& task);

	void BeginExecuteNotificationHelper(const Dictionary::Ptr& notificationMacros,
	    NotificationType type, const User::Ptr& user, bool ignore_timeperiod);
};

}

#endif /* NOTIFICATION_H */
