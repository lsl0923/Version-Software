#ifndef INDEXDELEGATE_H
#define INDEXDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

class IndexDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit IndexDelegate(QObject* parent = nullptr);
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

#endif // INDEXDELEGATE_H
