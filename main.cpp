#include "mainwindow.h"
#include <QApplication>
#include <QtSql>
#include <QString>
#include <QDebug>
#include <QTextStream>
#include <QVector>
#include <iostream>
#include "personwindow.h"
#include "person.h"

using namespace std;

class Worker {
public:
    Worker(long id, QString name, int age, double wage, bool active);
    Worker();
    ~Worker();
    QString toString() const;
public:
    long id;
    QString name;
    int age ;
    double wage;
    bool active;

};

Worker::Worker(long id, QString name, int age, double wage, bool active):
id(id),name(name), age(age), wage(wage), active(active){

}

Worker::Worker():
id(0), name(""), age(0), wage(0.0), active(false){

}

Worker::~Worker() {

}

QString Worker::toString() const {
   QString res = "" ;
   QTextStream stream(&res);
   stream << id << ", " << name << ", " << age << ", " << wage << ", " << active;
   return res ;
}



Worker* convertQueryToWorker(QSqlQuery query) {
 Worker * w = new Worker();
  long id = query.value(0).toLongLong();
  QString name = query.value(1).toString();
  int age = query.value(2).toInt();
  double wage = query.value(3).toDouble();
  bool active = query.value(4).toBool();
  w->id = id;
  w-> name = name;
  w->age = age;
  w->wage = wage;
  w->active = active;
  return w;
}



void testSqlite2() {
   QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
   db.setDatabaseName("simpleWorker.db");
   if(!db.open()) {
    qDebug() << "connection error" ;
    return ;
   }

   //drop table
   QString cmd = "";
   QTextStream stream(&cmd);
   stream << "drop table if exists Worker";
   QSqlQuery query ;
   query.prepare(cmd);

   bool res = query.exec();
   qDebug() << "dropTable = " << res ;

   cmd = "" ;
   stream << "create table if not exists Worker ("
          << "id int  not null , "
          << "name varchar(50) not null, "
          << "age num not null, "
          << "wage real not null, "
          << "active boolean not null"
          << ")";
   query.prepare(cmd);
   res = query.exec();
   qDebug() << "create table = " << res ;
   cmd = "";
   stream << "insert into Worker values (?, ?,?, ?,?)";
   query.prepare(cmd);
  QVector<Worker*> workerContainer ;
  workerContainer.append(new Worker(1, "foo", 10, 100.0, true));
  workerContainer.append(new Worker(2, "bar", 20 , 200.0, false));
  workerContainer.append(new Worker(3, "bim", 30 , 300.0, true));
  for(Worker* w: workerContainer) {
      query.bindValue(0, w->id);
      query.bindValue(1, w->name);
      query.bindValue(2, w->age);
      query.bindValue(3, w->wage);
      query.bindValue(4, w->active);
      res = query.exec();
      qDebug() << "insert = " << res ;
  }


  //select *
  cmd =  "" ;
  stream << "select * from Worker";
  query.prepare(cmd);
  query.exec();
  query.record();
  qDebug() << "***all Workers***" ;
  while(query.next()) {
      Worker* w = convertQueryToWorker(query);
      qDebug() << w->toString() ;
  }

}










