#include "mainwindow.h"
#include <QApplication>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <QDateTime>
#include <QInputDialog>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QSplitter>
#include <QTabWidget>
#include <QColor>
#include <QBrush>
#include <QDate>

class ClientDialog : public QDialog
{
public:
    ClientDialog(QWidget *parent = nullptr) : QDialog(parent) {
        setWindowTitle("Dodaj klienta");
        auto *layout = new QFormLayout(this);
        m_lineEditName = new QLineEdit;
        m_lineEditSurname = new QLineEdit;
        layout->addRow("Imię*", m_lineEditName);
        layout->addRow("Nazwisko*", m_lineEditSurname);
        auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        layout->addWidget(buttonBox);
        connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    }

    QString getName() const { return m_lineEditName->text(); }
    QString getSurname() const { return m_lineEditSurname->text(); }

private:
    QLineEdit *m_lineEditName;
    QLineEdit *m_lineEditSurname;
};

class OrderDialog : public QDialog
{
public:
    OrderDialog(const std::vector<Klient*>& klienci, QWidget *parent = nullptr) : QDialog(parent) {
        setWindowTitle("Dodaj zlecenie");
        auto *layout = new QFormLayout(this);
        m_comboClient = new QComboBox;
        for (const auto& k : klienci) {
            m_comboClient->addItem(QString::fromStdString(k->getImie() + " " + k->getNazwisko()));
        }
        m_lineEditBrand = new QLineEdit;
        m_lineEditModel = new QLineEdit;
        m_spinYear = new QSpinBox;
        m_spinYear->setRange(1900, QDate::currentDate().year());
        m_textEditUsterka = new QTextEdit;

        layout->addRow("Klient:", m_comboClient);
        layout->addRow("Marka:", m_lineEditBrand);
        layout->addRow("Model:", m_lineEditModel);
        layout->addRow("Rok:", m_spinYear);
        layout->addRow("Opis usterki:", m_textEditUsterka);

        auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        layout->addWidget(buttonBox);
        connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    }

    QString getClientName() const { return m_comboClient->currentText(); }
    QString getBrand() const { return m_lineEditBrand->text(); }
    QString getModel() const { return m_lineEditModel->text(); }
    int getYear() const { return m_spinYear->value(); }
    QString getOpisUsterki() const { return m_textEditUsterka->toPlainText(); }
    QComboBox* getClientComboBox() { return m_comboClient; }

private:
    QComboBox *m_comboClient;
    QLineEdit *m_lineEditBrand;
    QLineEdit *m_lineEditModel;
    QSpinBox *m_spinYear;
    QTextEdit *m_textEditUsterka;
};

class AddPartDialog : public QDialog
{
public:
    AddPartDialog(const std::vector<Czesc*>& czesci, QWidget *parent = nullptr) : QDialog(parent) {
        setWindowTitle("Dodaj część do zlecenia");
        auto *layout = new QFormLayout(this);

        m_comboPart = new QComboBox;
        for (const auto& c : czesci) {
            m_comboPart->addItem(QString::fromStdString(c->getNazwa()));
        }

        m_spinQuantity = new QSpinBox;
        m_spinQuantity->setRange(1, 999);

        layout->addRow("Część:", m_comboPart);
        layout->addRow("Ilość:", m_spinQuantity);

        auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        layout->addWidget(buttonBox);
        connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    }

    QString getPartName() const { return m_comboPart->currentText(); }
    int getQuantity() const { return m_spinQuantity->value(); }

private:
    QComboBox *m_comboPart;
    QSpinBox *m_spinQuantity;
};

class PartDialog : public QDialog
{
public:
    PartDialog(QWidget *parent = nullptr) : QDialog(parent) {
        setWindowTitle("Dodaj część");
        auto *layout = new QFormLayout(this);

        m_lineEditName = new QLineEdit;
        m_doubleSpinBoxPrice = new QDoubleSpinBox;
        m_doubleSpinBoxPrice->setRange(0.0, 999999.99);
        m_doubleSpinBoxPrice->setDecimals(2);
        m_spinBoxAmount = new QSpinBox;
        m_spinBoxAmount->setRange(0, 999999);

        layout->addRow("Nazwa*", m_lineEditName);
        layout->addRow("Cena*", m_doubleSpinBoxPrice);
        layout->addRow("Ilość*", m_spinBoxAmount);

        auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        layout->addWidget(buttonBox);
        connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    }

