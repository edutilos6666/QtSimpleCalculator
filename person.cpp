#include "person.h"

Person::Person():
    id(0), name(""), age(0), wage(0), active(false)
{

}

Person::Person(long id, QString name, int age, double wage, bool active):
    id(id), name(name), age(age), wage(wage), active(active)
{
}

Person::Person(QString name, int age, double wage, bool active):
    id(0), name(name), age(age), wage(wage), active(active)
{

}

PersonDAOImpl::PersonDAOImpl()
{
     stream.setString(&cmd);
    connect();
     dropTable();
     createTable();
     save(shared_ptr<Person>(new Person(1, "foo", 10, 100.0, true)));
     save(shared_ptr<Person>(new Person(2, "bar", 20, 200.0, false)));
     save(shared_ptr<Person>(new Person(3, "bim", 30, 300.0, true)));
     save(shared_ptr<Person>(new Person(4, "pako", 40, 400.0, false)));
}

PersonDAOImpl::~PersonDAOImpl()
{
    disconnect();
    delete query;
    delete db;
}

void PersonDAOImpl::connect()
{
  QSqlDatabase temp = (QSqlDatabase::addDatabase("QMYSQL"));
  db = &temp;
  query = new QSqlQuery();
  db->setHostName(host);
  db->setUserName(username);
  db->setPassword(password);
  db->setDatabaseName(dbName);
  bool res = db->open();
  if(res) {
      qDebug() << "connection succeeded.";
  } else {
      qDebug() << "connection failed.";
  }
}

void PersonDAOImpl::disconnect()
{
     try  {
        db->close();
    } catch(exception& ex)  {
        qDebug() << ex.what();
    }
}

void PersonDAOImpl::dropTable()
{
   //connect();
   cmd = "";
   stream << "drop table if exists Person";
   query->prepare(cmd);
   bool res = query->exec();
   qDebug() << "dropTable = " << res ;
   cmd = "" ;
   //disconnect();
}

void PersonDAOImpl::createTable()
{
   //connect();
     cmd = "";
     stream << "create table if not exists Person ( "
            << "id bigint primary key , "
            << "name varchar(50) not null, "
            << "age int  not null, "
            << "wage double not null, "
            << "active boolean not null "
            << ")";
     query->prepare(cmd);
     bool res = query->exec();
     qDebug() << "createTable = " << res ;
     cmd = "" ;
     //disconnect();
}

void PersonDAOImpl::save(shared_ptr<Person> p)
{
   //connect();
    cmd = "" ;
    stream << "insert into Person values(?, ?, ?, ?, ?)";
    query->prepare(cmd);
    query->bindValue(0, p->id);
    query->bindValue(1, p->name);
    query->bindValue(2, p->age);
    query->bindValue(3, p->wage);
    query->bindValue(4, p->active);
    bool res = query->exec();
    qDebug() << "save = " << res ;
    cmd = "" ;
    //disconnect();
}

void PersonDAOImpl::update(long id, shared_ptr<Person> p)
{
   //connect();
    cmd = "";
    stream << "update Person set name = ?, age = ? , wage = ?, active = ? where id = ?";
    query->prepare(cmd);
    query->bindValue(0, p->name);
    query->bindValue(1, p->age);
    query->bindValue(2, p->wage);
    query->bindValue(3, p->active);
    query->bindValue(4, id);
    bool res = query->exec();
    qDebug() << "update = " << res ;
    cmd = "";
    //disconnect();
}

void PersonDAOImpl::remove(long id)
{
   //connect();
    cmd = "";
    stream << "delete from Person where id = ?";
    query->prepare(cmd);
    query->bindValue(0, id);
    bool res = query->exec();
    qDebug() << "remove = " << res ;
    cmd = "";
    //disconnect();
}

shared_ptr<Person> PersonDAOImpl::findById(long id)
{
   //connect();
     cmd = "";
     stream << "select * from Person where id = ?";
     query->prepare(cmd);
     query->bindValue(0, id);
     bool res = query->exec();
     qDebug() << "findById = " << res ;
     query->record();

     cmd = "";
     query->next();
     shared_ptr<Person> res2 =  queryToPerson();
     //disconnect();
     return res2 ;
}

QList<shared_ptr<Person> > PersonDAOImpl::findAll()
{
   //connect();
   cmd = "";
   stream << "select * from Person";
   query->prepare(cmd);
   bool res = query->exec();
   qDebug() << "findAll = " << res ;
   query->record();
   QList<shared_ptr<Person>> all ;
   while(query->next()) {
       all.append(queryToPerson());
   }
   cmd = "";
   //disconnect();
   return all ;
}

shared_ptr<Person> PersonDAOImpl::queryToPerson()
{
    long id = query->value(0).toLongLong();
    QString name= query->value(1).toString();
    int age = query->value(2).toInt();
    double wage = query->value(3).toDouble();
    bool active = query->value(4).toBool();
    return shared_ptr<Person> {new Person(id, name, age, wage, active)};
}
