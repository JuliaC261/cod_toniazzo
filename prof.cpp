#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h> //biblioteca para obter dados de tempo (data, hora, etc)

#define ARQCARROS "_carros.txt"
#define ARQLOGS "_logs.txt"
#define ARQTESTE "D:\\aulas\\teste.txt"

FILE *parq; //escopo GLOBAL - visivel em todas as funcoes

//vamos definir uma STRUCT para guardar no arquivo...
typedef struct Carro {
    int cod;
    char nome[50];
    int ano;
    float valor;
    int status; //1 - ativo, 0 - inativo
};

//vamos criar a struct Carro
struct Carro meuCar; //vai ser usada em todo o programa como "auxiliar"

void primeiraExecucao(FILE* arqCarro, FILE* arqLog);

void gravarLog(FILE* arqLog, char msg[]);
void gravarCarro(FILE* arqCarro);
void listarCarros(FILE* arqCarro);
void listarCarrosExcluidos(FILE* arqCarro);
void excluirCarro(FILE* arqCarro, int pos);
void buscarCarros(FILE* arqCarro);
int posicaoRegistro(FILE* arqCarro,char busca[]); //preciso saber aonde está um determinado registro
void menu();


int main (){
    FILE *arqCarro, *arqLog, *arqTeste; //um ponteiro para cada arquivo que pretendo utilizar
    arqTeste = fopen(ARQTESTE,"w");
    int op, posRegistro;
    char busca[20];
    arqCarro = fopen(ARQCARROS,"r+");
    arqLog = fopen(ARQLOGS,"r+");
    //se eu não consegui abrir esses arquivos, significa que eles não existem.
    //Logo, vou criá-los...
    if(arqCarro == NULL || arqLog == NULL){
        primeiraExecucao(arqCarro, arqLog); //separei isso em uma função
        gravarLog(arqLog, "Sistema instalado e configurado");
    }

    //controle de acesso de login
    gravarLog(arqLog, "Usuario acessou o sistema");
    system("pause");
    //menu inicial exibido em Loop até digitar 0 para sair
    do {
        menu();
        scanf("%d",&op);
        switch(op){
            case 0: printf("\nObrigado por utilizar! \n");
                break;
            case 1:
                gravarCarro(arqCarro);
                break;

            case 2:
                listarCarros(arqCarro);
                break;
            case 3:
                printf("DIGITE O NOME EXATO DO CARRO: ");
                fflush(stdin);
                gets(busca);
                posRegistro = posicaoRegistro(arqCarro, busca);
                if(posRegistro == -1){
                  printf("\nRegistro nao encontrado!\n");
                } else {
                   printf("\nRegistro encontrado na posicao: %d\n", posRegistro+1);
                }
                break;

            case 4:
                listarCarros(arqCarro);
                printf("\nDIGITE O NOME EXATO DO CARRO A EXCLUIR: ");
                fflush(stdin);
                gets(busca);
                posRegistro = posicaoRegistro(arqCarro, busca);
                if(posRegistro == -1){
                  printf("\nRegistro nao encontrado para excluir!\n");
                } else {
                   printf("\nRegistro encontrado na posicao: %d\n", posRegistro+1);
                   excluirCarro(arqCarro, posRegistro);
                }

                break;

            case 68:
                listarCarrosExcluidos(arqCarro);
                gravarLog(arqLog, "Acessou o menu secreto");
                break;


            default: printf("\nOpcao invalida! \n");
        }
        getch();
    } while(op != 0);


}

void gravarCarro(FILE* arqCarro){
    printf("\nDigite os dados do carro: ");
    printf("\nNome: ");
    fflush(stdin);
    gets(meuCar.nome);
    printf("\nAno: ");
    scanf("%d",&meuCar.ano);
    printf("\nValor: ");
    scanf("%f",&meuCar.valor);
    meuCar.status = 1; //sempre como ativo
    meuCar.cod = 1000+meuCar.ano;
    //agora vamos mandar a stream de dados contida na struct meuCar para o arquivo
    arqCarro = fopen(ARQCARROS,"a");
    //fwrite(o que gravar, tamanho da variavel, quantas vezes, em qual arquivo)
    fwrite(&meuCar, sizeof(meuCar), 1, arqCarro);
    fclose(arqCarro); //efetivo a gravação do dado no arquivo
    printf("\nCarro inserido com sucesso! \n");

}

void listarCarros(FILE* arqCarro){
    int cont = 0;
    printf("LISTA DE CARROS: \n\n");
    printf("COD NOME \t\t\t | ANO  | VALOR \n");
    arqCarro = fopen(ARQCARROS,"r");

    //vamos fazer um laço para buscar carro a carro no arquivo enquanto não for o fim
    while(fread(&meuCar, sizeof(meuCar), 1, arqCarro)){
        //para cada registro, ler com fread
         if(meuCar.status == 1){
            printf("%d  %d %20s \t | %d   | R$%.2f \n", meuCar.status, meuCar.cod, meuCar.nome, meuCar.ano, meuCar.valor);
            cont++;
         } else {
            //printf("------------- Registro Excluido ------------- \n");
         }
    }
    printf("\n FIM DA LISTA. TOTAL: %d CARROS CADASTRADOS. \n", cont);
    fclose(arqCarro);

}