    QString getName() const { return m_lineEditName->text(); }
    double getPrice() const { return m_doubleSpinBoxPrice->value(); }
    int getAmount() const { return m_spinBoxAmount->value(); }

private:
    QLineEdit *m_lineEditName;
    QDoubleSpinBox *m_doubleSpinBoxPrice;
    QSpinBox *m_spinBoxAmount;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    setupMenuBar();
    refreshTables();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    setWindowTitle("System Zarządzania Warsztatem Samochodowym");
    resize(1200, 800);

    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);

    m_mainLayout = new QVBoxLayout(m_centralWidget);

    m_topLayout = new QHBoxLayout();

    m_btnAddClient = new QPushButton("Dodaj klienta", this);
    m_btnAddOrder = new QPushButton("Dodaj zlecenie", this);
    m_btnAddMechanic = new QPushButton("Dodaj mechanika", this);
    m_btnAddPartToOrder = new QPushButton("Dodaj część do zlecenia", this);
    m_btnAddPart = new QPushButton("Dodaj część", this);
    m_btnGenerateReport = new QPushButton("Generuj raport", this);
    m_btnLoadData = new QPushButton("Wczytaj dane", this);
    m_btnSaveData = new QPushButton("Zapisz dane", this);

    m_topLayout->addWidget(m_btnAddClient);
    m_topLayout->addWidget(m_btnAddOrder);
    m_topLayout->addWidget(m_btnAddMechanic);
    m_topLayout->addWidget(m_btnAddPartToOrder);
    m_topLayout->addWidget(m_btnAddPart);
    m_topLayout->addWidget(m_btnGenerateReport);
    m_topLayout->addWidget(m_btnLoadData);
    m_topLayout->addWidget(m_btnSaveData);
    m_topLayout->addStretch();

    m_tabWidget = new QTabWidget(this);

    // Tab Klienci
    m_clientTab = new QWidget;
    m_clientTabLayout = new QVBoxLayout(m_clientTab);
    m_tableClients = new QTableWidget(0, 3);
    m_tableClients->setHorizontalHeaderLabels({"ID", "Imię", "Nazwisko"});
    m_tableClients->horizontalHeader()->setStretchLastSection(true);
    m_tableClients->setAlternatingRowColors(true);
    m_clientButtonsLayout = new QHBoxLayout();
    m_btnEditClient = new QPushButton("Edytuj");
    m_btnDeleteClient = new QPushButton("Usuń");
    m_clientButtonsLayout->addWidget(m_btnEditClient);
    m_clientButtonsLayout->addWidget(m_btnDeleteClient);
    m_clientButtonsLayout->addStretch();
    m_clientTabLayout->addWidget(m_tableClients);
    m_clientTabLayout->addLayout(m_clientButtonsLayout);
    m_tabWidget->addTab(m_clientTab, "Klienci");

    // Tab Mechanicy
    m_mechanicTab = new QWidget;
    m_mechanicTabLayout = new QVBoxLayout(m_mechanicTab);
    m_tableMechanics = new QTableWidget(0, 4);
    m_tableMechanics->setHorizontalHeaderLabels({"ID", "Imię", "Nazwisko", "Stanowisko"});
    m_tableMechanics->horizontalHeader()->setStretchLastSection(true);
    m_tableMechanics->setAlternatingRowColors(true);
    m_mechanicButtonsLayout = new QHBoxLayout();
    m_btnEditMechanic = new QPushButton("Edytuj");
    m_btnDeleteMechanic = new QPushButton("Usuń");
    m_mechanicButtonsLayout->addWidget(m_btnEditMechanic);
    m_mechanicButtonsLayout->addWidget(m_btnDeleteMechanic);
    m_mechanicButtonsLayout->addStretch();
    m_mechanicTabLayout->addWidget(m_tableMechanics);
    m_mechanicTabLayout->addLayout(m_mechanicButtonsLayout);
    m_tabWidget->addTab(m_mechanicTab, "Mechanicy");

    // Tab Zlecenia
    m_orderTab = new QWidget;
    m_orderTabLayout = new QVBoxLayout(m_orderTab);
    m_tableOrders = new QTableWidget(0, 9);
    m_tableOrders->setHorizontalHeaderLabels({"ID", "Klient", "Pojazd", "Usterka", "Mechanik", "Status", "Czynności", "Koszt", "Części"});
    m_tableOrders->horizontalHeader()->setStretchLastSection(true);
    m_tableOrders->setAlternatingRowColors(true);
    m_orderButtonsLayout = new QHBoxLayout();
    m_btnAssignMechanic = new QPushButton("Przypisz mechanika");
    m_btnChangeStatus = new QPushButton("Zmień status");
    m_btnEditOrder = new QPushButton("Edytuj");
    m_btnDeleteOrder = new QPushButton("Usuń");
    m_orderButtonsLayout->addWidget(m_btnChangeStatus);
    m_orderButtonsLayout->addWidget(m_btnAssignMechanic);
    m_orderButtonsLayout->addWidget(m_btnEditOrder);
    m_orderButtonsLayout->addWidget(m_btnDeleteOrder);
    m_orderButtonsLayout->addStretch();
    m_orderTabLayout->addWidget(m_tableOrders);
    m_orderTabLayout->addLayout(m_orderButtonsLayout);
    m_tabWidget->addTab(m_orderTab, "Zlecenia");

    // Tab Części
    m_partsTab = new QWidget;
    m_partsTabLayout = new QVBoxLayout(m_partsTab);
    m_tableParts = new QTableWidget(0, 3);
    m_tableParts->setHorizontalHeaderLabels({"Nazwa", "Cena", "Ilość"});
    m_tableParts->horizontalHeader()->setStretchLastSection(true);
    m_tableParts->setAlternatingRowColors(true);
    m_partsButtonsLayout = new QHBoxLayout();
    m_btnEditPart = new QPushButton("Edytuj");
    m_btnDeletePart = new QPushButton("Usuń");
    m_partsButtonsLayout->addWidget(m_btnEditPart);
    m_partsButtonsLayout->addWidget(m_btnDeletePart);
    m_partsButtonsLayout->addStretch();
    m_partsTabLayout->addWidget(m_tableParts);
    m_partsTabLayout->addLayout(m_partsButtonsLayout);
    m_tabWidget->addTab(m_partsTab, "Części");

    // Tab Samochody
    m_carsTab = new QWidget;
    m_carsTabLayout = new QVBoxLayout(m_carsTab);
    m_tableCars = new QTableWidget(0, 4);
    m_tableCars->setHorizontalHeaderLabels({"ID", "Marka", "Model", "Właściciel"});
    m_tableCars->horizontalHeader()->setStretchLastSection(true);
    m_tableCars->setAlternatingRowColors(true);
    m_carButtonsLayout = new QHBoxLayout();
    m_btnEditCar = new QPushButton("Edytuj");
    m_btnDeleteCar = new QPushButton("Usuń");
    m_carButtonsLayout->addWidget(m_btnEditCar);
    m_carButtonsLayout->addWidget(m_btnDeleteCar);
    m_carButtonsLayout->addStretch();
    m_carsTabLayout->addWidget(m_tableCars);
    m_carsTabLayout->addLayout(m_carButtonsLayout);
    m_tabWidget->addTab(m_carsTab, "Samochody");

    m_statusLabel = new QLabel("Gotowy", this);

    m_mainLayout->addLayout(m_topLayout);
    m_mainLayout->addWidget(m_tabWidget);
    m_mainLayout->addWidget(m_statusLabel);

    connect(m_btnAddClient, &QPushButton::clicked, this, &MainWindow::onAddClientClicked);
    connect(m_btnAddOrder, &QPushButton::clicked, this, &MainWindow::onAddOrderClicked);
    connect(m_btnAddMechanic, &QPushButton::clicked, this, &MainWindow::onAddMechanicClicked);
    connect(m_btnAddPartToOrder, &QPushButton::clicked, this, &MainWindow::onAddPartToOrderClicked);
    connect(m_btnAddPart, &QPushButton::clicked, this, &MainWindow::onAddPartClicked);
    connect(m_btnAssignMechanic, &QPushButton::clicked, this, &MainWindow::onAssignMechanicClicked);
    connect(m_btnChangeStatus, &QPushButton::clicked, this, &MainWindow::onChangeStatusClicked);
    connect(m_btnGenerateReport, &QPushButton::clicked, this, &MainWindow::onGenerateReportClicked);
    connect(m_btnLoadData, &QPushButton::clicked, this, &MainWindow::onLoadDataClicked);
    connect(m_btnSaveData, &QPushButton::clicked, this, &MainWindow::onSaveDataClicked);
}

