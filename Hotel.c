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
struct cliente *arrayClientes;
int *arrayMaquinas;
struct cliente{
    char id [10];
    bool atendido; //0 no 1 esta siendo atendido 2 ya fue atendido
    char tipo [3];
    int  ascensor;
};

struct recepcionista{
    char id [16];
    int clientesAtendidos;
    char tipo [3];
}

pthread_t *arrayHilosClientes;
bool ascensorEnPlanta;
bool *MaquinasCheckIn;
int *recepcionistas;
struct cliente *arrayClientesEnAscensor;

int main(int argc,char *argv[]){
    
	/*Declaración de los hilos de los recepcionistas*/
	pthread_t recepcionista_1, recepcionista_2, recepcionista_3;

	/*Comprobación de que el número de argumentos introducidos es el correcto*/
    if(argc!=3){

        printf("Se ha introducido incorrectamente el numero de parámetros");
        exit(-1);  
    }
	
	/*Impresión del pid del proceso para enviar las señales*/
	printf("\nPID = %d\n", getpid());

	/*Paso de los argumentos a int y creación de arrays dinámicos*/
    nClientes=atoi(argv[1]);
    nMaquinas=atoi(argv[2]);

	arrayClientes = (int *)malloc(nClientes * sizeof(int)); //Está declarado como "struct cliente"
	recepcionistas = (int *)malloc(recepcionistas * sizeof(int));
    MaquinasCheckIn= (bool *) malloc (nMaquinas * sizeof (bool));
    arrayClientesEnAscensor = (struct cliente *)malloc(6 * sizeof(struct cliente *));
	
	arrayMaquinas = (int *)malloc(nMaquinas * sizeof(int));
	arrayHilosClientes = (pthread_t *)malloc (nClientes * sizeof(pthread_t));

	/*Enmascaración de señales*/
	if (signal(SIGUSR1, nuevoCliente) == SIG_ERR) {
		perror("Error en signal");
	
		exit(-1);
	}

	if (signal(SIGUSR2, nuevoCliente) == SIG_ERR) {
		perror("Error en signal");
	
		exit(-1);
	}

	if (signal(SIGINT, finalizaPrograma) == SIG_ERR) {
		perror("Error en la señal signal");
	
		exit(-1);
	}

	/*INICIALIZACIÓN DE LOS RECURSOS*/


	/*Inicialización de los semáforos*/

	if (pthread_mutex_init(&semaforoColaClientes, NULL/*Por defecto*/) != 0) exit (-1);
	if (pthread_mutex_init(&semaforoAscensor, NULL) != 0) exit (-1);
	if (pthread_mutex_init(&semaforoMaquinas, NULL) != 0) exit (-1);

	/*Creación de clientes básicos y VIP*/
	/*for( int i =0; i < nClientes; i++){
			
    }*/

    //Se inicializan por defecto todas las maquinas a false (no ocupadas)


    int clientesEnRecepcion = 0;

	/*Creación de los hilos de los recepcionistas*/
    //atributos en principio a NULL (por defecto)
	if (pthread_create (&recepcionista_1, NULL, /*Función que realiza (método)*/, /*Argumento del método*/) != 0) { //Comprobación de que el hilo se crea correctamente
		perror("Error en la creación del hilo");
		
		exit (-1);
	}
	if (pthread_create (&recepcionista_2, NULL, /*Función que realiza (método)*/, /*Argumento del método*/) != 0) {
		perror("Error en la creación del hilo");
		
		exit (-1);
		
	}
	if (pthread_create (&recepcionista_3, NULL, /*Función que realiza (método)*/, /*Argumento del método*/) != 0) {
		perror("Error en la creación del hilo");
		
		exit (-1);

	}

	/*Función join para que el main espera por la ejecución del hilo*/ //NO ESTOY SEGURO DE QUE TODOS NECESITEN SER JOINADOS
	pthread_join(recepcionista_1, void **retval/*Valor de retorno, por defecto = NULL*/); //Comprobar que tiene que devolver 
	pthread_join(recepcionista_2, void **retval/*Valor de retorno, por defecto = NULL*/);
	pthread_join(recepcionista_3, void **retval/*Valor de retorno, por defecto = NULL*/);

	/*Esperar por senales de forma infinita*/
	while(1) { //Imagino

	}
	
	/*Se libera la memoria de los arrays dinámicos*/
	free(arrayClientes);
	free(MaquinasCheckIn);
	free(recepcionistas);

    //exit 0;
	return 0; //El main termina con return 0 no un exit
}

