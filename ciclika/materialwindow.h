#ifndef MATERIALWINDOW_H
#define MATERIALWINDOW_H

#include <QWidget>
#include <QtSql>

class QComboBox;
class QTableView;
class QLabel;
class QSqlQueryModel;

class MaterialWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MaterialWindow(int materialId, QWidget *parent = 0);
    ~MaterialWindow();

private slots:
    void onCharacteristicSelected(int index);

private:
    int m_materialId;

    QComboBox *m_comboBox;
    QTableView *m_tableView;
    QTableView *m_tableView2;
    QLabel *m_label;
    QLabel *m_label2;

    QSqlQueryModel *m_valuesModel;
    QSqlQueryModel *m_sourcesModel;

    void setupUi();
    void loadCharacteristics();
};

#endif
