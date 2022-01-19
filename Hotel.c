#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <time.h>
#include <linux/unistd.h>
#include <pthread.h>
#include <errno.h>

#define ascensorMAX 6.0
#define recepcionMAX 20.0

void nuevoClienteCrear(int signum);
void AccionesCliente (void* nuevoCliente );
void maquinaAccion(int nuevoCliente);
void colaAccion(int nuevoCliente);
void alAscensor(void *nuevoCliente);
void borrarCliente(int posCliente);
void AccionesRecepcionista(void *recepcionistaStruct);
int maquinaLibre();
void finalizaPrograma(int sig);
pid_t gettid(void);
void writeLogMessage(char *id, char *msg);
int calculaAleatorios(int min, int max);

FILE *logFile;

int ocupacionAscensor;
int nClientes;
int nMaquinas;
int clientesEnRecepcion;
bool ascensorEnPlanta;
bool *MaquinasCheckIn;
pthread_mutex_t semaforoColaClientes;
pthread_mutex_t semaforoAscensor;
pthread_mutex_t semaforoMaquinas;
pthread_t *arrayHilosClientes;

struct cliente *arrayClientesEnAscensor;
struct cliente *colaClientes;

struct cliente{
    char *id;
    int atendido; //0 no 1 esta siendo atendido 2 ya fue atendido
    char *tipo;
    int  ascensor;
};

struct recepcionista{
    char *id;
    int clientesAtendidos;
    char *tipo;
};

int main(int argc,char *argv[]){
    
	/*Declaracion de los hilos de los recepcionistas*/
	pthread_t recepcionista11, recepcionista22, recepcionista33;
	/*Comprobacion de que el numero de argumentos introducidos es el correcto*/
    if(argc!=3){
        printf("Se ha introducido incorrectamente el numero de parametros");
        
        exit(-1);  
    }
	
	/*Impresion del pid del proceso para enviar las seniaales*/
	printf("\nPID = %d\n", getpid());

	/*Paso de los argumentos a int y creacion de arrays dinamicos*/
    nClientes=atoi(argv[1]);
    nMaquinas=atoi(argv[2]);
	
    MaquinasCheckIn= (bool *) malloc (nMaquinas * sizeof (bool));
    arrayClientesEnAscensor = (struct cliente *) malloc (6 * sizeof(struct cliente *));
	arrayHilosClientes = (pthread_t *) malloc (nClientes * sizeof(*arrayHilosClientes));
	colaClientes = (struct cliente *) malloc (nClientes * sizeof(struct cliente *)); //Pongo nClientes para probar

	/*Enmascaracion de seniales*/
	if (signal(SIGUSR1, nuevoClienteCrear) == SIG_ERR) {
		perror("Error en signal");
	
		exit(-1);
	}

	if (signal(SIGUSR2, nuevoClienteCrear) == SIG_ERR) {
		perror("Error en signal");
	
		exit(-1);
	}

	if (signal(SIGINT, finalizaPrograma) == SIG_ERR) {
		perror("Error en la seniaal signal");
	
		exit(-1);
	}

	/*INICIALIZACIoN DE LOS RECURSOS*/

	/*Inicializacion de los semaforos*/
	if (pthread_mutex_init(&semaforoColaClientes, NULL) != 0) exit (-1);
	if (pthread_mutex_init(&semaforoAscensor, NULL) != 0) exit (-1);
	if (pthread_mutex_init(&semaforoMaquinas, NULL) != 0) exit (-1);

	/*Contador de clientes*/
    int clientesEnRecepcion = 0;

	/*Lista de clientes id 0, atendido 0, tipo 0, serologia 0*/
    //Inicializar todo a 0?

	/*Lista de recepcionistas*/
	struct recepcionista *recepcionista1;
    recepcionista1->id="recepcionista_1";
    recepcionista1->clientesAtendidos=0;
    recepcionista1->tipo="DEF";

	struct recepcionista *recepcionista2;
    recepcionista2->id="recepcionista_2";
    recepcionista2->clientesAtendidos=0;
    recepcionista2->tipo="DEF";

	struct recepcionista *recepcionista3;
    recepcionista3->id="recepcionista_3";
    recepcionista3->clientesAtendidos=0;
    recepcionista3->tipo="VIP";

	/*Maquinas de check in (ponerlas todas como libres)*/
    //Se inicializan por defecto todas las maquinas a false (no ocupadas)

	/*Fichero de log*/
	logFile = fopen("registroLogs.log", "wt");
	fclose(logFile);

	/*Variables relativas al ascensor*/
    int ocupacionAscensor = 0;
    bool ascensorEnPlanta; //se inicializa por defecto en false

	/*Creacion de los hilos de los recepcionistas*/
	if (pthread_create (&recepcionista11, NULL, AccionesRecepcionista, (void *) recepcionista1) != 0) { //Comprobacion de que el hilo se crea correctamente
		perror("Error en la creacion del hilo");
		
		exit (-1);
	}

	if (pthread_create (&recepcionista22, NULL, AccionesRecepcionista, (void *)recepcionista2) != 0) {
		perror("Error en la creacion del hilo");
		
		exit (-1);
		
	}

	if (pthread_create (&recepcionista33, NULL, AccionesRecepcionista, (void *)recepcionista3) != 0) {
		perror("Error en la creacion del hilo");
		
		exit (-1);

	}

	/*Funcion join para que el main espera por la ejecucion del hilo*/
	pthread_join(recepcionista11, NULL); //Por defecto el valor de retorno es NULL 
	pthread_join(recepcionista22, NULL); 
	pthread_join(recepcionista33, NULL);

	/*Esperar por senales de forma infinita*/
	while(1) {
		pause(1); 
	}
	
	/*Se libera la memoria de los arrays dinamicos*/
	free(colaClientes);
	free(MaquinasCheckIn);
	free(arrayHilosClientes);
	free(arrayClientesEnAscensor);

	return 0;
}

