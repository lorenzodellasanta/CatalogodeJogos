#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -------------------------
// STRUCT DO REGISTRO
// -------------------------
typedef struct {
    char nome[30];
    char genero[20];
    char plataforma[20];
    char status;    // 'A' ativo, 'X' excluído
    float preco;
    int ano;
} reg;

// -------------------------
// LIMPAR BUFFER
// -------------------------
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// -------------------------
// TAMANHO DO ARQUIVO
// -------------------------
int tamanho(FILE *arq) {
    fseek(arq, 0, SEEK_END);
    long bytes = ftell(arq);
    return bytes / sizeof(reg);
}

// -------------------------
// INSERIR REGISTRO
// -------------------------
void inserir(FILE *arq) {
    reg jogo;
    jogo.status = 'A';

    printf("\n=== Cadastrar Jogo ===\n");
    limparBuffer();

    printf("Nome: ");
    fgets(jogo.nome, 30, stdin);
    jogo.nome[strcspn(jogo.nome, "\n")] = '\0';

    printf("Genero: ");
    fgets(jogo.genero, 20, stdin);
    jogo.genero[strcspn(jogo.genero, "\n")] = '\0';

    printf("Plataforma: ");
    fgets(jogo.plataforma, 20, stdin);
    jogo.plataforma[strcspn(jogo.plataforma, "\n")] = '\0';

    printf("Preco: ");
    scanf("%f", &jogo.preco);

    printf("Ano: ");
    scanf("%d", &jogo.ano);

    fseek(arq, 0, SEEK_END);
    fwrite(&jogo, sizeof(reg), 1, arq);

    printf("\nJogo cadastrado com sucesso!\n");
}

// -------------------------
// CONSULTAR CATALOGO
// -------------------------
void consultarCatalogo(FILE *arq) {
    reg jogo;
    int tam = tamanho(arq);
    int i;

    if (tam == 0) {
        printf("\nNenhum jogo cadastrado.\n");
        return;
    }

    printf("\n=== Catalogo de Jogos ===\n");

    fseek(arq, 0, SEEK_SET);

    for(i = 0; i < tam; i++) {
        fread(&jogo, sizeof(reg), 1, arq);

        if (jogo.status != 'A')
            continue;

        printf("\nRegistro #%d\n", i + 1);
        printf("Nome: %s\n", jogo.nome);
        printf("Genero: %s\n", jogo.genero);
        printf("Plataforma: %s\n", jogo.plataforma);
        printf("Preco: %.2f\n", jogo.preco);
        printf("Ano: %d\n", jogo.ano);
    }
}

