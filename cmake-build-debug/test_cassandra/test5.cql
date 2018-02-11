//create table primary key : partition Key , clustring order

CREATE KEYSPACE IF NOT EXISTS rochdi
           WITH replication = {'class': 'SimpleStrategy', 'replication_factor' : 3};

create keyspace rochdi  WITH replication = {'class': 'SimpleStrategy', 'replication_factor' : 3};
use rochdi;

//partition key


CREATE TABLE t(
    a int,
    b int,
    c int,
    d int,
    PRIMARY KEY ((a, b), c, d)
);



insert into t (a,b,c,d) values (  0 , 0 , 1 , 1 );
 insert into t (a,b,c,d) values (0 , 1 , 3 , 3);
insert into t (a,b,c,d) values (0,0,0,0);
  insert into t (a,b,c,d) values (1 , 1 , 4 , 4);
 insert into t (a,b,c,d) values ( 0 , 1 , 2 , 2);



SELECT * FROM t;

//clustring order

CREATE TABLE t2 (
    a int,
    b int,
    c int,
    PRIMARY KEY (a, b, c)
);

insert into t2 (a,b,c) values (0,1,9);
insert into t2 (a,b,c) values (0,2,2);
insert into t2 (a,b,c) values (0,3,3);
insert into t2 (a,b,c) values (0,0,4);

SELECT * FROM t2;


