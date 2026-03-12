#include <stdio.h>
#include <stdlib.h>
#include <lista.h>

// Crea una lista con un nodo.
void crear(TLista *pLista, char *valor)
{
  pLista->pPrimero = malloc(sizeof(TNodo));
  if (pLista->pPrimero != NULL) {
    pLista->pPrimero= NULL;
    pLista->pPrimero->pSiguiente = NULL;
  }
}

void destruir(TLista *pLista)
{
  if (pLista == NULL) return;
  
  TNodo *pAux1;
  TNodo *pAux2;  

  for (pAux1 = pLista->pPrimero; pAux1 != NULL;)
  {
    pAux2 = pAux1->pSiguiente;
    free(pAux1);
    pAux1 = pAux2;
  }

  // No hacer free(pLista) porque pLista no fue asignada con malloc
  // Solo se liberan los nodos, no la estructura de la lista en sí
  pLista->pPrimero = NULL;
}

// Inserta al principio de la lista.
void insertar(TLista *pLista, char valor)
{
  TNodo *pNuevo = malloc(sizeof(TNodo));
  if (pNuevo != NULL) {
    pNuevo->valor = valor;
    pNuevo->pSiguiente = pLista->pPrimero;
    pLista->pPrimero = pNuevo;
  }
}

void insertarFinal(TLista *pLista, char valor)
{
  TNodo *pNuevo = malloc(sizeof(TNodo));
  if (pNuevo != NULL) {
    pNuevo->valor = valor;
    pNuevo->pSiguiente = NULL;
    
    if (pLista->pPrimero == NULL) {
      // Lista vacía
      pLista->pPrimero = pNuevo;
    } else {
      // Buscar el último nodo
      TNodo *pAux = pLista->pPrimero;
      while (pAux->pSiguiente != NULL) {
        pAux = pAux->pSiguiente;
      }
      pAux->pSiguiente = pNuevo;
    }
  }
}

// Suponemos n = 1, 2, ... (index basado en 1)
void insertarN(TLista *pLista, int index, char valor)
{
  if (index < 1) return;  // Índice inválido
  
  TNodo *pNuevo = malloc(sizeof(TNodo));
  if (pNuevo == NULL) return;
  
  pNuevo->valor = valor;
  
  if (index == 1) {
    // Insertar al principio
    pNuevo->pSiguiente = pLista->pPrimero;
    pLista->pPrimero = pNuevo;
  } else {
    // Buscar la posición anterior a la inserción
    TNodo *pAux = pLista->pPrimero;
    int i;
    
    for (i = 1; i < index - 1 && pAux != NULL; i++) {
      pAux = pAux->pSiguiente;
    }
    
    if (pAux == NULL) {
      // Índice fuera de rango
      free(pNuevo);
    } else {
      pNuevo->pSiguiente = pAux->pSiguiente;
      pAux->pSiguiente = pNuevo;
    }
  }
}

// Elimina el primer elemento de la lista.
void eliminar(TLista *pLista)
{
  if (pLista->pPrimero != NULL) {
    TNodo *pAux = pLista->pPrimero;
    pLista->pPrimero = pLista->pPrimero->pSiguiente;
    free(pAux);
  }
}

void eliminarN(TLista *pLista, int index)
{
  if (index < 1 || pLista->pPrimero == NULL) return;
  
  TNodo *pEliminar;
  
  if (index == 1) {
    // Eliminar el primer elemento
    pEliminar = pLista->pPrimero;
    pLista->pPrimero = pLista->pPrimero->pSiguiente;
    free(pEliminar);
  } else {
    // Buscar el nodo anterior al que queremos eliminar
    TNodo *pAux = pLista->pPrimero;
    int i;
    
    for (i = 1; i < index - 1 && pAux != NULL; i++) {
      pAux = pAux->pSiguiente;
    }
    
    if (pAux != NULL && pAux->pSiguiente != NULL) {
      pEliminar = pAux->pSiguiente;
      pAux->pSiguiente = pEliminar->pSiguiente;
      free(pEliminar);
    }
  }
}

char* getElementoN(TLista *pLista, char index)
{
  if (index < 1 || pLista->pPrimero == NULL) return -1;  // Error
  
  TNodo *pAux = pLista->pPrimero;
  int i;
  
  for (i = 1; i < index && pAux != NULL; i++) {
    pAux = pAux->pSiguiente;
  }
  
  if (pAux == NULL) {
    return -1;  // Índice fuera de rango
  }
  
  return pAux->valor;
}

void imprimir(TLista *pLista)
{
  printf("Lista: ");
  TNodo *pAux = pLista->pPrimero;
  
  while (pAux != NULL) {
    printf("%d ", pAux->valor);
    pAux = pAux->pSiguiente;
  }
  printf("\n");
}

int longitud(TLista *pLista)
{
  int contador = 0;
  TNodo *pAux = pLista->pPrimero;
  
  while (pAux != NULL) {
    contador++;
    pAux = pAux->pSiguiente;
  }
  
  return contador;
}