// Copyright (c) 2011-2020 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef CSCOIN_QT_CSCOINADDRESSVALIDATOR_H
#define CSCOIN_QT_CSCOINADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class CScoinAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit CScoinAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

/** CScoin address widget validator, checks for a valid cscoin address.
 */
class CScoinAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit CScoinAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

#endif // CSCOIN_QT_CSCOINADDRESSVALIDATOR_H
