//drop keyspace


//valid
drop keyspace rochdi;

//erreur test n'exist pas
DROP KEYSPACE test ;

//n'affiche pas un erreur meme si test n'exsit pas
DROP KEYSPACE  IF EXISTS  test;


