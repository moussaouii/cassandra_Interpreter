//alterKeyspace
//l'option s'il exist il sera remplace
//sinon il sera ajouter au list option

//valid
 //l'option durable_writes sera ajoutee
alter KEYSPACE ismail with durable_writes = false;

//les valeurs des 2 options seront remplace
alter KEYSPACE outhmane
           WITH replication = {'class': 'SimpleStrategy', 'replication_factor' : 4}
            AND durable_writes = true;

// non valid
//keyspace n'exist pas
ALTER KEYSPACE Excelsior
          WITH replication = {'class': 'SimpleStrategy', 'replication_factor' : 4};

//missing class in option replication
ALTER KEYSPACE ismail
         WITH replication = { 'replication_factor' : 4};

//replication repter 2 fois
Alter KEYSPACE ismail
           WITH durable_writes = false
            AND durable_writes = false
            and replication = {'class': 'SimpleStrategy', 'replication_factor' : 4};


//9 n'est pas valid comme valeur de replication
alter KEYSPACE ismail
           WITH replication = 9;


