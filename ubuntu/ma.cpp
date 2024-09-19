#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <iostream>
#include <fstream>
#include <QProcess>



// Конструктор:
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),  // вызов родительского конструктора.
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Отключение кнопки для разворачивания окна
    // и возможности менять размеры окна:
    setFixedSize(width(), height());

    // Создание модели данных для таблицы с процессами:
    processesTableModel = new ProcessesTableModel();

    fm = new FilterMemory;
    fm->setSourceModel(processesTableModel);
    // Настройка таблицы с процессами:
    setupTable(ui->processesTableView, fm);
    //ui->processesTableView->setSortingEnabled(true);
    ui->processesTableView->sortByColumn(5, Qt::DescendingOrder);


    //QString proc_memory = fm->index(0,5).data().toString();

    std::ofstream out;
    std::string dir = QApplication::applicationDirPath().toStdString();
    std::string path= dir+"/"+std::string("log.txt");
    out.open(path, std::ios::app);
    for(int i=0;i<6;i++)
    {
        QString proc_pid = ui->processesTableView->model()->index(i,1).data().toString();
        QString proc_name = ui->processesTableView->model()->index(i,2).data().toString();
        QString proc_memory = ui->processesTableView->model()->index(i,5).data().toString();
        out<<"("<<i<<")"<<"pid: "<<proc_pid.toStdString()<<
        "; name process: "<< proc_name.toStdString()<<
        "; memory (Kb): "<<proc_memory.toStdString()<<"\n";
    }
    out.close();

    //QString input_file = QString("C:/Windows/SysWOW64/notepad.exe" + QString::fromStdString(path));
    QString input_file = QString::fromStdString(path);
    qDebug()<<QString("C:/Windows/SysWOW64/notepad.exe" + QString::fromStdString(path));
    QProcess input_notepad;
    input_notepad.setProgram("cmd.exe");
    input_notepad.setArguments(QStringList() << "/C" <<  "start" << input_file);
    input_notepad.startDetached();





//    QModelIndex proc_pid = ui->processesTableView->model()->index(1,5);
//     proc_pid.data().toString();
//    QModelIndex proc_name = ui->processesTableView->model()->index(0,5);
//    QModelIndex proc_memory = ui->processesTableView->model()->index(0,5);





    InitializeCriticalSection(&CrSectionUpd);

//    QObject::connect(&updater, SIGNAL(tick()),
//        this, SLOT(on_actionRefresh_triggered()));
//    updater.startUpdater();
}

// Метод для настройки внешнего вида таблиц процессов и потоков:
void MainWindow::setupTable(QTableView *tableView, QAbstractItemModel *tableModel)
{

    //tableModel->s;
    // Установка модели данных для таблицы:
    tableView->setModel(tableModel);

    // Выделять целиком всю строку, а не отдельные ячейки:
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Подогнать ширину столбцов:
    autosizeTable(tableView);

    // Не выделять строку с заголовками при выделении строки с данными:
    tableView->horizontalHeader()->setHighlightSections(false);

    // Убрать синее выделение строки и пунктирное выделение ячейки:
    tableView->setFocusPolicy(Qt::NoFocus);

    // Сделать возможным выделение только одной строки, а не многих:
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);


}

// Подогнать ширину столбцов:
void MainWindow::autosizeTable(QTableView *tableView)
{
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

// Деструктор:
MainWindow::~MainWindow()
{
    // Удаление из памяти пользовательского интерфейса:
    delete ui;

    // Удаление из памяти фильтрующей модели данных для таблицы потоков:
    delete threadsProxyModel;

    // Удаление из памяти модели данных для таблицы потоков:
    delete threadsTableModel;

    // Удаление из памяти модели данных для таблицы процессов:
    delete processesTableModel;

    destroyProcessesChart();

    DeleteCriticalSection(&CrSectionUpd);
}

// Метод, который вызывается при выделении строки в таблице с процессами:
void MainWindow::processSelectionChangedSlot(const QItemSelection &newSelection, const QItemSelection &oldSelection)
{

}

// Пункт меню Файл -> Завершение диспетчера:
void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

// Пункт меню Вид -> Обновить (F5):
void MainWindow::on_actionRefresh_triggered()
{
    int processesTableViewVertical = ui->processesTableView->verticalScrollBar()->value();

    EnterCriticalSection(&CrSectionUpd);

    // Обновляем модель данных для таблицы процессов:
    processesTableModel->refreshData();

    // Обновляем модель данных для таблицы потоков:
    threadsTableModel->refreshData();

    // Очищаем фильтрацию:
    threadsProxyModel->setPidFilter("");

    // Очистка статусной строки:
    cpuKernelTime->setText("");
    cpuUserTime->setText("");

    for (int i = 0; i < processesTableModel->rowCount(); i++)
    {
        if (lastSelectedPid == processesTableModel->data(processesTableModel->index(i, 1)).toInt())
        {
            ui->processesTableView->selectRow(i);
            break;
        }
    }
    ui->processesTableView->verticalScrollBar()->setValue(processesTableViewVertical);

    LeaveCriticalSection(&CrSectionUpd);
}

// Метод, который вызывается при выделении строки в таблице с потоками:
void MainWindow::threadSelectionChangedSlot(const QItemSelection &newSelection, const QItemSelection &oldSelection)
{
    // Неиспользуемая переменная:
    Q_UNUSED(oldSelection);

    QModelIndexList rowIndexes = newSelection.indexes();
    if (rowIndexes.size() > 0)
    {
        // Индекс выделенной строки в таблице потоков:
        int rowIndex = rowIndexes.first().row();

        // ИД потока:
        lastSelectedTid = threadsProxyModel->data(threadsProxyModel->index(rowIndex, 1)).toInt();
        //qDebug() << "TID =" << tid;

        // Время проведенноё потоком в режиме ядра:
        double kernelTime = threadsTableModel->getKernelTimeByTid(lastSelectedTid);
        QString kernelTimeStr = QString::number(kernelTime);
        cpuKernelTime->setText(QString("Время ядра ЦП: %1 с").arg(kernelTimeStr));

        // Время проведенноё потоком в режиме пользователя:
        double userTime = threadsTableModel->getUserTimeByTid(lastSelectedTid);
        QString userTimeStr = QString::number(userTime);
        cpuUserTime->setText(QString("Время пользователя ЦП: %1 с").arg(userTimeStr));
    }
}

// Освободить память выделенную под диаграмму процессов:
void MainWindow::destroyProcessesChart()
{
    if (processesChart)
    {
        foreach (QAbstractSeries* series, processesChart->series())
        {
            foreach (QAbstractAxis* axis, series->attachedAxes())
            {
                series->detachAxis(axis);
            }
            processesChart->removeSeries(series);
        }
    }

    if (processesAxisTime)
    {
        delete processesAxisTime;
        processesAxisTime = nullptr;
    }

    if (processesAxisMemory)
    {
        delete processesAxisMemory;
        processesAxisMemory = nullptr;
    }

    if (processesAxisThreadsCount)
    {
        delete processesAxisThreadsCount;
        processesAxisThreadsCount = nullptr;
    }

    if (processesChart)
    {
        delete processesChart;
        processesChart = nullptr;
    }

    if (processesChartView)
    {
        processesChartView->setParent(nullptr);
        delete processesChartView;
        processesChartView = nullptr;
    }
}

// Создать диаграмму процессов:
void MainWindow::createProcessesChart()
{
}

// Освободить память выделенную под диаграмму потоков:
void MainWindow::destroyThreadsChart()
{
}

// Создать диаграмму потоков:
void MainWindow::createThreadsChart()
{
}
