#include <QStringList>
#include <QDir>
#include <QTextStream>
#include <QDateTime>
#include "recursiv.h"
#include "mainwindow.h"

#define VERSION_2

Recursiv::Recursiv() //:QObject(parent)
{
    tab=-1; //поправка для табуляції
}

#ifdef VERSION_2 //вивід деревоподібно при рекурсії

QString Recursiv::ls_print(QString path)
{
    QString str,str1,temp_path;
    QFileInfo info(path); //отримуємо інформацію про даний шлях
    QStringList list;

        if (info.isDir()) //якщо папка
        {
            QDir dirtemp(path);
            dirtemp.setSorting(sortf); // сортуємо
            if (RProper)
            {
                list=dirtemp.entryList(QDir::Files); //список файлів
            }
            else
            {
                str.append(path+":\n");
                list=dirtemp.entryList(QDir::AllEntries | QDir::NoDotAndDotDot); //список файлів і папок
                QString tot;
                tot.setNum(list.size()); //рахуємо загальну кількість файлів та папок
                str.append("total: "+tot+"\n");
            }
        }else list.append(path);
        if (lProper==false)  // якщо виключено детальний вивід
        {
            if (RProper)  // для рекурсії
            {
                for (int i=0; i<list.size();++i) //додаємо до виведення кожний файл в папці
                {
                    for (int j=tab;j>=0;--j)
                        str.append(" │"); // додаємо пропуски перед файлами і папками
                    str.replace(str.size()-1,1,"├");
                    //str.append("├");
                    str.append(list[i]+'\n');
                }
            }
            else // не для рекурсії
            {
                for (int i=0; i<list.size();++i) //додаємо до виведення кожний файл або папку в папці
                {
                    temp_path=path+"\\"+list[i];
                    QFileInfo dirinfo(temp_path);
                    bool is_dir=dirinfo.isDir();
                        if (is_dir)  // якщо папка то берем її в квадратні дужки
                        {
                            list[i]="["+list[i]+"]";
                        }
                        str.append(list[i]+'\n');
                }
            }
        }
        else
        {
            if (RProper==false) // якщо не рекурсія
                for (int i=0; i<list.size();++i)
                {

                    QTextStream strim(&str1);  //створюємо текстовий потік пля можливості редагування зсувів тексту
                    strim.setFieldAlignment(QTextStream::AlignCenter); //виводимо по центру
                    QFileInfo dirinfo(path);
                    if (!dirinfo.isFile())
                    {
                        temp_path=path+"\\"+list[i];
                        QFileInfo temp_info(temp_path);
                        dirinfo=temp_info;
                    }
                    if(dirinfo.isFile()) strim<<"f";  //файл
                    else
                    if(dirinfo.isDir()) strim<<"d";  //директорія
                    else
                    if(dirinfo.isSymLink ()) strim<<"l";  // ярлик
                    else strim<<"-";
                    if(dirinfo.isExecutable ()) strim<<"e"; // виконавча програма
                    else strim<<"-";
                    if(dirinfo.isHidden()) strim<<"h";  //прихований
                    else strim<<"-";
                    if(dirinfo.isReadable()) strim<<"r"; //читабельний
                    else strim<<"-";
                    if(dirinfo.isWritable ()) strim<<"w"; //писабельний
                    else strim<<"-";
                    strim.setFieldWidth(15);
                    strim.setFieldAlignment(QTextStream::AlignRight);
                    strim<<dirinfo.size ();  //розмір
                    strim.setFieldWidth(0);
                    strim<<" "<<dirinfo.created().toLocalTime().toString();  //дата та час створення
                    strim.setFieldWidth(0);
                    if (dirinfo.isDir())
                    {
                        strim<<" [" <<dirinfo.fileName ()<<"]\n";  //якщо папка берем в квадратні дужки
                    }
                        else
                        strim<<' '<<dirinfo.fileName ()<<'\n';
                    str.append(str1);
                    str1.clear();
                }
        }

    return str;
}
#else  //виведення типу стандартного в терміналі

