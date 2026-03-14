//CODIGO TERMINADO y documentado pa el grupo :)
//includes
#include <errno.h>
#include <linux/limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <definitions.h>
#include <lista.h>

//declaracion de funciones
void procesar_argumentos(int argc, char *argv[], char **filename, char **pattern, int *lines);
void instalar_manejador_senhal();
void manejador_senhal(int sign);
void procesar_patrones(const char *fichero_patrones, TLista *patrones);
void procesar_linea(char *linea, TLista *patrones);
void iniciar_tabla_procesos(int n_procesos_contador, int n_procesos_procesador);
void crear_procesos(const char *nombre_fichero, TLista *patrones);
void lanzar_proceso_contador(const int indice_tabla, const char *linea, const char *numero_linea_str);
void lanzar_proceso_procesador(const int indice_tabla, const char *patron, const char *nombre_fichero);
void esperar_procesos();
void terminar_procesos(void);
void liberar_recursos(TLista *patrones);
//variables globales
int g_nProcesses;
struct TProcess_t *g_process_table;
//tLista ya no es aqui, si no dentro del main como variable local del main


int main(int argc, char *argv[])
//falta echarle un vistazo al main que hay cosas que no funcionan
{
  //variables locales
  char *nombre_fichero = NULL, *fichero_patrones = NULL; //dos char, uno con el nombre del fichero y otro con los patrones
  int lineas = 0; //se inicializa el numero de lineas procesadas a 0
  TLista patrones; //una lista pa guardar el numero de patrones
  //da error al asignar memoria con malloc, hay que revisarlo
  //patrones = malloc(sizeof(TLista));
  
//cambiar crear(&patrones)
//crea una lista
  crear(&patrones);
  //llenamos el fichero 
  procesar_argumentos(argc, argv, &nombre_fichero, &fichero_patrones, &lineas);
  //la movida del crtl +c
  instalar_manejador_senhal();
  //se lee el archivod e patrones
  procesar_patrones(fichero_patrones, &patrones);
  //crea una tabla de procesos
  iniciar_tabla_procesos(lineas, longitud(&patrones));
  //esto es pa crear los procesos hijos
  crear_procesos(nombre_fichero, &patrones);
  esperar_procesos();

  printf("\n[MANAGER] Terminacion del programa (todos los procesos terminados).\n");
  liberar_recursos(&patrones); //&patrones devuelve la posicion de memoria de patrones, que es la variable de tipo TLista

  return EXIT_SUCCESS;
}

void procesar_argumentos(int argc, char *argv[], char **nombrefichero, char **fichero_patrones, int *lineas)
{
  FILE *fp; //puntero a archivo
  int ch;
  //verifica que hay 3 argumentos exactos, que son el nombre de programa + 2 archivos que se tienen que introducir segun la especificacion del enunciado,
  //si no hay 3 da el error ese con el print y finiquita el programa
  if (argc != 3)
  {
    fprintf(stderr, "Error. Usa: ./exec/manager <fichero> <fichero_patrones>.\n");
    exit(EXIT_FAILURE);
  }
  //guarda los nombres de los ficheros
  *nombrefichero = argv[1];
  *fichero_patrones = argv[2];

  //pa contar las lineas del archivo de texto
  if((fp = fopen(*nombrefichero, "r")) == NULL){
    //fprintf es como printf de JAva, para añadir datos a media frase sin necesidad de cortarla
    fprintf(stderr, "Error al abrir %s \n", *nombrefichero);
    exit(EXIT_FAILURE);
  }
  *lineas = 0;
  //pa contar saltos de linea
  while ((ch = fgetc(fp)) != EOF){
//si el caracter que detecta es el salto de linea, aumenta las lineas
    if(ch == '\n')(*lineas)++;
  }
  //rewind(fp); //pa volver al principio
  fclose(fp); //pa cerrar
}