void testSqlite() {
 QSqlDatabase db =  QSqlDatabase::addDatabase("QSQLITE");
 QString path = "foo.db";
    db.setDatabaseName(path);


    if(db.open()) {
     qDebug() << "connection success." ;

     //drop table
       QString cmd = "DROP TABLE  Person";
       QSqlQuery query ;
       query.prepare(cmd);
       bool res = query.exec();
       qDebug() << "drop table = " << res ;



       cmd = "";
     QTextStream  stream(&cmd);
      stream << "CREATE TABLE Person (" << ""
             << "id int not null, " << ""
             << "name varchar(50) not null, " << ""
             << "age num not null, " << ""
             << "wage real not null" << ""
             << ")";

      qDebug() << cmd ;

      query.prepare(cmd);
      res = query.exec();

      qDebug() << "create table = " << res ;

      cmd = "" ;
      stream << "INSERT INTO Person VALUES(:id, :name, :age, :wage)";
      query.prepare(cmd);

      query.bindValue(":id", 1);
      query.bindValue(":name", "foo");
      query.bindValue(":age", 10);
      query.bindValue(":wage", 100.0);
     res =  query.exec();
    qDebug() << "insert = " << res ;
      query.bindValue(0, 2);
      query.bindValue(1, "bar");
      query.bindValue(2, 20);
      query.bindValue(3, 200.0);
      query.exec();


      query.bindValue(0, 3);
      query.bindValue(1, "bim");
      query.bindValue(2, 30);
      query.bindValue(3, 300.0);
      query.exec();


       cmd = "" ;
       stream << "SELECT * FROM Person";
       qDebug() << cmd ;
       query.prepare(cmd);
       query.exec();
       auto records = query.record();

       qDebug() << records ;
       query.record().indexOf("id");
       while(query.next()) {
          // qDebug() << "here";
           qDebug() << query.value(0).toLongLong() << ", " << query.value(1).toString()
                    << ", " << query.value(2).toInt() << ", " << query.value(3).toDouble();
       }

    } else {
         qDebug() << "error in opening database" ;
    }
}





void testMysql()  {
try {
  QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
  db.setHostName("localhost");
  db.setUserName("root");
  db.setPassword("");
  db.setDatabaseName("test");
  if(!db.open()) {
      qDebug() << "mysql connection error" ;
      return ;
  }


   qDebug() << "mysql connection success." ;

   /*
    *   long id;
    QString name;
    int age ;
    double wage;
    bool active;
    */



     QString cmd = "" ;
     QTextStream stream(&cmd);
     stream << "drop table if exists Worker";
     QSqlQuery query ;
     query.prepare(cmd);
     bool res = query.exec();
     qDebug() << "drop table = " << res ;
     cmd = "";
     stream << "create table if not exists Worker ("
            << "id bigint primary key auto_increment, "
            << "name varchar(50)  not null, "
            << "age int not null, "
            << "wage double not null, "
            << "active boolean not null"
            << ")";
     query.prepare(cmd);
     res = query.exec();
     qDebug() << "create table = " << res ;
     cmd = "";
     stream << "insert into Worker (name, age, wage, active) values (?, ?, ?, ?)";
     query.prepare(cmd);
     QVector<Worker*> workers ;
     workers.append(new Worker(1, "foo", 10, 100.0, true));
     workers.append(new Worker(2, "bar", 20, 200.0, false));
     workers.append(new Worker(3, "bim", 30 , 300.0, true));
     workers.append(new Worker(4, "pako", 40 , 400.0 , false));
     for(Worker* w: workers) {
         query.bindValue(0 , w->name);
         query.bindValue(1, w->age);
         query.bindValue(2, w->wage);
         query.bindValue(3, w->active);
         res = query.exec();
         qDebug() << "insert value = " << res ;
     }

     cmd = "";
     stream << "select * from Worker";
     query.prepare(cmd);
     query.exec();
     query.record();
     qDebug() << "*** all workers***";
     while(query.next()) {
         Worker* w = convertQueryToWorker(query);
         qDebug() << w->toString();
     }

    } catch(exception& ex) {
        qDebug() << ex.what();
    }
}


