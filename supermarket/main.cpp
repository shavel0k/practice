#include <QApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlError>
#include <QTableView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QSqlQuery>
#include <QUiLoader>
#include <QFile>
#include <QDebug>
#include <QDir>

void showTable(QWidget * window, QSqlDatabase & db, QSqlQueryModel * model, QTableView * tableView, const QString & tableName)
{
    QSqlQuery query(db);
    if (!query.exec("SELECT * FROM " + tableName))
    {
        QSqlError error = query.lastError();
        QString errorText = "Не удалось загрузить данные:\n" + error.text();
        QMessageBox::warning(window, "Ошибка", errorText);
        return;
    }

    model->setQuery(query);
    tableView->setModel(model);

    if (model->rowCount() == 0)
    {
        QMessageBox::information(window, "Информация", "Таблица пуста");
    }

    tableView->show();
}

int main(int argc, char * argv[])
{
    QApplication app(argc, argv);
    QString uiPath = QDir::currentPath() + "/supermarket.ui";
    QUiLoader loader;
    QFile file(uiPath);

    bool isFileOpen = file.open(QFile::ReadOnly);
    if (!isFileOpen)
    {
        QString errorMessage = "Не удалось загрузить UI-файл по пути:\n";
        errorMessage += uiPath;
        errorMessage += "\nОшибка: ";
        errorMessage += file.errorString();
        QMessageBox::critical(nullptr, "Ошибка", errorMessage);
        return 1;
    }

    QWidget *window = loader.load(&file);
    file.close();

    QTableView *tableView = window->findChild<QTableView*>("tableView");
    QPushButton *btnShowGoods = window->findChild<QPushButton*>("button1");
    QPushButton *btnShowSales = window->findChild<QPushButton*>("button2");
    QPushButton *btnShowCustomers = window->findChild<QPushButton*>("button3");

    if (!tableView || !btnShowGoods || !btnShowSales || !btnShowCustomers)
    {
        QMessageBox::critical(nullptr, "Ошибка", "Не найдены элементы интерфейса");
        return 1;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("supermarket");
    db.setUserName("postgres");
    db.setPassword("11111111");

    if (!db.open())
    {
        QSqlError error = db.lastError();
        QString errorText = "Не удалось подключиться к базе данных:\n" + error.text();
        QMessageBox::critical(window, "Ошибка подключения", errorText);
        return 1;
    }

    tableView->hide();
    tableView->verticalHeader()->setDefaultSectionSize(40);
    tableView->horizontalHeader()->setDefaultSectionSize(200);

    btnShowGoods->setText("Показать товары");
    btnShowSales->setText("Показать продажи");
    btnShowCustomers->setText("Показать клиентов");

    btnShowGoods->setFixedSize(300, 40);
    btnShowSales->setFixedSize(300, 40);
    btnShowCustomers->setFixedSize(300, 40);

    QSqlQueryModel *model = new QSqlQueryModel(window);

    QObject::connect(btnShowGoods, &QPushButton::clicked, [&]()
    {
        showTable(window, db, model, tableView, "goods");
    });

    QObject::connect(btnShowSales, &QPushButton::clicked, [&]()
    {
        showTable(window, db, model, tableView, "sales");
    });

    QObject::connect(btnShowCustomers, &QPushButton::clicked, [&]()
    {
        showTable(window, db, model, tableView, "customers");
    });

    window->resize(800, 600);
    window->setWindowTitle("Управление супермаркетом");
    window->show();

    return app.exec();
}
