#include "IndexDelegate.h"

IndexDelegate::IndexDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}

void IndexDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    // 调用父类绘制项内容
    QStyledItemDelegate::paint(painter, option, index);

    // 绘制右侧的固定下标
    QRect rect = option.rect;
    QString indexText = QString::number(index.row() + 1); // 下标从 1 开始

    // 计算右侧区域，用于绘制下标
    int rightPadding = 10; // 与右边界的间距
    QRect indexRect(rect.right() - 30 - rightPadding, rect.top(), 30, rect.height()); // 固定宽度为 30
    painter->drawText(indexRect, Qt::AlignRight | Qt::AlignVCenter, indexText);
}
