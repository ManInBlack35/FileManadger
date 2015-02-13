#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    int myexec(const QString & str);
    ~Dialog();

signals:
    void check(const bool & ch);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

public:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
