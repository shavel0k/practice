#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onClassSelected(int index);
    void onMaterialSelected(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QSqlQueryModel *classModel;
    QSqlQueryModel *materialModel;
};

#endif