QString Recursiv::ls_print(QString path)
{
    QString str,str1,temp_path;
    QFileInfo info(path); //отримуємо інформацію про даний шлях
    QStringList list;
    if (info.isDir())   //якщо папка
    {
        QDir dirtemp(path);
        dirtemp.setSorting(sortf);  // сортуємо
        str.append(path+":\n");
        list=dirtemp.entryList(QDir::AllEntries | QDir::NoDotAndDotDot); //список файлів і папок
        QString tot;
        tot.setNum(list.size());  //рахуємо загальну кількість файлів та папок
        str.append("total: "+tot+"\n");
    }else list.append("");
    if (lProper==false)
        for (int i=0; i<list.size();++i) //проходимо по кожному елементу списку з файлами та папками
        {
            temp_path=path+"\\"+list[i];
            QFileInfo dirinfo(temp_path);
            bool is_dir=dirinfo.isDir();
                if (is_dir)
                {
                    list[i]="["+list[i]+"]";  // папку виводимо в квадратних дужках
                }
                str.append(list[i]+'\n');  // файл просто виводимо
        }
    else  // якщо увімкнуто детальний вівід
        for (int i=0; i<list.size();++i)
        {

            QTextStream strim(&str1);  //створюємо текстовий потік пля можливості редагування зсувів тексту
            strim.setFieldAlignment(QTextStream::AlignCenter); //виводимо по центру
            QFileInfo dirinfo(path);
            if (!dirinfo.isFile())
            {
                temp_path=path+"\\"+list[i];
                QFileInfo temp_info(temp_path);
                dirinfo=temp_info;
            }
            if(dirinfo.isFile()) strim<<"f";  //файл
            else
            if(dirinfo.isDir()) strim<<"d";  //директорія
            else
            if(dirinfo.isSymLink ()) strim<<"l";  // ярлик
            else strim<<"-";
            if(dirinfo.isExecutable ()) strim<<"e"; // виконавча програма
            else strim<<"-";
            if(dirinfo.isHidden()) strim<<"h";  //прихований
            else strim<<"-";
            if(dirinfo.isReadable()) strim<<"r"; //читабельний
            else strim<<"-";
            if(dirinfo.isWritable ()) strim<<"w"; //писабельний
            else strim<<"-";
            strim.setFieldWidth(15);
            strim.setFieldAlignment(QTextStream::AlignRight);
            strim<<dirinfo.size ();  //розмір
            strim.setFieldWidth(0);
            strim<<" "<<dirinfo.created().toLocalTime().toString();  //дата та час створення
            strim.setFieldWidth(0);
            if (dirinfo.isDir())
            {
                strim<<" [" <<dirinfo.fileName ()<<"]\n";  //якщо папка берем в квадратні дужки
            }
                else
                strim<<' '<<dirinfo.fileName ()<<'\n';
            str.append(str1);
            str1.clear();
        }
    str.append("\n");
    return str;
}
#endif

void Recursiv::do_recurs (const QString & path, bool proper1, bool proper2, int sortingprop) //виконавчий слот
{
    lProper=proper1;
    RProper=proper2;
    sortf=static_cast<QDir::SortFlag> (sortingprop);  //пораметр сортування
    filelist=recurs (path);
    emit toProg(filelist);//зворотній зв'язок
    emit done();
}

QString Recursiv::recurs (const QString & path)
{
    QString str;

    QFileInfo info(path); //взнаєм інформацію про отриманий шлях
    if (info.isDir()) //перевіряємо чи це директорія(папка)
    {
        QDir dir_rec(path);
        dir_rec.setSorting(sortf);
        if (RProper==true) //рекурсія
        {
            QStringList list_rec(dir_rec.entryList(QDir::AllDirs | QDir::NoDotAndDotDot)); //взнаєм вміст поточної папки
            QString path1=dir_rec.canonicalPath();
#ifdef VERSION_2

            for (int j=tab;j>=0;--j)
                str.append(" │"); // додаємо пропуски перед файлами і папками
            str.replace(str.size()-1,1,"├");
            if (info.fileName().isEmpty()) str.append("["+path+"]"+"\n"); //директорію загортаємо в квадратні дужки
            else
                str.append("["+info.fileName()+"]"+"\n"); //директорію загортаємо в квадратні дужки
            ++tab;

#endif

            /* if (str.size()>50)
            {
                emit toProg(str);
                QThread::msleep(1000);
                str.clear();
            }//*/


            str.append(ls_print(path1));

            if (list_rec.size()>0)
                for (int i=0; i<list_rec.size();++i)
                {
                    path1=dir_rec.canonicalPath()+"\\"+list_rec[i]; //якщо в нас є перераховані папки чи файли в командах
                    str.append(recurs(path1)); // власне рекурсія
                }

#ifdef VERSION_2
            --tab;
#endif

        }else //якщо не рекурсія, просто виводимо
        {
            str.append(ls_print(path));
        }
    } else str.append(ls_print(path)); //якщо файл
    return str;
}