void testPsql() {
   QString host = "localhost",
           username = "edutilos",
           password = "";
   int port = 5432 ;

    try {
   QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
   db.setHostName(host);
   db.setUserName(username);
   db.setPassword(password);
   db.setPort(port);
   db.setDatabaseName("testdb");
   bool res = db.open();
   if(!res)  {
        qDebug() << "connection error" ;
        qDebug() << db.lastError().text();
        return ;
   }

       qDebug() << "connection success." ;

       //drop table Worker
       QString cmd = "";
       QTextStream stream(&cmd);
       stream << "drop table if exists Worker";
       QSqlQuery*  query;
       query = new QSqlQuery(cmd);
       qDebug() << cmd ;
       res = query->exec();
       qDebug() << "drop table = " << res ;

       //create table Worker
       cmd = "";
       stream << "create table if not exists Worker ("
              << "id serial primary key, "
              << "name varchar(50) not null, "
              << "age int not null, "
              << "wage real not null, "
              << "active boolean not null "
              << ")";
      query = new QSqlQuery(cmd);
       qDebug() << cmd ;
       res = query->exec();
       qDebug() << "create table = " << res ;

       cmd = "";

       //insert into table
       QVector<Worker*> workers ;
       workers.append(new Worker(1, "foo", 10, 100.0, true));
       workers.append(new Worker(2, "bar", 20, 200.0, false));
       workers.append(new Worker(3, "bim", 30 , 300.0, true));
       workers.append(new Worker(4, "pako", 40 , 400.0 , false));
       cmd ="";
       stream << "insert into Worker(name, age, wage, active) values(:name, :age, :wage, :active)";
       query->prepare(cmd);
       for(Worker *w:workers) {
           query->bindValue(":name", w->name);
           query->bindValue(":age", w->age);
           query->bindValue(":wage", w->wage);
           query->bindValue(":active", w->active);
           res = query->exec();
           qDebug() << "insert = " << res ;
       }

       workers.clear();
       workers.append(new Worker(1, "new_foo", 10, 100.0, true));
       workers.append(new Worker(2, "new_bar", 20, 200.0, false));
       workers.append(new Worker(3, "new_bim", 30 , 300.0, true));
       workers.append(new Worker(4, "new_pako", 40 , 400.0 , false));

       cmd = "";
       stream << "insert into Worker(name, age, wage, active) values(?, ?, ?,?)";
       query->prepare(cmd);
       for(Worker* w: workers) {
           query->bindValue(0, w->name);
           query->bindValue(1, w->age);
           query->bindValue(2, w->wage);
           query->bindValue(3, w->active);
           res = query->exec();
           qDebug() << "insert = " << res ;
       }

       cmd = "";

       qDebug() << "all workers" ;
       stream << "select * from worker";
       query->prepare(cmd);
       query->exec();
       query->record();
       while(query->next())  {
           Worker *w = convertQueryToWorker(*query);
           qDebug() << w->toString();
       }

       //delete from table
       cmd = "" ;
       stream << "delete from Worker where id > ?";
       query->prepare(cmd);
       query->bindValue(0, 4);
       res = query->exec();
       qDebug() << "delete = " << res ;
       cmd = "";

       //update table
       stream << "update Worker set name = ?, age = ?, wage = ?, active = ? where id = ?";
       query->prepare(cmd);
       query->bindValue(0 , "old_foo");
       query->bindValue(1, 66);
       query->bindValue(2, 666.6);
       query->bindValue(3, false);
       query->bindValue(4, 1);
       res = query->exec();
       qDebug() << "update = " << res ;
       //select all
       cmd = "";
       stream << "select * from worker";
       query->prepare(cmd);
       res = query->exec();
       qDebug() << "select res =" << res ;
       query->record();
       while(query->next()) {
           Worker* w = convertQueryToWorker(*query);
           qDebug() << w->toString();
       }

       delete query;
       db.close();

   } catch(exception &ex) {
       qDebug() << ex.what() ;
   }
}

int main(int argc, char *argv[])
{
 QApplication a(argc, argv);
    PersonWindow w(0 , new PersonDAOImpl());
    w.show();

//    return a.exec();
   //  testSqlite();
   // testSqlite2();
   //testMysql();
   //qDebug() << QSqlDatabase::drivers();
//   testPsql();
//   return 1;
   return a.exec();
}
