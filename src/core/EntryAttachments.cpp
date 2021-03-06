/*
 *  Copyright (C) 2012 Felix Geyer <debfx@fobos.de>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 or (at your option)
 *  version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "EntryAttachments.h"

EntryAttachments::EntryAttachments(QObject* parent)
    : QObject(parent)
{
}

QList<QString> EntryAttachments::keys() const
{
    return m_attachments.keys();
}

bool EntryAttachments::hasKey(const QString& key) const
{
    return m_attachments.keys().contains(key);
}

QList<QByteArray> EntryAttachments::values() const
{
    return m_attachments.values();
}

QByteArray EntryAttachments::value(const QString& key) const
{
    return m_attachments.value(key);
}

void EntryAttachments::set(const QString& key, const QByteArray& value)
{
    bool emitModified = false;
    bool addAttachment = !m_attachments.contains(key);

    if (addAttachment) {
        emit aboutToBeAdded(key);
    }

    if (addAttachment || m_attachments.value(key) != value) {
        m_attachments.insert(key, value);
        emitModified = true;
    }

    if (addAttachment) {
        emit added(key);
    }
    else {
        emit keyModified(key);
    }

    if (emitModified) {
        emit modified();
    }
}

void EntryAttachments::remove(const QString& key)
{
    if (!m_attachments.contains(key)) {
        Q_ASSERT(false);
        return;
    }

    emit aboutToBeRemoved(key);

    m_attachments.remove(key);

    emit removed(key);
    emit modified();
}

void EntryAttachments::clear()
{
    if (m_attachments.isEmpty()) {
        return;
    }

    emit aboutToBeReset();

    m_attachments.clear();

    emit reset();
    emit modified();
}

void EntryAttachments::copyDataFrom(const EntryAttachments* other)
{
    if (*this != *other) {
        emit aboutToBeReset();

        m_attachments = other->m_attachments;

        emit reset();
        emit modified();
    }
}

bool EntryAttachments::operator==(const EntryAttachments& other) const
{
    return m_attachments == other.m_attachments;
}

bool EntryAttachments::operator!=(const EntryAttachments& other) const
{
    return m_attachments != other.m_attachments;
}