void MainWindow::setupMenuBar()
{
    m_menuBar = menuBar();
    QMenu *fileMenu = m_menuBar->addMenu("Plik");

    m_actionLoad = new QAction("Wczytaj dane", this);
    m_actionSave = new QAction("Zapisz dane", this);
    m_actionReport = new QAction("Generuj raport", this);
    m_actionExit = new QAction("Zakończ", this);

    fileMenu->addAction(m_actionLoad);
    fileMenu->addAction(m_actionSave);
    fileMenu->addAction(m_actionReport);
    fileMenu->addSeparator();
    fileMenu->addAction(m_actionExit);

    connect(m_actionExit, &QAction::triggered, this, &QWidget::close);
    connect(m_actionLoad, &QAction::triggered, this, &MainWindow::onLoadDataClicked);
    connect(m_actionSave, &QAction::triggered, this, &MainWindow::onSaveDataClicked);
    connect(m_actionReport, &QAction::triggered, this, &MainWindow::onGenerateReportClicked);
}

void MainWindow::refreshTables()
{
    m_tableClients->setRowCount(0);
    int row = 0;
    for (const auto& klient : m_system.getKlienci()) {
        m_tableClients->insertRow(row);
        m_tableClients->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1)));
        m_tableClients->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(klient->getImie())));
        m_tableClients->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(klient->getNazwisko())));
        row++;
    }

    m_tableMechanics->setRowCount(0);
    row = 0;
    for (const auto& p : m_system.getPracownicy()) {
        m_tableMechanics->insertRow(row);
        m_tableMechanics->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1)));
        m_tableMechanics->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(p->getImie())));
        m_tableMechanics->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(p->getNazwisko())));
        m_tableMechanics->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(p->getStanowisko())));
        row++;
    }

    m_tableOrders->setRowCount(0);
    row = 0;
    for (const auto& zlecenie : m_system.getZlecenia()) {
        m_tableOrders->insertRow(row);
        m_tableOrders->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1)));
        m_tableOrders->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(zlecenie->getKlient()->getImie() + " " + zlecenie->getKlient()->getNazwisko())));
        m_tableOrders->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(zlecenie->getPojazd()->getMarka() + " " + zlecenie->getPojazd()->getModel())));
        m_tableOrders->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(zlecenie->getOpisUsterki())));

        QString mechanikInfo = "Brak";
        if (zlecenie->getMechanik()) {
            mechanikInfo = QString::fromStdString(zlecenie->getMechanik()->getImie() + " " + zlecenie->getMechanik()->getNazwisko());
        }
        m_tableOrders->setItem(row, 4, new QTableWidgetItem(mechanikInfo));

        QString status = "Nieznany";
        switch (zlecenie->getStatus()) {
        case Zlecenie::NOWE: status = "NOWE"; break;
        case Zlecenie::W_TRAKCIE: status = "W TRAKCIE"; break;
        case Zlecenie::ZAKONCZONE: status = "ZAKONCZONE"; break;
        }

        auto statusItem = new QTableWidgetItem(status);
        if (status == "NOWE") statusItem->setBackground(QColor(255, 255, 0));
        else if (status == "W TRAKCIE") statusItem->setBackground(QColor(255, 165, 0));
        else if (status == "ZAKONCZONE") statusItem->setBackground(QColor(0, 255, 0));
        m_tableOrders->setItem(row, 5, statusItem);

        QStringList czynnosciList;
        for (const auto& c : zlecenie->getCzynnosci()) {
            czynnosciList << QString::fromStdString(c);
        }
        m_tableOrders->setItem(row, 6, new QTableWidgetItem(czynnosciList.join(", ")));

        // Koszt tylko jeśli status to ZAKONCZONE
        QString kosztStr = "-";
        if (zlecenie->getStatus() == Zlecenie::ZAKONCZONE) {
            kosztStr = QString::number(zlecenie->getKoszt(), 'f', 2) + " zł";
        }
        m_tableOrders->setItem(row, 7, new QTableWidgetItem(kosztStr));

        // Części
        QStringList czesciList;
        for (const auto& cp : zlecenie->getCzesci()) {
            Czesc* czesc = cp.first;
            int ilosc = cp.second;
            czesciList << QString::fromStdString(czesc->getNazwa()) + "(" + QString::number(ilosc) + ")";
        }
        m_tableOrders->setItem(row, 8, new QTableWidgetItem(czesciList.join(", ")));

        row++;
    }

    // Refresh Części table
    m_tableParts->setRowCount(0);
    row = 0;
    for (const auto& czesc : m_system.getCzesci()) {
        m_tableParts->insertRow(row);
        m_tableParts->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(czesc->getNazwa())));
        m_tableParts->setItem(row, 1, new QTableWidgetItem(QString::number(czesc->getCena(), 'f', 2)));
        m_tableParts->setItem(row, 2, new QTableWidgetItem(QString::number(czesc->getIlosc())));
        row++;
    }

    // Refresh Samochody table
    m_tableCars->setRowCount(0);
    row = 0;
    for (const auto& zlecenie : m_system.getZlecenia()) {
        auto pojazd = zlecenie->getPojazd();
        if (pojazd) {
            m_tableCars->insertRow(row);
            m_tableCars->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1)));
            m_tableCars->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(pojazd->getMarka())));
            m_tableCars->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(pojazd->getModel())));
            m_tableCars->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(pojazd->getWlasciciel()->getImie() + " " + pojazd->getWlasciciel()->getNazwisko())));
            row++;
        }
    }
}

