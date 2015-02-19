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
    void do_recurs (const QString & path, bool proper1, bool proper2, int sortingprop);

signals:
    void done (const QString& str);

private:
    int tab;
    bool RProper;
    bool lProper;
    QDir::SortFlag sortf;
    QString filelist;
    QString recurs (const QString & path);
    QString ls_print(QString path);
};

#endif // RECURSIV_H
