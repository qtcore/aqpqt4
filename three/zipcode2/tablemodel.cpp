#include "aqp.hpp"
#include "global.hpp"
#include "tablemodel.hpp"

#include <QApplication>
#include <QDataStream>
#include <QFile>
#include <QFontMetrics>
#include <QStyleOptionComboBox>



const qint32 MagicNumber = 0x5A697043;
const qint16 FormatNumber = 100;
const int MaxColumns = 4;

// 重载输出所有项
QDataStream &operator <<(QDataStream &out, const ZipcodeItem &item)
{
    out << static_cast<quint16>(item.zipcode) << item.postOffice
           << item.county << item.state;
    return out;
}


//重载输入项
QDataStream &operator >>(QDataStream &in, ZipcodeItem &item)
{
    quint16 zipcode;
    in >> zipcode >> item.postOffice >> item.county >> item.state;
    item.zipcode = static_cast<int>(zipcode);

    return in;
}



//标记模型索引设置相应项-flags
Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags theFlags = QAbstractItemModel::flags(index);

    //如果模型索引有效
    if(index.isValid())

        //设置相应项可选择或可编辑--也可用
        theFlags |= Qt::ItemIsSelectable | Qt::ItemIsEditable
                | Qt::ItemIsEditable;
    return theFlags;

}

//关注数据与元数据被访问
QVariant TableModel::data(const QModelIndex &index, int role) const
{
    //若果模型索引无效，行-列没有在有效范围
    if (!index.isValid() ||
        index.row() < 0 || index.row() >= zipcodes.count() ||
        index.column() < 0 || index.column() >= MaxColumns)
        return QVariant();//返回一个不需要的有效QVariant变量

    //模型索引有效，，将得到的邮编列表中对应项的只读引用
    const ZipcodeItem &item = zipcodes.at(index.row());

    //处理尺寸发生改变的请求
    if (role == Qt::SizeHintRole) {
        QStyleOptionComboBox option;
        switch (index.column()) {
            case Zipcode: {
                option.currentText = QString::number(MaxZipcode);
                const QString header = headerData(Zipcode,
                        Qt::Horizontal, Qt::DisplayRole).toString();
                if (header.length() > option.currentText.length())
                    option.currentText = header;
                break;
            }
            case PostOffice: option.currentText = item.postOffice;
                             break;
            case County: option.currentText = item.county; break;
            case State: option.currentText = item.state; break;
            default: Q_ASSERT(false);
        }

        //创建一个fontMetrics对象并使用它计算文本所需尺寸
        QFontMetrics fontMetrics(data(index, Qt::FontRole)
                                 .value<QFont>());
        option.fontMetrics = fontMetrics;
        QSize size(fontMetrics.width(option.currentText),
                   fontMetrics.height());
        return qApp->style()->sizeFromContents(QStyle::CT_ComboBox,
                                               &option, size);
    }

    //处理数据的请求--显示或编辑数据
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
            case Zipcode: return item.zipcode;
            case PostOffice: return item.postOffice;
            case County: return item.county;
            case State: return item.state;
            default: Q_ASSERT(false);
        }
    }
    return QVariant();  //其他未处理情况，返回一个无效QVariant对象
}



//表格视图的行表头或列表头----如果方向是Qt::Vertical，则section指的是行，如果方向是Horizontal,则section指的是列
QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    //处理显示角色
    if(role != Qt::DisplayRole)
        return QVariant();  //如果不是显示角色--返回一个无效QVariant对象

    //如果方向是水平
    if(orientation == Qt::Horizontal) {
        //section代表--列
        switch (section) {
        case Zipcode: return tr("Zipcode");
        case PostOffice: return tr("Post Office");
        case County: return tr("County");
        case State: return tr("State");
        default: Q_ASSERT(false);
        }
    }
    return section + 1;  //返回一个以1起计数的行数
}


//行数
int TableModel::rowCount(const QModelIndex &index) const
{
    //返回给定的模型索引对应的行数（实际上是子项数）
    return index.isValid() ? 0 : zipcodes.count();
}



//列数-- 只针对树模型有效
//列表和表格模型必须返回 0
int TableModel::columnCount(const QModelIndex &index) const
{
    //返回指定模型索引对应的列数
    return index.isValid() ? 0 : MaxColumns;
}


//设置数据
bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    //模型索引无效，角色不可编辑，行，列没有在有效范围
    if(!index.isValid() || role != Qt::EditRole ||
            index.row() < 0 || index.row() >= zipcodes.count() ||
            index.column() < 0 || index.column() >= MaxColumns)
        return false;

    //模型索引有效
    ZipcodeItem &item = zipcodes[index.row()]; //创建ZipcodeItem对象的非常量（可编辑）引用

   //设置对应的列的数据为传入参数value的值
    switch (index.column()) {
    case Zipcode:{
         bool ok;
         int zipcode = value.toInt(&ok);
         if(!ok || zipcode < MinZipcode || zipcode > MaxZipcode)
             return false;
         item.zipcode = zipcode;
         break;
                 }
     case PostOffice: item.postOffice = value.toString();
        break;
    case County: item.county = value.toString();
        break;
    case State: item.state = value.toString();
        break;

    default: Q_ASSERT(false);

    }
    //发射数据改变信号--关联模型/视图,通常只有一个索引变化，传入两个相同的参数index
    emit dataChanged(index, index);
    return true;
}


//设置表头数据
bool setHeaderData(int, Qt::Orientation, const QVariant&,
                   int=Qt::EditRole)
     {return false;}  //通过返回false,来禁止编辑修改表头


//插入行数据
bool TableModel::insertRows(int row, int count, const QModelIndex&)
{
    beginInsertRows(QModelIndex(), row, row + count -1);
    for(int i=0; i<count; ++i)
        zipcodes.insert(row, ZipcodeItem());
    endInsertRows();
    return true;

}


//删除行数据
bool TableModel::removeRows(int row, int count, const QModelIndex&)
{
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for(int i=0; i<count; ++i)
        zipcodes.removeAt(row);
    endRemoveRows();
    return true;
}

//从文件中加载数据到表格
void TableModel::load(const QString &filename)
{
    if (!filename.isEmpty())
        m_filename = filename;
    if (m_filename.isEmpty())
        throw AQP::Error(tr("no filename specified"));
    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly))
        throw AQP::Error(file.errorString());

    QDataStream in(&file);
    qint32 magicNumber;
    in >> magicNumber;
    if (magicNumber != MagicNumber)
        throw AQP::Error(tr("unrecognized file type"));
    qint16 formatVersionNumber;
    in >> formatVersionNumber;
    if (formatVersionNumber > FormatNumber)
        throw AQP::Error(tr("file format version is too new"));
    in.setVersion(QDataStream::Qt_4_5);
    zipcodes.clear();

    ZipcodeItem item;
    while (!in.atEnd()) {
        in >> item;
        zipcodes << item;
    }
    qSort(zipcodes);
    reset();
}


//将表格数据保存到文件
void TableModel::save(const QString &filename)
{
    if(!filename.isEmpty())
        m_filename = filename;

    if(m_filename.isEmpty())
        throw AQP::Error(tr("no filename specified"));

    QFile file(m_filename);
    if(!file.open(QIODevice::WriteOnly))
        throw AQP::Error(file.errorString());

    QDataStream out(&file);
    out << MagicNumber << FormatNumber;
    out.setVersion(QDataStream::Qt_4_5);

    QListIterator<ZipcodeItem> i(zipcodes);
    while(i.hasNext())
        out << i.next();
}







