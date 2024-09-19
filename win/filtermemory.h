#ifndef FILTERMEMORY_H
#define FILTERMEMORY_H

#include <QApplication>
#include <QtWidgets>
#include <QDebug>

// Фильтр для модели данных потоков, чтобы показывать потоки
// только для выбранного процесса:
class FilterMemory : public QSortFilterProxyModel
{
    // Q_OBJECT - этот макрос обязателен для любого класса на Си++,
    // в котором планируется описать сигналы и/или слоты:
    Q_OBJECT

public:

    // Конструктор:
    explicit FilterMemory(QObject* parent = nullptr):
        QSortFilterProxyModel(parent)  // вызов родительского конструктора.
    {
        // Настройка регулярного выражения:
        //pidRegExp.setPatternOptions(Qt::CaseInsensitive);  // не чувствительно к регистру символов.
        //pidRegExp.setPatternSyntax(QRegularExpression::FixedString);  // использовать как фиксированную строку.
    }

    // Перезапись родительского метода, который фильтрует строки:
//    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override
//    {
//        // Индекс строки:
//        QModelIndex rowIndex = sourceModel()->index(sourceRow, 5, sourceParent);

//        // ИД процесса:
//        QString memory = sourceModel()->data(rowIndex).toString();

//        // Если ИД процесса не задан, то показываем строку,
//        // иначе проверяем удовлетворяет ли ИД процесса регулярному выражению:
//        return memory.isEmpty() ? true: false;
//    }

    bool lessThan(const QModelIndex &left,
                 const QModelIndex &right) const override
    {
        //if(sourceModel()->data(left).)

        int leftRowIndex =  sourceModel()->data( left ).toInt();
        int rightRowIndex = sourceModel()->data( right ).toInt();

        return leftRowIndex<rightRowIndex;

    }

    // Метод для установки в качестве фильтра регулярного выражения,
    // по которому будут фильтроваться строки на основе поля ИД процесса:
    //void setPidFilter(const QString& regExp);

private:

    // Регулярное выражение для фильтрации строк:
    QRegularExpression pidRegExp;

};


#endif // FILTERMEMORY_H
