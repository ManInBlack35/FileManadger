#ifndef RECURSIV_H
#define RECURSIV_H
#include <QString>
#include <QDir>
#include <QThread>

class Recursiv : public QObject
{
    Q_OBJECT

public:
    Recursiv();

public slots:
    void do_recurs (const QString &path, bool proper1, bool proper2, int sortingprop);
    void stop();

signals:
    void done ();
    void toProg (const QString& str);

private:
    int tab;
    bool RProper;
    bool lProper;
    bool run;
    QDir::SortFlag sortf;
    QString filelist;
    void recurs(const QString & path);
    void ls_print(QString path);
    QStringList fileList;
};

#endif // RECURSIV_H
