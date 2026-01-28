#include "DialogKlient.h"
#include "Klient.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>

DialogKlient::DialogKlient(QWidget *parent)
    : QDialog(parent), m_klient(nullptr)
{
    setupUI();
}

DialogKlient::DialogKlient(Klient *klient, QWidget *parent)
    : QDialog(parent), m_klient(klient)
{
    setupUI();

    if (m_klient) {
        setWindowTitle("Edytuj klienta");
        m_lineEditImie->setText(QString::fromStdString(m_klient->getImie()));
        m_lineEditNazwisko->setText(QString::fromStdString(m_klient->getNazwisko()));
    }
}

void DialogKlient::setupUI()
{
    setWindowTitle("Dodaj klienta");

    auto *mainLayout = new QVBoxLayout(this);
    auto *formLayout = new QFormLayout();

    m_lineEditImie = new QLineEdit;
    m_lineEditNazwisko = new QLineEdit;
    m_lineEditAdres = new QLineEdit;
    m_lineEditTelefon = new QLineEdit;
    m_lineEditEmail = new QLineEdit;

    formLayout->addRow("Imię*", m_lineEditImie);
    formLayout->addRow("Nazwisko*", m_lineEditNazwisko);
    formLayout->addRow("Adres", m_lineEditAdres);
    formLayout->addRow("Telefon", m_lineEditTelefon);
    formLayout->addRow("Email", m_lineEditEmail);

    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, [=]() {
        if (m_lineEditImie->text().isEmpty() || m_lineEditNazwisko->text().isEmpty()) {
            QMessageBox::warning(this, "Błąd", "Imię i nazwisko są wymagane.");
            return;
        }
        accept();
    });
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonBox);
}

Klient* DialogKlient::getKlient() const
{

    if (m_klient) {

        return m_klient;
    } else {
        if (!m_lineEditImie->text().isEmpty() && !m_lineEditNazwisko->text().isEmpty()) {
            return new Klient(m_lineEditImie->text().toStdString(),
                              m_lineEditNazwisko->text().toStdString());

        }
    }
    return nullptr;
}
