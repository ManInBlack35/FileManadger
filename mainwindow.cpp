#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileInfo>
#include <QProcess>
#include <QFile>
#include <QDir>
#include <QString>
#include <QDateTime>
#include <QTextStream>
#include <QStringList>
#include <QKeyEvent>
#include <QThread>

#include "recursiv.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),remdialog(new Dialog),
    ui(new Ui::MainWindow)
{
    map.insert(LS_COM,"ls"); //задаємо словник команд
    map [CD_COM]="cd";
    map [CP_COM]="cp";
    map [MV_COM]="mv";
    map [PWD_COM]="pwd";
    map [CAT_COM]="cat";
    map [RM_COM]="rm";
    lProper=rProper=sProper=bProper=RProper=fProper=false;

    rv =new Recursiv; //створюємо об'єкт класу для рекурсії
    myThread = new QThread(); //створюємо об'єкт класу для другого потоку
    rv->moveToThread(myThread);  // відправляємо рекурсію в інший потік

    QObject::connect(myThread, &QThread::finished, rv, &QObject::deleteLater); //видалення і закриття потоку

    QObject::connect(remdialog.data(),SIGNAL( check(const bool &)),
                     this,SLOT (checked(const bool &)));  // зв'язок з діалогом підтвердження

    QObject::connect(this, SIGNAL(onrecurs(const QString &,bool,bool,int)),
                     rv,SLOT(do_recurs(const QString &,bool,bool,int)));  // зв'язок головного вікна з рекурсією

    QObject::connect(rv, SIGNAL(done (const QString&)),
                     this, SLOT (recursiveslot (const QString&))); //зворотній зв'язок рекурсії з головним вікном

    sortf=QDir::DirsFirst; // задаємо параметри по замовчуванні
    dir.setCurrent("D:/WorkOther/");
    QString curentpath(dir.canonicalPath ());

    ui->setupUi(this); //створюємо головне вікно
    ui->label->setText(curentpath); // вказуємо шлях у лейбі

}

void MainWindow::recursiveslot (const QString& str)
{
    ui->textEdit->append(str); //вивід на екран результат роботи рекурсії
}

void MainWindow::closeEvent(QCloseEvent *event) //закриття програми
{
    if(myThread->isRunning() )
        myThread->quit();  //закриваємо потік пере закриттям програми
    myThread->wait();
    event->accept();
}



MainWindow::~MainWindow()
{
    delete myThread;
    delete ui;
}

void MainWindow::checked(const bool & ch)
{
    fProper=ch; //параметр з діалогу
}

QString MainWindow::recurs2(QString path) //рекурсія для видалення файлів і папок
{
    QString str;
    QFileInfo info(path);
    if (info.isDir()) //якщо директорія, передивляємося вміст
    {
        QDir dir_rec(path);
        dir_rec.setSorting(sortf);
        QStringList list_rec(dir_rec.entryList(QDir::AllEntries | QDir::NoDotAndDotDot)); //передивляємося вміст
        QString path1=dir_rec.canonicalPath();
        str.append(path1+"\n");
        if (list_rec.size()>0)
            for (int i=0; i<list_rec.size();++i)
            {
                path1=dir_rec.canonicalPath()+"\\"+list_rec[i];
                str.append(recurs2(path1));
            }
    } else str.append(path+"\n");
    return str;
}

