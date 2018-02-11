//type map , set , list

CREATE KEYSPACE IF NOT EXISTS ismail
           WITH replication = {'class': 'SimpleStrategy', 'replication_factor' : 3};

use ismail;

//map :

//valid
CREATE TABLE users (
    id text PRIMARY KEY,
    name text,
    favs map<text, text> // A map of text keys, and text values
);


//non valid  for type map
INSERT INTO users (id, name, favs)
           VALUES ('jsmith', 'John Smith', { 'fruit' :9 , 'band' : 'Beatles' });


//valid
INSERT INTO users (id, name, favs)
           VALUES ('jsmith', 'John Smith', { 'fruit' : 'Apple', 'band' : 'Beatles' });

select * from users;

// Replace the existing map entirely.
UPDATE users SET favs = { 'fruit' : 'Banana' } WHERE id = 'jsmith';

select * from users;


//set ;
CREATE TABLE images (
    name text PRIMARY KEY,
    owner text,
    tags set<text> // A set of text values
);


//non valid  for type set
INSERT INTO images (name, owner, tags)
                       VALUES ('cat.jpg', 'jsmith', { 'pet', {1:2}});

//valid
INSERT INTO images (name, owner, tags)
            VALUES ('cat.jpg', 'jsmith', { 'pet', 'cute' });

//puisque il n'exist pas  cat1.jpg , elle sera creer

select * from images;


UPDATE images SET tags = { 'kitten1', 'cat1', 'lol1' } WHERE name = 'cat1.jpg';

select * from images;

// Replace the existing set entirely
UPDATE images SET tags = { 'kitten', 'cat', 'lol' } WHERE name = 'cat.jpg';


// List:

CREATE TABLE plays (
    id text PRIMARY KEY,
    game text,
    players int,
    scores list<int> // A list of integers
);

//invalid for type list
INSERT INTO plays (id, game, players, scores)
           VALUES ('123-afde', 'quake', 3, [17, 'fff', 2]);
//valid
INSERT INTO plays (id, game, players, scores)
           VALUES ('123-afde', 'quake', 3, [17, 4, 2]);

INSERT INTO plays (id, game, players, scores)
           VALUES ('124-afde', 'quake', 3, [17, 4, 2]);

INSERT INTO plays (id, game, players, scores)
           VALUES ('125-afde', 'quake', 3, [17, 4, 2]);

select * from plays;

// Replace the existing list entirely
UPDATE plays SET scores = [ 3, 9, 4] WHERE id = '123-afde';


//il sera creer car il n'exist pas
UPDATE plays SET scores = [ 1, 2, 3] WHERE id = 'test';


//il sera creer car il n'exist pas
UPDATE plays SET scores = [ 4, 5, 6] WHERE id = 'test2';

select * from plays;