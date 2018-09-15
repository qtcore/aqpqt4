//过滤掉重复行的QSortFliterProxyModel类--创建自定义UniqueProxyModel子类



#include "uniqueproxymodel.hpp"

void UniqueProxyModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    connect(sourceModel, SIGNAL(modelReset()),
            this, SLOT(clearCache()));
    QSortFilterProxyModel::setSourceModel(sourceModel);
}


bool UniqueProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, Column,
                                             sourceParent);
    const QString &text = sourceModel()->data(index).toString();
    if(cache.contains(text))
        return false;
    cache << text;
    return true;

}