// -------------------------
// CONSULTAR POR NOME
// -------------------------
void consultarJogo(FILE *arq) {
    reg jogo;
    char nomeBusca[30];
    int encontrado = 0;

    limparBuffer();
    printf("\nDigite o nome do jogo: ");
    fgets(nomeBusca, 30, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    fseek(arq, 0, SEEK_SET);

    while (fread(&jogo, sizeof(reg), 1, arq) == 1) {
        if (jogo.status == 'A' && strcmp(jogo.nome, nomeBusca) == 0) {
            printf("\n=== Jogo encontrado ===\n");
            printf("Nome: %s\n", jogo.nome);
            printf("Genero: %s\n", jogo.genero);
            printf("Plataforma: %s\n", jogo.plataforma);
            printf("Preco: %.2f\n", jogo.preco);
            printf("Ano: %d\n", jogo.ano);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado)
        printf("\nJogo nao encontrado!\n");
}

// -------------------------
// ATUALIZAR REGISTRO
// -------------------------
void atualizar(FILE *arq) {
    int id;
    reg jogo;

    printf("\nDigite o número do registro que deseja atualizar: ");
    scanf("%d", &id);

    int tam = tamanho(arq);

    if (id < 1 || id > tam) {
        printf("Registro invalido!\n");
        return;
    }

    fseek(arq, (id - 1) * sizeof(reg), SEEK_SET);
    fread(&jogo, sizeof(reg), 1, arq);

    if (jogo.status != 'A') {
        printf("Registro excluído. Não é possível atualizar.\n");
        return;
    }

    limparBuffer();

    printf("\nNovo nome: ");
    fgets(jogo.nome, 30, stdin);
    jogo.nome[strcspn(jogo.nome, "\n")] = '\0';

    printf("Novo genero: ");
    fgets(jogo.genero, 20, stdin);
    jogo.genero[strcspn(jogo.genero, "\n")] = '\0';

    printf("Nova plataforma: ");
    fgets(jogo.plataforma, 20, stdin);
    jogo.plataforma[strcspn(jogo.plataforma, "\n")] = '\0';

    printf("Novo preco: ");
    scanf("%f", &jogo.preco);

    printf("Novo ano: ");
    scanf("%d", &jogo.ano);

    fseek(arq, (id - 1) * sizeof(reg), SEEK_SET);
    fwrite(&jogo, sizeof(reg), 1, arq);

    printf("\nRegistro atualizado com sucesso!\n");
}

// -------------------------
// EXCLUSAO LOGICA
// -------------------------
void excluir(FILE *arq) {
    int id;
    reg jogo;

    printf("\nDigite o número do registro que deseja excluir: ");
    scanf("%d", &id);

    int tam = tamanho(arq);

    if (id < 1 || id > tam) {
        printf("Registro invalido!\n");
        return;
    }

    fseek(arq, (id - 1) * sizeof(reg), SEEK_SET);
    fread(&jogo, sizeof(reg), 1, arq);

    if (jogo.status == 'X') {
        printf("\nRegistro já está excluído!\n");
        return;
    }

    jogo.status = 'X';

    fseek(arq, (id - 1) * sizeof(reg), SEEK_SET);
    fwrite(&jogo, sizeof(reg), 1, arq);

    printf("\nRegistro excluído com sucesso!\n");
}

// -------------------------
// GERA RELATORIO
// -------------------------
void exportarTxt(FILE *arq) {
    FILE *txt = fopen("C:\\Ling_C\\catalogo.txt", "w");
    reg jogo;

    if (txt == NULL) {
        printf("Erro ao criar catalogo.txt!\n");
        return;
    }

    fseek(arq, 0, SEEK_SET);

    fprintf(txt, "===== CATALOGO DE JOGOS =====\n\n");

    int count = 0;

    while (fread(&jogo, sizeof(reg), 1, arq) == 1) {
            fprintf(txt, "Nome: %s\n",       jogo.nome);
            fprintf(txt, "Genero: %s\n",     jogo.genero);
            fprintf(txt, "Plataforma: %s\n", jogo.plataforma);
            fprintf(txt, "Preco: %.2f\n",    jogo.preco);
            fprintf(txt, "Ano: %d\n",        jogo.ano);
            fprintf(txt, "Status: %c\n", 		 jogo.status);
            fprintf(txt, "-----------------------------\n");
            count++;
    }

    fclose(txt);

    printf("\nArquivo catalogo.txt criado com sucesso! (%d jogos exportados)\n", count);
}

// -------------------------
// MAIN
// -------------------------
int main() {
    FILE *arq = fopen("c:\\Ling_C\\catalogo.dat", "rb+");

    if (arq == NULL) {
        arq = fopen("c:\\Ling_C\\catalogo.dat", "wb+");
        if (arq == NULL) {
            printf("Erro ao criar o arquivo!");
            return 1;
        }
    }

    int op;

    do {
        printf("\n---------- Menu de opcoes ----------\n");
        printf("1. Consultar Catalogo\n");
        printf("2. Consultar Jogo\n");
        printf("3. Inserir Jogo\n");
        printf("4. Atualizar Jogo\n");
        printf("5. Excluir Jogo\n");
        printf("6. Exportar catálogo legivel\n");
        printf("7. Sair\n");
        printf("------------------------------------\n");
        printf("Itens no arquivo: %d\n", tamanho(arq));
        printf("Digite a opcao: ");
        scanf("%d", &op);

        switch (op) {
        case 1:
            consultarCatalogo(arq);
            break;
        case 2:
            consultarJogo(arq);
            break;
        case 3:
            inserir(arq);
            break;
        case 4:
            atualizar(arq);
            break;
        case 5:
            excluir(arq);
            break;
            
        case 6:
    		exportarTxt(arq);
    		break;

        case 7:
            printf("Fechando...\n");
            break;
            
        default:
            printf("Opcao invalida!\n");
        }

    } while (op != 7);

    fclose(arq);
    return 0;
}
