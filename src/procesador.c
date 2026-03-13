//no hay modificaciones aqui
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

    FILE *fp;
    char linea[256]; //tamaño del buffer por linea
    int encontrado = 0;
    //error
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <fichero_texto> <patron>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    //funciona
    char *nombre_fichero = argv[1];
    char *patron = argv[2];
    //if para error al abrir
    if ((fp = fopen(nombre_fichero, "r")) == NULL) {
        fprintf(stderr, "[PROCESADOR %d] Error al abrir %s\n", getpid(), nombre_fichero);
        exit(EXIT_FAILURE);
    }
    
    printf("[PROCESADOR %d] Buscando patrón '%s' en %s\n", 
           getpid(), patron, nombre_fichero);
    
    while (fgets(linea, sizeof(linea), fp) != NULL) {
      
      
        char *token = strtok(linea, " \t\n\r");
        while (token != NULL) {
      
      
            if (strcmp(token, patron) == 0) {
                printf("[PROCESADOR %d] Patrón '%s' ENCONTRADO\n", 
                       getpid(), patron);
                encontrado = 1;
                break;
            }
            token = strtok(NULL, " \t\n\r");
        }
        if (encontrado) break;
    }
    
    if (!encontrado) {
        printf("[PROCESADOR %d] Patrón '%s' NO encontrado\n", 
               getpid(), patron);
    }
    
    fclose(fp);
    return EXIT_SUCCESS;
}
//Procesador: estos procesos recibirán en el momento de su creación la ruta del fichero
//<archivo_texto> y un patrón. Su función consistirá en comprobar si este patrón se corresponde
//con alguna de las palabras que conforman el texto contenido en el fichero <archivo_texto>.