#ifndef PERSON_H
#define PERSON_H
#include <QString>
#include <QList>
#include <memory>
#include <QtSql>
#include <QDebug>
#include <iostream>
#include <QTextStream>

using namespace std;

class Person
{
public:
    Person();
    Person(long id, QString name, int age, double wage, bool active);
    Person(QString name, int age, double wage, bool active);


public:
 long id;
 QString name;
 int age;
 double wage;
 bool active;
};


class PersonDAO {
public:
    virtual void connect() = 0 ;
    virtual void disconnect() = 0;
    virtual void dropTable() = 0;
    virtual void createTable() = 0;
    virtual void save(shared_ptr<Person> p) = 0;
    virtual void update(long id, shared_ptr<Person> p) = 0;
    virtual void remove(long id) = 0;
    virtual shared_ptr<Person> findById(long id) = 0;
    virtual QList<shared_ptr<Person>> findAll() = 0 ;
};



class PersonDAOImpl: public PersonDAO {
public:
    PersonDAOImpl();
    ~PersonDAOImpl();
    void connect() ;
    void disconnect() ;
    void dropTable() ;
    void createTable() ;
    void save(shared_ptr<Person> p) ;
    void update(long id, shared_ptr<Person> p) ;
    void remove(long id) ;
    shared_ptr<Person> findById(long id) ;
    QList<shared_ptr<Person>> findAll() ;
    shared_ptr<Person> queryToPerson();
public:
    QSqlDatabase *db;
    QSqlQuery *query ;
    const QString host = "localhost";
    const QString username = "root";
    const QString password = "";
    const QString dbName = "test";
    QString cmd ;
    QTextStream stream;
};

#endif // PERSON_H
