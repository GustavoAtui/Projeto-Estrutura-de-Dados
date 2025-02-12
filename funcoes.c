#include "funcoes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int data_valida(int dia, int mes) {
  if (mes < 1 || mes > 12) {
    return 0;
  }
  if (dia < 1 || dia > 31) {
    return 0;
  }
  return 1;
}

Paciente *buscar_paciente(ListaPacientes *lista, const char *rg) {
  for (int i = 0; i < lista->quantidade; i++) {
    if (strcmp(lista->lista[i].rg, rg) == 0) {
      return &lista->lista[i];
    }
  }
  return NULL;
}
void cadastrar(ListaPacientes *lista, ABB *arvore) {
  int opcao;
  
  do {
    printf("\n--------Cadastrar--------\n");
    printf("1. Cadastro de Novo Paciente\n");
    printf("2. Consultar Paciente\n");
    printf("3. Mostrar Lista de Pacientes\n");
    printf("4. Atualizar dados de um Paciente\n");
    printf("5. Remover Paciente\n");
    printf("6. Voltar para o Menu\n");
    printf("\n-------------------------\n");
    scanf("%d", &opcao);
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
    

    switch (opcao) {
    case 1: {
      cadastrar_paciente(lista, arvore);
      break;
    }
    case 2: {
      // consultar
      char rg[15];
      printf("Digite o RG do paciente: ");
      scanf("%s", rg);
      Paciente *paciente = buscar_paciente(lista, rg);
      if (paciente != NULL) {
        printf("Nome: %s\n", paciente->nome);
        printf("Idade: %d\n", paciente->idade);
        printf("RG: %s\n", paciente->rg);
        printf("Data de Registro: %d/%d/%d\n", paciente->data_registro.dia,
               paciente->data_registro.mes, paciente->data_registro.ano);
      } else {
        printf("RG não encontrado.\n");
      }
      break;
    }
    case 3: {
      // lista  de pacientes
      printf("Lista de Pacientes:\n");
      for (int i = 0; i < lista->quantidade; i++) {
        printf("------------------------------\n");
        printf("Nome: %s\n", lista->lista[i].nome);
        printf("Idade: %d\n", lista->lista[i].idade);
        printf("RG: %s\n", lista->lista[i].rg);
        printf("Data de Registro: %d/%d/%d\n",
               lista->lista[i].data_registro.dia,
               lista->lista[i].data_registro.mes,
               lista->lista[i].data_registro.ano);
      }
      
      break;
      
    }
    case 4: {
      // atualizar
      char rg[15];
      printf("Digite o RG do paciente: ");
      scanf("%s", rg);
      Paciente *paciente = buscar_paciente(lista, rg);
      if (paciente != NULL) {
        printf("Digite o novo nome: ");
        scanf("%s", paciente->nome);
        printf("Digite a nova idade: ");
        scanf("%d", &paciente->idade);
        printf("Digite a nova data de registro (DD MM AA): ");
        scanf("%d %d %d", &paciente->data_registro.dia,
              &paciente->data_registro.mes, &paciente->data_registro.ano);
        printf("Dados atualizados com sucesso.\n");
      }
      break;
    }
    case 5: {
      // remover
      char rg[15];
      printf("Digite o RG do paciente para remover: ");
      scanf("%s", rg);

      int indice = -1;
      for (int i = 0; i < lista->quantidade; i++) {
        if (strcmp(lista->lista[i].rg, rg) == 0) {
          indice = i;
          break;
        }
      }

      if (indice == -1) {
        printf("Paciente não encontrado.\n");
      } else {

        for (int i = indice; i < lista->quantidade - 1; i++) {
          lista->lista[i] = lista->lista[i + 1];
        }
        lista->quantidade--;

        arvore->raiz = remover_na_arvore(arvore->raiz, rg);

        printf("Paciente removido com sucesso.\n");
      }
      break;
    }
    case 6: {
      printf("Voltando ao menu principal.\n");
      break;
    }
    default:
      printf("Opção inválida.\n");
    }
  } while (opcao != 6);
}

