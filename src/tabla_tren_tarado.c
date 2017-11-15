/*
 ============================================================================
 Name        : tabla_tren_tarado.c
 Author      : lovehinata
 Version     :
 Copyright   : a veces siento que
 Description : Hello World in C, Ansi-style
 ============================================================================

 XXX: https://code.google.com/codejam/contest/32013/dashboard#s=p1
 XXX: http://jamftw.blogspot.mx/2012/05/solutions-to-code-jam-problems.html
 */

#if 1
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <stdint.h>

#ifdef CACA_COMUN_LOG
#include <execinfo.h>
#endif

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#else
#include <time.h>
#include <stdarg.h>
#endif

#if 1
#ifndef LONG_LONG_MAX
#define LONG_LONG_MAX LONG_MAX
#endif

#define CACA_COMUN_TAM_MAX_LINEA (16*200000)
#define CACA_LOG_MAX_TAM_CADENA 2000

#define CACA_COMUN_BUF_STATICO (char[1000] ) { '\0' }

#define BITCH_VECTOR_NUM_BITS (sizeof(bitch_vector) * 8)

#define CACA_COMUN_ASSERT_DUROTE 0
#define CACA_COMUN_ASSERT_SUAVECITO 1
#define CACA_COMUN_ASSERT_NIMADRES 2

#define CACA_COMUN_VALOR_INVALIDO ((tipo_dato)UINT_MAX)
#define CACA_COMUN_IDX_INVALIDO ((natural)CACA_COMUN_VALOR_INVALIDO)

typedef unsigned int natural;
typedef natural tipo_dato;
typedef long long entero_largo;
typedef unsigned long long entero_largo_sin_signo;
typedef long long bitch_vector;
typedef char byteme;

typedef enum BOOLEANOS {
	falso = 0, verdadero
} bool;

/*
 #define CACA_COMUN_TIPO_ASSERT CACA_COMUN_ASSERT_SUAVECITO
 */
#define CACA_COMUN_TIPO_ASSERT CACA_COMUN_ASSERT_DUROTE

#define assert_timeout_dummy(condition) 0;

#if CACA_COMUN_TIPO_ASSERT == CACA_COMUN_ASSERT_DUROTE
#define assert_timeout(condition) assert(condition);
#endif
#if CACA_COMUN_TIPO_ASSERT == CACA_COMUN_ASSERT_SUAVECITO
#define assert_timeout(condition) if(!(condition)){sleep(10);abort();}
#endif
#if CACA_COMUN_TIPO_ASSERT == CACA_COMUN_ASSERT_NIMADRES
#define assert_timeout(condition) 0
#endif

