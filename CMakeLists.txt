cmake_minimum_required(VERSION 3.6)
project(cassandra1)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES  analyseur_cql.c lex.yy.c list.c list.h analyseur_cql.h structure.h header_analyseur.h error.c error.h
        murmur3.c murmur3.h
        src/dump.c src/error.c src/hashtable.c src/hashtable.h src/hashtable_seed.c src/jansson.h src/jansson_config.h
        src/jansson_private.h src/load.c src/lookup3.h src/memory.c src/pack_unpack.c src/strbuffer.c src/strbuffer.h
        src/strconv.c src/utf.c src/utf.h src/value.c semantique.c semantique.h boolean.h "execute_stmt.c" "execute_stmt.h" list_aux.c list_aux.h persistance.c persistance.h)

add_executable(cassandra1 ${SOURCE_FILES})
