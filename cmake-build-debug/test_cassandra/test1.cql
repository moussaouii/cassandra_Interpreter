//create Keyspace


//valid
CREATE KEYSPACE ismail
           WITH replication = {'class': 'SimpleStrategy', 'replication_factor' : 3};

CREATE KEYSPACE outhmane
           WITH replication = {'class': 'NetworkTopologyStrategy', 'DC1' : 1, 'DC2' : 3}
            AND durable_writes = false;

CREATE KEYSPACE rochdi
		WITH durable_writes = false
           	 AND replication = {'class': 'NetworkTopologyStrategy', 'DC1' : 1, 'DC2' : 3};
           
//erreur

//pas d'option replication(erreur synatxique il faut au moins une option)
CREATE KEYSPACE ismail1;

//deja exist
CREATE KEYSPACE outhmane
           WITH replication = {'class': 'NetworkTopologyStrategy', 'DC1' : 1, 'DC2' : 3}
            AND durable_writes = false;

//invalid input 'kkk'
CREATE KEYSPACE rochdi1
		WITH durable_writes = false;

CREATE KEYSPACE rochdi1
WITH durable_writes = false AND replication = {'class': 'NetworkTopologyStrategy', 'DC1' : 'kkk' , 'DC2' : 3};

//other option not supported
create keyspace outhmne1
           WITH replication = {'class': 'NetworkTopologyStrategy', 'DC1' : 1, 'DC2' : 3}
            AND other_option = false;

//no sub option class

CREATE KEYSPACE outhmane2
           WITH replication ={ 'replication_factor' : 3};

//sub option not supported

CREATE KEYSPACE ismail3
           WITH replication = {'class': 'SimpleStrategy', 'replication_factor' : 3 , 'subOption':10};


//erreur option  2 fois defini
create keyspace outhmne1
          WITH replication = {'class': 'NetworkTopologyStrategy', 'DC1' : 1, 'DC2' : 3}
            and replication = {'class': 'SimpleStrategy', 'replication_factor' : 3};