void listarCarrosExcluidos(FILE* arqCarro){
    int cont = 0;
    printf("LISTA DE CARROS EXCLUIDOS: \n\n");
    printf("COD NOME \t\t\t | ANO  | VALOR \n");
    arqCarro = fopen(ARQCARROS,"r");

    //vamos fazer um laço para buscar carro a carro no arquivo enquanto não for o fim
    while(fread(&meuCar, sizeof(meuCar), 1, arqCarro)){
        //para cada registro, ler com fread
         if(meuCar.status == 0){
            printf("%d  %d %20s \t | %d   | R$%.2f \n", meuCar.status, meuCar.cod, meuCar.nome, meuCar.ano, meuCar.valor);
            cont++;
         }
    }
    printf("\n FIM DA LISTA. TOTAL: %d CARROS EXCLUIDOS. \n", cont);
    fclose(arqCarro);

}

void excluirCarro(FILE* arqCarro, int pos){
   int movimentacao;
   printf("\n EXCLUINDO UM CARRO: \n\n");
   arqCarro = fopen(ARQCARROS,"r+");
   //preciso movimentar o ponteiro para o local que tem o arquivo que eu quero excluir
   //excluir nesse exemplo: mudar o status de ativo (1) para inativo (0)
   movimentacao = pos * sizeof(meuCar);
   fseek(arqCarro, movimentacao, SEEK_SET);
   printf("\nEstou no byte para ler: %d", ftell(arqCarro));
   //leio o que tem nesse lugar
   fread(&meuCar, sizeof(meuCar), 1, arqCarro);
   meuCar.status = 0;
   printf("\nExcluindo carro %s %d - %d \n",meuCar.nome, meuCar.ano, meuCar.status);
   //fclose(arqCarro);
   //arqCarro = fopen(ARQCARROS,"a");
   fseek(arqCarro, movimentacao, SEEK_SET);
   printf("\nEstou no byte para atualizar: %d", ftell(arqCarro));
   fwrite(&meuCar, sizeof(meuCar), 1, arqCarro);
   printf("\nRegistro excluido com sucesso!");
   FILE *arqLog = fopen(ARQLOGS,"r+");
   gravarLog(arqLog, "Excluiu um carro");
   fclose(arqLog);
   fclose(arqCarro);
}


int posicaoRegistro(FILE* arqCarro, char busca[]){
   arqCarro = fopen(ARQCARROS,"r");
   int pos = -1; //variável do tipo flag
   int cont = 0; //variável do tipo contador
   //esse while ou acaba quando chegar ao fim dos registros ou quando achar o registro desejado (executa break)
   while(fread(&meuCar, sizeof(meuCar), 1, arqCarro)){
      if( strcmpi(busca, meuCar.nome) == 0){
         pos = cont; //armazeno qual é a posição lógica do registro no arquivo
         break; //sai fora do LAÇO WHILE
      }
      cont++;
   }
   return pos; //se retornar -1 chegou no fim e não achou o registro. Senão, retorna a posição
}

void primeiraExecucao(FILE* arqCarro, FILE* arqLog){
    printf("\nPrimeira execucao - criando os arquivos do sistema");
    arqCarro = fopen(ARQCARROS,"w");
    arqLog = fopen(ARQLOGS,"w");
    if(arqCarro != NULL && arqLog != NULL){
        fprintf(arqLog, "Primeira execucao - Arquivos criados com sucesso!");
        fclose(arqCarro);
        fclose(arqLog);
    } else perror("Erro na primeira execucao");

}

void gravarLog(FILE* arqLog, char msg[]){
  time_t t = time(NULL); //crio uma estrutura que pega o tempo do PC no momento
  struct tm tm = *localtime(&t); //tm quebra o tempo em um formato intelegível
  arqLog = fopen(ARQLOGS,"a");
  fprintf(arqLog,"\n[%d-%02d-%02d %02d:%02d:%02d]",  tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
  fprintf(arqLog," %s", msg);
  fclose(arqLog);
}

void menu(){
    system("cls");
    printf("\tSISTEMA DE CADASTRO DE CARROS 1.0 \n\n");
    printf("\n1 - Gravar um carro");
    printf("\n2 - Listar carros");
    printf("\n3 - Buscar um carro"); //tentar fazer
    printf("\n4 - Excluir um carro"); //tentar fazer
    printf("\n5 - Ver Logs"); //tentar fazer
    printf("\n0 - Sair");
    printf("\nsua opcao: ");
}
