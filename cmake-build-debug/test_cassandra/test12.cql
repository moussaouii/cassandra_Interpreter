//update

CREATE KEYSPACE IF NOT EXISTS test
           WITH replication = {'class': 'SimpleStrategy', 'replication_factor' : 3};

use test;

create table IF NOT EXISTS test( a int ,
                b int,
                c int ,
                d text ,
                e text,
                primary key((a,b),c));



//error

insert into test (a,b,c,d,e) values (1,2,3,'test1','test1');

insert into test (a,b,c,d,e) values (1,2,4,'test2', 'test2');

insert into test (a,b,c,d,e) values (1,2,5,'test3', 'test3');


insert into test (a,b,c,d,e) values (3,4,5,'test4','test4');

insert into test (a,b,c,d,e) values (3,4,6,'test5', 'test5');

insert into test (a,b,c,d,e) values (3,4,7,'test6', 'test6');


insert into test (a,b,c,d,e) values (3,5,5,'test4','test4');

insert into test (a,b,c,d,e) values (3,5,6,'test5', 'test5');



select * from test;

update test set d='newTest1' , e='newTest1' where a=1 and b=2 and c=3;

update test set d='newTest4' , e='newTest4' where a=3 and b=4 and c=5;

update test set d='newTest5' , e='newTest5' where a=3 and b=5 and c=6;


select * from test;