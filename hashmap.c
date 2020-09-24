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

  //revisar capacidad
  float capacidadMax = 0.7*map->capacity;
  if(map->size >= capacidadMax)
    enlarge(map);


 //ingreso del bucket nuevo
  map->buckets[pos] = createPair(key, value);
  map->current = pos;
  map->size += 1;
}

void enlarge(HashMap * map) 
{
  /*
  enlarge_called = 1; //no borrar (testing purposes)

  //guardar datos en buckets nuevo
  Pair ** old_buckets;
  long pos = 0;
  while(pos <= map->capacity)
  {
    map->old_buckets[pos] = map->buckets[pos];
    map->buckets[pos]->key = NULL;
    pos++;
  }

  //duplicar capacidad y actualizar size
  map->capacity *= 2;
  map->size = 0;

  //agregar a nuevo buckets
  pos = 0;
  while(pos <= map->capacity)
  {
    insertMap(map, map->old_buckets[pos]->key, map->old_buckets[pos]->value);
    pos++;
  }
  */
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
  /*
  //buscar casilla
  long pos = hash(key, map->capacity);

  while(is_equal(map->buckets[pos]->key, key) == 0) pos++;


  //eliminar el dato
  map->buckets[pos]->key = NULL;
  map->size--;
  */
}

void * searchMap(HashMap * map,  char * key)
{ 
  /*
  long pos = hash(key, map->capacity);

  while(is_equal(map->buckets[pos]->key, key) == 0) pos++;

  if(map->buckets[pos] == NULL)
    return NULL;

  map->current = pos;

  return map->buckets[pos]->value;
  */

  return NULL;
}

void * firstMap(HashMap * map) 
{
  long pos = 0;
  while((map->buckets[pos] == NULL) || map->buckets[pos]->key == NULL) pos++;

  map->current = pos;
  return map->buckets[pos]->value;
}

void * nextMap(HashMap * map) 
{
  long pos = map->current;
  while(map->buckets[pos] == NULL)
   pos++;

  map->current = pos;

  return map->buckets[pos]->value;
}