void MainWindow::Ls_com(QStringList &proper)
{
    QString path;
    dir.setSorting(sortf);
    int kil=proper.size()-1; //кількість додаткових аргументів

    myThread->start();

    if(kil>0 && proper[1][0]=='-') //якщо в нас є якісь прапорці серед аргументів...
    {
        kil--; //віднімаєм "-"
        QString prop=proper[1]; //видділяємо необхідну стрічку
        int temp=0;
        if (prop.indexOf('l')>0) //якщо стрічки містить прапорець "l" (розширене представлення)
        {
            lProper=true;
        }
        if (prop.indexOf('R')>0) //якщо стрічки містить прапорець "R" (рекурсія)
        {
            RProper=true;
        }
        temp=prop.indexOf('s'); // шукаємо прапорець "s" (сортування)
        if (temp>0)
        {
            if(temp<prop.size()-1) // якщо в нас після прапорця "s" є ще якісь прапорці сортування
                switch (prop[temp+1].toLatin1()) {
                case 'n':             // за ім'ям
                    sortf=QDir::Name;
                    break;
                case 't':            // за датою створення
                    sortf=QDir::Time;
                    break;
                case 's':           // за розміром
                    sortf=QDir::Size;
                    break;
                case 'e':          // за типом (розширенням)
                    sortf=QDir::Type;
                    break;
                default:
                    sortf=QDir::Name; // якщо невідомі елементи то сортуємо за ім'ям
                    break;
                }
            else sortf=QDir::DirsFirst; // по замовчуванні спочатку каталоги

        }
        if (kil>0) //якщо ми маємо додаткові аргументи (перечислені директорії/файли)
        {
            int i=2;
            do
            {
                path=dir.canonicalPath()+"\\"+proper[i];
                int keySort= static_cast <int> (sortf);
                emit onrecurs(path,lProper,RProper,keySort); // передаємо далі зі збереженням параметрів
                ++i;
            }while (i<proper.size());
        }else
        {
            int keySort= static_cast <int> (sortf);
            emit onrecurs(dir.canonicalPath(),lProper,RProper,keySort); //просто додаємо поточну директорію
            //str.append(recurs(dir.canonicalPath()));
        }
    } else
        if (kil>0) // якщо в нас є ще аргументи, але без прапорців
        {
            int i=1;
            do
            {
                path=dir.canonicalPath()+"\\"+proper[i];
                int keySort= static_cast <int> (sortf);
                emit onrecurs(path,lProper,RProper,keySort);
                //str.append(recurs(path));
                --kil;
                ++i;
            }while (kil>0);
        }else
        {
            int keySort= static_cast <int> (sortf);
            emit onrecurs(dir.canonicalPath(),lProper,RProper,keySort);
            //str.append(recurs(dir.canonicalPath()));
        }
    RProper=false; //?????
    lProper=false;
}

QStringList MainWindow::my_split (QString &str) // розділяє нашу стрічку на аргументи
{
    QStringList list;
    int space=0;
    do
    {
        space=str.toStdString().find_first_of(" \t\n\f\r'\""); // шукаємо пробільні символи і символи " і '
        if (str[0]=='\'' || str[0]=='\"')                      // якщо аргумент з пропусками, і обмежений " ", або ' '
        {
            char divider;
            if (str[0]=='\'') divider='\'';
                else divider='\"';
            int next_space=str.indexOf(divider,1);
            list.push_back(str.mid(1,next_space-1)); //додаємо в список аргументів
            str.remove(0,next_space+1);              //видаляємо зі стрічки
        }
        else                                      // решта аргументів ділимо по пропусках
        {
            if (str[0].isSpace())
                str.remove(0,1);                  //видаляємо перший (і всі наступні) пробіл
            else
            {
                list.push_back(str.mid(0,space));//додаємо в список аргументів
                str.remove(0,space);             //видаляємо зі стрічки
            }
        }

    }while (space>=0);
    list.removeAll(""); // видаляємо зі списку пусті поля
    //list.removeDuplicates ();
    return list;
}

QString MainWindow::dirdel(const QString & path) //підкоманда видалення папки
{
    QString information;
    QDir temp_dir(path);
    if (fProper)            //f-ключ видаленя без підтверждення
    {
        if(temp_dir.rmdir(path))
            information.append("deleted"+path);
           else information.append("cannot delete directory:"+path);
    }
    else
    {
        int qw=remdialog->myexec(path);       //виклик діалога підтвердження
        if (qw)
        {
            if (temp_dir.rmdir(path)) information.append("deleted"+path);
                else information.append("cannot delete directory:"+path); //якщо не змогло видалити
        }
         else information.append(path+"\n was not deleted"); //якщо ми відмінили видалення
    }
    return information;
}

QString MainWindow::filedel(const QString & path) //підкоманда видалення файлу в поточній директорії
{
    QString information;
    QDir temp_dir(path);
    if (fProper) temp_dir.remove(path);      //f-ключ видаленя без підтверждення
    else
    {
        int qw=remdialog->myexec(path);     //виклик діалога підтвердження
        if (qw)
        {
            if (temp_dir.remove(path)) information.append("deleted"+path);
                else information.append("cannot delete file:"+path); //якщо не змогло видалити
        }
        else information.append(path+"\n was not deleted");  //якщо ми відмінили видалення
    }
    return information;
}