void nuevoClienteCrear(int signum){

    pthread_mutex_lock(&semaforoColaClientes);
    
    if(clientesEnRecepcion<recepcionMAX){

        struct cliente nuevoCliente;
        clientesEnRecepcion++;
        char numeroEnId [3];
        sprintf(numeroEnId,"%d", clientesEnRecepcion);
        nuevoCliente.id=strcat("cliente_",numeroEnId);

		/*Indicacion de si es cliente VIP o no por medio de seniaales*/
        switch(signum) {
			case SIGUSR1:
				if (signal(SIGUSR1, nuevoClienteCrear) == SIG_ERR) {
					perror("Error en signal");
     
                    exit(-1);
				}
 
                nuevoCliente.tipo = "DEF"; //Default
 
                break;
			case SIGUSR2:
				if (signal(SIGUSR2, nuevoClienteCrear) == SIG_ERR) {
					perror("Error en signal");
     
                    exit(-1);
                }
 
                nuevoCliente.tipo = "VIP"; //VIP
		}

        nuevoCliente.atendido=0;
        nuevoCliente.ascensor=0;
        
        colaClientes[clientesEnRecepcion-1]=nuevoCliente;
        pthread_t hiloCliente;
        arrayHilosClientes[clientesEnRecepcion-1]=hiloCliente;
        pthread_create(&arrayHilosClientes[clientesEnRecepcion], NULL, AccionesCliente, (void*)(clientesEnRecepcion-1));
        pthread_mutex_unlock(&semaforoColaClientes);

    }else{
        pthread_mutex_unlock(&semaforoColaClientes);
        return;
    }
    
}

void AccionesCliente (void* nuevoCliente ){
    (int *) nuevoCliente;

    //nuevoCliente= (int *) malloc (nClientes * sizeof (int)); -> reserva dinámica de memoria

    int maquinaDirecta=calculaAleatorios(1,100);
    if(maquinaDirecta<=10){
        //meter todo esto a un metodo directamente para poder pasarlo 
        //10% va a la maquina directamente
        maquinaAccion((int )nuevoCliente);
    }else{
        colaAccion((int )nuevoCliente);
    }
    //Ascensor, una vez atendidos 30% el resto van directamente a su habitacion

	//free(nuevoCliente); -> liberación de memoria
}

void maquinaAccion(int nuevoCliente){
    int meQuedo;

    do{
        pthread_mutex_lock(&semaforoMaquinas);
        
        int maquinaAOcupar=maquinaLibre();

        if(maquinaAOcupar!=-1){

            MaquinasCheckIn[maquinaAOcupar]=true;
            pthread_mutex_unlock(&semaforoMaquinas);
            sleep(6);

            pthread_mutex_lock(&semaforoMaquinas);
            MaquinasCheckIn[maquinaAOcupar]=true;
            pthread_mutex_unlock(&semaforoMaquinas);
            
            //alAscensor(nuevoCliente)
        }else{

            pthread_mutex_unlock(&semaforoMaquinas);
            sleep(3);
            meQuedo=calculaAleatorios(1,2);
            //1 se queda 2 se cansa de esperar

        }

    }while(meQuedo==1); //repite si se queda si no pasa a buscar cola

    colaAccion(nuevoCliente);
}