void MainWindow::onAddClientClicked()
{
    ClientDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString name = dialog.getName();
        QString surname = dialog.getSurname();
        if (!name.isEmpty() && !surname.isEmpty()) {
            Klient *k = new Klient(name.toStdString(), surname.toStdString());
            m_system.dodajKlienta(k);
            refreshTables();
            m_statusLabel->setText("Dodano klienta: " + name + " " + surname);
        }
    }
}

void MainWindow::onAddOrderClicked()
{
    if (m_system.getKlienci().empty()) {
        QMessageBox::information(this, "Brak klientów", "Najpierw dodaj klienta.");
        return;
    }
    OrderDialog dialog(m_system.getKlienci(), this);
    if (dialog.exec() == QDialog::Accepted) {
        QString brand = dialog.getBrand();
        QString model = dialog.getModel();
        int year = dialog.getYear();
        QString opisUsterki = dialog.getOpisUsterki();
        if (!brand.isEmpty() && !model.isEmpty() && !opisUsterki.isEmpty()) {
            QString selectedClientText = dialog.getClientName();
            Klient* klient = nullptr;
            for (const auto& k : m_system.getKlienci()) {
                if (QString::fromStdString(k->getImie() + " " + k->getNazwisko()) == selectedClientText) {
                    klient = k;
                    break;
                }
            }
            if (klient) {
                Samochod *s = new Samochod(brand.toStdString(), model.toStdString(), year, klient);
                Zlecenie *z = new Zlecenie(klient, s, opisUsterki.toStdString());
                m_system.dodajZlecenie(z);
                refreshTables();
                m_statusLabel->setText("Dodano zlecenie dla: " + selectedClientText);
            } else {
                QMessageBox::warning(this, "Błąd", "Nie znaleziono wybranego klienta.");
            }
        } else {
            QMessageBox::warning(this, "Błąd", "Marka, model i opis usterki są wymagane.");
        }
    }
}

