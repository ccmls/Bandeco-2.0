
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N_ESTUDANTES 4

/**
 * variáveis para guardar estado dos estudantes
 * F = esperando na fila
 * D = dine
 * OL = ok to leave (terminou de comer)
 * L = leave
**/

typedef enum
{
  F,
  D,
  OL,
  L
} estado;

estado estadoE[N_ESTUDANTES];

//variáveis que irão ser usadas para controle
volatile int eating = 0;       //estudante está comendo
volatile int readyToLeave = 0; //estudante está pronto para sair

// declaração dos locks
pthread_mutex_t mutex;     
pthread_cond_t cond; 

/* Retorna o estado do sistema, com:
   - quantas pessoas estão na saída; 
   - quantas estão na entrada; 
   - quantas estão comendo;
   - quantas estão esperando para sair.
*/
void countSaidaEntrada(int *n_pessoas_saida, int *n_pessoas_entrada, int *n_pessoas_comendo, int *n_pessoas_esperando) {
  int i, count = 0, entrada = 0, saida = 0, comendo = 0, esperando = 0;
  for (int i = 0; i < N_ESTUDANTES; i++) {
    switch (estadoE[i]) {
      case F:
        entrada++;
        break;
      case L:
        saida++;
        break;
      case D:
        comendo++;
        break;
      case OL:
        esperando++;
        break;
    }
  }
  *n_pessoas_saida = saida;
  *n_pessoas_entrada = entrada;
  *n_pessoas_comendo = comendo;
  *n_pessoas_esperando = esperando;
}

/*
* Descrição da impressão:
* o -> é cada estudante
* $$$ -> catracas de entrada
* \|%|/ -> bandeja cheia (estudante está comendo)
* \| |/ -> bandeja vazia (estudante terminou de comer e está pronto para sair)
* números na MESA -> cadeiras que vão sentar os N estudantes
* \o/ <mensagem> \o/ -> pessoas chamando para festa na saida do bandejão
*/

/**********************************************************************
********************** IMPRESSÃO DA ENTRADA ***************************
***********************************************************************/
void imprime_entrada(int n_pessoas_entrada){
      int total = 6*N_ESTUDANTES + 7;
      printf("ENTRADA\n");
      printf("=== $$$$ ===");
      for (int i = 12; i<total ; i++){
        printf("=");
      }
      printf("\n");
      int pos_final = total-6;
      int pos_inicial = pos_final - 2*n_pessoas_entrada;
      for (int i = 0; i<pos_inicial; i++){
        printf(" ");
      }
      for(int i = 0; i <n_pessoas_entrada; i++){
        printf(" o");
      }
      printf("\n");
      for(int i = 0; i <total-8; i++){
        printf("-");
      }
      printf("    ----\n");
} 

/**********************************************************************
************************* IMPRESSÃO DA SAÍDA **************************
***********************************************************************/
void festa_aleatoria() {
  const char *festas[5] = {"biiiiiixopada", "coOoOoOorote break", "festa da cêêêêênicass", "feeeeeeesta da rinha", "foOoOogo na Babilônia"};
  printf("\\o/ %s \\o/\n", festas[random()%5]);
}

void imprime_saida(int n_pessoas_saida) {
  int i;
  printf("SAÍDA\n");
  
  for (i = 0; i < (N_ESTUDANTES * 6 - 1); i++) {
    printf("-");
  }
  printf("    ----\n");
  
  printf("    ");
  for (i = 0; i < n_pessoas_saida; i++) {
    printf(" o");
  }
  printf("\n");
  
  printf("----    ");
  for (i = 0; i < (N_ESTUDANTES * 6 - 1); i++) {
    printf("-");
  }
  printf("\n");
  festa_aleatoria();
}