void colaAccion(int nuevoCliente){

    pthread_mutex_lock(&semaforoColaClientes);
        (int *)nuevoCliente;
        if(colaClientes[(nuevoCliente)].atendido==1){

            pthread_mutex_unlock(&semaforoColaClientes);

            while(colaClientes[(nuevoCliente)].atendido==1){
                //Cuando termine el recepcionista este valor se establecera en 2
                sleep(2);
                //comprueba si ha acabado cada 2 secs
            }
            //AccionAscensor(nuevoCliente);

        }else{
            pthread_mutex_unlock(&semaforoColaClientes);

            int comportamiento = calculaAleatorios(1,100);
            if(comportamiento<=20){
                maquinaAccion(nuevoCliente);
                return;
            }else if(comportamiento<=30){
                pthread_mutex_lock(&semaforoColaClientes);
                borrarCliente((int )nuevoCliente);
                pthread_mutex_unlock(&semaforoColaClientes);
                pthread_cancel(gettid());
            }else{
                int pierdoTurnoPorBano =calculaAleatorios(1,20);
                if(pierdoTurnoPorBano==1){
                    //pierde el turno por ir al banyo se enfada y se va
                    pthread_mutex_lock(&semaforoColaClientes);
                    borrarCliente((int )nuevoCliente);
                    pthread_mutex_unlock(&semaforoColaClientes);

                    pthread_cancel(gettid());
                }
            }

        }
}


void alAscensor(void *nuevoCliente){
    
    pthread_mutex_unlock(&colaClientes);
    int cojoAscensor= calculaAleatorios(1,10);
    if(cojoAscensor<=3){

        borrarCliente((int )nuevoCliente);
        pthread_mutex_unlock(&colaClientes);
        pthread_cancel(gettid());

    }
    pthread_mutex_unlock(&colaClientes);

    pthread_mutex_lock(&semaforoAscensor);
    while(!ascensorEnPlanta){
        sleep(3);
    }

    if(ocupacionAscensor<6){
        ocupacionAscensor++;
        if(ocupacionAscensor==6){
            int tiempo= calculaAleatorios(3,6);
            sleep(tiempo);
            
            //log

        }else{
            //log
            //esperamos
        }
    }
}

void borrarCliente(int posCliente){
    for(int i=posCliente; i<(clientesEnRecepcion-1); i++){
        colaClientes[i]=colaClientes[i+1];
    }
    struct cliente structvacia;
    colaClientes[clientesEnRecepcion-1]=structvacia;
}


void AccionesRecepcionista(void *recepcionistaStruct){
        int clienteAtendiendo=-1;
        do{
            pthread_mutex_lock(&semaforoColaClientes);
            
            for (int i=0;i<clientesEnRecepcion; i++){
                if(colaClientes[i].atendido==0&&colaClientes[i].tipo==((struct recepcionista *)recepcionistaStruct)->tipo){
                    clienteAtendiendo=i;
                }
            }
            if (clienteAtendiendo==-1){
                pthread_mutex_unlock(&semaforoColaClientes);
                sleep(1);
            }
        } while (clienteAtendiendo==-1);
        int * pos = &clienteAtendiendo;
        colaClientes[clienteAtendiendo].atendido=1;
        pthread_mutex_unlock(&semaforoColaClientes);

        int tipoAtencion= calculaAleatorios(1,100);

        if(tipoAtencion<=80){
            int tiempoEspera=calculaAleatorios(1,4);
            sleep(tiempoEspera);
            // Implementar Se mandaria al ascensor al cliente 
            //AlAscensor(clienteAtendiendo)
        }else if(tipoAtencion<=90){
            //Algun problema
            int tiempoEspera=calculaAleatorios(2,6);
            sleep(tiempoEspera);
            //AlAscensor(clienteAtendiendo)
        }else{
            //No esta vacunado
            int tiempoEspera=calculaAleatorios(6,10);
            sleep(tiempoEspera);
            borrarCliente((int) pos);
            pthread_cancel(arrayHilosClientes[clienteAtendiendo]);
            //Implementar Le echamos
        }
        colaClientes[clienteAtendiendo].atendido=2;
        ((struct recepcionista *)recepcionistaStruct)->clientesAtendidos++;
        //Implementar Eliminamos cliente de la cola
        if(((struct recepcionista *)recepcionistaStruct)->tipo=="DEF"){
            if(((struct recepcionista *)recepcionistaStruct)->clientesAtendidos==5){
                sleep(5);
            }
        }
    
}


int maquinaLibre(){
    for(int i=0; i<nMaquinas; i++){
        if(MaquinasCheckIn[i]==false){
            return i;
        }
    }
    return -1;
}

/*Manejadora para la terminacion del programa*/
void finalizaPrograma(int sig) {
	if (signal(SIGINT, finalizaPrograma) == SIG_ERR) {
		perror("Error en signal");
	
		exit(-1);
	}
	//Al finalizar debe terminar de atender a todos los clientes en cola, pero ya no podran subir en el ascensor
	
	printf("\nHasta luego...\n");

	exit(0);
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
    logFile = fopen("logFileName", "a");
    fprintf(logFile, "[%s] %s: %s\n", stnow, id, msg);
    fclose(logFile);

}

//Metodo para calcular numeros aleatorios 
int calculaAleatorios(int min, int max) {

    srand(time(NULL));
    return rand() % (max-min+1) + min;

}