#ifdef CACA_COMUN_LOG
#define caca_log_debug(formato, args...) \
		do \
		{ \
			size_t profundidad = 0; \
			void *array[CACA_LOG_MAX_TAM_CADENA]; \
 			profundidad = backtrace(array, CACA_LOG_MAX_TAM_CADENA); \
			caca_log_debug_func(formato,__FILE__, __func__, __LINE__,profundidad,##args); \
		} \
		while(0);
#else
#define caca_log_debug(formato, args...) 0;
#endif

#define caca_comun_max(x,y) ((x) < (y) ? (y) : (x))
#define caca_comun_min(x,y) ((x) < (y) ? (x) : (y))

void caca_log_debug_func(const char *format, ...);

void caca_comun_current_utc_time(struct timespec *ts) {

#ifdef __MACH__
	clock_serv_t cclock;
	mach_timespec_t mts;
	host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
	clock_get_time(cclock, &mts);
	mach_port_deallocate(mach_task_self(), cclock);
	ts->tv_sec = mts.tv_sec;
	ts->tv_nsec = mts.tv_nsec;
#else
#ifdef CACA_COMUN_LOG
	clock_gettime(CLOCK_REALTIME, ts);
#endif
#endif

}

void caca_comun_timestamp(char *stime) {
	time_t ltime;
	struct tm result;
	long ms;
#ifndef ONLINE_JUDGE
	struct timespec spec;
#endif
	char parte_milisecundos[50];

	ltime = time(NULL);

#ifndef ONLINE_JUDGE
	localtime_r(&ltime, &result);
	asctime_r(&result, stime);
#endif

	*(stime + strlen(stime) - 1) = ' ';

#ifndef ONLINE_JUDGE
	caca_comun_current_utc_time(&spec);
	ms = round(spec.tv_nsec / 1.0e3);
#endif
	sprintf(parte_milisecundos, "%ld", ms);
	strcat(stime, parte_milisecundos);
}
#ifdef CACA_COMUN_LOG
void caca_log_debug_func(const char *format, ...) {

	va_list arg;
	va_list arg2;
	const char *PEDAZO_TIMESTAMP_HEADER = "tiempo: %s; ";
	const char *HEADER =
	"archivo: %s; funcion: %s; linea %d; nivel: %zd caca 8====D ";
	char formato[CACA_LOG_MAX_TAM_CADENA + sizeof(HEADER)
	+ sizeof(PEDAZO_TIMESTAMP_HEADER)] = {'\0'};
	char pedazo_timestamp[sizeof(PEDAZO_TIMESTAMP_HEADER) + 100] = {'\0'};
	char cadena_timestamp[100] = {'\0'};

	caca_comun_timestamp(cadena_timestamp);
	sprintf(pedazo_timestamp, PEDAZO_TIMESTAMP_HEADER, cadena_timestamp);

	strcpy(formato, pedazo_timestamp);
	strcat(formato, HEADER);
	strcat(formato, format);
	strcat(formato, "\n");

	va_start(arg, format);
	va_copy(arg2, arg);
	vprintf(formato, arg2);
	va_end(arg2);
	va_end(arg);
	setbuf(stdout, NULL);
}
#endif

#ifdef CACA_COMUN_LOG
static char *caca_comun_arreglo_a_cadena(tipo_dato *arreglo, int tam_arreglo,
		char *buffer) {
	int i;
	char *ap_buffer = NULL;
	int characteres_escritos = 0;
#ifdef ONLINE_JUDGE
	return NULL;
#endif

	memset(buffer, 0, 100);
	ap_buffer = buffer;

	for (i = 0; i < tam_arreglo; i++) {
		characteres_escritos += sprintf(ap_buffer + characteres_escritos,
				"%12d", *(arreglo + i));
		if (i < tam_arreglo - 1) {
			*(ap_buffer + characteres_escritos++) = ',';
		}
	}
	*(ap_buffer + characteres_escritos) = '\0';
	return ap_buffer;
}

static char *caca_comun_arreglo_a_cadena_natural(natural *arreglo,
		natural tam_arreglo, char *buffer) {
	int i;
	char *ap_buffer = NULL;
	int characteres_escritos = 0;
#ifdef ONLINE_JUDGE
	return NULL;
#endif

	memset(buffer, 0, 100);
	ap_buffer = buffer;

	for (i = 0; i < tam_arreglo; i++) {
		characteres_escritos += sprintf(ap_buffer + characteres_escritos, "%2u",
				*(arreglo + i));
		if (i < tam_arreglo - 1) {
			*(ap_buffer + characteres_escritos++) = ',';
		}
	}
	*(ap_buffer + characteres_escritos) = '\0';
	return ap_buffer;
}
char *caca_comun_matrix_a_cadena(tipo_dato *matrix, natural filas_tam,
		natural columas_tam, char *buffer) {
	int i, j;
	natural inicio_buffer_act = 0;
	for (int i = 0; i < filas_tam; i++) {
		caca_comun_arreglo_a_cadena(matrix + i * columas_tam, columas_tam,
				buffer + inicio_buffer_act);
		inicio_buffer_act += strlen(buffer + inicio_buffer_act);
		buffer[inicio_buffer_act++] = '\n';
		/*		caca_log_debug("pero q mierda inicio buffer act %u %s",inicio_buffer_act,buffer);*/
	}
	return buffer;
}
#else
static char *caca_comun_arreglo_a_cadena(tipo_dato *arreglo, int tam_arreglo,
		char *buffer) {
	return NULL;
}
static char *caca_comun_arreglo_a_cadena_natural(natural *arreglo,
		natural tam_arreglo, char *buffer) {
	return NULL;
}
char *caca_comun_matrix_a_cadena(tipo_dato *matrix, natural filas_tam,
		natural columas_tam, char *buffer) {
	return NULL;
}
#endif
void caca_comun_strreplace(char s[], char chr, char repl_chr) {
	int i = 0;
	while (s[i] != '\0') {
		if (s[i] == chr) {
			s[i] = repl_chr;
		}
		i++;
	}
}

static int caca_comun_lee_matrix_long_stdin(tipo_dato *matrix, int *num_filas,
		int *num_columnas, int num_max_filas, int num_max_columnas) {
	int indice_filas = 0;
	int indice_columnas = 0;
	tipo_dato numero = 0;
	char *siguiente_cadena_numero = NULL;
	char *cadena_numero_actual = NULL;
	char *linea = NULL;

	linea = calloc(CACA_COMUN_TAM_MAX_LINEA, sizeof(char));

	while (indice_filas < num_max_filas
			&& fgets(linea, CACA_COMUN_TAM_MAX_LINEA, stdin)) {
		indice_columnas = 0;
		cadena_numero_actual = linea;
		caca_comun_strreplace(linea, '\n', '\0');
		if (!strlen(linea)) {
			caca_log_debug("weird, linea vacia");
			continue;
		}
		for (siguiente_cadena_numero = linea;; siguiente_cadena_numero =
				cadena_numero_actual) {
			numero = strtol(siguiente_cadena_numero, &cadena_numero_actual, 10);
			if (cadena_numero_actual == siguiente_cadena_numero) {
				break;
			}
			*(matrix + indice_filas * num_max_columnas + indice_columnas) =
					numero;
			indice_columnas++;
		}
		if (num_columnas) {
			num_columnas[indice_filas] = indice_columnas;
		}
		indice_filas++;
		caca_log_debug("las filas son %d, con clos %d", indice_filas,
				indice_columnas);
	}

	*num_filas = indice_filas;
	free(linea);
	return 0;
}

static inline natural caca_comun_cuenta_bitchs(tipo_dato num) {
	natural bitch_cnt = 0;
	tipo_dato num_mod = 0;
	num_mod = num;
	while (num_mod) {
		num_mod &= ~(num_mod & (-num_mod));
		bitch_cnt++;
	}
	return bitch_cnt;
}

#endif
#endif

#if 0

#define QUEUE_VALOR_INVALIDO ((void *)LONG_LONG_MAX)

/* a link in the queue, holds the info and point to the next Node*/
typedef struct Node_t {
	void *datos;
	struct Node_t *prev;
}queue_nodo;

/* the HEAD of the Queue, hold the amount of node's that are in the queue*/
typedef struct queue_t {
	queue_nodo *head;
	queue_nodo *tail;
	int size;
	int limit;
}queue_t;

queue_t *queue_construye(int limit) {
	queue_t *queue = (queue_t*) malloc(sizeof(queue_t));
	if (queue == NULL) {
		return NULL;
	}
	if (limit <= 0) {
		limit = 65535;
	}
	queue->limit = limit;
	queue->size = 0;
	queue->head = NULL;
	queue->tail = NULL;

	return queue;
}

bool queue_vacia(queue_t* pQueue) {
	if (pQueue == NULL) {
		return verdadero;
	}
	if (pQueue->size == 0) {
		return verdadero;
	} else {
		return falso;
	}
}

void *queue_decula(queue_t *pQueue) {
	/*the queue is empty or bad param*/
	queue_nodo *item;
	void *mierda;
	if (queue_vacia(pQueue))
	return QUEUE_VALOR_INVALIDO;
	item = pQueue->head;
	mierda = item->datos;
	pQueue->head = (pQueue->head)->prev;
	pQueue->size--;
	free(item);
	return mierda;
}
void queue_destruye(queue_t *queue) {
	while (!queue_vacia(queue)) {
		queue_decula(queue);
	}
	free(queue);
}

bool queue_encula(queue_t *pQueue, void *mierda) {
	/* Bad parameter */
	queue_nodo *item = (queue_nodo *) calloc(1, sizeof(queue_nodo));
	if ((pQueue == NULL) || (item == NULL)) {
		return falso;
	}
// if(pQueue->limit != 0)
	if (pQueue->size >= pQueue->limit) {
		free(item);
		return falso;
	}
	item->datos = mierda;
	/*the queue is empty*/
	item->prev = NULL;
	if (pQueue->size == 0) {
		pQueue->head = item;
		pQueue->tail = item;

	} else {
		/*adding item to the end of the queue*/
		pQueue->tail->prev = item;
		pQueue->tail = item;
	}
	pQueue->size++;
	return verdadero;
}

/*
 int main() {
 int i;
 Queue *pQ = ConstructQueue(7);
 NODE *pN;

 for (i = 0; i < 9; i++) {
 pN = (NODE*) malloc(sizeof(NODE));
 pN->data.info = 100 + i;
 Enqueue(pQ, pN);
 }

 while (!isEmpty(pQ)) {
 pN = Dequeue(pQ);
 printf("\nDequeued: %d", pN->data);
 free(pN);
 }
 DestructQueue(pQ);
 return (EXIT_SUCCESS);
 }
 */

#endif

#if 1

// XXX: https://lispmachine.wordpress.com/2009/05/13/queue-in-c/
struct my_struct {
	void *valor;
	struct my_struct* next;
};

typedef struct my_list {
	struct my_struct* head;
	struct my_struct* tail;
} listilla_fifo;

/* Will always return the pointer to my_list */
struct my_list* list_add_element(struct my_list* s, void *valor) {
	struct my_struct* p = malloc(1 * sizeof(*p));

	if ( NULL == p) {
		fprintf(stderr, "IN %s, %s: malloc() failed\n", __FILE__, "list_add");
		return s;
	}

	p->valor = valor;
	p->next = NULL;

	if ( NULL == s) {
		printf("Queue not initialized\n");
		free(p);
		return s;
	} else if ( NULL == s->head && NULL == s->tail) {
		/* printf("Empty list, adding p->num: %d\n\n", p->num);  */
		s->head = s->tail = p;
		return s;
	} else if ( NULL == s->head || NULL == s->tail) {
		fprintf(stderr,
				"There is something seriously wrong with your assignment of head/tail to the list\n");
		free(p);
		return NULL;
	} else {
		/* printf("List not empty, adding element to tail\n"); */
		s->tail->next = p;
		s->tail = p;
	}

	return s;
}

/* This is a queue and it is FIFO, so we will always remove the first element */
static inline void *list_remove_element(struct my_list* s) {
	struct my_struct* h = NULL;
	struct my_struct* p = NULL;
	void *valor = NULL;

	if ( NULL == s) {
		printf("List is empty\n");
		return s;
	} else if ( NULL == s->head && NULL == s->tail) {
		printf("Well, List is empty\n");
		return s;
	} else if ( NULL == s->head || NULL == s->tail) {
		printf("There is something seriously wrong with your list\n");
		printf("One of the head/tail is empty while other is not \n");
		abort();
	}

	h = s->head;
	valor = h->valor;
	p = h->next;
	free(h);
	s->head = p;
	if ( NULL == s->head)
		s->tail = s->head; /* The element tail was pointing to is free(), so we need an update */

	return valor;
}

/* ---------------------- small helper fucntions ---------------------------------- */
struct my_list* list_free(struct my_list* s) {
	while (s->head) {
		list_remove_element(s);
	}

	return s;
}

struct my_list* list_new(void) {
	struct my_list* p = malloc(1 * sizeof(*p));

	assert_timeout(p);

	p->head = p->tail = NULL;

	return p;
}

void list_print_element(const struct my_struct* p) {
	if (p) {
		printf("Num = %p\n", p->valor);
	} else {
		printf("Can not print NULL struct \n");
	}
}

void list_print(const struct my_list* ps) {
	struct my_struct* p = NULL;

	if (ps) {
		for (p = ps->head; p; p = p->next) {
			list_print_element(p);
		}
	}

	printf("------------------\n");
}

static inline bool list_empty(struct my_list *s) {
	return !s->head;
}

#endif

#if 1

#define ARBOL_VERGA_VALOR_INVALIDO CACA_COMUN_VALOR_INVALIDO
#define ARBOL_VERGA_MAX_LLAVES 5
#define ARBOL_VERGA_MAX_HIJOS ARBOL_VERGA_MAX_LLAVES+1
#define ARBOL_VERGA_MAX_NODOS_LIBRES 100000
#define ARBOL_VERGA_IDX_INVALIDO CACA_COMUN_IDX_INVALIDO
#define ARBOL_VERGA_APUNTADOR_INVALIDO ((void*)ULONG_LONG_MAX)
#define ARBOL_VERGA_MIN_LLAVES (ARBOL_VERGA_MAX_LLAVES/2)
#define ARBOL_VERGA_POSICION_MEDIA_LLAVES ARBOL_VERGA_MIN_LLAVES

typedef int (*arbol_verga_compara_llave_fn)(void *, void *);

typedef struct arbol_verga_nodo {
	void *llaves_arbol_verga_nodo[ARBOL_VERGA_MAX_LLAVES];
	struct arbol_verga_nodo *hijos_arbol_verga_nodo[ARBOL_VERGA_MAX_HIJOS];
	natural llaves_cnt_arbol_verga_nodo;
} arbol_verga_nodo;

typedef struct arbol_verga_ctx {
	arbol_verga_nodo *raiz_arbol_verga_ctx;
	listilla_fifo *nodos_libres_arbol_verga_ctx;
	natural nodos_libres_cnt_arbol_verga_ctx;
	arbol_verga_nodo nodos_libres_mem_arbol_verga_ctx[ARBOL_VERGA_MAX_NODOS_LIBRES];
	arbol_verga_compara_llave_fn funcion_cmp_arbol_verga_ctx;
} arbol_verga_ctx;

typedef struct arbol_verga_datos_llave {
	void *llave_arbol_verga_datos_llave;
	natural posicion_arbol_verga_datos_llave;
} arbol_verga_datos_llave;

typedef struct arbol_verga_resultado_separar {
	arbol_verga_nodo *nodo_nuevo_arbol_verga_resultado_separar;
	tipo_dato *llave_para_promover_arbol_verga_resultado_separar;
} arbol_verga_resultado_separar;

static inline arbol_verga_ctx *arbol_verga_init(
		arbol_verga_compara_llave_fn funcion_cmp) {
	arbol_verga_ctx *ctx = NULL;

	ctx = calloc(1, sizeof(arbol_verga_ctx));
	assert_timeout(ctx);

	ctx->nodos_libres_arbol_verga_ctx = list_new();

	memset(ctx->nodos_libres_mem_arbol_verga_ctx, ARBOL_VERGA_VALOR_INVALIDO,
			sizeof(ctx->nodos_libres_mem_arbol_verga_ctx));

	for (int i = 0; i < ARBOL_VERGA_MAX_NODOS_LIBRES; i++) {
		list_add_element(ctx->nodos_libres_arbol_verga_ctx,
				ctx->nodos_libres_mem_arbol_verga_ctx + i);
	}
	ctx->nodos_libres_cnt_arbol_verga_ctx = ARBOL_VERGA_MAX_NODOS_LIBRES;

	ctx->funcion_cmp_arbol_verga_ctx = funcion_cmp;
	return ctx;
}

static inline void arbol_verga_fini(arbol_verga_ctx *ctx) {
	list_free(ctx->nodos_libres_arbol_verga_ctx);
	free(ctx);
}

static inline void arbol_verga_valida_hijos_invalidos(arbol_verga_ctx *ctx,
		arbol_verga_nodo *nodo, natural inicio_hijos_invalidos) {
	for (int i = inicio_hijos_invalidos; i < ARBOL_VERGA_MAX_HIJOS; i++) {
		assert_timeout(
				nodo->hijos_arbol_verga_nodo[i]==ARBOL_VERGA_APUNTADOR_INVALIDO);
	}
}
static inline arbol_verga_nodo *arbol_verga_alloca_nodo(arbol_verga_ctx *ctx) {
	arbol_verga_nodo *nodo = NULL;

	nodo = list_remove_element(ctx->nodos_libres_arbol_verga_ctx);

	assert_timeout(
			nodo->llaves_cnt_arbol_verga_nodo==ARBOL_VERGA_VALOR_INVALIDO);

	arbol_verga_valida_hijos_invalidos(ctx, nodo, 0);
	ctx->nodos_libres_cnt_arbol_verga_ctx--;
	nodo->llaves_cnt_arbol_verga_nodo = 0;

	return nodo;
}

static inline void arbol_verga_libera_nodo(arbol_verga_ctx *ctx,
		arbol_verga_nodo *nodo) {

	memset(nodo, ARBOL_VERGA_VALOR_INVALIDO, sizeof(arbol_verga_nodo));
	arbol_verga_valida_hijos_invalidos(ctx, nodo, 0);
	list_add_element(ctx->nodos_libres_arbol_verga_ctx, nodo);
	ctx->nodos_libres_cnt_arbol_verga_ctx++;
}

static inline arbol_verga_nodo *arbol_verga_borra_llave_datos_llave(
		arbol_verga_ctx *ctx, arbol_verga_nodo *nodo,
		arbol_verga_datos_llave *datos_llave, bool borra_hijo_der) {
	natural pos_llave = 0;
	natural offset_hijo = 0;
	void *(*llaves)[ARBOL_VERGA_MAX_LLAVES] = NULL;
	arbol_verga_nodo *(*hijos)[ARBOL_VERGA_MAX_HIJOS] = NULL;
	arbol_verga_nodo *huerfano = NULL;

	llaves = &nodo->llaves_arbol_verga_nodo;
	hijos = &nodo->hijos_arbol_verga_nodo;

	pos_llave = datos_llave->posicion_arbol_verga_datos_llave;
	if (!borra_hijo_der) {
		offset_hijo = 0;
	} else {
		offset_hijo = 1;
	}

	caca_log_debug(
			"la llave a borrar %p, supone estar en pos %u, lo q realmente ai %p",
			datos_llave->llave_arbol_verga_datos_llave, pos_llave,
			(*llaves)[pos_llave]);
	assert_timeout(
			datos_llave->llave_arbol_verga_datos_llave == (*llaves)[pos_llave]);

	caca_log_debug("borrando llave %p de pos %u",
			datos_llave->llave_arbol_verga_datos_llave, pos_llave);

	for (int i = pos_llave; i < nodo->llaves_cnt_arbol_verga_nodo - 1; i++) {
		caca_log_debug("llave en pos %u antes era %p aora %p", i, (*llaves)[i],
				(*llaves)[i+1]);
		assert_timeout((*llaves)[i+1]!=ARBOL_VERGA_APUNTADOR_INVALIDO);
		(*llaves)[i] = (*llaves)[i + 1];
	}
	(*llaves)[nodo->llaves_cnt_arbol_verga_nodo - 1] =
	ARBOL_VERGA_APUNTADOR_INVALIDO;

	caca_log_debug("borrada llave %p en pos %u",
			datos_llave->llave_arbol_verga_datos_llave, pos_llave);

	huerfano = (*hijos)[pos_llave + offset_hijo];
	for (int i = pos_llave + offset_hijo;
			i < nodo->llaves_cnt_arbol_verga_nodo + 1 - 1; i++) {
		(*hijos)[i] = (*hijos)[i + 1];
	}
	(*hijos)[nodo->llaves_cnt_arbol_verga_nodo + 1 - 1] =
	ARBOL_VERGA_APUNTADOR_INVALIDO;

	nodo->llaves_cnt_arbol_verga_nodo--;

	arbol_verga_valida_hijos_invalidos(ctx, nodo,
			nodo->llaves_cnt_arbol_verga_nodo + 1);

	return huerfano;
}

static inline void arbol_verga_inserta_llave_datos_llave(arbol_verga_ctx *ctx,
		arbol_verga_nodo *nodo, arbol_verga_datos_llave *datos_llave,
		arbol_verga_nodo *nuevo_hijo, bool inserta_hijo_der) {
	void *(*llaves)[ARBOL_VERGA_MAX_LLAVES] = NULL;
	arbol_verga_nodo *(*hijos)[ARBOL_VERGA_MAX_HIJOS] = NULL;
	natural offset_hijo = 0;

	caca_log_debug("insertando %p en pos %u",
			datos_llave->llave_arbol_verga_datos_llave,
			datos_llave->posicion_arbol_verga_datos_llave);

	assert_timeout(nodo->llaves_cnt_arbol_verga_nodo<ARBOL_VERGA_MAX_HIJOS);

	llaves = &nodo->llaves_arbol_verga_nodo;
	hijos = &nodo->hijos_arbol_verga_nodo;

	if (inserta_hijo_der) {
		offset_hijo = 1;
	} else {
		offset_hijo = 0;
	}

	for (int i = nodo->llaves_cnt_arbol_verga_nodo;
			i > datos_llave->posicion_arbol_verga_datos_llave; i--) {
		caca_log_debug("pos %u tenia %p aora tiene llave %p", i, (*llaves)[i],
				(*llaves)[i-1]);
		assert_timeout((*llaves)[i-1]!=ARBOL_VERGA_APUNTADOR_INVALIDO);
		(*llaves)[i] = (*llaves)[i - 1];
	}

	assert_timeout(nodo->llaves_cnt_arbol_verga_nodo<ARBOL_VERGA_MAX_HIJOS);

	(*llaves)[datos_llave->posicion_arbol_verga_datos_llave] =
			datos_llave->llave_arbol_verga_datos_llave;

	assert_timeout(nodo->llaves_cnt_arbol_verga_nodo<ARBOL_VERGA_MAX_HIJOS);

	caca_log_debug("llave %p insertada en %u",
			datos_llave->llave_arbol_verga_datos_llave,
			datos_llave->posicion_arbol_verga_datos_llave);

	for (int i = nodo->llaves_cnt_arbol_verga_nodo + 1;
			i > datos_llave->posicion_arbol_verga_datos_llave + offset_hijo;
			i--) {
		(*hijos)[i] = (*hijos)[i - 1];
	}

	assert_timeout(nodo->llaves_cnt_arbol_verga_nodo<ARBOL_VERGA_MAX_HIJOS);

	(*hijos)[datos_llave->posicion_arbol_verga_datos_llave + offset_hijo] =
			nuevo_hijo;

	nodo->llaves_cnt_arbol_verga_nodo++;
	assert_timeout(nodo->llaves_cnt_arbol_verga_nodo<=ARBOL_VERGA_MAX_LLAVES);
	arbol_verga_valida_hijos_invalidos(ctx, nodo,
			nodo->llaves_cnt_arbol_verga_nodo + 1);
}

#define arbol_verga_obten_hijo_en_pos(nodo,pos) ((nodo)->hijos_arbol_verga_nodo[pos])
#define arbol_verga_obten_hijo_izq(nodo, pos_llave) arbol_verga_obten_hijo_en_pos(nodo,pos_llave)
#define arbol_verga_obten_hijo_der(nodo, pos_llave) arbol_verga_obten_hijo_en_pos(nodo,pos_llave+1)
#define arbol_verga_obten_primer_hijo(nodo) (arbol_verga_obten_hijo_izq(nodo, 0))
#define arbol_verga_obten_ultimo_hijo(nodo) (arbol_verga_obten_hijo_der(nodo,(nodo)->llaves_cnt_arbol_verga_nodo-1))
#define arbol_verga_obten_llave_en_pos(nodo,pos) ((nodo)->llaves_arbol_verga_nodo[pos])
#define arbol_verga_pon_llave_en_pos(nodo,pos,llave) (nodo)->llaves_arbol_verga_nodo[pos]=(llave)
#define arbol_verga_pon_hijo_en_pos(nodo,pos,hijo) (nodo)->hijos_arbol_verga_nodo[pos]=(hijo)
#define arbol_verga_obten_primer_hijo(nodo) (arbol_verga_obten_hijo_izq(nodo, 0))
#define arbol_verga_obten_primer_llave(nodo) (arbol_verga_obten_llave_en_pos(nodo,0))
#define arbol_verga_obten_ultima_llave(nodo) (arbol_verga_obten_llave_en_pos(nodo,(nodo)->llaves_cnt_arbol_verga_nodo-1))
#define arbol_verga_obten_ultima_pos_llave(nodo) ((nodo)->llaves_cnt_arbol_verga_nodo-1)
#define arbol_verga_pon_hijo_izq_en_pos(nodo,pos,hijo) arbol_verga_pon_hijo_en_pos(nodo,pos,hijo)
#define arbol_verga_pon_hijo_der_en_pos(nodo,pos,hijo) arbol_verga_pon_hijo_en_pos(nodo,pos+1,hijo)
#define arbol_verga_datos_genera_datos_llave_local(llave,pos_llave) &(arbol_verga_datos_llave){.llave_arbol_verga_datos_llave=llave,.posicion_arbol_verga_datos_llave=pos_llave}
#define arbol_verga_nodo_tiene_suficientes_llaves(nodo) ((nodo)->llaves_cnt_arbol_verga_nodo>ARBOL_VERGA_MIN_LLAVES)
#define arbol_verga_nodo_tiene_llaves_llenas(nodo) ((nodo)->llaves_cnt_arbol_verga_nodo==ARBOL_VERGA_MAX_LLAVES)

static inline void arbol_verga_rota_izquierda(arbol_verga_ctx *ctx,
		arbol_verga_nodo *hijo_izq, arbol_verga_nodo *padre,
		arbol_verga_nodo *hijo_der, arbol_verga_datos_llave *datos_llave) {
	arbol_verga_nodo *hijo_de_llave_en_padre = NULL;
	arbol_verga_nodo *hijo_a_promover = NULL;
	void *llave_a_promover = NULL;

	llave_a_promover = arbol_verga_obten_primer_llave(hijo_der);
	hijo_a_promover = arbol_verga_obten_primer_hijo(hijo_der);

	assert_timeout(hijo_der->llaves_cnt_arbol_verga_nodo>ARBOL_VERGA_MIN_LLAVES);
	caca_log_debug("insertando en ijo izq");
	arbol_verga_inserta_llave_datos_llave(ctx, hijo_izq,
			&(arbol_verga_datos_llave ) { .llave_arbol_verga_datos_llave =
							datos_llave->llave_arbol_verga_datos_llave,
							.posicion_arbol_verga_datos_llave =
									hijo_izq->llaves_cnt_arbol_verga_nodo },
			hijo_a_promover, verdadero);

	caca_log_debug("borrando de padre");
	hijo_de_llave_en_padre = arbol_verga_borra_llave_datos_llave(ctx, padre,
			datos_llave, verdadero);

	caca_log_debug("el hijo de llave esperado %p", hijo_de_llave_en_padre);

	caca_log_debug("reinsertando en padre");
	arbol_verga_inserta_llave_datos_llave(ctx, padre,
			&(arbol_verga_datos_llave ) { .llave_arbol_verga_datos_llave =
							llave_a_promover,
							.posicion_arbol_verga_datos_llave =
									datos_llave->posicion_arbol_verga_datos_llave },
			hijo_de_llave_en_padre, verdadero);

	caca_log_debug("borrando de hijo der");
	arbol_verga_nodo *tmp1 = arbol_verga_borra_llave_datos_llave(ctx, hijo_der,
			&(arbol_verga_datos_llave ) { .llave_arbol_verga_datos_llave =
							llave_a_promover,
							.posicion_arbol_verga_datos_llave = 0 }, falso);

}

static inline void arbol_verga_rota_derecha(arbol_verga_ctx *ctx,
		arbol_verga_nodo *hijo_izq, arbol_verga_nodo *padre,
		arbol_verga_nodo *hijo_der, arbol_verga_datos_llave *datos_llave) {
	arbol_verga_nodo *hijo_de_llave_en_padre = NULL;
	arbol_verga_nodo *hijo_a_promover = NULL;
	void *llave_a_promover = NULL;

	llave_a_promover = arbol_verga_obten_ultima_llave(hijo_izq);
	hijo_a_promover = arbol_verga_obten_ultimo_hijo(hijo_izq);

	assert_timeout(hijo_izq->llaves_cnt_arbol_verga_nodo>ARBOL_VERGA_MIN_LLAVES);
	arbol_verga_inserta_llave_datos_llave(ctx, hijo_der,
			&(arbol_verga_datos_llave ) { .llave_arbol_verga_datos_llave =
							datos_llave->llave_arbol_verga_datos_llave,
							.posicion_arbol_verga_datos_llave = 0 },
			hijo_a_promover, falso);

	hijo_de_llave_en_padre = arbol_verga_borra_llave_datos_llave(ctx, padre,
			datos_llave, verdadero);

	arbol_verga_inserta_llave_datos_llave(ctx, padre,
			&(arbol_verga_datos_llave ) { .llave_arbol_verga_datos_llave =
							llave_a_promover,
							.posicion_arbol_verga_datos_llave =
									datos_llave->posicion_arbol_verga_datos_llave },
			hijo_de_llave_en_padre, verdadero);

	arbol_verga_nodo *tmp1 =
			arbol_verga_borra_llave_datos_llave(ctx, hijo_izq,
					&(arbol_verga_datos_llave ) {
									.llave_arbol_verga_datos_llave =
											llave_a_promover,
									.posicion_arbol_verga_datos_llave =
											arbol_verga_obten_ultima_pos_llave(
													hijo_izq) }, verdadero);
}

static inline bool arbol_verga_encuentra_llave_en_nodo(arbol_verga_ctx *ctx,
		arbol_verga_nodo *nodo, void *llave,
		arbol_verga_datos_llave *datos_llave) {
	bool encontrada = falso;
	natural idx = 0;
	int resultado_comp = 0;

	for (idx = 0; idx < nodo->llaves_cnt_arbol_verga_nodo; idx++) {
		resultado_comp = ctx->funcion_cmp_arbol_verga_ctx(
				arbol_verga_obten_llave_en_pos(nodo, idx), llave);
		if (resultado_comp >= 0) {
			if (arbol_verga_obten_llave_en_pos(nodo,idx) == llave) {
				encontrada = verdadero;
			}
			break;
		}
	}

	datos_llave->llave_arbol_verga_datos_llave = llave;
	datos_llave->posicion_arbol_verga_datos_llave = idx;

	return encontrada;
}

static inline void arbol_verga_inserta_llave_en_nodo(arbol_verga_ctx *ctx,
		arbol_verga_nodo *nodo, void *llave, arbol_verga_nodo *nuevo_hijo,
		bool inserta_hijo_der) {
	arbol_verga_datos_llave *datos_llave = &(arbol_verga_datos_llave ) {
					.llave_arbol_verga_datos_llave =
					ARBOL_VERGA_APUNTADOR_INVALIDO,
					.posicion_arbol_verga_datos_llave =
					ARBOL_VERGA_IDX_INVALIDO };
	bool encontrada_llave = falso;

	encontrada_llave = arbol_verga_encuentra_llave_en_nodo(ctx, nodo, llave,
			datos_llave);

	assert_timeout(!encontrada_llave);

	arbol_verga_inserta_llave_datos_llave(ctx, nodo, datos_llave, nuevo_hijo,
			inserta_hijo_der);
}

static inline arbol_verga_nodo *arbol_verga_borra_llave_en_nodo(
		arbol_verga_ctx *ctx, arbol_verga_nodo *nodo, void *llave,
		bool borra_hijo_der) {
	arbol_verga_datos_llave *datos_llave = &(arbol_verga_datos_llave ) {
					.llave_arbol_verga_datos_llave =
					ARBOL_VERGA_APUNTADOR_INVALIDO,
					.posicion_arbol_verga_datos_llave =
					ARBOL_VERGA_IDX_INVALIDO };
	bool encontrada_llave = falso;

	encontrada_llave = arbol_verga_encuentra_llave_en_nodo(ctx, nodo, llave,
			datos_llave);

	assert_timeout(encontrada_llave);

	caca_log_debug("la llave %p se encontro en %u", llave,
			datos_llave->posicion_arbol_verga_datos_llave);

	return arbol_verga_borra_llave_datos_llave(ctx, nodo, datos_llave,
			borra_hijo_der);
}

#define arbol_verga_inserta_al_final_con_datos_llave(ctx,nodo,llave, nuevo_hijo) \
	arbol_verga_inserta_llave_datos_llave(ctx, nodo, \
										 &(arbol_verga_datos_llave ) { .llave_arbol_verga_datos_llave = llave, \
																	  .posicion_arbol_verga_datos_llave =arbol_verga_obten_ultima_pos_llave(nodo) +1}, \
										 nuevo_hijo,verdadero)

static inline void arbol_verga_mergea_nodos(arbol_verga_ctx *ctx,
		arbol_verga_nodo *hijo_izq, arbol_verga_nodo *padre,
		arbol_verga_nodo *hijo_der, arbol_verga_datos_llave *datos_llave) {

	assert_timeout(
			hijo_izq->llaves_cnt_arbol_verga_nodo<=ARBOL_VERGA_MIN_LLAVES);
	assert_timeout(
			hijo_der->llaves_cnt_arbol_verga_nodo<=ARBOL_VERGA_MIN_LLAVES);

	arbol_verga_inserta_al_final_con_datos_llave(ctx, hijo_izq,
			datos_llave->llave_arbol_verga_datos_llave,
			arbol_verga_obten_primer_hijo(hijo_der));

	for (int i = 0; i <= arbol_verga_obten_ultima_pos_llave(hijo_der); i++) {
		arbol_verga_inserta_al_final_con_datos_llave(ctx, hijo_izq,
				arbol_verga_obten_llave_en_pos(hijo_der,i),
				arbol_verga_obten_hijo_der(hijo_der,i));
	}

	arbol_verga_borra_llave_datos_llave(ctx, padre, datos_llave, verdadero);

	arbol_verga_libera_nodo(ctx, hijo_der);
}

static inline void *arbol_verga_encuentra_siguiente_llave(arbol_verga_ctx *ctx,
		arbol_verga_nodo *nodo) {
	void *resu = NULL;
	while (nodo != ARBOL_VERGA_APUNTADOR_INVALIDO) {
		resu = arbol_verga_obten_primer_llave(nodo);
		nodo = arbol_verga_obten_primer_hijo(nodo);
	}

	return resu;
}

static inline void *arbol_verga_encuentra_anterior_llave(arbol_verga_ctx *ctx,
		arbol_verga_nodo *nodo) {
	void *resu = NULL;
	while (nodo != ARBOL_VERGA_APUNTADOR_INVALIDO) {
		resu = arbol_verga_obten_ultima_llave(nodo);
		nodo = arbol_verga_obten_ultimo_hijo(nodo);
	}

	return resu;
}

typedef enum arbol_verga_caso_prestador {
	izq_arbol_verga_caso_prestador,
	der_arbol_verga_caso_prestador,
	ninguno_presta_izq_existe_arbol_verga_caso_prestador,
	ninguno_presta_der_existe_arbol_verga_caso_prestador,
	ninguno_arbol_verga_caso_prestador
} arbol_verga_caso_prestador;
#define ninguno_arbol_verga_caso_prestador ninguno_presta_izq_existe_arbol_verga_caso_prestador

static inline arbol_verga_caso_prestador arbol_verga_encuentra_prestador(
		arbol_verga_ctx *ctx, arbol_verga_nodo *padre, natural nodo_izq_idx,
		natural nodo_der_idx, arbol_verga_nodo **hijo_prestador) {
	arbol_verga_caso_prestador cacaso = ninguno_arbol_verga_caso_prestador;
	natural tam_izq = 0;
	natural tam_der = 0;

	*hijo_prestador = NULL;
	if (((int) nodo_izq_idx) >= 0
			&& nodo_izq_idx <= padre->llaves_cnt_arbol_verga_nodo) {
		arbol_verga_nodo *nodo_prestador = arbol_verga_obten_hijo_en_pos(padre,
				nodo_izq_idx);
		tam_izq = nodo_prestador->llaves_cnt_arbol_verga_nodo;
		cacaso = ninguno_presta_izq_existe_arbol_verga_caso_prestador;
		caca_log_debug("existe ijo izq con %u llaves", tam_izq);
		if (tam_izq > ARBOL_VERGA_MIN_LLAVES) {
			cacaso = izq_arbol_verga_caso_prestador;
			*hijo_prestador = nodo_prestador;
		}
	}
	if (nodo_der_idx <= padre->llaves_cnt_arbol_verga_nodo) {
		arbol_verga_nodo *nodo_prestador = arbol_verga_obten_hijo_en_pos(padre,
				nodo_der_idx);
		tam_der = nodo_prestador->llaves_cnt_arbol_verga_nodo;
		if (cacaso == ninguno_arbol_verga_caso_prestador) {
			cacaso = ninguno_presta_der_existe_arbol_verga_caso_prestador;
		}
		caca_log_debug("existe ijo der con %u llaves", tam_der);
		if (tam_der > ARBOL_VERGA_MIN_LLAVES && tam_der > tam_izq) {
			cacaso = der_arbol_verga_caso_prestador;
			*hijo_prestador = nodo_prestador;
		}
	}

	return cacaso;
}

static inline bool arbol_verga_nodo_es_hoja(arbol_verga_ctx *ctx,
		arbol_verga_nodo *nodo) {
	bool es_hoja = falso;

	es_hoja = arbol_verga_obten_primer_hijo(
			nodo) == ARBOL_VERGA_APUNTADOR_INVALIDO;

	if (es_hoja) {
		for (int i = 0; i <= nodo->llaves_cnt_arbol_verga_nodo; i++) {
			assert_timeout(
					arbol_verga_obten_hijo_en_pos(nodo,i)==ARBOL_VERGA_APUNTADOR_INVALIDO);
		}
	}

	return es_hoja;
}

static inline bool arbol_verga_borra_llave(arbol_verga_ctx *ctx,
		arbol_verga_nodo *nodo, void *llave) {
	bool res = verdadero;
	bool llave_encontrada = falso;
	bool liberar_nodo = falso;
	arbol_verga_datos_llave *datos_llave = &(arbol_verga_datos_llave ) { 0 };

	llave_encontrada = arbol_verga_encuentra_llave_en_nodo(ctx, nodo, llave,
			datos_llave);

	if (!llave_encontrada) {
		if (!arbol_verga_nodo_es_hoja(ctx, nodo)) {
			natural posicion_siguiente_nodo =
					datos_llave->posicion_arbol_verga_datos_llave;

			if (!arbol_verga_nodo_tiene_suficientes_llaves(
					arbol_verga_obten_hijo_en_pos(nodo,posicion_siguiente_nodo))) {
				natural posicion_prestador_izq = posicion_siguiente_nodo - 1;
				natural posicion_prestador_der = posicion_siguiente_nodo + 1;
				arbol_verga_nodo *nodo_prestador = NULL;

				arbol_verga_caso_prestador cacaso =
						arbol_verga_encuentra_prestador(ctx, nodo,
								posicion_prestador_izq, posicion_prestador_der,
								&nodo_prestador);

				switch (cacaso) {
				case izq_arbol_verga_caso_prestador:
					arbol_verga_rota_derecha(ctx, nodo_prestador, nodo,
							arbol_verga_obten_hijo_en_pos(nodo,
									posicion_siguiente_nodo),
							arbol_verga_datos_genera_datos_llave_local(
									arbol_verga_obten_llave_en_pos(nodo,posicion_prestador_izq),
									posicion_prestador_izq));
					break;
				case der_arbol_verga_caso_prestador:
					arbol_verga_rota_izquierda(ctx,
							arbol_verga_obten_hijo_en_pos(nodo,
									posicion_siguiente_nodo), nodo,
							nodo_prestador,
							arbol_verga_datos_genera_datos_llave_local(
									arbol_verga_obten_llave_en_pos(nodo,posicion_siguiente_nodo),
									posicion_siguiente_nodo));
					break;
				default:
					switch (cacaso) {
					case ninguno_presta_izq_existe_arbol_verga_caso_prestador:
						arbol_verga_mergea_nodos(ctx,
								arbol_verga_obten_hijo_en_pos(nodo,
										posicion_prestador_izq), nodo,
								arbol_verga_obten_hijo_en_pos(nodo,
										posicion_siguiente_nodo),
								arbol_verga_datos_genera_datos_llave_local(
										arbol_verga_obten_llave_en_pos(nodo,posicion_prestador_izq),
										posicion_prestador_izq));

						posicion_siguiente_nodo = posicion_prestador_izq;
						break;
					case ninguno_presta_der_existe_arbol_verga_caso_prestador:
						arbol_verga_mergea_nodos(ctx,
								arbol_verga_obten_hijo_en_pos(nodo,
										posicion_siguiente_nodo), nodo,
								arbol_verga_obten_hijo_en_pos(nodo,
										posicion_prestador_der),
								arbol_verga_datos_genera_datos_llave_local(
										arbol_verga_obten_llave_en_pos(nodo,posicion_siguiente_nodo),
										posicion_siguiente_nodo));
						break;
					default:
						abort();
						break;
					}
					if (!nodo->llaves_cnt_arbol_verga_nodo) {
						ctx->raiz_arbol_verga_ctx =
								arbol_verga_obten_primer_hijo(nodo);
						liberar_nodo = verdadero;
					}
					break;
				}
			}
			res = arbol_verga_borra_llave(ctx,
					arbol_verga_obten_hijo_en_pos(nodo,
							posicion_siguiente_nodo), llave);
			if (liberar_nodo) {
				arbol_verga_libera_nodo(ctx, nodo);
			}

		} else {
			res = falso;
		}
	} else {
		natural posicion_llave = datos_llave->posicion_arbol_verga_datos_llave;
		if (!arbol_verga_nodo_es_hoja(ctx, nodo)) {
			natural posicion_prestador_izq = posicion_llave;
			natural posicion_prestador_der = posicion_llave + 1;
			arbol_verga_nodo *nodo_prestador = NULL;

			arbol_verga_caso_prestador cacaso = arbol_verga_encuentra_prestador(
					ctx, nodo, posicion_prestador_izq, posicion_prestador_der,
					&nodo_prestador);

			switch (cacaso) {
			case izq_arbol_verga_caso_prestador:
			case der_arbol_verga_caso_prestador: {
				arbol_verga_nodo *nodo_a_restaurar = NULL;
				void *llave_nueva = NULL;
				if (cacaso == izq_arbol_verga_caso_prestador) {
					llave_nueva = arbol_verga_encuentra_anterior_llave(ctx,
							nodo_prestador);
				} else {
					llave_nueva = arbol_verga_encuentra_siguiente_llave(ctx,
							nodo_prestador);
				}

				nodo_a_restaurar = arbol_verga_borra_llave_datos_llave(ctx,
						nodo, datos_llave, verdadero);

				arbol_verga_inserta_llave_datos_llave(ctx, nodo,
						arbol_verga_datos_genera_datos_llave_local(llave_nueva,
								posicion_llave), nodo_a_restaurar, verdadero);

				arbol_verga_borra_llave(ctx, nodo_prestador, llave_nueva);
			}
				break;
			default:
				if (cacaso
						== ninguno_presta_der_existe_arbol_verga_caso_prestador
						|| cacaso
								== ninguno_presta_izq_existe_arbol_verga_caso_prestador) {
					arbol_verga_mergea_nodos(ctx,
							arbol_verga_obten_hijo_en_pos(nodo,
									posicion_prestador_izq), nodo,
							arbol_verga_obten_hijo_en_pos(nodo,
									posicion_prestador_der), datos_llave);

					if (!nodo->llaves_cnt_arbol_verga_nodo) {
						ctx->raiz_arbol_verga_ctx =
								arbol_verga_obten_primer_hijo(nodo);
						liberar_nodo = verdadero;
					}

					res = arbol_verga_borra_llave(ctx,
							arbol_verga_obten_hijo_en_pos(nodo,
									posicion_prestador_izq), llave);

					if (liberar_nodo) {
						arbol_verga_libera_nodo(ctx, nodo);
					}
				} else {
					abort();
				}
				break;
			}
		} else {
			bool borrar_hijo_der = falso;
			if (datos_llave->posicion_arbol_verga_datos_llave) {
				borrar_hijo_der = verdadero;
			}
			arbol_verga_borra_llave_datos_llave(ctx, nodo, datos_llave,
					borrar_hijo_der);
		}
	}
	arbol_verga_valida_hijos_invalidos(ctx, nodo,
			nodo->llaves_cnt_arbol_verga_nodo + 1);
	return res;
}

static inline void arbol_verga_separa(arbol_verga_ctx *ctx,
		arbol_verga_nodo *nodo, arbol_verga_resultado_separar *resultado_sep) {
	arbol_verga_nodo *nuevo_nodo = NULL;
	arbol_verga_nodo *nodo_a_separar = nodo;

	nuevo_nodo = arbol_verga_alloca_nodo(ctx);

	resultado_sep->llave_para_promover_arbol_verga_resultado_separar =
			arbol_verga_obten_llave_en_pos(nodo_a_separar,
					ARBOL_VERGA_POSICION_MEDIA_LLAVES);

	caca_log_debug("la llave a promover %p",
			resultado_sep->llave_para_promover_arbol_verga_resultado_separar);

	arbol_verga_inserta_llave_datos_llave(ctx, nuevo_nodo,
			arbol_verga_datos_genera_datos_llave_local(
					arbol_verga_obten_llave_en_pos(nodo_a_separar,ARBOL_VERGA_POSICION_MEDIA_LLAVES+1),
					0),
			arbol_verga_obten_hijo_en_pos(nodo_a_separar,
					ARBOL_VERGA_POSICION_MEDIA_LLAVES+1), falso);

	caca_log_debug(
			"en nueov nodo_a_separar insertada llave %p hijo %p en pos %u",
			arbol_verga_obten_primer_llave(nuevo_nodo),
			arbol_verga_obten_primer_hijo(nuevo_nodo), 0);

	arbol_verga_pon_hijo_en_pos(nuevo_nodo,
			nuevo_nodo->llaves_cnt_arbol_verga_nodo,
			arbol_verga_obten_hijo_en_pos(nodo_a_separar,ARBOL_VERGA_POSICION_MEDIA_LLAVES+2));

	caca_log_debug("aora en nuevo nodo_a_separar seteado segundo hijo %p ",
			arbol_verga_obten_ultimo_hijo(nuevo_nodo));

	caca_log_debug("se empezara copia de %u, terminando en %u",
			ARBOL_VERGA_POSICION_MEDIA_LLAVES+2,
			arbol_verga_obten_ultima_pos_llave(nodo_a_separar));
	for (int i = ARBOL_VERGA_POSICION_MEDIA_LLAVES + 2;
			i <= arbol_verga_obten_ultima_pos_llave(nodo_a_separar); i++) {
		void *llave_actual = arbol_verga_obten_llave_en_pos(nodo_a_separar, i);
		arbol_verga_inserta_llave_datos_llave(ctx, nuevo_nodo,
				arbol_verga_datos_genera_datos_llave_local(llave_actual,
						nuevo_nodo->llaves_cnt_arbol_verga_nodo),
				arbol_verga_obten_hijo_der(nodo_a_separar, i), verdadero);
		caca_log_debug(
				"insertando llave %p hijo %p en pos orig %u en nuevo en pos %u",
				llave_actual, arbol_verga_obten_hijo_der(nodo_a_separar,i), i,
				nuevo_nodo->llaves_cnt_arbol_verga_nodo);
	}

	while (nodo_a_separar->llaves_cnt_arbol_verga_nodo > ARBOL_VERGA_MIN_LLAVES) {
		void *llave_actual = arbol_verga_obten_ultima_llave(nodo_a_separar);
		caca_log_debug("borrando llave %p de pos %u de oroginal", llave_actual,
				arbol_verga_obten_ultima_pos_llave(nodo_a_separar));

		arbol_verga_borra_llave_datos_llave(ctx, nodo_a_separar,
				arbol_verga_datos_genera_datos_llave_local(llave_actual,
						arbol_verga_obten_ultima_pos_llave(nodo_a_separar)),
				verdadero);
	}

	resultado_sep->nodo_nuevo_arbol_verga_resultado_separar = nuevo_nodo;
}

static inline void arbol_verga_separa_y_promueve(arbol_verga_ctx *ctx,
		arbol_verga_nodo *nodo, natural posicion_hijo_a_separar) {
	arbol_verga_nodo *nodo_a_separar = NULL;
	arbol_verga_nodo *nodo_nuevo = NULL;
	arbol_verga_resultado_separar *resultado_sep =
			&(arbol_verga_resultado_separar ) { 0 };

	nodo_a_separar = arbol_verga_obten_hijo_en_pos(nodo,
			posicion_hijo_a_separar);

	arbol_verga_separa(ctx, nodo_a_separar, resultado_sep);

	caca_log_debug("separado hijo %u", posicion_hijo_a_separar);

	arbol_verga_inserta_llave_datos_llave(ctx, nodo,
			arbol_verga_datos_genera_datos_llave_local(
					resultado_sep->llave_para_promover_arbol_verga_resultado_separar,
					posicion_hijo_a_separar),
			resultado_sep->nodo_nuevo_arbol_verga_resultado_separar, verdadero);

	caca_log_debug("insertada llave %p en pos %u de padre",
			resultado_sep->llave_para_promover_arbol_verga_resultado_separar,
			posicion_hijo_a_separar);

}

static inline void arbol_verga_inserta_recursivo(arbol_verga_ctx *ctx,
		arbol_verga_nodo *nodo, void *llave) {

	assert_timeout(nodo->llaves_cnt_arbol_verga_nodo<ARBOL_VERGA_MAX_LLAVES);
	if (arbol_verga_nodo_es_hoja(ctx, nodo)) {
		arbol_verga_inserta_llave_en_nodo(ctx, nodo, llave,
		ARBOL_VERGA_APUNTADOR_INVALIDO, verdadero);
	} else {
		natural pos_sig_nodo_para_buscar = 0;
		arbol_verga_nodo *sig_nodo_para_buscar = NULL;
		arbol_verga_datos_llave *datos_llave =
				arbol_verga_datos_genera_datos_llave_local(
						ARBOL_VERGA_APUNTADOR_INVALIDO,
						ARBOL_VERGA_VALOR_INVALIDO);
		bool llave_enc = arbol_verga_encuentra_llave_en_nodo(ctx, nodo, llave,
				datos_llave);
		assert_timeout(!llave_enc);
		pos_sig_nodo_para_buscar =
				datos_llave->posicion_arbol_verga_datos_llave;

		sig_nodo_para_buscar = arbol_verga_obten_hijo_en_pos(nodo,
				pos_sig_nodo_para_buscar);

		if (arbol_verga_nodo_tiene_llaves_llenas(sig_nodo_para_buscar)) {
			arbol_verga_nodo *hijo_izq = NULL;
			arbol_verga_nodo *hijo_der = NULL;
			void *llave_promovida = NULL;
			arbol_verga_separa_y_promueve(ctx, nodo, pos_sig_nodo_para_buscar);

			llave_promovida = arbol_verga_obten_llave_en_pos(nodo,
					pos_sig_nodo_para_buscar);
			hijo_izq = arbol_verga_obten_hijo_izq(nodo,
					pos_sig_nodo_para_buscar);
			hijo_der = arbol_verga_obten_hijo_der(nodo,
					pos_sig_nodo_para_buscar);

			int resultado_comp = ctx->funcion_cmp_arbol_verga_ctx(llave,
					llave_promovida);
			if (resultado_comp < 0) {
				sig_nodo_para_buscar = hijo_izq;
			} else {
				sig_nodo_para_buscar = hijo_der;
			}
		}

		arbol_verga_inserta_recursivo(ctx, sig_nodo_para_buscar, llave);
	}

	arbol_verga_valida_hijos_invalidos(ctx, nodo,
			nodo->llaves_cnt_arbol_verga_nodo + 1);
}

static inline void arbol_verga_inserta(arbol_verga_ctx *ctx, void *llave) {
	arbol_verga_nodo *raiz = NULL;

	raiz = ctx->raiz_arbol_verga_ctx;

	caca_log_debug("insertando llave %p", llave);

	if (!raiz) {
		raiz = arbol_verga_alloca_nodo(ctx);
		arbol_verga_inserta_llave_datos_llave(ctx, raiz,
				arbol_verga_datos_genera_datos_llave_local(llave, 0),
				ARBOL_VERGA_APUNTADOR_INVALIDO, verdadero);
		ctx->raiz_arbol_verga_ctx = raiz;
		caca_log_debug("insertada directo en la raiz");

	} else {
		if (arbol_verga_nodo_tiene_llaves_llenas(raiz)) {
			arbol_verga_nodo *nueva_raiz = NULL;
			arbol_verga_resultado_separar *resu_sep =
					&(arbol_verga_resultado_separar ) { 0 };

			arbol_verga_separa(ctx, raiz, resu_sep);

			nueva_raiz = arbol_verga_alloca_nodo(ctx);

			arbol_verga_inserta_llave_datos_llave(ctx, nueva_raiz,
					arbol_verga_datos_genera_datos_llave_local(
							resu_sep->llave_para_promover_arbol_verga_resultado_separar,
							0),
					resu_sep->nodo_nuevo_arbol_verga_resultado_separar,
					verdadero);
			arbol_verga_pon_hijo_izq_en_pos(nueva_raiz, 0, raiz);

			raiz = ctx->raiz_arbol_verga_ctx = nueva_raiz;
			caca_log_debug("separada raiz, nueva tiene %u",
					raiz->llaves_cnt_arbol_verga_nodo);
		}
		arbol_verga_inserta_recursivo(ctx, raiz, llave);
	}
}

static inline void arbol_verga_dumpea_inorder_recursivo(arbol_verga_ctx *ctx,
		arbol_verga_nodo *nodo, void **llaves, natural *llaves_cnt) {
	int i;
	if (nodo == ARBOL_VERGA_APUNTADOR_INVALIDO) {
		return;
	}
	for (i = 0; i < nodo->llaves_cnt_arbol_verga_nodo; i++) {
		arbol_verga_dumpea_inorder_recursivo(ctx,
				arbol_verga_obten_hijo_en_pos(nodo, i), llaves, llaves_cnt);
//		printf("%u ", arbol_verga_obten_llave_en_pos(nodo, i));
		llaves[(*llaves_cnt)++] = arbol_verga_obten_llave_en_pos(nodo, i);
	}
	arbol_verga_dumpea_inorder_recursivo(ctx,
			arbol_verga_obten_hijo_en_pos(nodo, i), llaves, llaves_cnt);
}

static inline void arbol_verga_dumpea_llaves_nodo(arbol_verga_ctx *ctx,
		arbol_verga_nodo *nodo) {
	for (int i = 0; i < nodo->llaves_cnt_arbol_verga_nodo; i++) {
		printf("%p ", arbol_verga_obten_llave_en_pos(nodo, i));
	}
//	printf("\n");
}

typedef struct arbol_verga_nodo_profundidad {
	arbol_verga_nodo *nodo_arbol_verga_nodo_profundidad;
	natural profundidad_arbol_verga_nodo_profundidad;
	bool ultimo_arbol_verga_nodo_profundidad;
} arbol_verga_nodo_profundidad;

static inline void arbol_verga_dumpea_bfs(arbol_verga_ctx *ctx, void *llaves,
		natural *llaves_cnt) {
	listilla_fifo *cola = NULL;
	arbol_verga_nodo_profundidad *cacas = NULL;
	natural cacas_cnt = 0;
	arbol_verga_nodo_profundidad *caca_act = NULL;
	natural prof_ant = ARBOL_VERGA_VALOR_INVALIDO;

	cacas = calloc(ARBOL_VERGA_MAX_NODOS_LIBRES,
			sizeof(arbol_verga_nodo_profundidad));

	cola = list_new();

	caca_act = cacas + cacas_cnt++;
	caca_act->nodo_arbol_verga_nodo_profundidad = ctx->raiz_arbol_verga_ctx;
	caca_act->profundidad_arbol_verga_nodo_profundidad = 0;
	list_add_element(cola, caca_act);

	while (!list_empty(cola)) {
		arbol_verga_nodo *nodo = NULL;
		natural prof = 0;
		bool ultimo_act = falso;
		caca_act = list_remove_element(cola);
		nodo = caca_act->nodo_arbol_verga_nodo_profundidad;
		prof = caca_act->profundidad_arbol_verga_nodo_profundidad;
		ultimo_act = caca_act->ultimo_arbol_verga_nodo_profundidad;
		if (prof_ant != prof) {
			printf("\n");
			prof_ant = prof;
		} else {
			printf(" - ");
		}
		arbol_verga_dumpea_llaves_nodo(ctx, nodo);
		if (ultimo_act) {
//			printf(" -- ");
		}
		if (!arbol_verga_nodo_es_hoja(ctx, nodo)) {
			for (int i = 0; i <= nodo->llaves_cnt_arbol_verga_nodo; i++) {
				caca_act = cacas + cacas_cnt++;
				assert_timeout(
						arbol_verga_obten_hijo_en_pos(nodo,i)!=ARBOL_VERGA_APUNTADOR_INVALIDO);
				caca_act->nodo_arbol_verga_nodo_profundidad =
						arbol_verga_obten_hijo_en_pos(nodo, i);
				caca_act->profundidad_arbol_verga_nodo_profundidad = prof + 1;
				caca_act->ultimo_arbol_verga_nodo_profundidad = i
						== arbol_verga_obten_ultima_pos_llave(nodo);
				list_add_element(cola, caca_act);
			}
		}
	}
	printf("\n");

	list_free(cola);
	free(cacas);
}

static inline void arbol_verga_dumpea_inorder(arbol_verga_ctx *ctx,
		void *llaves, natural *llaves_cnt) {
	*llaves_cnt = 0;
	arbol_verga_dumpea_inorder_recursivo(ctx, ctx->raiz_arbol_verga_ctx, llaves,
			llaves_cnt);
//	printf("\n");
}

static inline void *arbol_verga_encuentra_mayor(arbol_verga_ctx *ctx,
		arbol_verga_nodo *nodo, void *llave) {
	void *res = NULL;
	bool llave_enc = falso;
	natural pos_sig_nodo_para_buscar = 0;
	arbol_verga_datos_llave *datos_llave =
			arbol_verga_datos_genera_datos_llave_local(
					ARBOL_VERGA_APUNTADOR_INVALIDO, ARBOL_VERGA_VALOR_INVALIDO);

	if (!ctx->raiz_arbol_verga_ctx || !nodo
			|| nodo->llaves_cnt_arbol_verga_nodo < 1) {
		return NULL;
	}

	llave_enc = arbol_verga_encuentra_llave_en_nodo(ctx, nodo, llave,
			datos_llave);
	assert_timeout(!llave_enc);
	pos_sig_nodo_para_buscar = datos_llave->posicion_arbol_verga_datos_llave;

	if (arbol_verga_nodo_es_hoja(ctx, nodo)) {
		if (pos_sig_nodo_para_buscar <= arbol_verga_obten_ultima_pos_llave(nodo)) {
			res = arbol_verga_obten_llave_en_pos(nodo,
					pos_sig_nodo_para_buscar);
		}
	} else {
		arbol_verga_nodo *sig_nodo_para_buscar = NULL;

		sig_nodo_para_buscar = arbol_verga_obten_hijo_en_pos(nodo,
				pos_sig_nodo_para_buscar);

		res = arbol_verga_encuentra_mayor(ctx, sig_nodo_para_buscar, llave);
		if (!res) {
			if (pos_sig_nodo_para_buscar
					<= arbol_verga_obten_ultima_pos_llave(nodo)) {
				res = arbol_verga_obten_llave_en_pos(nodo,
						pos_sig_nodo_para_buscar);
			}
		}
	}

	return res;
}

#endif

#if 1
typedef natural hm_iter;
#define HASH_MAP_VALOR_INVALIDO ((hm_iter)CACA_COMUN_VALOR_INVALIDO)
typedef struct hash_map_entry {
	entero_largo llave;
	entero_largo valor;
} hm_entry;
typedef struct hash_map_cubeta {
	uint64_t hash;
	hm_entry *entry;
} hm_cubeta;
typedef struct hash_map_robin_hood_back_shift {
	hm_cubeta *buckets_;
	uint64_t num_buckets_;
	uint64_t num_buckets_used_;
	uint64_t probing_min_;
	uint64_t probing_max_;
} hm_rr_bs_tabla;
int hash_map_robin_hood_back_shift_init(hm_rr_bs_tabla *ht, int num_cubetas) {
	ht->num_buckets_ = num_cubetas;
	ht->buckets_ = (hm_cubeta *) calloc(ht->num_buckets_, sizeof(hm_cubeta));
	ht->num_buckets_used_ = 0;
	ht->probing_max_ = num_cubetas;
	return 0;
}
int hash_map_robin_hood_back_shift_fini(hm_rr_bs_tabla *ht) {
	if (ht->buckets_ != NULL) {
		for (uint32_t i = 0; i < ht->num_buckets_; i++) {
			if (ht->buckets_[i].entry != NULL) {
				free(ht->buckets_[i].entry);
				ht->buckets_[i].entry = NULL;
			}
		}
		free(ht->buckets_);
	}
	return 0;
}
static inline int hash_map_robin_hood_back_shift_llena_distancia_a_indice_inicio(
		hm_rr_bs_tabla *ht, uint64_t index_stored, uint64_t *distance) {
	hm_cubeta cubeta = ht->buckets_[index_stored];
	*distance = 0;
	if (cubeta.entry == NULL)
		return -1;
	uint64_t num_cubetas = ht->num_buckets_;
	uint64_t index_init = cubeta.hash % num_cubetas;
	if (index_init <= index_stored) {
		*distance = index_stored - index_init;
	} else {
		*distance = index_stored + (num_cubetas - index_init);
	}
	return 0;
}
hm_iter hash_map_robin_hood_back_shift_obten(hm_rr_bs_tabla *ht,
		const entero_largo key, entero_largo *value) {
	uint64_t num_cubetas = ht->num_buckets_;
	uint64_t prob_max = ht->probing_max_;
//	uint64_t hash = hash_function_caca(key);
	uint64_t hash = key % num_cubetas;
	uint64_t index_init = hash;
	uint64_t probe_distance = 0;
	uint64_t index_current;
	bool found = falso;
	uint32_t i;
	*value = HASH_MAP_VALOR_INVALIDO;
	for (i = 0; i < num_cubetas; i++) {
		index_current = (index_init + i) % num_cubetas;
		hm_entry *entrada = ht->buckets_[index_current].entry;
		if (entrada == NULL) {
			break;
		}
		hash_map_robin_hood_back_shift_llena_distancia_a_indice_inicio(ht,
				index_current, &probe_distance);
		if (i > probe_distance) {
			break;
		}
		if (entrada->llave == key) {
			*value = entrada->valor;
			found = verdadero;
			break;
		}
	}
	if (found)
		return index_current;
	return HASH_MAP_VALOR_INVALIDO;
}
hm_iter hash_map_robin_hood_back_shift_pon(hm_rr_bs_tabla *ht, entero_largo key,
		entero_largo value, bool *nuevo_entry) {
	uint64_t num_cubetas = ht->num_buckets_;
	uint64_t prob_max = ht->probing_max_;
	uint64_t prob_min = ht->probing_min_;
	hm_cubeta *cubetas = ht->buckets_;
	*nuevo_entry = verdadero;
	if (ht->num_buckets_used_ == num_cubetas) {
		*nuevo_entry = falso;
		return HASH_MAP_VALOR_INVALIDO;
	}
	ht->num_buckets_used_ += 1;
//	uint64_t hash = hash_function_caca(key);
	uint64_t hash = key % num_cubetas;
	uint64_t index_init = hash;
	hm_entry *entry = (hm_entry *) calloc(1, sizeof(hm_entry));
	entry->llave = key;
	entry->valor = value;
	uint64_t index_current = index_init % num_cubetas;
	uint64_t probe_current = 0;
	uint64_t probe_distance;
	hm_entry *entry_temp;
	uint64_t hash_temp;
	uint64_t i;
	for (i = 0; i < num_cubetas; i++) {
		index_current = (index_init + i) % num_cubetas;
		hm_cubeta *cubeta = cubetas + index_current;
		if (cubeta->entry == NULL) {
			cubeta->entry = entry;
			cubeta->hash = hash;
			if (index_current > prob_max) {
				ht->probing_max_ = index_current;
			}
			if (index_current < prob_min) {
				ht->probing_min_ = index_current;
			}
			break;
		} else {
			if (cubeta->entry->llave == key) {
				free(entry);
				*nuevo_entry = falso;
				break;
			}
			hash_map_robin_hood_back_shift_llena_distancia_a_indice_inicio(ht,
					index_current, &probe_distance);
			if (probe_current > probe_distance) {
				// Swapping the current bucket with the one to insert
				entry_temp = cubeta->entry;
				hash_temp = cubeta->hash;
				cubeta->entry = entry;
				cubeta->hash = hash;
				entry = entry_temp;
				hash = hash_temp;
				probe_current = probe_distance;
			}
		}
		probe_current++;
	}
	return index_current;
}
int hash_map_robin_hood_back_shift_borra(hm_rr_bs_tabla *ht,
		const tipo_dato key) {
	uint64_t num_cubetas = ht->num_buckets_;
	uint64_t prob_max = ht->probing_max_;
	uint64_t prob_min = ht->probing_max_;
	uint64_t hash = key % num_cubetas;
	uint64_t index_init = hash;
	bool found = falso;
	uint64_t index_current = 0;
	uint64_t probe_distance = 0;
	hm_entry *entrada = NULL;
	for (uint64_t i = 0; i < num_cubetas; i++) {
		index_current = (index_init + i) % num_cubetas;
		entrada = ht->buckets_[index_current].entry;
		hash_map_robin_hood_back_shift_llena_distancia_a_indice_inicio(ht,
				index_current, &probe_distance);
		if (entrada == NULL || i > probe_distance) {
			break;
		}
		if (entrada->llave == key) {
			found = verdadero;
			break;
		}
	}
	if (found) {
		free(entrada);
		uint64_t i = 1;
		uint64_t index_previous = 0, index_swap = 0;
		for (i = 1; i < num_cubetas; i++) {
			index_previous = (index_current + i - 1) % num_cubetas;
			index_swap = (index_current + i) % num_cubetas;
			hm_cubeta *cubeta_swap = ht->buckets_ + index_swap;
			hm_cubeta *cubeta_previous = ht->buckets_ + index_previous;
			if (cubeta_swap->entry == NULL) {
				cubeta_previous->entry = NULL;
				break;
			}
			uint64_t distance;
			if (hash_map_robin_hood_back_shift_llena_distancia_a_indice_inicio(
					ht, index_swap, &distance) != 0) {
				fprintf(stderr, "Error in FillDistanceToInitIndex()");
			}
			if (!distance) {
				cubeta_previous->entry = NULL;
				break;
			}
			cubeta_previous->entry = cubeta_swap->entry;
			cubeta_previous->hash = cubeta_swap->hash;
		}
		if (i < num_cubetas) {
			if (index_previous == prob_min) {
				prob_min++;
				if (prob_min >= num_cubetas) {
					prob_min = 0;
				} else {
					while (prob_min < num_cubetas
							&& ht->buckets_[prob_min].entry) {
						prob_min++;
					}
					if (prob_min >= num_cubetas) {
						prob_min = num_cubetas;
					}
				}
				ht->probing_min_ = prob_min;
			}
			if (index_previous == prob_max) {
				prob_max--;
				if (prob_max >= num_cubetas) {
					prob_max = num_cubetas;
				} else {
					while (((int64_t) prob_max) >= 0
							&& ht->buckets_[prob_max].entry) {
						prob_max--;
					}
					if (prob_max >= num_cubetas) {
						prob_max = 0;
					}
				}
				ht->probing_max_ = prob_max;
			}
		}
		ht->num_buckets_used_--;
		return 0;
	}
	return 1;
}
static inline int hash_map_robin_hood_back_shift_indice_inicio(
		hm_rr_bs_tabla *ht) {
	return ht->probing_min_;
}
static inline int hash_map_robin_hood_back_shift_indice_final(
		hm_rr_bs_tabla *ht) {
	return ht->probing_max_;
}
static inline bool hash_map_robin_hood_back_shift_indice_existe(
		hm_rr_bs_tabla *ht, hm_iter indice) {
	return !!ht->buckets_[indice].entry;
}
static inline entero_largo hash_map_robin_hood_back_shift_indice_obten_llave(
		hm_rr_bs_tabla *ht, hm_iter indice) {
	assert_timeout(indice <= ht->probing_max_ && indice >= ht->probing_min_);
	hm_entry *entrada = ht->buckets_[indice].entry;
	assert_timeout(entrada);
	return entrada->llave;
}
static inline void hash_map_robin_hood_back_shift_indice_pon_valor(
		hm_rr_bs_tabla *ht, hm_iter indice, entero_largo valor) {
	assert_timeout(indice <= ht->probing_max_ && indice >= ht->probing_min_);
	hm_entry *entrada = ht->buckets_[indice].entry;
	assert_timeout(entrada);
	entrada->valor = valor;
}
int hash_map_robin_hood_back_shift_indice_borra(hm_rr_bs_tabla *ht,
		hm_iter indice) {
	assert_timeout(indice <= ht->probing_max_ && indice >= ht->probing_min_);
	uint64_t num_cubetas = ht->num_buckets_;
	uint64_t prob_max = ht->probing_max_;
	uint64_t prob_min = ht->probing_max_;
	uint64_t index_current = indice;
	hm_entry *entrada = ht->buckets_[index_current].entry;
	assert_timeout(entrada);
	free(entrada);
	uint64_t i = 1;
	uint64_t index_previous = 0, index_swap = 0;
	for (i = 1; i < num_cubetas; i++) {
		index_previous = (index_current + i - 1) % num_cubetas;
		index_swap = (index_current + i) % num_cubetas;
		hm_cubeta *cubeta_swap = ht->buckets_ + index_swap;
		hm_cubeta *cubeta_previous = ht->buckets_ + index_previous;
		if (cubeta_swap->entry == NULL) {
			cubeta_previous->entry = NULL;
			break;
		}
		uint64_t distance;
		if (hash_map_robin_hood_back_shift_llena_distancia_a_indice_inicio(ht,
				index_swap, &distance) != 0) {
			fprintf(stderr, "Error in FillDistanceToInitIndex()");
		}
		if (!distance) {
			cubeta_previous->entry = NULL;
			break;
		}
		cubeta_previous->entry = cubeta_swap->entry;
		cubeta_previous->hash = cubeta_swap->hash;
	}
	if (i < num_cubetas) {
		if (index_previous == prob_min) {
			prob_min++;
			if (prob_min >= num_cubetas) {
				prob_min = 0;
			} else {
				while (prob_min < num_cubetas && ht->buckets_[prob_min].entry) {
					prob_min++;
				}
				if (prob_min >= num_cubetas) {
					prob_min = num_cubetas;
				}
			}
			ht->probing_min_ = prob_min;
		}
		if (index_previous == prob_max) {
			prob_max--;
			if (prob_max >= num_cubetas) {
				prob_max = num_cubetas;
			} else {
				while (((int64_t) prob_max) >= 0 && ht->buckets_[prob_max].entry) {
					prob_max--;
				}
				if (prob_max >= num_cubetas) {
					prob_max = 0;
				}
			}
			ht->probing_max_ = prob_max;
		}
	}
	ht->num_buckets_used_--;
	return 0;
}
static inline entero_largo hash_map_robin_hood_back_shift_indice_obten_valor(
		hm_rr_bs_tabla *ht, hm_iter indice) {
	assert_timeout(indice <= ht->probing_max_ && indice >= ht->probing_min_);
	hm_entry *entrada = ht->buckets_[indice].entry;
	assert_timeout(entrada);
	return entrada->valor;
}
static inline bool hash_map_robin_hood_back_shift_esta_vacio(hm_rr_bs_tabla *ht) {
	assert_timeout(ht->num_buckets_used_ <= ht->num_buckets_);
	return !ht->num_buckets_used_;
}

static inline void hash_map_robin_hood_back_shift_reemplazar(hm_rr_bs_tabla *ht,
		entero_largo llave, entero_largo valor) {
	hm_iter iter = 0;
	entero_largo *valor_int = &(entero_largo ) { 0 };

	iter = hash_map_robin_hood_back_shift_obten(ht, llave, valor_int);

	assert_timeout(iter!=HASH_MAP_VALOR_INVALIDO);

	hash_map_robin_hood_back_shift_indice_pon_valor(ht, iter, valor);
}

static inline void hash_map_robin_hood_back_shift_insertar_nuevo(
		hm_rr_bs_tabla *ht, entero_largo llave, entero_largo valor) {
	hm_iter iter = 0;
	bool nuevo = falso;
	iter = hash_map_robin_hood_back_shift_pon(ht, llave, valor, &nuevo);

	assert_timeout(iter!=HASH_MAP_VALOR_INVALIDO);
	assert_timeout(nuevo);
}

#endif

#if 1

//http://www.thelearningpoint.net/computer-science/data-structures-heaps-with-c-program-source-code
#define HEAP_SHIT_MAX_NODOS (200*200)
#define HEAP_SHIT_MAX_LLAVES HUARONVERGA_MAX_LLAVE
#define HEAP_SHIT_VALOR_INVALIDO CACA_COMUN_VALOR_INVALIDO

typedef struct heap_shit_nodo {
	tipo_dato prioridad;
	tipo_dato llave;
	void *valor;
} heap_shit_nodo;

typedef struct heap_shit {
	bool min;
	natural heap_size;
	heap_shit_nodo heap[HEAP_SHIT_MAX_NODOS];
	hm_rr_bs_tabla *tablon_llave_a_idx_heap;
} heap_shit;

/*Initialize Heap*/
static inline heap_shit *heap_shit_init(bool es_min) {
	heap_shit *heap = calloc(1, sizeof(heap_shit));
	assert_timeout(heap);
	heap->heap_size = 0;
	heap->min = es_min;
	memset(heap->heap, HEAP_SHIT_VALOR_INVALIDO, sizeof(heap->heap));
	heap->tablon_llave_a_idx_heap = calloc(1, sizeof(hm_rr_bs_tabla));
	assert_timeout(heap->tablon_llave_a_idx_heap);
	hash_map_robin_hood_back_shift_init(heap->tablon_llave_a_idx_heap,
	HEAP_SHIT_MAX_NODOS);
	return heap;
}

void heap_shit_fini(heap_shit *heap_ctx) {
	hash_map_robin_hood_back_shift_fini(heap_ctx->tablon_llave_a_idx_heap);
	free(heap_ctx->tablon_llave_a_idx_heap);
	free(heap_ctx);
}

static inline bool heap_shit_nodo_valido(heap_shit_nodo *nodo) {
	assert_timeout(
			(nodo->llave!=HEAP_SHIT_VALOR_INVALIDO && nodo->prioridad!=HEAP_SHIT_VALOR_INVALIDO) || (nodo->prioridad==nodo->llave));

	return nodo->llave != HEAP_SHIT_VALOR_INVALIDO;
}

static inline void heap_shit_valida_nodos(heap_shit *heap_ctx) {
	for (int i = 1; i <= heap_ctx->heap_size; i++) {
		assert_timeout(heap_shit_nodo_valido(heap_ctx->heap + i));
	}
}

static inline natural heap_shit_idx_padre(natural idx_nodo) {
	return idx_nodo >> 1;
}

static inline natural heap_shit_idx_hijo_izq(natural idx_nodo) {
	return idx_nodo << 1;
}

/*Insert an element into the heap */
static inline void heap_shit_insert(heap_shit *heap_ctx,
		heap_shit_nodo *nodo_nuevo) {
	natural heap_size = heap_ctx->heap_size;
	heap_shit_nodo *heap = NULL;
	hm_rr_bs_tabla *mapeo_inv = heap_ctx->tablon_llave_a_idx_heap;
	bool nuevo;

	heap = heap_ctx->heap;

	heap_size++;
	heap[heap_size] = *nodo_nuevo; /*Insert in the last place*/
	/*Adjust its position*/
	natural now = heap_size;
	/*
	 while (((heap_ctx->min
	 && (heap[now / 2] == (tipo_dato) FRAUDUCACA_VALOR_INVALIDO ?
	 -1 : (int) heap[now / 2]) > (int) element)
	 || (!heap_ctx->min && (natural) heap[now / 2] < (natural) element))) {
	 */
	while (heap_shit_nodo_valido(heap + heap_shit_idx_padre(now))
			&& ((heap_ctx->min
					&& heap[heap_shit_idx_padre(now)].prioridad
							> nodo_nuevo->prioridad)
					|| (!heap_ctx->min
							&& heap[heap_shit_idx_padre(now)].prioridad
									< nodo_nuevo->prioridad))) {
//printf("caca now %u de heap %u elem %u\n",now,heap[now],element);
		natural idx_padre = heap_shit_idx_padre(now);
		tipo_dato llave_padre = heap[idx_padre].llave;
		assert_timeout(llave_padre!= HEAP_SHIT_VALOR_INVALIDO);

		heap[now] = heap[idx_padre];
		hash_map_robin_hood_back_shift_reemplazar(mapeo_inv, llave_padre, now);
		caca_log_debug("llave %d tiene valor %u ", llave_padre, now);

		now = idx_padre;
	}
//printf("raise now %u con heap %u y elem %u res %u\n",now,heap[now / 2],element, (unsigned int)heap[now / 2]>(unsigned int)element);

	heap[now] = *nodo_nuevo;
	hash_map_robin_hood_back_shift_insertar_nuevo(mapeo_inv, nodo_nuevo->llave,
			now);

	heap_ctx->heap_size = heap_size;
//	heap_shit_valida_nodos(heap_ctx);
}

/*
 tipo_dato prioridad;
 tipo_dato llave;
 void *valor;
 */
#define heap_shit_insertar(heap_ctx,prioridad_in,llave_in,valor_in) heap_shit_insert(heap_ctx,&(heap_shit_nodo) {.prioridad=prioridad_in,.llave=llave_in,.valor=valor_in})
#define heap_shit_insertar_valor_unico(heap_ctx,valor) heap_shit_insertar(heap_ctx,valor,valor,(void *)((entero_largo)valor))

static inline void *heap_shit_delete(heap_shit *heap_ctx, natural idx_a_borrar) {
	natural heap_size = heap_ctx->heap_size;
	natural child, now;
	heap_shit_nodo minElement = { 0 };
	heap_shit_nodo lastElement = { 0 };
	heap_shit_nodo *heap;
	hm_rr_bs_tabla *mapeo_inv = heap_ctx->tablon_llave_a_idx_heap;
	void *resultado;
	bool nuevo;

	heap = heap_ctx->heap;

	assert_timeout(heap_size >= idx_a_borrar);
	assert_timeout(idx_a_borrar);

	/* heap[1] is the minimum element. So we remove heap[1]. Size of the heap is decreased.
	 Now heap[1] has to be filled. We put the last element in its place and see if it fits.
	 If it does not fit, take minimum element among both its children and replaces parent with it.
	 Again See if the last element fits in that place.*/
	minElement = heap[idx_a_borrar];
	resultado = minElement.valor;
	lastElement = heap[heap_size--];

	now = idx_a_borrar;
	/*
	 if (((heap_ctx->min
	 && (heap[now >> 1] == (tipo_dato) FRAUDUCACA_VALOR_INVALIDO ?
	 -1 : (int) heap[now >> 1]) > (int) lastElement)
	 || (!heap_ctx->min
	 && (natural) heap[now >> 1] < (natural) lastElement))) {
	 */
	if (heap_shit_nodo_valido(heap + heap_shit_idx_padre(now))
			&& ((heap_ctx->min
					&& heap[heap_shit_idx_padre(now)].prioridad
							> lastElement.prioridad)
					|| (!heap_ctx->min
							&& (heap[heap_shit_idx_padre(now)].prioridad
									< lastElement.prioridad)))) {
		/*
		 while (((heap_ctx->min
		 && (heap[now / 2] == (tipo_dato) FRAUDUCACA_VALOR_INVALIDO ?
		 -1 : (int) heap[now / 2]) > (int) lastElement)
		 || (!heap_ctx->min
		 && (natural) heap[now / 2] < (natural) lastElement))) {
		 */
		//printf("caca now %u de heap %u elem %u\n",now,heap[now],element);
		while (heap_shit_nodo_valido(heap + heap_shit_idx_padre(now))
				&& ((heap_ctx->min
						&& heap[heap_shit_idx_padre(now)].prioridad
								> lastElement.prioridad)
						|| (!heap_ctx->min
								&& (heap[heap_shit_idx_padre(now)].prioridad
										< lastElement.prioridad)))) {
			natural idx_padre = heap_shit_idx_padre(now);
			tipo_dato llave_padre = heap[idx_padre].llave;

			assert_timeout(llave_padre != HEAP_SHIT_VALOR_INVALIDO);

			heap[now] = heap[idx_padre];

			hash_map_robin_hood_back_shift_reemplazar(mapeo_inv, llave_padre,
					now);

			caca_log_debug("llave %d tiene valor %u", llave_padre, now);

			now = idx_padre;
		}
	} else {

		/* now refers to the index at which we are now */
		for (now = idx_a_borrar; heap_shit_idx_hijo_izq(now) <= heap_size; now =
				child) {
			/* child is the index of the element which is minimum among both the children */
			/* Indexes of children are i*2 and i*2 + 1*/
			child = heap_shit_idx_hijo_izq(now);
			/*child!=heap_size beacuse heap[heap_size+1] does not exist, which means it has only one
			 child */
			if (child != heap_size
					&& ((heap_ctx->min
							&& heap[child + 1].prioridad < heap[child].prioridad)
							|| (!heap_ctx->min
									&& heap[child + 1].prioridad
											> heap[child].prioridad))) {
				child++;
			}
			/* To check if the last element fits ot not it suffices to check if the last element
			 is less than the minimum element among both the children*/
			//printf("last %u heap %u\n",lastElement,heap[child]);
			if ((heap_ctx->min && lastElement.prioridad > heap[child].prioridad)
					|| (!heap_ctx->min
							&& lastElement.prioridad < heap[child].prioridad)) {
				heap[now] = heap[child];

				hash_map_robin_hood_back_shift_reemplazar(mapeo_inv,
						heap[child].llave, now);
				caca_log_debug("llave %d tiene valor %u", heap[child].llave,
						now);
			} else /* It fits there */
			{
				break;
			}
		}
	}

	hash_map_robin_hood_back_shift_borra(mapeo_inv, minElement.llave);
	caca_log_debug("borrada llave %d", minElement.llave);
	if (heap_size && idx_a_borrar != heap_size + 1) {
		heap[now] = lastElement;
		hash_map_robin_hood_back_shift_reemplazar(mapeo_inv, lastElement.llave,
				now);
		caca_log_debug("llave %d tiene valor %u ", lastElement.llave, now);
	}
	heap_ctx->heap_size = heap_size;
	//heap_shit_valida_nodos(heap_ctx);
	return resultado;
}

static inline void *heap_shit_borrar_directo(heap_shit *heap_ctx,
		tipo_dato llave) {
	natural heap_size = heap_ctx->heap_size;
	hm_rr_bs_tabla *indices_valores = heap_ctx->tablon_llave_a_idx_heap;
	entero_largo idx_a_borrar;

	assert_timeout(heap_size);

	natural idx_hm = hash_map_robin_hood_back_shift_obten(indices_valores,
			llave, &idx_a_borrar);
	caca_log_debug("borrando llave %d en idx %u en idx hm %u", llave,
			idx_a_borrar, idx_hm);
	assert_timeout(idx_hm!=HASH_MAP_VALOR_INVALIDO);
	assert_timeout(idx_a_borrar != HEAP_SHIT_VALOR_INVALIDO);

	return heap_shit_delete(heap_ctx, idx_a_borrar);
}

static inline void *heap_shit_borra_torpe(heap_shit *heap_ctx) {
	if (heap_ctx->heap_size) {
		return heap_shit_borrar_directo(heap_ctx, heap_ctx->heap[1].llave);
	} else {
		assert_timeout(heap_ctx->heap[1].llave==HEAP_SHIT_VALOR_INVALIDO);
		return NULL;
	}
}

#if  0
void heap_shit_dumpear(heap_shit *heap_ctx) {
	natural heap_size = heap_ctx->heap_size;
	natural *mapeo_inv = heap_ctx->mapeo_idx_heap_a_pos_ocurrencia;
	for (int i = 0; i <= heap_size; i++) {
		caca_log_debug("mierda %u es %u", i, heap_ctx->heap[i]);
	}
	caca_log_debug("los mapeos inversos:");
	for (int i = 0; i < MAX_NUMEROS; i++) {
		caca_log_debug("la llave %u mapea a %u", i, mapeo_inv[i]);
	}
	caca_log_debug("las ocurrencias de cada pendejo:");
	for (int i = 0; i < MAX_INDEX; i++) {
		if (heap_ctx->num_indices_valores[i]) {
			caca_log_debug("el num %u tiene las ocurrencias %s", i,
					caca_arreglo_a_cadena_natural(heap_ctx->indices_valores[i],heap_ctx->num_indices_valores[i],FRAUDUCACA_BUF_STATICO));
		}
	}
}

void heap_shit_valida_referencias_inversas(heap_shit *heap_ctx) {
	natural num_elems = 0;
	natural num_elems_mapeo = 0;
	natural heap_size = heap_ctx->heap_size;
	tipo_dato *heap = heap_ctx->heap;
	natural (*indices_valores)[MAX_NUMEROS] = heap_ctx->indices_valores;
	natural *num_indices_valores = heap_ctx->num_indices_valores;
	natural *mapeo_inv = heap_ctx->mapeo_idx_heap_a_pos_ocurrencia;
	bitch_vector valores_ya_validados[(MAX_NUMEROS / 64) + 1] = {0};
	for (int i = 1; i < heap_size + 1; i++) {
		tipo_dato num_act = heap[i];
		natural *indices_valores_act = indices_valores[num_act];
		natural num_indices_valores_act = num_indices_valores[num_act];
		if (num_indices_valores_act
				&& !caca_comun_checa_bit(valores_ya_validados, num_act)) {
			for (int j = 0; j < num_indices_valores_act; j++) {
				tipo_dato num_act_ocu = 0;
				natural indice_valor_act = 0;
				natural indice_pos_ocurrencia_en_mapa = 0;

				indice_valor_act = indices_valores_act[j];
				num_act_ocu = heap[indice_valor_act];
				assert_timeout(num_act_ocu == num_act);

				indice_pos_ocurrencia_en_mapa = mapeo_inv[indice_valor_act];
				assert_timeout(
						indice_pos_ocurrencia_en_mapa
						!= FRAUDUCACA_VALOR_INVALIDO);

				assert_timeout(j == indice_pos_ocurrencia_en_mapa);

				num_elems++;
			}
			caca_comun_asigna_bit(valores_ya_validados, num_act);
		}
	}
	assert_timeout(heap_size == num_elems);
	for (int i = 0; i < MAX_NUMEROS; i++) {
		natural idx_en_heap = 0;
		natural idx_en_pos_ocurrencias = 0;
		natural num_indices_valores_act = 0;
		tipo_dato num_act = 0;

		idx_en_heap = i;
		idx_en_pos_ocurrencias = mapeo_inv[i];
		if (idx_en_pos_ocurrencias != FRAUDUCACA_VALOR_INVALIDO) {
			num_act = heap[idx_en_heap];

			num_indices_valores_act = num_indices_valores[num_act];
			assert_timeout(idx_en_pos_ocurrencias < num_indices_valores_act);

			num_elems_mapeo++;
		}
	}

	assert_timeout(heap_size == num_elems_mapeo);
}

void heap_shit_valida_invariante(heap_shit *heap_ctx, natural idx_heap) {
	natural heap_size = heap_ctx->heap_size;
	tipo_dato *heap = heap_ctx->heap;
	if (idx_heap < heap_size) {
		tipo_dato num_act = heap[idx_heap];
		natural idx_heap_hijo_izq = (idx_heap << 1);
		if (idx_heap_hijo_izq < heap_size) {
			tipo_dato num_act_hijo_izq = heap[idx_heap_hijo_izq];
			if (heap_ctx->min) {
				assert_timeout(num_act <= num_act_hijo_izq);
			} else {
				assert_timeout(num_act >= num_act_hijo_izq);
			}
		}
		if (idx_heap_hijo_izq + 1 < heap_size) {
			tipo_dato num_act_hijo_der = heap[idx_heap_hijo_izq + 1];
			if (heap_ctx->min) {
				assert_timeout(num_act <= num_act_hijo_der);
			} else {
				assert_timeout(num_act >= num_act_hijo_der);
			}
		}
		heap_shit_valida_invariante(heap_ctx, idx_heap_hijo_izq);
		heap_shit_valida_invariante(heap_ctx, idx_heap_hijo_izq + 1);
	}
}

void heap_shit_valida_mierda(heap_shit *heap_ctx) {
	heap_shit_valida_referencias_inversas(heap_ctx);
	heap_shit_valida_invariante(heap_ctx, 1);
}
#endif

#endif

#if 1

typedef struct puto_cardinal {
	union {
		struct {
			int coordenada_y_puto_cardinal;
			int coordenada_x_puto_cardinal;
		} separados_puto_cardinal;
		entero_largo coordenadas_juntas_puto_cardinal;
	} datos_puto_cardinal;
	void *extra;
} puto_cardinal;

#define coord_x datos_puto_cardinal.separados_puto_cardinal.coordenada_x_puto_cardinal
#define coord_y datos_puto_cardinal.separados_puto_cardinal.coordenada_y_puto_cardinal
#define coord_xy datos_puto_cardinal.coordenadas_juntas_puto_cardinal

static inline short puto_cardinal_compacta_a_corto(puto_cardinal *nodo) {
	int coord_xy_compacta = 0;

	coord_xy_compacta = (nodo->coord_x << 8) | nodo->coord_y;

	return coord_xy_compacta;
}

#define puto_cardinal_a_cadena_buffer_local(puto) puto_cardinal_a_cadena((puto),CACA_COMUN_BUF_STATICO)

#define puto_cardinal_mem_local (&(puto_cardinal){0})

static inline puto_cardinal *puto_cardinal_suma(puto_cardinal *p_res,
		puto_cardinal *p_op1, puto_cardinal *p_op2) {
	p_res->coord_x = p_op1->coord_x + p_op2->coord_x;
	p_res->coord_y = p_op1->coord_y + p_op2->coord_y;
	return p_res;
}

#define puto_cardinal_suma_local(p_op1,p_op2) puto_cardinal_suma(&(puto_cardinal){0},p_op1,p_op2)
#define puto_cardinal_suma_a_op1(p_op1,p_op2) puto_cardinal_suma(p_op1,&(puto_cardinal){.coord_x=p_op1->coord_x,.coord_y=p_op1->coord_y},p_op2)

static inline char *puto_cardinal_a_cadena(puto_cardinal *puto, char *buffer) {
	*buffer = '\0';

	sprintf(buffer, "%d,%d(%llx,%hx)", puto->coord_x, puto->coord_y,
			puto->coord_xy, (short )puto_cardinal_compacta_a_corto(puto));

	return buffer;
}

static inline char *puto_cardinal_arreglo_a_cacadena(puto_cardinal *putos,
		natural putos_tam, char *buffer) {
	*buffer = '\0';
	char *buffer_tmp = buffer;

	for (int i = 0; i < putos_tam; i++) {
		natural scritos = sprintf(buffer_tmp, "%s-",
				puto_cardinal_a_cadena(putos+i, CACA_COMUN_BUF_STATICO));
		buffer_tmp += scritos;

	}

	return buffer;
}

#define puto_cardinal_obten_entero_de_extra(puto) ((int)((natural)(puto)->extra & 0xffffffff))
#define puto_cardinal_pon_entero_en_extra(puto,valor) ((puto)->extra=(void *)(entero_largo_sin_signo)((natural)valor))

#endif

typedef puto_cardinal horario;
#define hora_inicio coord_x
#define hora_fin coord_y
#define horas_amontonadas coord_xy

#define TABLA_TREN_TARADO_MAX_HORARIOS 100

horario tabla_tren_horarios_a[TABLA_TREN_TARADO_MAX_HORARIOS] = { 0 };
horario tabla_tren_horarios_b[TABLA_TREN_TARADO_MAX_HORARIOS] = { 0 };

int tabla_tren_tarado_compara_horarios_cb(void *pa, void *pb) {
	puto_cardinal *a = pa;
	puto_cardinal *b = pb;
	int res = 0;

	if (a->horas_amontonadas == b->horas_amontonadas) {
		res = puto_cardinal_obten_entero_de_extra(
				a) - puto_cardinal_obten_entero_de_extra(b);
	} else {
		if (a->horas_amontonadas < b->horas_amontonadas) {
			res = -1;
		} else {
			if (a->horas_amontonadas > b->horas_amontonadas) {
				res = 1;
			} else {
				res = 0;
			}
		}
	}

	return res;
}

static inline char * tabla_tren_tarado_horario_a_cadena(horario *h, char *buf) {
	buf[0] = '\0';

	sprintf(buf, "%02u:%02u - %02u:%02u %u", h->hora_inicio/60,
			h->hora_inicio%60, h->hora_fin/60, h->hora_fin%60,
			puto_cardinal_obten_entero_de_extra(h));

	return buf;
}

#define tabla_tren_tarado_horario_a_cadena_buf_local(h) tabla_tren_tarado_horario_a_cadena(h,CACA_COMUN_BUF_STATICO)

#define tabla_tren_tarado_horario_viene_de_a(h) (puto_cardinal_obten_entero_de_extra(h)<1000)

static inline void tabla_tren_tarado_core(horario *horarios_a,
		horario *horarios_b, natural horarios_a_tam, natural horarios_b_tam,
		natural *trenes_a, natural *trenes_b) {
	heap_shit *hctx = NULL;
	arbol_verga_ctx *abctx_a = NULL;
	arbol_verga_ctx *abctx_b = NULL;

	hctx = heap_shit_init(verdadero);

	abctx_a = arbol_verga_init(tabla_tren_tarado_compara_horarios_cb);
	abctx_b = arbol_verga_init(tabla_tren_tarado_compara_horarios_cb);

	for (int i = 0; i < horarios_a_tam; i++) {
		horario *horario_act = horarios_a + i;

		heap_shit_insertar(hctx, horario_act->hora_fin,
				puto_cardinal_obten_entero_de_extra(horario_act), horario_act);

		arbol_verga_inserta(abctx_a, horario_act);
	}

	for (int i = 0; i < horarios_b_tam; i++) {
		horario *horario_act = horarios_b + i;

		heap_shit_insertar(hctx, horario_act->hora_fin,
				puto_cardinal_obten_entero_de_extra(horario_act), horario_act);
		arbol_verga_inserta(abctx_b, horario_act);
	}

	while (hctx->heap_size) {
		bool res_borrar = falso;
		arbol_verga_ctx *abctx_act = NULL;
		horario *horario_busqueda = &(horario ) { 0 };
		horario *horario_act = NULL;
		horario *horario_inicial_act = heap_shit_borra_torpe(hctx);
		caca_log_debug("horario inicial act %s",
				tabla_tren_tarado_horario_a_cadena_buf_local(horario_inicial_act));

		if (tabla_tren_tarado_horario_viene_de_a(horario_inicial_act)) {
			abctx_act = abctx_b;
			res_borrar = arbol_verga_borra_llave(abctx_a,
					abctx_a->raiz_arbol_verga_ctx, horario_inicial_act);
			(*trenes_a)++;
		} else {
			abctx_act = abctx_a;
			res_borrar = arbol_verga_borra_llave(abctx_b,
					abctx_b->raiz_arbol_verga_ctx, horario_inicial_act);
			(*trenes_b)++;
		}
		assert_timeout(res_borrar);

		horario_busqueda->hora_inicio = horario_inicial_act->hora_fin;
		horario_busqueda->hora_fin = horario_inicial_act->hora_fin;
		puto_cardinal_pon_entero_en_extra(horario_busqueda, -1);

		while ((horario_act = arbol_verga_encuentra_mayor(abctx_act,
				abctx_act->raiz_arbol_verga_ctx, horario_busqueda))) {
			horario *horario_borrado = NULL;
			caca_log_debug("el horario act %s",
					tabla_tren_tarado_horario_a_cadena_buf_local(horario_act));

			horario_borrado = heap_shit_borrar_directo(hctx,
					puto_cardinal_obten_entero_de_extra(horario_act));
			assert_timeout(horario_borrado);

			res_borrar = arbol_verga_borra_llave(abctx_act,
					abctx_act->raiz_arbol_verga_ctx, horario_act);
			assert_timeout(res_borrar);

			if (tabla_tren_tarado_horario_viene_de_a(horario_act)) {
				abctx_act = abctx_b;
			} else {
				abctx_act = abctx_a;
			}

			horario_busqueda->hora_inicio = horario_act->hora_fin;
			horario_busqueda->hora_fin = horario_act->hora_fin;
			puto_cardinal_pon_entero_en_extra(horario_busqueda, -1);
		}

	}

	arbol_verga_fini(abctx_a);
	arbol_verga_fini(abctx_b);

	heap_shit_fini(hctx);
}

static inline void tabla_tren_tarado_main() {
	natural num_cacasos = 0;
	scanf("%u", &num_cacasos);

	for (int j = 0; j < num_cacasos; j++) {
		natural vuelta_tiempo = 0;
		natural horarios_a_tam = 0;
		natural horarios_b_tam = 0;
		natural trenes_a = 0;
		natural trenes_b = 0;

		scanf("%u", &vuelta_tiempo);

		scanf("%u %u", &horarios_a_tam, &horarios_b_tam);

		for (natural i = 0; i < horarios_a_tam; i++) {
			natural horas_inicio = 0;
			natural minutos_inicio = 0;
			natural horas_fin = 0;
			natural minutos_fin = 0;

			horario *horario_act = tabla_tren_horarios_a + i;

			scanf("%u:%u %u:%u", &horas_inicio, &minutos_inicio, &horas_fin,
					&minutos_fin);

			horario_act->hora_inicio = horas_inicio * 60 + minutos_inicio;
			horario_act->hora_fin = horas_fin * 60 + minutos_fin
					+ vuelta_tiempo;
			puto_cardinal_pon_entero_en_extra(horario_act, i);
		}

		for (natural i = 0; i < horarios_b_tam; i++) {
			natural horas_inicio = 0;
			natural minutos_inicio = 0;
			natural horas_fin = 0;
			natural minutos_fin = 0;

			horario *horario_act = tabla_tren_horarios_b + i;

			scanf("%u:%u %u:%u", &horas_inicio, &minutos_inicio, &horas_fin,
					&minutos_fin);

			horario_act->hora_inicio = horas_inicio * 60 + minutos_inicio;
			horario_act->hora_fin = horas_fin * 60 + minutos_fin
					+ vuelta_tiempo;
			puto_cardinal_pon_entero_en_extra(horario_act, (i+1)*1000);
		}

		tabla_tren_tarado_core(tabla_tren_horarios_a, tabla_tren_horarios_b,
				horarios_a_tam, horarios_b_tam, &trenes_a, &trenes_b);

		printf("Case #%u: %u %u\n", j + 1, trenes_a, trenes_b);
	}
}

int main(void) {
	tabla_tren_tarado_main();
	return EXIT_SUCCESS;
}