void cadastrar_paciente(ListaPacientes *lista, ABB *arvore) {
  if (lista->quantidade >= MAX_PACIENTES) {
    printf("Lista de pacientes está cheia!\n");
    return;
  }

  Paciente novo_paciente;

  printf("Digite o nome do paciente: ");
  scanf(" %s", novo_paciente.nome);

  printf("Digite a idade do paciente: ");
  scanf("%d", &novo_paciente.idade);

  printf("Digite o RG do paciente: ");
  scanf(" %s", novo_paciente.rg);

  if (buscar_paciente(lista, novo_paciente.rg) != NULL) {
    printf("Erro: RG já cadastrado.\n");
    return;
  }

  do {
    printf("Digite o dia do registro: ");
    scanf("%d", &novo_paciente.data_registro.dia);

    printf("Digite o mês do registro: ");
    scanf("%d", &novo_paciente.data_registro.mes);

    printf("Digite o ano do registro: ");
    scanf("%d", &novo_paciente.data_registro.ano);

    if (!data_valida(novo_paciente.data_registro.dia,
                     novo_paciente.data_registro.mes)) {
      printf("Data inválida. Tente novamente.\n");
    }
  } while (!data_valida(novo_paciente.data_registro.dia,
                        novo_paciente.data_registro.mes));

  lista->lista[lista->quantidade] = novo_paciente;
  lista->quantidade++;

  inserir_na_arvore(arvore, novo_paciente);
 

  printf("Paciente cadastrado com sucesso!\n");
}

void enfileirar(FilaAtendimento *fila, Paciente *paciente, NoPilha **topo) {
  EFila *novo = (EFila *)malloc(sizeof(EFila));
  if (novo == NULL) {
    printf("Erro ao alocar memória.\n");
    return;
  }
  novo->paciente = *paciente;
  novo->prox = NULL;
  if (fila->tail == NULL) {
    fila->head = novo;
  } else {
    fila->tail->prox = novo;
  }
  fila->tail = novo;
  fila->qtde++;

  push_operacao(topo, ENFILEIRAR, *paciente);

  printf("Paciente %s enfileirado com sucesso!\n", paciente->nome);
}

void desenfileirar(FilaAtendimento *fila, NoPilha **topo) {
  if (fila->head == NULL) {
    printf("Fila vazia.\n");
    return;
  }
  EFila *temp = fila->head;
  Paciente paciente = temp->paciente;
  fila->head = temp->prox;
  if (fila->head == NULL) {
    fila->tail = NULL;
  }

  push_operacao(topo, DESENFILEIRAR, paciente);

  printf("Paciente %s atendido e removido da fila.\n", temp->paciente.nome);
  free(temp);
  fila->qtde--;
}

void mostrar_fila(FilaAtendimento *fila) {
  if (fila->head == NULL) {
    printf("Fila vazia.\n");
    return;
  }
  EFila *temp = fila->head;
  printf("Fila de Atendimento:\n");
  while (temp != NULL) {
    printf("Paciente: %s, Idade: %d, RG: %s\n", temp->paciente.nome,
           temp->paciente.idade, temp->paciente.rg);
    temp = temp->prox;
  }
}

void atendimento(ListaPacientes *lista, FilaAtendimento *fila, NoPilha **topo) {
  int opcao;
  do {
    printf("\n--------Atendimento--------\n");
    printf("1. Enfileirar Paciente\n");
    printf("2. Desenfileirar Paciente\n");
    printf("3. Mostrar Fila\n");
    printf("4. Voltar ao Menu Principal\n");
    printf("---------------------------\n");
    scanf("%d", &opcao);
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif

    switch (opcao) {
    case 1: {
      char rg[15];
      printf("Digite o RG do paciente para enfileirar: ");
      scanf(" %[^\n]", rg);

      Paciente *paciente = buscar_paciente(lista, rg);
      if (paciente != NULL) {
        enfileirar(fila, paciente, topo);
      } else {
        printf("Erro: Paciente não cadastrado.\n");
      }
      break;
    }
    case 2: {
      desenfileirar(fila, topo);
      break;
    }
    case 3:
      mostrar_fila(fila);
      break;
    case 4:
      printf("Voltando ao menu principal.\n");
      break;
    default:
      printf("Opção inválida.\n");
    }
  } while (opcao != 4);
}

