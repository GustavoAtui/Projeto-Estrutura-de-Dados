#ifndef FUNCOES_H
#define FUNCOES_H

#define MAX_PACIENTES 10



typedef struct {
    int dia;
    int mes;
    int ano;
} Data;


typedef struct Paciente {
    char nome[50];
    int idade;
    char rg[15];
    Data data_registro;
} Paciente;

typedef struct EABB {
    Paciente dados; 
    struct EABB *filhoEsq;
    struct EABB *filhoDir; 
} EABB;


typedef struct {
    EABB *raiz;
    int qtde;
} ABB;


typedef struct {
    Paciente lista[MAX_PACIENTES];
    int quantidade;
} ListaPacientes;


typedef struct EFila {
    Paciente paciente;
    struct EFila *prox;
} EFila;


typedef struct {
    EFila *head; 
    EFila *tail;
    int qtde;   
} FilaAtendimento;

typedef enum {
    ENFILEIRAR,
    DESENFILEIRAR
} TipoOperacao;


typedef struct {
    TipoOperacao tipo;   
    Paciente paciente;    
} Operacao;


typedef struct NoPilha {
    Operacao operacao;
    struct NoPilha *prox;
} NoPilha;



int lerarquivo(ListaPacientes *lista,ABB *arvore);
int salvararquivo(ListaPacientes *lista);
void cadastrar_paciente(ListaPacientes *lista, ABB *arvore);
void atendimento(ListaPacientes *lista, FilaAtendimento *fila, NoPilha **topo);
void menu(); 
void inserir_na_arvore(ABB *arvore, Paciente paciente);
void listar_por_ano(EABB *raiz, int ano);
void listar_por_mes(EABB *raiz, int mes);
void listar_por_dia(EABB *raiz, int dia);
void listar_por_idade(EABB *raiz, int idade);
EABB* remover_na_arvore(EABB *raiz, const char *rg);
void push_operacao(NoPilha **topo, TipoOperacao tipo, Paciente paciente);
Operacao pop_operacao(NoPilha **topo, int *sucesso);
void desfazer_ultima_operacao(FilaAtendimento *fila, NoPilha **topo);

#endif