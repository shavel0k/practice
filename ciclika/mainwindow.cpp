#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "materialwindow.h"
#include <QMessageBox>
#include <QSqlQuery>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    classModel(new QSqlQueryModel(this)),
    materialModel(new QSqlQueryModel(this))
{
    ui->setupUi(this);

    classModel->setQuery("SELECT id, name FROM classes");
    if (classModel->lastError().isValid())
    {
        QMessageBox::critical(this, "Ошибка", classModel->lastError().text());
    }

    ui->comboBox->setModel(classModel);
    ui->comboBox->setModelColumn(1);
    ui->comboBox->setCurrentIndex(-1);

    ui->listView->setModel(materialModel);

    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onClassSelected(int)));
    connect(ui->listView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onMaterialSelected(QModelIndex)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onClassSelected(int index)
{
    if (index < 0) return;

    int classId = classModel->data(classModel->index(index, 0)).toInt();

    QString query = QString(
        "SELECT m.id, m.name "
        "FROM materials m "
        "JOIN materials_in_classes mic ON m.id = mic.material_id "
        "WHERE mic.class_id = %1"
    ).arg(classId);

    materialModel->setQuery(query);
    if (materialModel->lastError().isValid())
    {
        QMessageBox::critical(this, "Ошибка", materialModel->lastError().text());
    }

    ui->listView->setModelColumn(1);
}

void MainWindow::onMaterialSelected(const QModelIndex &index)
{
    if (!index.isValid()) return;

    int materialId = materialModel->data(materialModel->index(index.row(), 0)).toInt();

    MaterialWindow *mw = new MaterialWindow(materialId);
    mw->show();
}