/**********************************************************************
************************* IMPRESSÃO DA MESA ***************************
***********************************************************************/
void imprime_mesa(int n_pessoas_comendo, int n_pessoas_esperando) {
  printf("MESA DO BANDECO\n");
  for (int i = 0; i < N_ESTUDANTES*6+7; i++)
  {
    printf("_");
  }
  printf("\n");
  printf("    ");
  for (int i = 0; i < N_ESTUDANTES; i++)
  {
    printf("  %d   ", i+1);
  }
  
  printf("\n");
  
  if(n_pessoas_comendo==0){
    if(n_pessoas_esperando==0){
      for (int i = 0; i < N_ESTUDANTES * 6 + 7; i++)
        printf("_");
    }else{
      printf("___");
      for (int i = 0; i < n_pessoas_esperando; i++)
        printf("_\\| |/");
      if(n_pessoas_esperando<N_ESTUDANTES){
        for (int i = 0; i < ((N_ESTUDANTES*6+7)-(n_pessoas_esperando*6)-3); i++)
          printf("_");
        printf("\n");
        printf("   ");
        for (int i = 0; i < (n_pessoas_esperando); i++)
          printf("   o  ");
      }
    }
    
  }else if(n_pessoas_comendo>0){
    printf("___");
    if(n_pessoas_esperando>0){
      for(int i=0; i < n_pessoas_comendo; i++)
        printf("_\\|%%|/");
      for (int i = 0; i < n_pessoas_esperando; i++)
        printf("_\\| |/");
      if((n_pessoas_comendo+n_pessoas_esperando)<N_ESTUDANTES){
        for (int i = 0; i < (((N_ESTUDANTES*6+7)-((n_pessoas_comendo+n_pessoas_esperando))*6)-3); i++)
          printf("_");
      }else{
        printf("____");
      }
    }
    else
    {
      for (int i = 0; i < n_pessoas_comendo; i++)
        printf("_\\|%%|/");
      if (n_pessoas_comendo  < N_ESTUDANTES)
      {
        for (int i = 0; i < (((N_ESTUDANTES*6+7)-(n_pessoas_comendo)*6)-3); i++)
          printf("_");
      }else{
        printf("____");
      }
    }
    
    printf("\n");
    printf("   ");
    for (int i = 0; i < (n_pessoas_comendo + n_pessoas_esperando); i++)
      printf("   o  ");
  }
}

/**********************************************************************
************************* IMPRESSÃO DE TUDO ***************************
***********************************************************************/
void imprime() {
  int n_pessoas_saida = 0, n_pessoas_entrada = 0, n_pessoas_comendo, n_pessoas_esperando, i;
  countSaidaEntrada(&n_pessoas_saida, &n_pessoas_entrada, &n_pessoas_comendo, &n_pessoas_esperando);
  printf("\n");
  imprime_entrada(n_pessoas_entrada);
  printf("\n\n");
  imprime_mesa(n_pessoas_comendo, n_pessoas_esperando);
  printf("\n\n");
  imprime_saida(n_pessoas_saida);
  printf("\n\n");
  for (i = 0; i < 6 * N_ESTUDANTES + 11; i++)
    printf("*");
  printf("\n\n");
}

//função para a thread dos estudantes
void *estudante(void *v) {
  int id = *(int *)v;

  // o estudante pegou a comida
  // lock para a thread alterar seu estado para esperando na fila
  pthread_mutex_lock(&mutex);
  estadoE[id] = F;
  imprime();
  pthread_mutex_unlock(&mutex);

  /**
   * se duas pessoas estiverem comendo e uma estiver pronta para sair
   * a que pegou a comida e esta entrando agora libera a outra para sair
   **/
  // lock para a thread dizer que está comendo e alterar seu estado para dine
  pthread_mutex_lock(&mutex);
  eating++;
  if (eating == 2 && readyToLeave == 1)
  {
    // thread sinaliza para as outras
    pthread_cond_signal(&cond);
    readyToLeave--;
  }

  //o estudante vai comer: dine()
  estadoE[id] = D;
  imprime();
  pthread_mutex_unlock(&mutex);
  //espera um pouquino comendo
  sleep(random() % 3);

  //terminou de comer e vai se preparar para sair
  // lock para a thread alterar seu estado para pronto pra sair
  pthread_mutex_lock(&mutex);
  eating--;
  readyToLeave++;
  
  //terminou de comer: está pronto para sair
  estadoE[id] = OL;
  imprime();

  /**
   * se tem uma pessoa comendo e uma pronta para sair então o estudante
   * libera o contador eating e espera o outro terminar de comer para sair
   * se não tem ninguém comendo e tem dois esperando para sair
   * então libera os dois, decrementa e libera o contador
   * Se não acontecer nenhum desses casos, então ele não pode sair
   **/
  if (eating == 1 && readyToLeave == 1)
  {
    pthread_cond_wait(&cond, &mutex);
  }
  else if (eating == 0 && readyToLeave == 2)
  {
    pthread_cond_signal(&cond);
    readyToLeave -= 2;
  }
  else
  {
    readyToLeave--;
  }

  //estudante sai da mesa
  estadoE[id] = L;
  imprime();

  pthread_mutex_unlock(&mutex);
}

/**********************************************************************
******************************** MAIN *********************************
***********************************************************************/
int main() {
  pthread_t thr_estudante[N_ESTUDANTES];
  int i, id_estudante[N_ESTUDANTES];
  
  srand((unsigned) time(NULL));

  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond, NULL);

  for (i = 0; i < N_ESTUDANTES; i++)
  {
    id_estudante[i] = i;
    pthread_create(&thr_estudante[i], NULL, estudante, (void *)&id_estudante[i]);
  }

  for (i = 0; i < N_ESTUDANTES; i++)
    pthread_join(thr_estudante[i], NULL);
    
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);
  
  return 0;
}