void MainWindow::onAddMechanicClicked()
{
    bool ok;
    QString name = QInputDialog::getText(this, "Dodaj mechanika", "Imię:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;
    QString surname = QInputDialog::getText(this, "Dodaj mechanika", "Nazwisko:", QLineEdit::Normal, "", &ok);
    if (!ok || surname.isEmpty()) return;
    QString pass = "default123";
    Mechanik* mech = new Mechanik(name.toStdString(), surname.toStdString(), pass.toStdString());
    m_system.dodajPracownika(mech);
    refreshTables();
    m_statusLabel->setText("Dodano mechanika: " + name + " " + surname);
}

void MainWindow::onAssignMechanicClicked()
{
    int currentRow = m_tableOrders->currentRow();
    if (currentRow < 0) {
        QMessageBox::information(this, "Brak zlecenia", "Wybierz zlecenie z listy.");
        return;
    }
    if (currentRow >= static_cast<int>(m_system.getZlecenia().size())) {
        QMessageBox::warning(this, "Błąd", "Nie można odnaleźć zlecenia.");
        return;
    }

    Zlecenie* zlecenie = m_system.getZlecenia()[currentRow];
    QStringList mechanicNames;
    Mechanik* selectedMechanic = nullptr;
    int selectedIndex = -1;
    int idx = 0;
    for (const auto& p : m_system.getPracownicy()) {
        Mechanik* mech = dynamic_cast<Mechanik*>(p);
        if (mech) {
            mechanicNames << QString::fromStdString(mech->getImie() + " " + mech->getNazwisko());
            if (mech == zlecenie->getMechanik()) {
                selectedIndex = idx;
            }
            idx++;
        }
    }
    if (mechanicNames.isEmpty()) {
        QMessageBox::information(this, "Brak mechaników", "Najpierw dodaj mechaników.");
        return;
    }

    bool ok;
    QString selectedName = QInputDialog::getItem(this, "Przypisz mechanika", "Wybierz mechanika:", mechanicNames, selectedIndex, false, &ok);
    if (!ok) return;

    int searchIdx = 0;
    for (const auto& p : m_system.getPracownicy()) {
        Mechanik* mech = dynamic_cast<Mechanik*>(p);
        if (mech && QString::fromStdString(mech->getImie() + " " + mech->getNazwisko()) == selectedName) {
            selectedMechanic = mech;
            break;
        }
        searchIdx++;
    }

    if (selectedMechanic) {
        zlecenie->ustawMechanika(selectedMechanic);
        refreshTables();
        m_statusLabel->setText("Przypisano mechanika " + selectedName + " do zlecenia.");
    } else {
        QMessageBox::warning(this, "Błąd", "Nie udało się przypisać mechanika.");
    }
}

void MainWindow::onChangeStatusClicked()
{
    int currentRow = m_tableOrders->currentRow();
    if (currentRow < 0) {
        QMessageBox::information(this, "Brak zlecenia", "Wybierz zlecenie z listy.");
        return;
    }
    if (currentRow >= static_cast<int>(m_system.getZlecenia().size())) {
        QMessageBox::warning(this, "Błąd", "Nie można odnaleźć zlecenia.");
        return;
    }

    Zlecenie* zlecenie = m_system.getZlecenia()[currentRow];
    QStringList statuses = {"NOWE", "W TRAKCIE", "ZAKONCZONE"};
    QString currentStatus = "Nieznany";
    switch (zlecenie->getStatus()) {
    case Zlecenie::NOWE: currentStatus = "NOWE"; break;
    case Zlecenie::W_TRAKCIE: currentStatus = "W TRAKCIE"; break;
    case Zlecenie::ZAKONCZONE: currentStatus = "ZAKONCZONE"; break;
    }
    int currentIndex = statuses.indexOf(currentStatus);

    bool ok;
    QString selectedStatus = QInputDialog::getItem(this, "Zmień status", "Wybierz status:", statuses, currentIndex, false, &ok);
    if (!ok) return;

    if (selectedStatus == "NOWE") {
        zlecenie->ustawStatus(Zlecenie::NOWE);
    } else if (selectedStatus == "W TRAKCIE") {
        zlecenie->ustawStatus(Zlecenie::W_TRAKCIE);
    } else if (selectedStatus == "ZAKONCZONE") {
        zlecenie->ustawStatus(Zlecenie::ZAKONCZONE);
        // Symulacja SMS po zakończeniu zlecenia
        QMessageBox::information(this, "SMS", "Klient został powiadomiony o zakończeniu zlecenia.");
    }

    refreshTables();
    m_statusLabel->setText("Zmieniono status zlecenia na: " + selectedStatus);
}

void MainWindow::onGenerateReportClicked()
{
    QString path = QFileDialog::getSaveFileName(this, "Zapisz raport", "", "Pliki tekstowe (*.txt)");
    if (!path.isEmpty()) {
        m_system.generujRaport(path.toStdString());
        QMessageBox::information(this, "Raport", "Raport zapisany do: " + path);
        m_statusLabel->setText("Raport wygenerowany: " + path);
    }
}

void MainWindow::onLoadDataClicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Wczytaj dane", "", "Pliki tekstowe (*.txt)");
    if (!path.isEmpty()) {
        m_system.wczytaj(path.toStdString());
        refreshTables();
        QMessageBox::information(this, "Wczytano", "Dane wczytane z: " + path);
        m_statusLabel->setText("Dane wczytane z: " + path);
    }
}