void instalar_manejador_senhal()
{
  if (signal(SIGINT, manejador_senhal) == SIG_ERR)
  {
    fprintf(stderr, "[MANAGER] Error al instalar el manejador se senhal: %s.\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
}
//esto se lo carga to, el terminar proceso a los hijos, liberar pues libera y exit termina
void manejador_senhal(int sign)
{
  printf("\n[MANAGER] Terminacion del programa (Ctrl + C).\n");
  terminar_procesos();
  liberar_recursos(NULL);
  exit(EXIT_SUCCESS);
}

void procesar_patrones(const char *fichero_patrones, TLista *patrones)
{
  FILE *fp;
  char linea[PATH_MAX]; //buffer pa cada linea

  if ((fp = fopen(fichero_patrones, "r")) == NULL)
  {
    fprintf(stderr, "Error al abrir el fichero %s\n", fichero_patrones);
    exit(EXIT_FAILURE);
  }
//lee
  while (fgets(linea, sizeof(linea), fp) != NULL)
  {
    procesar_linea(linea, patrones);
  }

  fclose(fp);
}

void procesar_linea(char *linea, TLista *patrones){ 
  char *linea_copy = malloc(strlen(linea) + 1); //hace una copia de la linea
  strcpy(linea_copy, linea);
  char *token; 
  int contadorPalabras = 0;
  token = strtok(linea_copy, " "); //obtiene la primera palabra

  while(token!=NULL){
    contadorPalabras++;

    char* palabraCopiada = strdup(token); //Copiamos la palabra pillada
    insertarFinal(patrones, palabraCopiada); // metemos la palabra en la lista de patrones
    token = strtok(NULL, " "); //Con esto seguimos troceando la linea separando todas las palabras


  }
  printf("La linea posee %d palabras \n", contadorPalabras);
  

  //libera
 free(linea_copy);


  // Modificar lista.c para aceptar char* ; tzmbien arreglar insertar final, que no se que poner 


 // token = strtok(linea_copy, " "); //obtiene la primera palabra
    //libera
}
void iniciar_tabla_procesos(int n_procesos_contador, int n_procesos_procesador)
{
  //calcula el total de procesos
  g_nProcesses = n_procesos_contador + n_procesos_procesador;
  //reserva memoria para la talba
  g_process_table = malloc(g_nProcesses * sizeof(struct TProcess_t));

  //inicia la tabla con PID = 0 en todos 
  for (int i = 0; i < g_nProcesses; i++)
  {
    g_process_table[i].pid = 0;
    g_process_table[i].clase = NULL;
  }
}
//Por cada línea creará un proceso contador.

void crear_procesos(const char *nombre_fichero, TLista *patrones)
{
  FILE *fp;
  char linea[PATH_MAX], numero_linea_str[12]; //buffer pa cada linea y string para el numero de linea
  int indice_tabla = 0; //indice en la talba del procesos
  int num_linea = 1; //numero de linea actual
  

  if ((fp = fopen(nombre_fichero, "r")) == NULL)
  {
    fprintf(stderr, "Error al abrir el fichero %s\n", nombre_fichero);
    exit(EXIT_FAILURE);
  }


  while (fgets(linea, sizeof(linea), fp) != NULL)
  {
    //convertimos cada numero de linea a string
    sprintf(numero_linea_str, "%d", num_linea);
    lanzar_proceso_contador(indice_tabla, linea, numero_linea_str);
    //falta aumentar de alguna forma la linea donde se encuentra   
    indice_tabla++;
    num_linea++;
  }

  fclose(fp);

  //Lanza un proceso por patron
  for (int i = 1; i <= longitud(patrones); i++)
  {
    //obtiene el patron de la lista
    char *patron = getElementoN(patrones, i);
    //lanza la funcion por patron
    lanzar_proceso_procesador(indice_tabla, patron, nombre_fichero); //getElementoN(patrones, i) eliminado para probar sin esto
    indice_tabla++;
  }

  printf("[MANAGER] %d procesos creados.\n", indice_tabla);
//uso un sleep porque los hijos me crasheaban y no les daba tiempo a iniciar, entonces con el sleep paramos el programa 1 seg y asi les da tiempo a funcionar
  sleep(1);


}
//se crean los hijos del tipo contador
void lanzar_proceso_contador(const int indice_tabla, const char *linea, const char *numero_linea_str)
{
  pid_t pid;
//se crean los nenes
  switch (pid = fork())
  {
  case -1: //caso de error
    fprintf(stderr, "[MANAGER] Error al lanzar proceso contador: %s.\n", strerror(errno));
    terminar_procesos();
//    liberar_recursos();
    exit(EXIT_FAILURE);
  case 0: //caso existoso
    if (execl(RUTA_CONTADOR, CLASE_CONTADOR, linea, numero_linea_str, NULL) == -1)
    {
      fprintf(stderr, "[MANAGER] Error usando execl() en el poceso %s: %s.\n", CLASE_CONTADOR, strerror(errno));
      exit(EXIT_FAILURE);
    }
  }
//codigo del padre para guardar la info del hijo en la tabla
  g_process_table[indice_tabla].pid = pid;
  g_process_table[indice_tabla].clase = CLASE_CONTADOR;
}
//lo mismo que la funcion anterior pero ahora en vez de hijos contadores son procesadores
void lanzar_proceso_procesador(const int indice_tabla, const char *patron, const char *nombre_fichero)
{
  pid_t pid;

  switch (pid = fork())
  {
  case -1:
    fprintf(stderr, "[MANAGER] Error al lanzar proceso procesador: %s.\n", strerror(errno));
    terminar_procesos();
//    liberar_recursos();
    exit(EXIT_FAILURE);
  case 0:
    if (execl(RUTA_PROCESADOR, CLASE_PROCESADOR, nombre_fichero, patron, NULL) == -1)
    {
      fprintf(stderr, "[MANAGER] Error usando execl() en el proceso %s: %s.\n", CLASE_PROCESADOR, strerror(errno));
      exit(EXIT_FAILURE);
    }
  }

  g_process_table[indice_tabla].pid = pid;
  g_process_table[indice_tabla].clase = CLASE_PROCESADOR;
}

//espera a los procesos hijos, a que terminen
void esperar_procesos()
{
  int i, n_processes = g_nProcesses;
  pid_t pid;
//bucle en el que espera mientas haya hijos activosindice_tabla
  while (n_processes > 0)
  {
    pid = wait(NULL);
    for (i = 0; i < g_nProcesses; i++)
    {
      //condocion para que si un hijo entra en la tabla y ha terminado baje el numero de hijos activos
      if (pid == g_process_table[i].pid)
      {
        printf("[MANAGER] Proceso %s terminado [%d]...\n", g_process_table[i].clase, g_process_table[i].pid);
        g_process_table[i].pid = 0;
        n_processes--;
        break;
      }
    }
  }
}
//libera memoria 
void liberar_recursos(TLista *patrones)
{
  if(g_process_table != NULL)
  {
    free(g_process_table);
    g_process_table = NULL;
  }
  if(patrones != NULL)
  {
    destruir(patrones);
  }
  
}
//para enviar ctrl c  a los hijos activos en caso de que se queden tontos
void terminar_procesos(void)
{
  int i;

  printf("\n----- [MANAGER] Terminar con los procesos hijos ejecutándose ----- \n");
  for (i = 0; i < g_nProcesses; i++) //se recorre la tabla de procesos
  {
    //si encuentra un proceso  se lo carga
    if (g_process_table[i].pid != 0)
    {
      printf("[MANAGER] Terminando proceso %s [%d]...\n", g_process_table[i].clase, g_process_table[i].pid);
      if (kill(g_process_table[i].pid, SIGINT) == -1)
      {
        fprintf(stderr, "[MANAGER] Error al usar kill() en proceso %d: %s.\n", g_process_table[i].pid, strerror(errno));
      }
    }
  }
}

//NOTA: el cacharro este { } se pone debajo de la fucnion y no a su lado como en java, que luego me da error
//Manager: este proceso será responsable de crear un número determinado de procesos de tipo
//procesador y de tipo contador, gestionando de manera adecuada su finalización y liberando
//los recursos previamente reservados. Este proceso recibirá por la línea de órdenes la ruta de dos ficheros.
//<archivo_texto>: será abierto por el proceso manager.
//Leerá su contenido línea a línea.
//<archivo_patrones>: este será procesado por el proceso manager.
//Leerá su contenido palabra a palabra.
//Por cada palabra insertará un nodo en una lista empleando la estructura de datos creada en la práctica P1.1.1.
//Procesando dicha lista, creará un proceso procesador por cada nodo de la misma.


//// Esto devuelve un char, pero necesitas un char* (el patrón)
//char *patron = getElementoN(&patrones, i); en getElementoN 
