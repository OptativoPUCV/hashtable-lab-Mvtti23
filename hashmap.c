#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"

typedef struct Pair Pair;
typedef struct HashMap HashMap;
int enlarge_called=0;

struct Pair {
     char * key;
     void * value;
};

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value)
{
  long pos = hash(key, map->capacity);
  
  //resolucion de colisiones
  while(map->buckets[pos] != NULL) pos++;
  if(pos == map->capacity) pos = 0;
  while(map->buckets[pos] != NULL) pos++;
  
  //revisar capacidad
  float capacidadMax = 0.7*map->capacity;
  if(map->size >= capacidadMax)
    enlarge(map);

 //ingreso del bucket nuevo
  map->buckets[pos] = createPair(key, value);
  map->current = pos;
  map->size++;
}

void enlarge(HashMap * map) 
{
  enlarge_called = 1; //no borrar (testing purposes)

  //Crear buckets auxiliar
  Pair ** aux_buckets = map->buckets;

  //nuevo buckets
  long new_capacity = map->capacity*2;
  map->buckets = (Pair **) calloc (new_capacity ,sizeof(Pair *));
  map->size = 0;
  
  //insertar en bucket agrandado
  long pos = 0;
  while(pos < map->capacity)
  {
    if(aux_buckets[pos] == NULL || aux_buckets[pos]->key == NULL) pos++;
    else
    {
      insertMap(map, aux_buckets[pos]->key, aux_buckets[pos]->value);
      map->size++;
      pos++;
    }
  }
}

HashMap * createMap(long capacity)
{
  HashMap * map = (HashMap *)malloc(sizeof(HashMap));
  map->buckets = (Pair **) calloc (capacity ,sizeof(Pair *));
  map->capacity = capacity;
  map->current = -1;

  return map;
}


void eraseMap(HashMap * map,  char * key)
{ 
  //buscar clave
  long pos = hash(key, map->capacity);

  while(map->buckets[pos] != NULL && is_equal(map->buckets[pos]->key, key) == 0) pos++;
  if(pos == map->capacity) pos = 0;
  while(map->buckets[pos] != NULL && is_equal(map->buckets[pos]->key, key) == 0) pos++;


  //si encuentra el dato
  if(map->buckets[pos] != NULL)
  {
    map->buckets[pos]->key = NULL;
    map->size--;
  }
}

void * searchMap(HashMap * map,  char * key)
{ 
  long pos = hash(key, map->capacity);
  
  while(map->buckets[pos] != NULL && is_equal(map->buckets[pos]->key, key) == 0) pos++;
  if(map->buckets[pos] == NULL) return NULL;
  if(pos == map->capacity) pos = 0;
  while(map->buckets[pos] != NULL && is_equal(map->buckets[pos]->key, key) == 0) pos++;
  
  //en caso de encontrar la clave buscada
  map->current = pos;
  return map->buckets[pos]->value;
}

void * firstMap(HashMap * map) 
{
  
  long pos = 0;

  while(map->buckets[pos] == NULL || map->buckets[pos]->key == NULL) pos++;

  map->current = pos;

  return map->buckets[pos]->value;
}

void * nextMap(HashMap * map) 
{ 
  long pos = map->current+1;

  while(map->buckets[pos] == NULL || map->buckets[pos]->key == NULL) 
  {
    if(pos == map->capacity) return NULL;
    else pos++;
  }

  map->current = pos;

  return map->buckets[pos]->value;
}