void MainWindow::onSaveDataClicked()
{
    QString path = QFileDialog::getSaveFileName(this, "Zapisz dane", "", "Pliki tekstowe (*.txt)");
    if (!path.isEmpty()) {
        m_system.zapisz(path.toStdString());
        QMessageBox::information(this, "Zapisano", "Dane zapisane do: " + path);
        m_statusLabel->setText("Dane zapisane do: " + path);
    }
}

void MainWindow::onAddPartToOrderClicked()
{
    int currentRow = m_tableOrders->currentRow();
    if (currentRow < 0) {
        QMessageBox::information(this, "Brak zlecenia", "Wybierz zlecenie z listy.");
        return;
    }
    if (currentRow >= static_cast<int>(m_system.getZlecenia().size())) {
        QMessageBox::warning(this, "Błąd", "Nie można odnaleźć zlecenia.");
        return;
    }

    if (m_system.getCzesci().empty()) {
        QMessageBox::information(this, "Brak części", "Najpierw dodaj części.");
        return;
    }

    Zlecenie* zlecenie = m_system.getZlecenia()[currentRow];
    AddPartDialog dialog(m_system.getCzesci(), this);
    if (dialog.exec() == QDialog::Accepted) {
        QString partName = dialog.getPartName();
        int quantity = dialog.getQuantity();

        Czesc* selectedPart = nullptr;
        for (const auto& c : m_system.getCzesci()) {
            if (QString::fromStdString(c->getNazwa()) == partName) {
                selectedPart = c;
                break;
            }
        }

        if (selectedPart) {
            zlecenie->dodajCzesc(selectedPart, quantity);
            refreshTables();
            m_statusLabel->setText("Dodano część " + partName + " (" + QString::number(quantity) + " szt.) do zlecenia.");
        } else {
            QMessageBox::warning(this, "Błąd", "Nie znaleziono wybranej części.");
        }
    }
}

void MainWindow::onAddPartClicked()
{
    PartDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString name = dialog.getName();
        double price = dialog.getPrice();
        int amount = dialog.getAmount();

        if (!name.isEmpty() && price >= 0 && amount >= 0) {
            Czesc *c = new Czesc(name.toStdString(), price, amount);
            m_system.dodajCzesc(c);
            refreshTables();
            m_statusLabel->setText("Dodano część: " + name);
        } else {
            QMessageBox::warning(this, "Błąd", "Nazwa, cena i ilość są wymagane i muszą być nieujemne.");
        }
    }
}
