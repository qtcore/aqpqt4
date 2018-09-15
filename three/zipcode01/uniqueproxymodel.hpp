#ifndef UNIQUEPROXYMODEL_HPP
#define UNIQUEPROXYMODEL_HPP


#include <QSet>
#include <QSortFilterProxyModel>

class UniqueProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

     public:
    explicit UniqueProxyModel(int column, QObject *parent = 0)
        : QSortFilterProxyModel(parent), Column(column) { }

    void setSourceModel(QAbstractItemModel *sourceModel);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

private slots:
    void clearCache() { cache.clear(); }

private:
    const int Column;
    mutable QSet<QString> cache;

};

#endif // UNIQUEPROXYMODEL_HPP
