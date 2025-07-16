#include "materialwindow.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QVBoxLayout>
#include <QComboBox>
#include <QTableView>
#include <QLabel>
#include <QHeaderView>
#include <QDesktopWidget>
#include <QApplication>

MaterialWindow::MaterialWindow(int materialId, QWidget *parent) :
    QWidget(parent),
    m_materialId(materialId),
    m_comboBox(new QComboBox(this)),
    m_tableView(new QTableView(this)),
    m_tableView2(new QTableView(this)),
    m_label(new QLabel("Характеристики:", this)),
    m_label2(new QLabel("Источники:", this)),
    m_valuesModel(new QSqlQueryModel(this)),
    m_sourcesModel(new QSqlQueryModel(this))
{
    setupUi();
    loadCharacteristics();

    m_tableView->hide();
    m_label->hide();
    m_tableView2->hide();
    m_label2->hide();

    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int width = screenGeometry.width() * 0.8;
    int height = screenGeometry.height() * 0.8;
    resize(width, height);
    setMinimumSize(1000, 700);
}

MaterialWindow::~MaterialWindow(){}

void MaterialWindow::setupUi()
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    m_comboBox->setObjectName("comboBox");

    layout->addWidget(m_comboBox);
    layout->addWidget(m_label);
    layout->addWidget(m_tableView);
    layout->addWidget(m_label2);
    layout->addWidget(m_tableView2);

    m_tableView->setModel(m_valuesModel);
    m_tableView2->setModel(m_sourcesModel);

    m_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableView2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(m_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onCharacteristicSelected(int)));
}

void MaterialWindow::loadCharacteristics()
{
    QSqlQuery query;
    query.prepare("SELECT id, characteristic FROM material_characteristics WHERE material_id = :id");
    query.bindValue(":id", m_materialId);

    if (!query.exec())
    {
        QMessageBox::critical(this, "Ошибка", query.lastError().text());
        return;
    }

    m_comboBox->clear();
    m_comboBox->addItem("Выберите характеристику");

    while (query.next())
    {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        m_comboBox->addItem(name, id);
    }
}

void MaterialWindow::onCharacteristicSelected(int index)
{
    m_tableView->hide();
    m_label->hide();
    m_tableView2->hide();
    m_label2->hide();

    if (index <= 0) return;

    int charId = m_comboBox->itemData(index).toInt();
    bool hasValues = false;
    bool hasSources = false;

    QString valuesQueryStr = QString(
        "SELECT time, temperature, value "
        "FROM characteristics_values "
        "WHERE mat_char_id = %1"
    ).arg(charId);

    m_valuesModel->setQuery(valuesQueryStr);
    if (m_valuesModel->lastError().isValid())
    {
        QMessageBox::critical(this, "Ошибка", m_valuesModel->lastError().text());
    }
    else if (m_valuesModel->rowCount() > 0)
    {
        m_tableView->show();
        m_label->show();
        hasValues = true;
        m_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    }

    QString sourcesQueryStr = QString(
        "SELECT sourse, application, table_num, picture, page "
        "FROM characteristics_sourses "
        "WHERE mat_char_id = %1"
    ).arg(charId);

    m_sourcesModel->setQuery(sourcesQueryStr);
    if (m_sourcesModel->lastError().isValid())
    {
        QMessageBox::critical(this, "Ошибка", m_sourcesModel->lastError().text());
    }
    else if (m_sourcesModel->rowCount() > 0)
    {
        m_tableView2->show();
        m_label2->show();
        hasSources = true;
        m_tableView2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    }

    if (!hasValues && !hasSources)
    {
        QMessageBox::information(this, "Информация", "Для выбранной характеристики нет данных");
    }
}
