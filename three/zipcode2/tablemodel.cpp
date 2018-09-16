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

// �������������
QDataStream &operator <<(QDataStream &out, const ZipcodeItem &item)
{
    out << static_cast<quint16>(item.zipcode) << item.postOffice
           << item.county << item.state;
    return out;
}


//����������
QDataStream &operator >>(QDataStream &in, ZipcodeItem &item)
{
    quint16 zipcode;
    in >> zipcode >> item.postOffice >> item.county >> item.state;
    item.zipcode = static_cast<int>(zipcode);

    return in;
}



//���ģ������������Ӧ��-flags
Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags theFlags = QAbstractItemModel::flags(index);

    //���ģ��������Ч
    if(index.isValid())

        //������Ӧ���ѡ���ɱ༭--Ҳ����
        theFlags |= Qt::ItemIsSelectable | Qt::ItemIsEditable
                | Qt::ItemIsEditable;
    return theFlags;

}

//��ע������Ԫ���ݱ�����
QVariant TableModel::data(const QModelIndex &index, int role) const
{
    //����ģ��������Ч����-��û������Ч��Χ
    if (!index.isValid() ||
        index.row() < 0 || index.row() >= zipcodes.count() ||
        index.column() < 0 || index.column() >= MaxColumns)
        return QVariant();//����һ������Ҫ����ЧQVariant����

    //ģ��������Ч�������õ����ʱ��б��ж�Ӧ���ֻ������
    const ZipcodeItem &item = zipcodes.at(index.row());

    //����ߴ緢���ı������
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

        //����һ��fontMetrics����ʹ���������ı�����ߴ�
        QFontMetrics fontMetrics(data(index, Qt::FontRole)
                                 .value<QFont>());
        option.fontMetrics = fontMetrics;
        QSize size(fontMetrics.width(option.currentText),
                   fontMetrics.height());
        return qApp->style()->sizeFromContents(QStyle::CT_ComboBox,
                                               &option, size);
    }

    //�������ݵ�����--��ʾ��༭����
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
            case Zipcode: return item.zipcode;
            case PostOffice: return item.postOffice;
            case County: return item.county;
            case State: return item.state;
            default: Q_ASSERT(false);
        }
    }
    return QVariant();  //����δ�������������һ����ЧQVariant����
}



//�����ͼ���б�ͷ���б�ͷ----���������Qt::Vertical����sectionָ�����У����������Horizontal,��sectionָ������
QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    //������ʾ��ɫ
    if(role != Qt::DisplayRole)
        return QVariant();  //���������ʾ��ɫ--����һ����ЧQVariant����

    //���������ˮƽ
    if(orientation == Qt::Horizontal) {
        //section����--��
        switch (section) {
        case Zipcode: return tr("Zipcode");
        case PostOffice: return tr("Post Office");
        case County: return tr("County");
        case State: return tr("State");
        default: Q_ASSERT(false);
        }
    }
    return section + 1;  //����һ����1�����������
}


//����
int TableModel::rowCount(const QModelIndex &index) const
{
    //���ظ�����ģ��������Ӧ��������ʵ��������������
    return index.isValid() ? 0 : zipcodes.count();
}



//����-- ֻ�����ģ����Ч
//�б�ͱ��ģ�ͱ��뷵�� 0
int TableModel::columnCount(const QModelIndex &index) const
{
    //����ָ��ģ��������Ӧ������
    return index.isValid() ? 0 : MaxColumns;
}


//��������
bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    //ģ��������Ч����ɫ���ɱ༭���У���û������Ч��Χ
    if(!index.isValid() || role != Qt::EditRole ||
            index.row() < 0 || index.row() >= zipcodes.count() ||
            index.column() < 0 || index.column() >= MaxColumns)
        return false;

    //ģ��������Ч
    ZipcodeItem &item = zipcodes[index.row()]; //����ZipcodeItem����ķǳ������ɱ༭������

   //���ö�Ӧ���е�����Ϊ�������value��ֵ
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
    //�������ݸı��ź�--����ģ��/��ͼ,ͨ��ֻ��һ�������仯������������ͬ�Ĳ���index
    emit dataChanged(index, index);
    return true;
}


//���ñ�ͷ����
bool setHeaderData(int, Qt::Orientation, const QVariant&,
                   int=Qt::EditRole)
     {return false;}  //ͨ������false,����ֹ�༭�޸ı�ͷ


//����������
bool TableModel::insertRows(int row, int count, const QModelIndex&)
{
    beginInsertRows(QModelIndex(), row, row + count -1);
    for(int i=0; i<count; ++i)
        zipcodes.insert(row, ZipcodeItem());
    endInsertRows();
    return true;

}


//ɾ��������
bool TableModel::removeRows(int row, int count, const QModelIndex&)
{
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for(int i=0; i<count; ++i)
        zipcodes.removeAt(row);
    endRemoveRows();
    return true;
}

//���ļ��м������ݵ����
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


//��������ݱ��浽�ļ�
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







