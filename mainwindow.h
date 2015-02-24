#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QMap>
#include <QDir>
#include <QVector>
#include <QThread>
#include <QProgressBar>

#include "dialog.h"
#include "recursiv.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum Commands {LS_COM=1, CD_COM,CP_COM,MV_COM, PWD_COM, CAT_COM,RM_COM};
    QMap <Commands,QString> map;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_lineEdit_returnPressed();
    void checked(const bool & ch);
    void recursiveslot2 (const QString &str);
    void recursiveslot ();

signals:
    void onrecurs (const QString & path, bool proper1, bool proper2, int sortingprop);
    void stopProcess ();

protected:
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    QThread *myThread;
    QString toscreen;
    QScopedPointer<Dialog> remdialog;
    Ui::MainWindow *ui;
    bool fProper,lProper, rProper, sProper,bProper,RProper;
    bool is_ok;
    QDir::SortFlag sortf;
    QString dirdel(const QString & path);
    QString filedel(const QString & path);
    QString recurs2(QString path);
    QVector <QString> log_command;
    void remove_com(QStringList list);
    QString recurs(QString path);
    QString ls_print(QString path);
    void Ls_com(QStringList & proper);
    QStringList my_split(QString & str);
    QDir dir;
    QString command;
    Recursiv* rv;
    int log_curs;
};

#endif // MAINWINDOW_H