void MainWindow::remove_com (QStringList list)          //команда видалення
{
    int i=1;
    int kil=list.size();            //кількість елементів для видалення
    if(kil>1 && list[i][0]=='-')    //перевірка наявності ключів
    {
        if(list[i].indexOf('f'))
            fProper=true;
        if(list[i].indexOf('r'))
            rProper=true;
        if(list[i].indexOf('R'))
            RProper=true;
        i++;
    }
    while(i<list.size())        //проходимо по елементам списку
    {
        QString path;
        path=dir.canonicalPath()+'\\'+list[i];
        path=dir.cleanPath(path);
        bool bl=dir.exists(path);  //чи існує такий елемент
        if (bl)
        {
            QFileInfo info(path);
            if (info.isFile())      //перевірка на файл
                ui->textEdit->append(filedel(path));
            else
            if (info.isDir())       //якщо директорія
            {
                if (RProper)  //рекурсія
                {
                    QStringList list;
                    QString recurs(recurs2(path));
                    list=recurs.split('\n');        //створюємо список файлів і папок  для видалення через рекурсію
                    list.pop_back();
                    for (int i=list.size()-1;i>=0;--i) //проходимо по списку і видаляємо кожен елемент
                    {
                         QFileInfo info_temp(list[i]);
                         if (info_temp.isFile())
                            ui->textEdit->append(filedel(list[i]));  //викликаємо підкоманду видалення файлу і отримуємо результат
                         else
                            ui->textEdit->append(dirdel(list[i]));  //викликаємо підкоманду видалення директорії і отримуємо результат
                    }
                }
                else ui->textEdit->append(dirdel(path));  //викликаємо підкоманду видалення директорії і отримуємо результат
            }
        }
        else
        {
            if (!rProper) ui->textEdit->append("error filename or directory: "+path); //якщо не знайшовся даний елемент
        }
        ++i;
    }
    fProper=false; // обнулення змінних, які відповідають за ключі
    rProper=false;
    RProper=false;

}

void MainWindow::on_lineEdit_returnPressed() // початок виконання команд
{
    command=ui->lineEdit->text();
    log_command.push_back(command); //записуємо команди в лог для можливості повтору
    log_curs=log_command.size();
    ui->lineEdit->clear();
    command=command.simplified();       //чистимо від різних лишніх пробільних символів
    ui->textEdit->append('>'+command);  //зворотній звязок
    QStringList list_parametrs=my_split(command); //розділяємо стрічку по елементам

    Commands c=map.key(list_parametrs[0]); //знаходимо відповідну команду
    QString str;
    switch (c) {
    case RM_COM:                    //команда видалення
        remove_com(list_parametrs);
        break;
    case LS_COM:                    //перегляд директорії
        Ls_com(list_parametrs);
        break;
    case PWD_COM:                   //пидивитися поточну директорію
        ui->textEdit->append(dir.canonicalPath());
        break;
    case CD_COM:                    //змінити директорію
        if (list_parametrs.size()>2)
        {
            ui->textEdit->append("No such files or derectory");
            break;
        }
        {
            bool diropen=dir.cd(list_parametrs[1]);
            if(diropen==false)
                ui->textEdit->append("No such files or derectory");
        }
        ui->label->setText(dir.canonicalPath ());
        break;

    case CAT_COM:  //переглянути файл
    {
        QString fileName;
        if (list_parametrs.size()>1 && list_parametrs[1][0]=='-'
                && list_parametrs[1][1]=='b') //перевіряємо наявність ключів
        {
            bProper=true;
            fileName=dir.canonicalPath ()+'/'+ list_parametrs[2];
        }
        else fileName=dir.canonicalPath ()+'/'+  list_parametrs[1];
        //QDir asd=dir.current();
        if(QFile::exists(fileName))
        {
             QFile file(fileName);
             if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
             {

                 QString err=file.errorString();
                 str=err;
                 ui->textEdit->append(str);
                 file.close();
             }
             else
             {
                 while(!file.atEnd())
                         {

                             QString str1 = file.readLine();
                             ui->textEdit->append(str1);
                         }
                 file.close();
             }
        }
        else ui->textEdit->append("error file name");

        bProper=false;
    }
        break;

    default: //все решта просто спробувати відкрити
        if (dir.exists(list_parametrs[0]))
        {
            QString temp_path(dir.canonicalPath()+'\\'+list_parametrs[0]);
            QFileInfo info (temp_path);
            if (info.isDir())
            {
                bool diropen=dir.cd(list_parametrs[0]);
                if(diropen==false)
                    ui->textEdit->append("No such files or derectory");
                    ui->label->setText(dir.canonicalPath ());
            }
            else
            {
                QString program = dir.canonicalPath ()+"/"+list_parametrs[0];
                list_parametrs.pop_front();
                QProcess *myProcess = new QProcess(this);
                myProcess->start(program,list_parametrs);
            }
        } else ui->textEdit->append("команда не найдена");
        break;
    }
}

void  MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Up:
        if (log_curs>0 && log_curs<=log_command.size())
        {
            --log_curs;
            ui->lineEdit->setText(log_command[log_curs]);
        }
        event->accept();
        break;
    case Qt::Key_Down:
        if (log_curs>=0 && log_curs<log_command.size()-1)
        {
            ++log_curs;
            ui->lineEdit->setText(log_command[log_curs]);
        }
        event->accept();
        break;
    case Qt::Key_Escape:
        //break;
    default:
        event->ignore();
        QWidget::keyPressEvent(event);
    }
}
