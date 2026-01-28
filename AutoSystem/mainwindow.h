#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextEdit>
#include <QHeaderView>
#include <QInputDialog>
#include <QDateTime>
#include <QGroupBox>
#include <QSplitter>
#include <QTabWidget>
#include <QColor>
#include <QBrush>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QDate>
#include <QTextStream>
#include <QFile>

#include "Osoba.h"
#include "Klient.h"
#include "Pracownik.h"
#include "Samochod.h"
#include "Zlecenie.h"
#include "Czesc.h"
#include "Warsztat.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddClientClicked();
    void onAddOrderClicked();
    void onAddMechanicClicked();
    void onAssignMechanicClicked();
    void onChangeStatusClicked();
    void onGenerateReportClicked();
    void onLoadDataClicked();
    void onSaveDataClicked();
    void onAddPartToOrderClicked();
    void onAddPartClicked();

private:
    void setupUI();
    void setupMenuBar();
    void refreshTables();
    void updateOrderDetails();

    QMenuBar *m_menuBar;
    QAction *m_actionExit;
    QAction *m_actionLoad;
    QAction *m_actionSave;
    QAction *m_actionReport;

    QWidget *m_centralWidget;
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_topLayout;

    QPushButton *m_btnAddClient;
    QPushButton *m_btnAddOrder;
    QPushButton *m_btnAddMechanic;
    QPushButton *m_btnAddPartToOrder;
    QPushButton *m_btnAddPart;
    QPushButton *m_btnGenerateReport;
    QPushButton *m_btnLoadData;
    QPushButton *m_btnSaveData;

    QTabWidget *m_tabWidget;
    QWidget *m_clientTab;
    QVBoxLayout *m_clientTabLayout;
    QTableWidget *m_tableClients;
    QHBoxLayout *m_clientButtonsLayout;
    QPushButton *m_btnEditClient;
    QPushButton *m_btnDeleteClient;

    QWidget *m_mechanicTab;
    QVBoxLayout *m_mechanicTabLayout;
    QTableWidget *m_tableMechanics;
    QHBoxLayout *m_mechanicButtonsLayout;
    QPushButton *m_btnEditMechanic;
    QPushButton *m_btnDeleteMechanic;

    QWidget *m_orderTab;
    QVBoxLayout *m_orderTabLayout;
    QTableWidget *m_tableOrders;
    QHBoxLayout *m_orderButtonsLayout;
    QPushButton *m_btnAssignMechanic;
    QPushButton *m_btnChangeStatus;
    QPushButton *m_btnEditOrder;
    QPushButton *m_btnDeleteOrder;

    QWidget *m_partsTab;
    QVBoxLayout *m_partsTabLayout;
    QTableWidget *m_tableParts;
    QHBoxLayout *m_partsButtonsLayout;
    QPushButton *m_btnEditPart;
    QPushButton *m_btnDeletePart;

    QWidget *m_carsTab;
    QVBoxLayout *m_carsTabLayout;
    QTableWidget *m_tableCars;
    QHBoxLayout *m_carButtonsLayout;
    QPushButton *m_btnEditCar;
    QPushButton *m_btnDeleteCar;

    QLabel *m_statusLabel;
    Warsztat m_system;
};

#endif