void pesquisar(ABB *arvore) {
  int opcao;
  do {
    printf("\n--------Pesquisar--------\n");
    printf("1. Pesquisar por Ano\n");
    printf("2. Pesquisar por Mês\n");
    printf("3. Pesquisar por Dia\n");
    printf("4. Pesquisar por Idade\n");
    printf("5. Voltar ao Menu Principal\n");
    printf("---------------------------\n");
    scanf("%d", &opcao);
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif

    switch (opcao) {
    case 1: {
      int ano;
      printf("Digite o ano para pesquisar: ");
      scanf("%d", &ano);
      listar_por_ano(arvore->raiz, ano);
      break;
    }
    case 2: {
      int mes;
      printf("Digite o mês para pesquisar: ");
      scanf("%d", &mes);
      listar_por_mes(arvore->raiz, mes);
      break;
    }
    case 3: {
      int dia;
      printf("Digite o dia para pesquisar: ");
      scanf("%d", &dia);
      listar_por_dia(arvore->raiz, dia);
      break;
    }
    case 4: {
      int idade;
      printf("Digite a idade para pesquisar: ");
      scanf("%d", &idade);
      listar_por_idade(arvore->raiz, idade);
      break;
    }
    case 5:
      printf("Voltando ao menu principal.\n");
      break;
    default:
      printf("Opção inválida.\n");
    }
  } while (opcao != 5);
}

void inserir_na_arvore(ABB *arvore, Paciente paciente) {
  EABB *novo = (EABB *)malloc(sizeof(EABB));
  novo->dados = paciente;
  novo->filhoEsq = novo->filhoDir = NULL;
  int opcao;
  if (arvore->raiz == NULL) {
    arvore->raiz = novo;
  } else {
    EABB *atual = arvore->raiz;
    EABB *anterior = NULL;
    
      while (atual != NULL) {
        anterior = atual;
        if (paciente.data_registro.ano < atual->dados.data_registro.ano) {
          atual = atual->filhoEsq;
        } else {
          atual = atual->filhoDir;
        }
      }
  
      if (paciente.data_registro.ano < anterior->dados.data_registro.ano) {
        anterior->filhoEsq = novo;
      } else {
        anterior->filhoDir = novo;
      }
    }
  
    arvore->qtde++;
  }
  


void listar_por_ano(EABB *raiz, int ano) {
  if (raiz != NULL) {
    listar_por_ano(raiz->filhoEsq, ano);

    if (raiz->dados.data_registro.ano == ano) {
      printf("Nome: %s, Idade: %d, RG: %s, Data:%d/%d/%d\n", raiz->dados.nome,
             raiz->dados.idade, raiz->dados.rg, raiz->dados.data_registro.dia,
             raiz->dados.data_registro.mes, raiz->dados.data_registro.ano);
    }

    listar_por_ano(raiz->filhoDir, ano);
  }
}

void listar_por_mes(EABB *raiz, int mes) {
  if (raiz != NULL) {
    listar_por_mes(raiz->filhoEsq, mes);

    if (raiz->dados.data_registro.mes == mes) {
      printf("Nome: %s, Idade: %d, RG: %s, Data:%d/%d/%d\n", raiz->dados.nome,
             raiz->dados.idade, raiz->dados.rg, raiz->dados.data_registro.dia,
             raiz->dados.data_registro.mes, raiz->dados.data_registro.ano);
    }

    listar_por_mes(raiz->filhoDir, mes);
  }
}

void listar_por_dia(EABB *raiz, int dia) {
  
  if (raiz != NULL) {
    listar_por_dia(raiz->filhoEsq, dia);

    if (raiz->dados.data_registro.dia == dia) {
      printf("Nome: %s, Idade: %d, RG: %s, Data:%d/%d/%d\n", raiz->dados.nome,
             raiz->dados.idade, raiz->dados.rg, raiz->dados.data_registro.dia,
             raiz->dados.data_registro.mes, raiz->dados.data_registro.ano);
    }

    listar_por_dia(raiz->filhoDir, dia);
  }
}

void listar_por_idade(EABB *raiz, int idade) {
  if (raiz != NULL) {
    listar_por_idade(raiz->filhoEsq, idade);

    if (raiz->dados.idade == idade) {
      printf("Nome: %s, Idade: %d, RG: %s, Data:%d/%d/%d\n", raiz->dados.nome,
             raiz->dados.idade, raiz->dados.rg, raiz->dados.data_registro.dia,
             raiz->dados.data_registro.mes, raiz->dados.data_registro.ano);
    }

    listar_por_idade(raiz->filhoDir, idade);
  }
}

