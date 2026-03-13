#ifndef _LISTA_
#define _LISTA_

typedef struct TNodo
{
  char *valor;

  struct TNodo *pSiguiente;

} TNodo;

typedef struct
{
  TNodo *pPrimero;
} TLista;

void crear(TLista *pLista);
void destruir(TLista *pLista);

// inserta al princpio de la lista.
void insertar(TLista *pLista, char *valor);
void insertarFinal(TLista *pLista, char *valor);
void insertarN(TLista *pLista, int index, char *valor);

// elimina el último de la lista.
void eliminar(TLista *pLista);
void eliminarN(TLista *pLista, int index);

char* getElementoN(TLista *pLista, int index);

void imprimir(TLista *pLista);
int longitud(TLista *pLista);

#endif
