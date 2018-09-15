#ifndef ITEMDELEGATE_HPP
#define ITEMDELEGATE_HPP


#include <QStyledItemDelegate>

class QModelIndex;
class QPainter;
class QStyleOptionViewItem;


class ItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit ItemDelegate(QObject *parent=0)
        : QStyledItemDelegate(parent) { }

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

};

#endif // ITEMDELEGATE_HPP
