#include <stdio.h>
#include <stdlib.h>
#include <lista.h>
#include <string.h>

// Crea una lista con un nodo.
void crear(TLista *pLista)
{
  if (pLista != NULL) {
    pLista->pPrimero = NULL;
  }
}

//funcion para eliminar datos de la lista
void destruir(TLista *pLista)
{
  if (pLista == NULL) return;
  
  TNodo *pAux1 = pLista->pPrimero;
  TNodo *pAux2;  

  while (pAux1 != NULL)
  {
    pAux2 = pAux1->pSiguiente; //guarda el siguiente
    if (pAux1->valor != NULL)  //libera el string que esta en el nodo
      free(pAux1->valor);
    free(pAux1);
    pAux1 = pAux2;
  }

  //la lista queda vacia despues de esto
  pLista->pPrimero = NULL;
}

// Inserta al principio de la lista.
void insertar(TLista *pLista, char *valor)
{
  if ((pLista == NULL) || (valor == NULL))
  {
    return;
  }

  TNodo *pNuevo = malloc(sizeof(TNodo));
  if (pNuevo == NULL) 
  { //control de errores
    return;
  }

  pNuevo->valor = malloc(strlen(valor) + 1);
  if (pNuevo->valor == NULL)
  {
    free(pNuevo);
    return;
  }

  strcpy(pNuevo->valor, valor);
  pNuevo->pSiguiente = pLista->pPrimero;
  pLista->pPrimero = pNuevo;
}

//aqui se inserta un nuevo nodo al final del todo, las funciones y la implementacion es muy mjy similar a insertar
void insertarFinal(TLista *pLista, char *valor)
{
  if (pLista == NULL || valor == NULL) 
  {
    return;
  }

  TNodo *pNuevo = malloc(sizeof(TNodo));
  if (pNuevo == NULL) 
  { //control de errores
    return;
  }

  pNuevo->valor = malloc(strlen(valor) + 1);
  if (pNuevo->valor == NULL) {
    free(pNuevo);
    return;
  }

  strcpy(pNuevo->valor, valor);
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

// Suponemos n = 1, 2, ... (index basado en 1)
void insertarN(TLista *pLista, int index, char *valor)
{
  if (index < 1) return;  // Índice inválido
  
  TNodo *pNuevo = malloc(sizeof(TNodo));
  if (pNuevo == NULL) return;
  
  pNuevo->valor = malloc(strlen(valor) + 1);
  if (pNuevo->valor == NULL) { // comprobamos que no sea igual a NULL
    free(pNuevo);
    return;
  }

  strcpy(pNuevo->valor, valor);

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
      //se meten los free para evitar las fugas de memoria, es decir, que no queden las variables por el ancho mar de la memoria del ordenador
      free(pNuevo->valor);
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
  if (pLista == NULL || pLista->pPrimero == NULL) 
  {
    return;
  }
  
  TNodo *pEliminar = pLista->pPrimero;
  pLista->pPrimero = pLista->pPrimero->pSiguiente;
  
  // Liberaramos todo, tanto el string (dato, como el nodo donde se encontraba
  if (pEliminar->valor != NULL) {
    free(pEliminar->valor);
  }
  free(pEliminar);
}

void eliminarN(TLista *pLista, int index)
{
  if (pLista == NULL || index < 1 || pLista->pPrimero == NULL) return;
  
  TNodo *pEliminar;
  
  if (index == 1) {
    // Eliminar el primer elemento
    pEliminar = pLista->pPrimero;
    pLista->pPrimero = pLista->pPrimero->pSiguiente;
    if (pEliminar->valor != NULL) {
      free(pEliminar->valor);
    }
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
      if (pEliminar->valor != NULL) {
        free(pEliminar->valor);
      }
      free(pEliminar);
    }
  }
}

//igual que el anterior, se queda de serie
char* getElementoN(TLista *pLista, int index)
{
  if (pLista == NULL || index < 1 || pLista->pPrimero == NULL) return NULL;  // Error
  // -1 == null
  TNodo *pAux = pLista->pPrimero;
  int i;
  
  for (i = 1; i < index && pAux != NULL; i++) {
    pAux = pAux->pSiguiente;
  }
  
  if (pAux == NULL) {
    return NULL;  // Índice fuera de rango
  }
  
  return pAux->valor;
}

void imprimir(TLista *pLista)
{
  printf("Lista: ");
  TNodo *pAux = pLista->pPrimero;
  
  while (pAux != NULL) {
    printf("%s ", pAux->valor);
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
