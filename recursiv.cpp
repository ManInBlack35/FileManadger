#include <QStringList>
#include <QDir>
#include <QTextStream>
#include <QDateTime>
#include "recursiv.h"
#include "mainwindow.h"

Recursiv::Recursiv()
{
}

QString Recursiv::ls_print(QString path)
{
    QString str,str1,temp_path;
    QFileInfo info(path);
    QStringList list;
    if (info.isDir())
    {
        QDir dirtemp(path);
        dirtemp.setSorting(sortf);
        str.append(path+":\n");
        list=dirtemp.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);
        QString tot;
        tot.setNum(list.size());
        str.append("total: "+tot+"\n");
    }else list.append("");
    if (lProper==false)
        for (int i=0; i<list.size();++i)
        {
            temp_path=path+"\\"+list[i];
            QFileInfo dirinfo(temp_path);
            bool is_dir=dirinfo.isDir();
                if (is_dir)
                {
                    list[i]="["+list[i]+"]";
                }
                str.append(list[i]+'\n');
        }
    else
        for (int i=0; i<list.size();++i)
        {

            QTextStream strim(&str1);
            strim.setFieldAlignment(QTextStream::AlignCenter);
            QFileInfo dirinfo(path);
            if (!dirinfo.isFile())
            {
                temp_path=path+"\\"+list[i];
                QFileInfo temp_info(temp_path);
                dirinfo=temp_info;
            }
            if(dirinfo.isFile()) strim<<"f";
            else
            if(dirinfo.isDir()) strim<<"d";
            else
            if(dirinfo.isSymLink ()) strim<<"l";
            else strim<<"-";
            if(dirinfo.isExecutable ()) strim<<"e";
            else strim<<"-";
            if(dirinfo.isHidden()) strim<<"h";
            else strim<<"-";
            if(dirinfo.isReadable()) strim<<"r";
            else strim<<"-";
            if(dirinfo.isWritable ()) strim<<"w";
            else strim<<"-";
            strim.setFieldWidth(15);
            strim.setFieldAlignment(QTextStream::AlignRight);
            strim<<dirinfo.size ();
            strim.setFieldWidth(0);
            strim<<" "<<dirinfo.created().toLocalTime().toString();
            strim.setFieldWidth(0);
            if (dirinfo.isDir())
            {
                strim<<" [" <<dirinfo.fileName ()<<"]\n";
            }
                else
                strim<<' '<<dirinfo.fileName ()<<'\n';
            str.append(str1);
        }
    str.append("\n");
    return str;
}

void Recursiv::do_recurs (const QString & path, bool proper1, bool proper2, int sortingprop)
{
    lProper=proper1;
    RProper=proper2;
    sortf=static_cast<QDir::SortFlag> (sortingprop);
    filelist=recurs (path);
    emit done(filelist);
}

QString Recursiv::recurs (const QString & path)
{
    QString str;
    QFileInfo info(path); //взнаєм інформацію про отриманий шлях
    if (info.isDir()) //перевіряємо чи це директорія(папка)
    {
        QDir dir_rec(path); //
        dir_rec.setSorting(sortf);
        if (RProper==true)
        {
            QStringList list_rec(dir_rec.entryList(QDir::AllDirs | QDir::NoDotAndDotDot));
            QString path1=dir_rec.canonicalPath();
            str.append(ls_print(path1)+"\n");

            if (list_rec.size()>0)
                for (int i=0; i<list_rec.size();++i)
                {
                    path1=dir_rec.canonicalPath()+"\\"+list_rec[i];
                    str.append(recurs(path1));
                }
        }else
        {
            QString path=dir_rec.canonicalPath();
            str.append(ls_print(path));
        }
    } else str.append(ls_print(path));
    return str;
}
