#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
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
int nMaquinas;
struct cliente{
    int id;
    bool atendido;
    char tipo;
    int  ascensor;
};

bool ascensorEnPlanta;
bool *MaquinasCheckIn;
int *recepcionistas;

int main(int argc,char *argv[]){
    
    if(argc!=3){

        printf("Se ha introducido incorrectamente el numero");
        exit(-1);  

    }


    nClientes=atoi(argv[1]);
    nMaquinas=atoi(argv[2]
    for( int i =0; i<nClientes; ){

    }
    MaquinasCheckIn= (bool *) malloc (nMaquinas) * sizeof (bool));
    //Se inicializan por defecto todas las maquinas a cero (libres)


    srand(time(NULL));
    int clientesEnRecepcion = 0;

    /**
    *   Los clientes seran asignados como vip o normales aleatoriamente
    */
    int clientesEnRecepcion = 0;


    exit 0;
}

void nuevoCliente(int signum){

    if(signal(SIGUSR2, nuevoCliente)== SIG_ERR) {
        perror("\nError en la llamada a la señal\n");  
        exit(-1);
    }

    if(signal(SIGUSR1, nuevoCliente)== SIG_ERR) {
        perror("\nError en la llamada a la señal\n");  
        exit(-1);
    }

    pthread_mutex_lock(&semaforoColaClientes);
    
    if(clientesEnRecepcion<recepcionMAX){
        cliente nuevoCliente;
        clientesEnRecepcion++;
        nuevoCliente.id=clientesEnRecepcion;

        if(signum==SIGUSR1){
            nuevoCliente.tipo="DEF";
        }else if(signum==SIGUSR2){
            nuevoCliente.tipo="VIP"; 
        }else     if(signum == SIG_ERR) {
            perror("\nError en la llamada a la señal\n");  
        }

        nuevoCliente.atendido=0;
        nuevoCliente.ascensor=0;
        pthread_t hiloCliente;
        pthread_create(&hiloCliente, NULL, AccionesCliente, (void*)nuevoCliente);

    }
    pthread_mutex_unlock(&semaforoColaClientes);
}

void AccionesCliente (void* nuevoCliente ){
    (cliente *) nuevoCliente;
    int maquinaDirecta=calculaAleatorios(1,100);
    if(accion<=10){
        //meter todo esto a un metodo directamente para poder pasarlo 
        //10% va a la maquina directamente
        do{
            pthread_mutex_lock(&semaforoMaquinas);
            int meQuedo;
        
            int maquinaAOcupar=maquinaLibre();

            if(maquinaAOcupar!=-1){

                MaquinasCheckIn[maquinaAOcupar]=true;
                pthread_mutex_unlock(&semaforoMaquinas);
                sleep(6);
                meQuedo=2; // no se queda ya esta atendido
                //iria al ascensor (falta implementar todo eso) si puede se escribiria el log y lo eliminamos

            }else{

                pthread_mutex_unlock(&semaforoMaquinas);
                sleep(3);
                meQuedo=calculaAleatorios(1,2);
                //1 se queda 2 se cansa de esperar
            }

        }while(meQuedo==1); //repite si se queda si no pasa a buscar cola
    }else{
        pthread_mutex_lock(&semaforoColaClientes);
        if(nuevoCliente.atendido=1){
            pthread_mutex_unlock(&semaforoColaClientes);

            while(nuevoCliente.atendido==1){
                //Cuando termine el recepcionista este valor se establecera en 2
                sleep(2);
                //comprueba si ha acabado cada 2 secs
            }
            //va a coger los ascensores (logs y demas)


        }else{
            pthread_mutex_unlock(&semaforoColaClientes);

            int comportamiento = calculaAleatorios(1,100);
            if(comportamiento<=20){
                //vuelve a maquinas
            }else if(comportamiento<=30){
                //se va (eliminamos el hilo)
            }else{
                int pierdoTurnoPorBaño =calculaAleatorios(1,20);
                if(pierdoTurnoPorBaño==1){
                    //pierde el turno por ir al banyo se enfada y se va
                }
            }

        }

    }
    //Ascensor, una vez atendidos 30% el resto van directamente a su habitacion
}

int maquinaLibre(){
    for(int i=0; i<nMaquinas; i++){
        if(MaquinasCheckIn[]==false){
            return i;
        }
    }
    return -1;
}


pid_t gettid(void) {

    return syscall(__NR_gettid);

} 

//Escribir mensajes 

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

//Metodo para calcular numeros aleatorios 
int calculaAleatorios(int min, int max) {

    srand(time(NULL));
    return rand() % (max-min+1) + min;

}