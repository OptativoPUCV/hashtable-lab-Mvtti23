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
  while(map->buckets[pos] != NULL && (pos < map->capacity-1)) pos++;
  if(pos == map->capacity-1)
    pos = 0;

  while(map->buckets[pos] != NULL && (pos < map->capacity-1)) pos++;
  

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

  //Crear mapa auxiliar
  HashMap * aux_map = (HashMap *)malloc(sizeof(HashMap));
  aux_map->buckets = (Pair **) calloc (map->capacity ,sizeof(Pair *));
  aux_map->capacity = map->capacity;
  aux_map->current = -1;

  //copiar map en map auxiliar
  long pos = 0;
  while(pos <= map->size)
  {
    insertMap(aux_map, map->buckets[pos]->key, map->buckets[pos]->value);
    aux_map->size++;
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
    insertMap(map, aux_map->buckets[pos]->key, aux_map->buckets[pos]->value);
    map->size++;
    pos++;
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
  
  //buscar casilla
  long pos = hash(key, map->capacity);

  while((is_equal(key, map->buckets[pos]->key) == 0) && (map->buckets[pos] != NULL) && (pos < map->capacity-1)) pos++;


  //eliminar el dato
  map->buckets[pos]->key = NULL;
  map->size--;
  
}

void * searchMap(HashMap * map,  char * key)
{ 
  long pos = hash(key, map->capacity);
  if(is_equal(key, map->buckets[pos]->key) == 1) //si es la casilla correspondiente
  {
    map->current = pos;
    return map->buckets[pos]->value;
  }

  for(long i = 0 ; i <= map->capacity ; i++)
  {
    if(is_equal(key, map->buckets[i]->key) == 1) //si encuentra la clave buscada
    {
      pos = i;
    }

    if(map->buckets[i] == NULL)
      return NULL;
  }

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
  
  long pos = map->current;

  while(map->buckets[pos] == NULL || map->buckets[pos]->key == NULL) 
  {
    pos++;
    map->current++;
  }

  map->current = pos;

  return map->buckets[pos]->value;
}
