#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <time.h>




int main(int argc,char *argv[]){
    
    
    if(argc!=3){

        printf("Se ha introducido incorrectamente el numero");
        exit(-1);  

    }

    int nClientes=atoi(argv[1]);
    int *MaquinasCheckIn= (int *) malloc (atoi(argv[2]) * sizeof (int));
    //Se inicializan por defecto todas las maquinas a cero (libres)


    srand(time(NULL));
    int clientesEnRecepcion = 0;

    /**
    *   Los clientes serán asignados como vip o normales aleatoriamente
    */
    int clientesEnRecepcion = 0;


    exit 0;
}

