#include "mainwindow.h"
#include <QApplication>
#include <QtSql>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("ciclika");
    db.setUserName("postgres");
    db.setPassword("11111111");

    if (!db.open())
    {
        QMessageBox::critical(0, "Ошибка", "Не удалось подключиться к базе данных: " + db.lastError().text());
        return 1;
    }

    MainWindow w;
    w.show();
    return a.exec();
}
