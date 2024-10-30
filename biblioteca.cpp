#include <iostream>
#include <cstdio>
#include <cstring>
#include <locale>

using namespace std;

struct Emprestimos {
    char dt_emp[10], dt_dev[10], usuario[255];
};

struct Livro {
    int codigo, paginas;
    char area[30], titulo[255], autores[255], editora[50];
    Emprestimos emp;
};

void cadastrarLivro(Livro &livro) {
    cout << "Código: ";
    cin >> livro.codigo;
    cout << "Área: ";
    cin >> livro.area;
    cout << "Título: ";
    cin >> livro.titulo;
    cout << "Autor(es): ";
    cin >> livro.autores;
    cout << "Editora: ";
    cin >> livro.editora;
    cout << "Nº páginas: ";
    cin >> livro.paginas;
}

void manipularArquivo(FILE *&arquivo, const char *modo) {
    arquivo = fopen("dados.dat", modo);
    if (!arquivo) {
        arquivo = fopen("dados.dat", "wb");
    }
}

void listarLivros(FILE *arquivo, bool apenasDisponiveis = false) {
    Livro livro;
    while (fread(&livro, sizeof(Livro), 1, arquivo) == 1) {
        if (!apenasDisponiveis || strcmp(livro.emp.dt_emp, "") == 0) {
            cout << "Código: " << livro.codigo << endl
                 << "Área: " << livro.area << endl
                 << "Título: " << livro.titulo << endl
                 << "Autor(es): " << livro.autores << endl
                 << "Editora: " << livro.editora << endl
                 << "Nº páginas: " << livro.paginas << endl
                 << "---------------------------------------" << endl;
        }
    }
}

int main() {
    int opc_princ;
    char opc;
    FILE *arquivo;

    do {
        cout << "1 - Cadastro" << endl;
        cout << "2 - Alteração" << endl;
        cout << "3 - Exclusão" << endl;
        cout << "4 - Empréstimo" << endl;
        cout << "5 - Devolução" << endl;
        cout << "6 - Consulta de livro" << endl;
        cout << "7 - Livros disponíveis" << endl;
        cout << "8 - Listagem geral de livros" << endl;
        cout << "9 - Sair" << endl << endl;
        cout << "Digite a opção desejada: ";
        cin >> opc_princ;

        switch (opc_princ) {
            case 1: // Cadastro
                do {
                    cout << "Deseja cadastrar um livro (S ou N)? ";
                    cin >> opc;
                    if (opc == 'S') {
                        Livro livro;
                        cadastrarLivro(livro);
                        manipularArquivo(arquivo, "ab");
                        fwrite(&livro, sizeof(Livro), 1, arquivo);
                        cout << "Livro cadastrado com sucesso!" << endl;
                        fclose(arquivo);
                    }
                } while (opc == 'S');
                break;

            case 2: // Alteração
                manipularArquivo(arquivo, "rb+");
                int codAlterar;
                cout << "Digite o código do livro que deseja alterar: ";
                cin >> codAlterar;
                Livro livro;
                while (fread(&livro, sizeof(Livro), 1, arquivo) == 1) {
                    if (livro.codigo == codAlterar) {
                        cadastrarLivro(livro);
                        fseek(arquivo, -sizeof(Livro), SEEK_CUR);
                        fwrite(&livro, sizeof(Livro), 1, arquivo);
                        cout << "Livro alterado com sucesso!" << endl;
                        break;
                    }
                }
                fclose(arquivo);
                break;

            case 3: // Exclusão
                cout << "Digite o código do livro que deseja excluir: ";
                cin >> codAlterar;
                manipularArquivo(arquivo, "rb");
                FILE *arquivoAux = fopen("dados.aux", "wb");
                while (fread(&livro, sizeof(Livro), 1, arquivo) == 1) {
                    if (livro.codigo != codAlterar) {
                        fwrite(&livro, sizeof(Livro), 1, arquivoAux);
                    }
                }
                fclose(arquivo);
                fclose(arquivoAux);
                remove("dados.dat");
                rename("dados.aux", "dados.dat");
                cout << "Livro excluído com sucesso!" << endl;
                break;

            case 4: // Empréstimo
                manipularArquivo(arquivo, "rb+");
                cout << "Digite o código do livro que deseja emprestar: ";
                cin >> codAlterar;
                while (fread(&livro, sizeof(Livro), 1, arquivo) == 1) {
                    if (livro.codigo == codAlterar) {
                        fseek(arquivo, -sizeof(Livro), SEEK_CUR);
                        cout << "Data de empréstimo: ";
                        cin >> livro.emp.dt_emp;
                        cout << "Data de devolução: ";
                        cin >> livro.emp.dt_dev;
                        cout << "Usuário: ";
                        cin >> livro.emp.usuario;
                        fwrite(&livro, sizeof(Livro), 1, arquivo);
                        cout << "Empréstimo registrado!" << endl;
                        break;
                    }
                }
                fclose(arquivo);
                break;

            case 5: // Devolução
                manipularArquivo(arquivo, "rb+");
                cout << "Digite o código do livro que deseja devolver: ";
                cin >> codAlterar;
                while (fread(&livro, sizeof(Livro), 1, arquivo) == 1) {
                    if (livro.codigo == codAlterar) {
                        fseek(arquivo, -sizeof(Livro), SEEK_CUR);
                        strcpy(livro.emp.dt_emp, "");
                        strcpy(livro.emp.dt_dev, "");
                        strcpy(livro.emp.usuario, "");
                        fwrite(&livro, sizeof(Livro), 1, arquivo);
                        cout << "Devolução registrada com sucesso!" << endl;
                        break;
                    }
                }
                fclose(arquivo);
                break;

            case 6: // Consulta de livro
                manipularArquivo(arquivo, "rb");
                cout << "Digite o código do livro que deseja pesquisar: ";
                cin >> codAlterar;
                while (fread(&livro, sizeof(Livro), 1, arquivo) == 1) {
                    if (livro.codigo == codAlterar) {
                        cout << "Código: " << livro.codigo << endl
                             << "Área: " << livro.area << endl
                             << "Título: " << livro.titulo << endl
                             << "Autor(es): " << livro.autores << endl
                             << "Editora: " << livro.editora << endl
                             << "Nº páginas: " << livro.paginas << endl;
                        break;
                    }
                }
                fclose(arquivo);
                break;

            case 7: // Livros disponíveis
                manipularArquivo(arquivo, "rb");
                listarLivros(arquivo, true);
                fclose(arquivo);
                break;

            case 8: // Listagem geral de livros
                manipularArquivo(arquivo, "rb");
                listarLivros(arquivo);
                fclose(arquivo);
                break;

            case 9: // Sair
                cout << "Obrigado por usar nossa solução!" << endl;
                break;

            default:
                cout << "Informe uma opção válida!" << endl;
                break;
        }

        cout << "\e[2J" << "\e[0;0H"; // Limpa a tela
    } while (opc_princ != 9);

    return 0;
}



















