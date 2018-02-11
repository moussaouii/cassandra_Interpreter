//error update

CREATE KEYSPACE IF NOT EXISTS test
           WITH replication = {'class': 'SimpleStrategy', 'replication_factor' : 3};

use test;

create table IF NOT EXISTS test( a int ,
                b int,
                c int ,
                d text ,
                e text,
                primary key((a,b),c));


update test set d='newTest1' , e='newTest1' where a=1 and b=2 ;

update test set d='newTest4' , e='newTest4' where a=3 and b=4 and j=2;

update test set d='newTest5' , e='newTest5' where a=3 and b=5 and c='ddd';

