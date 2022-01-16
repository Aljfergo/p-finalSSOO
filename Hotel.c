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
    char id;
    bool atendido; //0 no 1 esta siendo atendido 2 ya fue atendido
    char tipo [3];
    int  ascensor;
};

struct recepcionista{
    char id [16];
    int clientesAtendidos;
    char tipo [3];
}

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
    //Se inicializan por defecto todas las maquinas a false (no ocupadas)


    srand(time(NULL));
    int clientesEnRecepcion = 0;

    /**
    *   Los clientes seran asignados como vip o normales aleatoriamente
    */
    


    exit 0;
}

void nuevoCliente(int signum){

    pthread_mutex_lock(&semaforoColaClientes);
    
    if(clientesEnRecepcion<recepcionMAX){
        cliente nuevoCliente;

        clientesEnRecepcion++;
        nuevoCliente.id=clientesEnRecepcion;

        if(signum==SIGUSR1){
            nuevoCliente.tipo="DEF"; //Default
        }else if(signum==SIGUSR2){
            nuevoCliente.tipo="VIP"; //VIP
        }else if(signum == SIG_ERR) {
            perror("\nError en la llamada a la senal\n");  
        }

        nuevoCliente.atendido=0;
        nuevoCliente.ascensor=0;

        colaClientes[clientesEnRecepcion-1]=nuevoCliente;
        pthread_t hiloCliente;
        pthread_create(&hiloCliente, NULL, AccionesCliente, (void*)clientesEnRecepcion);
        pthread_mutex_unlock(&semaforoColaClientes);
    
    }else{
        pthread_mutex_unlock(&semaforoColaClientes);
        return;
    }
    
}

void AccionesCliente (void* nuevoCliente ){

    int maquinaDirecta=calculaAleatorios(1,100);
    if(accion<=10){
        //meter todo esto a un metodo directamente para poder pasarlo 
        //10% va a la maquina directamente
        maquinaAccion(nuevoCliente);
        
    }else{
        colaAccion(nuevoCliente);

    }
    //Ascensor, una vez atendidos 30% el resto van directamente a su habitacion
}

void maquinaAccion(void* nuevoCliente){
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
}

void colaAccion(void *nuevoCliente){
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
                maquinaAccion(nuevoCliente)
                return;
            }else if(comportamiento<=30){
                pthread_cancel(gettid());
            }else{
                int pierdoTurnoPorBano =calculaAleatorios(1,20);
                if(pierdoTurnoPorBano==1){
                    //pierde el turno por ir al banyo se enfada y se va
                    pthread_cancel(gettid());
                }
            }

        }
}


void AccionesRecepcionista(void *tipoDeRecepcionista){
    (char *) tipoDeRecepcionista;
    do{
        pthread_mutex_lock(&semaforoColaClientes);
        int clienteAtendiendo=-1;
        for (int i=0;i<clientesEnRecepcion; i++){
            if(colaClientes[i].atendido==0){
                clienteAtendiendo=i;
            }
        }
        if (clienteAtendiendo==-1){
             pthread_mutex_unlock(&semaforoColaClientes);
            sleep(1);
        }
    }while(clienteAtendiendo==-1);
    colaClientes[clienteAtendiendo].atendido=1;
    pthread_mutex_unlock(&semaforoColaClientes);

    int tipoAtencion= calculaAleatorios(1,100);
    if(tipoAtencion<=80){
        //Todo perfecto
        int tiempoEspera=calculaAleatorios(1,4);
        sleep(tiempoEspera);
        // Implementar Se mandaria al ascensor al cliente 
    }else if(tipo Atencion<=90){
        //Algun problema
        int tiempoEspera=calculaAleatorios(2,6);
        sleep(tiempoEspera);
        //Implementar ascensor
    }else{
        //No esta vacunado
        int tiempoEspera=calculaAleatorios(6,10);
        sleep(tiempoEspera);
        //Implementar Le echamos
    }
    colaClientes[clienteAtendiendo]=2;
    contadorDeClientes++
    //Implementar Eliminamos cliente de la cola
    if(tipoDeRecepcionista=="DEF"){
        //if(contadorDeClientes==5){
        // sleep(5);
        //}
    }



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