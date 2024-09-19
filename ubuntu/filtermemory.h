#ifndef FILTERMEMORY_H
#define FILTERMEMORY_H

#include <QApplication>
#include <QtWidgets>
#include <QDebug>
#include <QModelIndex>
#include <QSortFilterProxyModel>


class FilterMemory : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit FilterMemory(QObject* parent = nullptr):QSortFilterProxyModel(parent)
    {

    }

    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override
    {
        int leftRowIndex = sourceModel()->data(left).toInt();
        int rightRowIndex = sourceModel()->data(right).toInt();

        return leftRowIndex<rightRowIndex;
    }

};

#endif // FILTERMEMORY_H
