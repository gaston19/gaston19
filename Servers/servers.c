#include "funciones.h"
#include "definiciones.h"
#include <pthread.h>
#define DESCUENTA_DESDE 		1
#define DESCUENTA_HASTA 		10

pthread_mutex_t mutex;

typedef struct tipo_vias tvias;
struct tipo_vias
{
	int 	cantidad_threads;	
	int 	nro_via;
	int 	id_colamensaje;
	int	cantidad_liberacion;
	
};


void *ThreadVia (void *parametro)
{
	int 			id_cola_mensajes;
	int			cantidad_threads;
	int			nro_via;
	int			i,j;	
	int			cant_vehiculos_cola=0;
	int			done=0;
	int			descuento=0;
	mensaje		msg;

	tvias *datos_thread 	= (tvias*) parametro;
	nro_via	= datos_thread->nro_via;
	cantidad_threads 	= datos_thread->cantidad_threads;
	id_cola_mensajes 	= datos_thread->id_colamensaje;
	
	printf("Soy la via %d\n", nro_via+1);

	while(done==0)
	{
		recibir_mensaje(id_cola_mensajes, MSG_VIAS+nro_via, &msg);	//bloqueate
		//pthread_mutex_lock (&mutex);
	
			switch (msg.int_evento)
			{
				case EVT_VEHICULOS:
					cant_vehiculos_cola =cant_vehiculos_cola+atoi(msg.char_mensaje);
					printf("\nVIA:%d Cantidad de Vehiculos %d\n", nro_via, cant_vehiculos_cola);
					descuento=rand()%(DESCUENTA_HASTA+1-DESCUENTA_DESDE)+DESCUENTA_DESDE;			
					printf("\nVIA:%d Saco %d Vehiculos \n", nro_via, descuento);
					cant_vehiculos_cola=cant_vehiculos_cola-descuento;

					if(cant_vehiculos_cola<0)
						cant_vehiculos_cola=0;
		
					printf("\nVIA:%d Cantidad de Vehiculos %d\n", nro_via, cant_vehiculos_cola);
				
					if(cant_vehiculos_cola>10)
					{
						cant_vehiculos_cola=0;
						datos_thread->cantidad_liberacion++;
					}
				break;		    	
				case EVT_FINALIZAR:
					printf("\nVIA:%d Finalizar\n", nro_via);
					done=1;
				break;	
				default:
					printf("\nVIA: %d Evento sin definir\n",nro_via);
				break;
			}
		//pthread_mutex_unlock (&mutex);
	};
	pthread_exit ((void *)"Listo");
}




int main(int argc, char *argv[])
{
	int 			id_cola_mensajes;
	int 			i, j;
	int 			cantidad_threads=1;
	int tot_liberaciones=0;
	
	mensaje			msg;
//	tvias 		*datos_thread;

	if (argc>1)
	{
		cantidad_threads = atoi(argv[1]);
	}
	printf("la cantidad de vias ingresada es %d\n", cantidad_threads);
	
	id_cola_mensajes 	= creo_id_cola_mensajes(CLAVE_BASE);
	srand(time(NULL));

	borrar_mensajes(id_cola_mensajes); //Borra todos los mensajes que haya en la cola.

	pthread_t* idHilo = (pthread_t* ) malloc(sizeof(pthread_t)*cantidad_threads);
	pthread_attr_t 	atributos;
	pthread_attr_init (&atributos);
	pthread_attr_setdetachstate (&atributos, PTHREAD_CREATE_JOINABLE);
	
	datos_thread = (tvias*) malloc(sizeof(tvias)*cantidad_threads);

	for(i=0; i<cantidad_threads; i++)
	{
		datos_thread[i].nro_via = i;
		datos_thread[i].id_colamensaje = id_cola_mensajes;
		datos_thread[i].cantidad_threads=cantidad_threads;
		datos_thread[i].cantidad_liberacion=0;

		pthread_create (&idHilo[i], &atributos, ThreadVia, &datos_thread[i]);
	}
			
	
	for(i=0; i<cantidad_threads; i++)
	{
		pthread_join (idHilo[i], NULL);
		printf("PPAL: Recibi de la via %d: %d\n", i+1, datos_thread[i].cantidad_liberacion);
		tot_liberaciones+= datos_thread[i].cantidad_liberacion;
	}	
	printf("La cantidad total de liberaciones es %d \n", tot_liberaciones);

	free(idHilo);
	free(datos_thread);	
	return 0;
}

    
