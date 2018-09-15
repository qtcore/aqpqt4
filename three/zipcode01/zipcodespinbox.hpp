#ifndef ZIPCODESPINBOX_HPP
#define ZIPCODESPINBOX_HPP

#include "global.hpp"
#include <QSpinBox>


class ZipcodeSpinBox : public QSpinBox
{
    Q_OBJECT

public:
    explicit ZipcodeSpinBox(QWidget *parent)
        : QSpinBox(parent)
    {
        setRange(MinZipcode, MaxZipcode);
        setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    }

protected:
    QString textFromValue(int value) const
    { return QString("%1").arg(value, 5, 10, QChar('0')); }

};

#endif // ZIPCODESPINBOX_HPP