void nuevoCliente(int signum){

    pthread_mutex_lock(&semaforoColaClientes);
    
    if(clientesEnRecepcion<recepcionMAX){

        struct cliente *nuevoCliente;
        clientesEnRecepcion++;
        char numeroEnId [3];
        itoa(clientesEnRecepcion, numeroEnId, 10);
        nuevoCliente->id=strcat("cliente_",numeroEnId);

        /*if(signum==SIGUSR1){
            nuevoCliente->tipo="DEF"; //Default
        }else if(signum==SIGUSR2){
            nuevoCliente->tipo="VIP"; //VIP
        }else if(signum == SIG_ERR) {
            perror("\nError en la llamada a la senal\n");  
        }*/

		/*Indicación de si es cliente VIP o no por medio de señales*/
        switch(signum) {
			case SIGUSR1:
				if (signal(SIGUSR1, nuevoCliente) == SIG_ERR) {
					perror("Error en signal");
     
                    exit(-1);
				}
 
                nuevoCliente -> tipo = "DEF"; //Default
 
                break;
			case SIGUSR2:
				if (signal(SIGUSR2, nuevoCliente) == SIG_ERR) {
					perror("Error en signal");
     
                    exit(-1);
                }
 
                nuevoCliente -> tipo = "VIP"; //VIP
		}


        nuevoCliente->atendido=0;
        nuevoCliente->ascensor=0;

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



void colaAccion(void *nuevoCliente){

    pthread_mutex_lock(&semaforoColaClientes);

        if(arrayClientes[(int *)nuevoCliente]->atendido==1){

            pthread_mutex_unlock(&semaforoColaClientes);

            while(arrayClientes[(int *)nuevoCliente]->atendido==1){
                //Cuando termine el recepcionista este valor se establecera en 2
                sleep(2);
                //comprueba si ha acabado cada 2 secs
            }
            //AccionAscensor(nuevoCliente);

        }else{
            pthread_mutex_unlock(&semaforoColaClientes);

            int comportamiento = calculaAleatorios(1,100);
            if(comportamiento<=20){
                maquinaAccion(nuevoCliente)
                return;
            }else if(comportamiento<=30){
                pthread_mutex_lock(&semaforoColaClientes);
                borrarCliente((int *)nuevoCliente);
                pthread_mutex_unlock(&semaforoColaClientes);
                pthread_cancel(gettid());
            }else{
                int pierdoTurnoPorBano =calculaAleatorios(1,20);
                if(pierdoTurnoPorBano==1){
                    //pierde el turno por ir al banyo se enfada y se va
                    pthread_mutex_lock(&semaforoColaClientes);
                    borrarCliente((int *)nuevoCliente);
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

        borrarCliente(nuevoCliente);
        pthread_mutex_unlock(&colaClientes);
        pthread_cancel(gettid());

    }
    pthread_mutex_unlock(&colaClientes);

    pthread_mutex_lock(&semaforoAscensor);
    while(!ascensorEnPlanta){
        sleep(3);
    }

    if(clientesAscensor<6){
        clientesAscensor++;
        if(clientesAscensor==6){
            int tiempo= calculaAleatorios(3,6);
            sleep(tiempo)
            
            //log

        }else{
            //log
            //esperamos
        }
    }


    
    while(!ascensorEnMovimiento){
        
    }
}

void borrarCliente(int *posCliente){
    for(int i=(int *)posCliente; i<this.clientesEnRecepcion-1; i++){
        arrayClientes[i]=arrayClientes[i+1];
    }
    arrayClientes[clientesEnRecepcion-1]=null;
}


void AccionesRecepcionista(void *recepcionistaStruct){
    while(true){
        do{
            pthread_mutex_lock(&semaforoColaClientes);
            int clienteAtendiendo=-1;
            for (int i=0;i<clientesEnRecepcion; i++){
                if(colaClientes[i]->atendido==0&&colaClientes[i]->tipo==((struct recepcionistaStruct *)recepcionista)->tipoAtencion){
                    clienteAtendiendo=i;
                }
            }
            if (clienteAtendiendo==-1){
                pthread_mutex_unlock(&semaforoColaClientes);
                sleep(1);
            }
        }while(clienteAtendiendo==-1);
        colaClientes[clienteAtendiendo]->atendido=1;
        pthread_mutex_unlock(&semaforoColaClientes);

        int tipoAtencion= calculaAleatorios(1,100);
        if(tipoAtencion<=80){
            //Todo perfecto
            int tiempoEspera=calculaAleatorios(1,4);
            sleep(tiempoEspera);
            // Implementar Se mandaria al ascensor al cliente 
            //AlAscensor(clienteAtendiendo)
        }else if(tipo Atencion<=90){
            //Algun problema
            int tiempoEspera=calculaAleatorios(2,6);
            sleep(tiempoEspera);
            //AlAscensor(clienteAtendiendo)
        }else{
            //No esta vacunado
            int tiempoEspera=calculaAleatorios(6,10);
            sleep(tiempoEspera);
            borrarCliente(clienteAtendiendo);
            pthread_cancel(arrayHilosClientes[clienteAtendiendo]);
            //Implementar Le echamos
        }
        colaClientes[clienteAtendiendo]->atendido=2;
        ((struct recepcionistaStruct *)recepcionista)->contadorDeClientes++;
        //Implementar Eliminamos cliente de la cola
        if(((struct recepcionistaStruct *)recepcionista)->tipoAtencion=="DEF"){
            if(((struct recepcionistaStruct *)recepcionista)->contadorDeClientes==5){
                sleep(5);
            }
        }
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

/*Manejadora para la terminación del programa*/
void finalizaPrograma(int sig) {
	if (signal(SIGINT, finalizaPrograma) == SIG_ERR) {
		perror("Error en signal");
	
		exit(-1);
	}
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
    logFile = fopen(logFileName, "a");
    fprintf(logFile, "[%s] %s: %s\n", stnow, id, msg);
    fclose(logFile);

}

//Metodo para calcular numeros aleatorios 
int calculaAleatorios(int min, int max) {

    srand(time(NULL));
    return rand() % (max-min+1) + min;

}
