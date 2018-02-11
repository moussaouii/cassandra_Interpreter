//error insert

CREATE KEYSPACE IF NOT EXISTS test
           WITH replication = {'class': 'SimpleStrategy', 'replication_factor' : 3};

use test;

create table IF NOT EXISTS test( a int ,
                b int,
                c int ,
                d text ,
                e text,
                primary key((a,b),c));



//InvalidRequest: line: 17  message="Unmatched column names/values"
insert into test (a,b,c,d,e) values (1,2,3,'test1','test2','test3');

//InvalidRequest: line: 19 message="Invalid value 1 for column 'e' of type text"
insert into test (a,b,c,d,e) values (1,2,3,'test1',1);

//InvalidRequest: line: 21 message="Some partition key parts are missing: c"
insert into test (a,b,d,e) values (1,2,'test1','test2');

//InvalidRequest: line: 32 message="Undefined column name : f"
insert into test (a,b,c,f) values (1,2,3,'test2');

//InvalidRequest: line: 25 message="Multiple definition of identifier a in List Column"
insert into test (a,b,c,a) values (1,2,3,5);

//InvalidRequest: line: 27 message="Invalid value null  for PRIMARY KEY component a"
insert into test (a,b,c,d,e) values (null,2,3,'test1','test2');

