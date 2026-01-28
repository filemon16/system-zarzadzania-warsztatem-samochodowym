#ifndef DIALOGKLIENT_H
#define DIALOGKLIENT_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QCheckBox>
#include <QDateEdit>
#include <QComboBox>


class Klient;

class DialogKlient : public QDialog
{
    Q_OBJECT

public:
    explicit DialogKlient(QWidget *parent = nullptr);
    explicit DialogKlient(Klient *klient, QWidget *parent = nullptr);

    Klient* getKlient() const;

private:
    void setupUI();

    QLineEdit *m_lineEditImie;
    QLineEdit *m_lineEditNazwisko;
    QLineEdit *m_lineEditAdres;
    QLineEdit *m_lineEditTelefon;
    QLineEdit *m_lineEditEmail;

    Klient *m_klient;
};

#endif
