/*
 * Copyright (C) 2010-2012 Jeremy Lainé
 * Contact: http://code.google.com/p/qdjango/
 *
 * This file is part of the QDjango Library.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 */

#include <QtTest>

#include "QDjango.h"
#include "QDjango_p.h"
#include "QDjangoQuerySet.h"
#include "QDjangoWhere.h"
#include "fields.h"

#define Q QDjangoWhere

template<class T>
void init(const QStringList &sql)
{
    const QDjangoMetaModel metaModel = QDjango::registerModel<T>();
    QCOMPARE(metaModel.createTableSql(), sql);
    QCOMPARE(metaModel.createTable(), true);
}

template<class T, class K>
void setAndGet(const K &value)
{
    T v1;
    v1.setValue(value);
    QCOMPARE(v1.save(), true);

    T v2;
    QVERIFY(QDjangoQuerySet<T>().get(Q(QLatin1String("pk"), Q::Equals, v1.pk()), &v2) != 0);
    QCOMPARE(v2.value(), value);
}

template<class T>
void cleanup()
{
    const QDjangoMetaModel metaModel = QDjango::registerModel<T>();
    QCOMPARE(metaModel.dropTable(), true);
}

void tst_Bool::testValue()
{
    init<tst_Bool>(QStringList()
        << QLatin1String("CREATE TABLE \"tst_bool\" (\"id\" integer NOT NULL PRIMARY KEY AUTOINCREMENT, \"value\" bool NOT NULL)"));
    setAndGet<tst_Bool>(true);
    setAndGet<tst_Bool>(false);
    cleanup<tst_Bool>();
}

void tst_ByteArray::testValue()
{
    init<tst_ByteArray>(QStringList()
        << QLatin1String("CREATE TABLE \"tst_bytearray\" (\"id\" integer NOT NULL PRIMARY KEY AUTOINCREMENT, \"value\" blob NOT NULL)"));
    setAndGet<tst_ByteArray>(QByteArray("01234567", 8));
    setAndGet<tst_ByteArray>(QByteArray("\x00\x01\x02\x03\x04\x05\x06\x07", 8));
    cleanup<tst_ByteArray>();
}

void tst_Date::testValue()
{
    init<tst_Date>(QStringList()
        << QLatin1String("CREATE TABLE \"tst_date\" (\"id\" integer NOT NULL PRIMARY KEY AUTOINCREMENT, \"value\" date NOT NULL)"));
    setAndGet<tst_Date>(QDate(2012, 1, 8));
    cleanup<tst_Date>();
}

void tst_DateTime::testValue()
{
    init<tst_DateTime>(QStringList()
        << QLatin1String("CREATE TABLE \"tst_datetime\" (\"id\" integer NOT NULL PRIMARY KEY AUTOINCREMENT, \"value\" datetime NOT NULL)"));
    setAndGet<tst_DateTime>(QDateTime(QDate(2012, 1, 8), QTime(3, 4, 5)));
    cleanup<tst_DateTime>();
}

void tst_Double::testValue()
{
    init<tst_Double>(QStringList()
        << QLatin1String("CREATE TABLE \"tst_double\" (\"id\" integer NOT NULL PRIMARY KEY AUTOINCREMENT, \"value\" real NOT NULL)"));
    setAndGet<tst_Double>(double(3.14159));;
    cleanup<tst_Double>();
}

void tst_Integer::testValue()
{
    init<tst_Integer>(QStringList()
        << QLatin1String("CREATE TABLE \"tst_integer\" (\"id\" integer NOT NULL PRIMARY KEY AUTOINCREMENT, \"value\" integer NOT NULL)"));
    setAndGet<tst_Integer>(0);
    setAndGet<tst_Integer>(-2147483647);
    setAndGet<tst_Integer>(2147483647);
    cleanup<tst_Integer>();
}

void tst_LongLong::testValue()
{
    init<tst_LongLong>(QStringList()
        << QLatin1String("CREATE TABLE \"tst_longlong\" (\"id\" integer NOT NULL PRIMARY KEY AUTOINCREMENT, \"value\" bigint NOT NULL)"));
    setAndGet<tst_LongLong>(qlonglong(0));
    setAndGet<tst_LongLong>(qlonglong(-9223372036854775807ll));
    setAndGet<tst_LongLong>(qlonglong(9223372036854775807ll));
    cleanup<tst_LongLong>();
}

void tst_String::testValue()
{
    init<tst_String>(QStringList()
        << QLatin1String("CREATE TABLE \"tst_string\" (\"id\" integer NOT NULL PRIMARY KEY AUTOINCREMENT, \"value\" varchar(255) NOT NULL)"));
    setAndGet<tst_String>(QLatin1String("foo bar"));
    cleanup<tst_String>();
}

void tst_Time::testValue()
{
    init<tst_Time>(QStringList()
        << QLatin1String("CREATE TABLE \"tst_time\" (\"id\" integer NOT NULL PRIMARY KEY AUTOINCREMENT, \"value\" time NOT NULL)"));
    setAndGet<tst_Time>(QTime(3, 4, 5));
    cleanup<tst_Time>();
}

void tst_Options::testOptions()
{
    init<tst_Options>(QStringList()
        << QLatin1String(
            "CREATE TABLE \"tst_options\" ("
                "\"id\" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "
                "\"indexField\" integer NOT NULL, "
                "\"nullField\" integer, "
                "\"uniqueField\" integer NOT NULL UNIQUE"
            ")")
        << QLatin1String("CREATE INDEX \"tst_options_ac243651\" ON \"tst_options\" (\"indexField\")"));
}
