#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

// -------------------------
// STRUCT DO REGISTRO
// -------------------------
typedef struct {
    char nome[30];
    char genero[20];
    char plataforma[20];
    char status;    // 'A' = ativo, 'X' = excluído
    float preco;
    int ano;
} reg;

// -------------------------
// FUNÇÃO: devolve quantidade de registros
// -------------------------
int tamanho(FILE *arq) {
    fseek(arq, 0, SEEK_END);
    long bytes = ftell(arq);
    return bytes / sizeof(reg);
}

// -------------------------
// INSERIR
// -------------------------
void inserir(FILE *arq) {

    reg jogo;
    jogo.status = 'A'; // ativo

    cout << "\n=== Cadastrar Jogo ===\n";

    cin.ignore(1000, '\n');

    cout << "Nome: ";
    fgets(jogo.nome, 30, stdin);
    jogo.nome[strcspn(jogo.nome, "\n")] = '\0';

    cout << "Genero: ";
    fgets(jogo.genero, 20, stdin);
    jogo.genero[strcspn(jogo.genero, "\n")] = '\0';

    cout << "Plataforma: ";
    fgets(jogo.plataforma, 20, stdin);
    jogo.plataforma[strcspn(jogo.plataforma, "\n")] = '\0';

    cout << "Preco: ";
    cin >> jogo.preco;

    cout << "Ano: ";
    cin >> jogo.ano;

    // grava no final do arquivo
    fseek(arq, 0, SEEK_END);
    fwrite(&jogo, sizeof(reg), 1, arq);

    cout << "\nJogo cadastrado com sucesso!\n";
}

// -------------------------
// CONSULTAR CATALOGO
// -------------------------
void consultarCatalogo(FILE *arq) {

    reg jogo;
    int tam = tamanho(arq);

    if (tam == 0) {
        cout << "\nNenhum jogo cadastrado!\n";
        return;
    }

    cout << "\n=== Catalogo de Jogos ===\n";

    fseek(arq, 0, SEEK_SET);

    for (int i = 0; i < tam; i++) {
        fread(&jogo, sizeof(reg), 1, arq);

        if (jogo.status != 'A') continue;

        cout << "\nRegistro #" << i + 1;
        cout << "\nNome: " << jogo.nome;
        cout << "\nGenero: " << jogo.genero;
        cout << "\nPlataforma: " << jogo.plataforma;
        cout << "\nPreco: " << jogo.preco;
        cout << "\nAno: " << jogo.ano << "\n";
    }
}

// -------------------------
// CONSULTAR JOGO POR NOME
// -------------------------
void consultarJogo(FILE *arq) {

    cin.ignore(1000, '\n');

    char nomeBusca[30];
    reg jogo;
    bool encontrado = false;

    cout << "\nDigite o nome do jogo: ";
    fgets(nomeBusca, 30, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    fseek(arq, 0, SEEK_SET);

    while (fread(&jogo, sizeof(reg), 1, arq) == 1) {

        if (jogo.status == 'A' && strcmp(jogo.nome, nomeBusca) == 0) {

            cout << "\n=== Jogo encontrado ===\n";
            cout << "Nome: " << jogo.nome << endl;
            cout << "Genero: " << jogo.genero << endl;
            cout << "Plataforma: " << jogo.plataforma << endl;
            cout << "Preco: " << jogo.preco << endl;
            cout << "Ano: " << jogo.ano << endl;

            encontrado = true;
            break;
        }
    }

    if (!encontrado)
        cout << "\nJogo nao encontrado!\n";
}

// -------------------------
// ATUALIZAR REGISTRO
// -------------------------
void atualizar(FILE *arq) {

    int id;
    reg jogo;

    cout << "\nDigite o número do registro para atualizar: ";
    cin >> id;

    int tam = tamanho(arq);

    if (id < 1 || id > tam) {
        cout << "Registro invalido!\n";
        return;
    }

    fseek(arq, (id - 1) * sizeof(reg), SEEK_SET);
    fread(&jogo, sizeof(reg), 1, arq);

    if (jogo.status != 'A') {
        cout << "Registro excluido, não pode atualizar!";
        return;
    }

    cin.ignore(1000, '\n');

    cout << "\nNovo nome: ";
    fgets(jogo.nome, 30, stdin);
    jogo.nome[strcspn(jogo.nome, "\n")] = '\0';

    cout << "Novo genero: ";
    fgets(jogo.genero, 20, stdin);
    jogo.genero[strcspn(jogo.genero, "\n")] = '\0';

    cout << "Nova plataforma: ";
    fgets(jogo.plataforma, 20, stdin);
    jogo.plataforma[strcspn(jogo.plataforma, "\n")] = '\0';

    cout << "Novo preco: ";
    cin >> jogo.preco;

    cout << "Novo ano: ";
    cin >> jogo.ano;

    fseek(arq, (id - 1) * sizeof(reg), SEEK_SET);
    fwrite(&jogo, sizeof(reg), 1, arq);

    cout << "\nRegistro atualizado com sucesso!\n";
}

// -------------------------
// EXCLUIR (marcação lógica)
// -------------------------
void excluir(FILE *arq) {

    int id;
    reg jogo;

    cout << "\nDigite o número do registro para excluir: ";
    cin >> id;

    int tam = tamanho(arq);

    if (id < 1 || id > tam) {
        cout << "Registro invalido!\n";
        return;
    }

    fseek(arq, (id - 1) * sizeof(reg), SEEK_SET);
    fread(&jogo, sizeof(reg), 1, arq);

    if (jogo.status != 'A') {
        cout << "Registro ja esta excluido!";
        return;
    }

    jogo.status = 'X';

    fseek(arq, (id - 1) * sizeof(reg), SEEK_SET);
    fwrite(&jogo, sizeof(reg), 1, arq);

    cout << "\nRegistro excluido com sucesso!\n";
}

// -------------------------
// MAIN
// -------------------------
int main() {

    FILE *arq = fopen("c:\\Ling_C\\catalogo.dat", "rb+");

    if (arq == NULL) {
        arq = fopen("c:\\Ling_C\\catalogo.dat", "wb+");
        if (arq == NULL) {
            cout << "Falha ao abrir o arquivo!";
            return 1;
        }
    }

    int op;
    do {
        cout << "\n---------- Menu de opcoes ----------\n";
        cout << "1. Consultar Catalogo\n";
        cout << "2. Consultar Jogo\n";
        cout << "3. Inserir jogo\n";
        cout << "4. Atualizar jogo\n";
        cout << "5. Excluir jogo\n";
        cout << "6. Sair\n";
        cout << "------------------------------------\n";
        cout << "Itens: " << tamanho(arq) << "\n";
        cout << "Digite a opcao: ";

        cin >> op;

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
                cout << "Fechando...\n";
                break;

            default:
                cout << "Opcao invalida!\n";
                break;
        }

    } while (op != 6);

    fclose(arq);

    return 0;
}
