//error delete

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

//InvalidRequest: line: 19 message="Some partition key parts are missing: 'b'"
delete from test where a=1;

//InvalidRequest: line: 21 message="Undefined column name : h"
delete from test where h='dd';

//InvalidRequest: line: 23 message="Non PRIMARY KEY columns found in where clause: 'd'"
delete from test where a=1 and b=2 and d='test1';