EABB *remover_na_arvore(EABB *raiz, const char *rg) {
  if (raiz == NULL) {
    return NULL; 
  }
  if (strcmp(rg, raiz->dados.rg) < 0) {
    raiz->filhoEsq = remover_na_arvore(raiz->filhoEsq, rg);
  } else if (strcmp(rg, raiz->dados.rg) > 0) {
    raiz->filhoDir = remover_na_arvore(raiz->filhoDir, rg);
  } else {
    if (raiz->filhoEsq == NULL && raiz->filhoDir == NULL) {
      free(raiz);
      return NULL;
    }
    else if (raiz->filhoEsq == NULL) {
      EABB *temp = raiz->filhoDir;
      free(raiz);
      return temp;
    } else if (raiz->filhoDir == NULL) {
      EABB *temp = raiz->filhoEsq;
      free(raiz);
      return temp;
    }
    else {
      EABB *temp = raiz->filhoDir;
      while (temp->filhoEsq != NULL) {
        temp = temp->filhoEsq;
      }
      raiz->dados = temp->dados;
      raiz->filhoDir = remover_na_arvore(raiz->filhoDir, temp->dados.rg);
    }
  }
  return raiz;
}

void push_operacao(NoPilha **topo, TipoOperacao tipo, Paciente paciente) {
  NoPilha *novo = (NoPilha *)malloc(sizeof(NoPilha));
  novo->operacao.tipo = tipo;
  novo->operacao.paciente = paciente;
  novo->prox = *topo;
  *topo = novo;
}

void desfazer_operacao(FilaAtendimento *fila, NoPilha **topo) {
  if (*topo == NULL) {
    printf("Nenhuma operação para desfazer.\n");
    return;
  }
  char confirmacao;
  printf("Deseja desfazer a última operação? (s/n): ");
  scanf(" %c", &confirmacao);
  

  if (confirmacao != 's' && confirmacao != 'S') {
    printf("Operação de desfazer cancelada.\n");
    return;
  }

  Operacao operacao = (*topo)->operacao;
  NoPilha *remover = *topo;
  *topo = (*topo)->prox;
  free(remover);

  if (operacao.tipo == ENFILEIRAR) {
    EFila *atual = fila->head;
    EFila *anterior = NULL;

    while (atual != NULL &&
           strcmp(atual->paciente.rg, operacao.paciente.rg) != 0) {
      anterior = atual;
      atual = atual->prox;
    }

    if (atual == NULL) {
      printf("Erro: Paciente não encontrado na fila.\n");
      return;
    }

    if (anterior == NULL) { 
      fila->head = atual->prox;
    } else {
      anterior->prox = atual->prox;
    }
    if (atual == fila->tail) { 
      fila->tail = anterior;
    }
    free(atual);
    fila->qtde--;

    printf("Desfez enfileiramento para o paciente %s.\n",
           operacao.paciente.nome);
  } else if (operacao.tipo == DESENFILEIRAR) {

    EFila *novo = (EFila *)malloc(sizeof(EFila));
    novo->paciente = operacao.paciente;
    novo->prox = fila->head;
    fila->head = novo;
    if (fila->tail == NULL) {
      fila->tail = novo;
    }
    fila->qtde++;

    printf("Desfez desenfileiramento para o paciente %s.\n",
           operacao.paciente.nome);
  }
}

int salvararquivo(ListaPacientes *lista) {
  FILE *arqpacientes = fopen("arqpacientes", "wb");
  if (arqpacientes == NULL) {
    return 1;
  }
  fwrite(lista, sizeof(ListaPacientes), MAX_PACIENTES, arqpacientes);
  fclose(arqpacientes);
  return 0;
}
int lerarquivo(ListaPacientes *lista,ABB *arvore) {
  FILE *arqpacientes = fopen("arqpacientes", "rb");
  if (arqpacientes == NULL) {
    return 1;
  }
  fread(lista, sizeof(ListaPacientes), MAX_PACIENTES, arqpacientes);
  fclose(arqpacientes);
  for (int i = 0; i < lista->quantidade; i++) {
      inserir_na_arvore(arvore, lista->lista[i]);
  }
  return 0;
}
void menu() {
  printf("-----------MENU-----------\n");
  printf("1. Cadastrar Paciente\n");
  printf("2. Atendimento\n");
  printf("3. Pesquisar\n");
  printf("4. Desfazer Atendimento\n");
  printf("5. Carregar\n");
  printf("6. Salvar\n");
  printf("7. Sobre os Desenvolvedores\n");
  printf("8. Sair\n");
}
