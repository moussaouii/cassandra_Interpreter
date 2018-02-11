//drop et truncate table



//afficher tous les keyspaces
describe keyspaces;

//afficher tous les tables
describe tables;

//afficher les table de keyspace
use rochdi;
describe tables ;

//rochdi.test n'exist pas (pas d'erreur if exist)
 DROP TABLE IF EXISTS  test;

//error rochdi.test n'exist pas
DROP Table test;


//valide
drop table t;



//(les deux valide)
truncate table t2;
truncate t2;


