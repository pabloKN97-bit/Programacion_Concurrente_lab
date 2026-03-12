#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void contar(char *line, int line_number);

int main(int argc, char *argv[])
{
  //TODO

  if (argc !=3){
    //fprintf("Comprobador que funcina el error \n");
    exit(EXIT_FAILURE);
  }
  contar(argv[1], atoi(argv[2]));

  return EXIT_SUCCESS;
}

void contar(char *linea, int numero_linea)
{
  int n_palabras = 0, dentro_palabra = 0;
  const char *it = linea;

  do
  {
    switch (*it)
    {
    case '\0':
    case ' ':
    case '\t':
    case '\n':
    case '\r':
      if (dentro_palabra)
      {
        dentro_palabra = 0;
        n_palabras++;
      }
      break;
    default:
      dentro_palabra = 1;
    }
  } while (*it++);

  printf("[CONTADOR %d] La linea '%d' tiene %d palabras\n", getpid(), numero_linea, n_palabras);
}

//Contador: estos procesos recibirán en el momento de su creación un número de línea y una
//línea. Su función consistirá en contar el número de palabras que conforman la línea recibida.