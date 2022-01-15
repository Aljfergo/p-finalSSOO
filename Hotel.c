#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <time.h>

#define ascensorMAX 6.0
#define recepcionMAX 20.0

FILE *logFile;
pthread_mutex_t semaforoColaClientes;
pthread_mutex_t semaforoAscensor;
int ocupacionAscensor;
pthread_mutex_t semaforoMaquinas;
int nClientes;

struct cliente{
    int id;
    bool atendido;
    String tipo;
    int  ascensor;
};

bool ascensorEnPlanta;
int *MaquinasCheckIn;
int *recepcionistas;

int main(int argc,char *argv[]){
    
    
    if(argc!=3){

        printf("Se ha introducido incorrectamente el numero");
        exit(-1);  

    }


    nClientes=atoi(argv[1]);

    for( int i =0; i<nClientes; ){

    }
    MaquinasCheckIn= (int *) malloc (atoi(argv[2]) * sizeof (int));
    //Se inicializan por defecto todas las maquinas a cero (libres)


    srand(time(NULL));
    int clientesEnRecepcion = 0;

    /**
    *   Los clientes serán asignados como vip o normales aleatoriamente
    */
    int clientesEnRecepcion = 0;


    exit 0;
}

void nuevoCliente(){


    pthread_mutex_lock(&semaforoColaClientes);
    if(clientesEnRecepcion<recepcionMAX){
        cliente nuevoCliente;
        clientesEnRecepcion++;
        nuevoCliente.id=clientesEnRecepcion;
        //nuevoCliente.tipo=
        nuevoCliente.atendido=0;
        nuevoCliente.ascensor=0;
        pthread_t hiloCliente;
        pthread_create(&hiloCliente, NULL, AccionesCliente, (void*)nuevoCliente);

    }
    pthread_mutex_unlock(&semaforoColaClientes);
}

void AccionesCliente (void* nuevoCliente ){
    (cliente *) nuevoCliente;
    int accion=calculaAleatorios()

}


pid_t gettid(void) {
    return syscall(__NR_gettid);
} 

void writeLogMessage(char *id, char *msg) {
    // Calculamos la hora actual
    time_t now = time(0);
    struct tm *tlocal = localtime(&now);
    char stnow[25];
    strftime(stnow, 25, "%d/%m/%y %H:%M:%S", tlocal);
    // Escribimos en el log
    logFile = fopen(logFileName, "a");
    fprintf(logFile, "[%s] %s: %s\n", stnow, id, msg);
    fclose(logFile);
}

//Metodo para calcular numeros aleatorios (
int calculaAleatorios(int min, int max) {
    srand(time(NULL));
    return rand() % (max-min+1) + min;
}