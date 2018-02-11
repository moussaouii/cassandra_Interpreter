//create table erreur semantique
//il y a 2 possibilite soit ecrire use keyspace en premier soit namekeyspace.nametable

//on doit specifier le keyspace

CREATE TABLE t (
    pk int,
    t int,
    v text,
    s text static,
    PRIMARY KEY (pk, t)
);


use outhmane;

//InvalidRequest:  message="Static column species cannot be part of the PRIMARY KEY"

CREATE TABLE monkeySpecies (
        species text static PRIMARY KEY,
        common_name text,
        population varint ,
        average_size int
    ) WITH comment='Important biological records'
       AND read_repair_chance = 1.0;


//InvalidRequest:message="Multiple PRIMARY KEYs specifed (exactly one required)"

 CREATE TABLE monkeySpecies (
        species text PRIMARY KEY,
        common_name text,
        population varint PRIMARY KEY ,
        average_size int
    ) WITH comment='Important biological records'
       AND read_repair_chance = 1.0;

//InvalidRequest: message="Multiple PRIMARY KEYs specifed (exactly one required)"

 CREATE TABLE timeline (
        userid uuid PRIMARY KEY,
        posted_month int,
        posted_time uuid,
        body text,
        posted_by text,
        PRIMARY KEY (userid, posted_month, posted_time)
    ) WITH compaction = { 'class' : 'LeveledCompactionStrategy' };


//InvalidRequest: message="No PRIMARY KEY specifed (exactly one required)"

 CREATE TABLE timeline (
        userid uuid ,
        posted_month int,
        posted_time uuid,
        body text,
        posted_by text

    ) WITH compaction = { 'class' : 'LeveledCompactionStrategy' };




//InvalidRequest: message="Unknown definition usgggerid referenced in PRIMARY KEY"

CREATE TABLE timeline (
        userid uuid ,
        posted_month int,
        posted_time uuid,
        body text,
        posted_by text,
    PRIMARY KEY (usgggerid, posted_month, posted_time)
    ) WITH compaction = { 'class' : 'LeveledCompactionStrategy' };


//column a redefinie

create table test (
    a int,
    b int,
    a text,
    c int primary key
);

//column a redefinie in primary key

create table test (
    a int,
    b int,
    c int ,
    primary key(a,a)
);


//invalid type for primary key (collection)

create table test (
    a map<int,int> primary key,
    b int,
    c int
);


//InvalidRequest: line: 6  message="No keyspace has been specified. USE a keyspace, or explicitly specify keyspace.tablename"
//InvalidRequest: line: 24  message="Static column species cannot be part of the PRIMARY KEY"
//InvalidRequest: line: 35  message="Multiple PRIMARY KEYs specifed (exactly one required)"
//InvalidRequest: line: 46  message="Multiple PRIMARY KEYs specifed (exactly one required)"
//InvalidRequest: line: 59  message="No PRIMARY KEY specifed (exactly one required)"
//InvalidRequest: line: 72  message=" Unknown definition usgggerid referenced in PRIMARY KEY"
//InvalidRequest: line: 83 message="Multiple definition of identifier a"
//InvalidRequest: line: 91 message="Multiple definition of identifier a in Primary Key"
//InvalidRequest: line: 101 message="Invalid  collection type  for PRIMARY KEY component a"
