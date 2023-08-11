#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct Estudante
{
    int id;
    char nome[100];
    int dia, mes, ano;
    char nacionalidade[50];
} estudante;

typedef struct Nmats
{
    int id;
    int nMateria;
    float ects;
} nmats;

int adicionarEstudante(estudante **e, nmats **nm, int quant)
{
    if (quant)
    {
        estudante *novo = malloc(sizeof(estudante));
        nmats *cad = malloc(sizeof(nmats));

        if (novo == NULL || cad == NULL)
        {
            printf("\nErro na alocação de memória. Não foi possível realizar o cadastro.\n");
            return 0;
        }

        printf("\ndigite o codigo do estudante: ");
        scanf("%d", &novo->id);
        getchar();

        printf("\ndigite o nome do estudante: ");
        scanf(" %99[^\n]", novo->nome); // Adicionado espaço no formato para consumir nova linha
        getchar();

        printf("\ndigite a data de nascimento do estudante (dd-mm-aaaa): "); // Formato esperado
        scanf("%d-%d-%d", &novo->dia, &novo->mes, &novo->ano);
        getchar();

        printf("\ndigite a nacionalidade do estudante: ");
        scanf(" %49[^\n]", novo->nacionalidade); // Adicionado espaço no formato para consumir nova linha
        getchar();

        printf("\ndigite o numero de cadeiras inscritas do estudante: ");
        scanf("%d", &cad->nMateria);
        getchar();

        printf("\ndigite o total de creditos do estudante: ");
        scanf("%f", &cad->ects);
        getchar();

        *e = realloc(*e, sizeof(estudante) * (quant + 1));
        *nm = realloc(*nm, sizeof(nmats) * (quant + 1));
        (*e)[quant] = *novo;
        (*nm)[quant] = *cad;

        return 1;
    }
    else
    {
        printf("\nNão foi possível realizar o cadastro.\n");
        return 0;
    }
}


int lerArquivoDados(estudante **e, nmats **nm, char f[], char f2[])
{
    FILE *file = fopen(f, "r");
    FILE *fileMat = fopen(f2, "r");
    int quant = 0, i;

    // Verificando se os arquivos foram abertos corretamente
    if (file == NULL)
    {
        printf("Nao foi possivel ler o arquivo %s!\n", f);
        return 0;
    }

    if (fileMat == NULL)
    {
        printf("Nao foi possivel ler o arquivo %s!\n", f2);
        fclose(file);
        return 0;
    }

    // Contar o número de linhas para obter a quantidade de registros
    char c;
    while ((c = fgetc(file)) != EOF)
    {
        if (c == '\n')
        {
            quant++;
        }
    }
    // Voltar ao início do arquivo
    rewind(file);
    *e = realloc(*e, sizeof(estudante) * quant);
    if (*e == NULL)
    {
        printf("Erro na realocação de memória.\n");
        fclose(file);
        fclose(fileMat);
        return 0;
    }


    for (i = 0; i < quant; i++)
    {
        if (fscanf(file, "%d ", &(*e)[i].id) != 1 ||
                fscanf(file, "%99[^\t] ", (*e)[i].nome) != 1 ||
                fscanf(file, "%d-%d-%d", &(*e)[i].dia, &(*e)[i].mes, &(*e)[i].ano) != 3 ||
                fscanf(file, " %49[^\n]", (*e)[i].nacionalidade) != 1)
        {
            printf("Erro na leitura do arquivo %s! Linha %d\n", f, i + 1);
            fclose(file);
            fclose(fileMat);
            return 0;
        }

        // Limpar o buffer de entrada, consumindo o caractere de nova linha
        int c;
        while ((c = fgetc(file)) != '\n' && c != EOF);
    }
    fclose(file);

    // segundo arquivo:
    *nm = (nmats *)malloc(sizeof(nmats) * quant);

    for (i = 0; i < quant; i++)
    {
        if (fscanf(fileMat, "%d", &(*nm)[i].id) != 1 ||
                fscanf(fileMat, " %d", &(*nm)[i].nMateria) != 1 ||
                fscanf(fileMat, "%f", &(*nm)[i].ects) != 1)
        {
            printf("Erro na leitura do arquivo %s! Linha %d\n", f2, i + 1);
            fclose(fileMat);
            return 0;
        }

        // Limpar o buffer de entrada, consumindo o caractere de nova linha
        int c;
        while ((c = fgetc(fileMat)) != '\n' && c != EOF);
    }
    fclose(fileMat);

    return quant;
}

float calcMediaMatriculas(nmats *nm, int quant)
{
    float somaMatriculas = 0.0;
    if (quant == 0)
    {
        printf("\nNão há alunos cadastrados.\n");
        return 0.0;
    }
    for (int i = 0; i < quant; i++)
    {
        somaMatriculas += nm[i].nMateria;
    }
    printf("\ntotal de matriculas: %.2f\n", somaMatriculas);
    return somaMatriculas/quant;

}

void imprimirArquivoDados(estudante *e, nmats *nm, int quant)
{
    int i;
    if (quant > 0)
    {
        printf("\n\tCodigo\tNome\t\tData Nasc\tNacionalidade\tN_matriculas\tECTS\n");
        printf("\t-----------------------------------------------\n");
        for (i = 0; i < quant; i++)
        {
            printf("\t%4d\t%-20s\t%2d-%2d-%4d\t%s\t%d\t%.2f\n",
                   e[i].id, e[i].nome, e[i].dia, e[i].mes, e[i].ano, e[i].nacionalidade, nm[i].nMateria, nm[i].ects );
        }
    }
    else
    {
        printf("Nao ha dados!\n");
    }
}

void verFinalistas(estudante *e, nmats *nm, int quant)
{
    int i,encontrados = 0;
    if (quant > 0)
    {
        printf("\n\tCodigo\tNome\t\tData Nasc\tNacionalidade\tCadeiras\tECTS\n");
        printf("\t-----------------------------------------------\n");
        for (i = 0; i < quant; i++)
        {
            if(nm[i].ects>=154)
            {

                printf("\t%4d\t%-20s\t%2d-%2d-%4d\t%s\t%d\t%.2f\n",
                       e[i].id, e[i].nome, e[i].dia, e[i].mes, e[i].ano, e[i].nacionalidade, nm[i].nMateria, nm[i].ects );
                encontrados = 1;
            }
        }
        if(!encontrados)
        {
            printf("\n\tNAO HA FINALISTAS!!!\n");
        }
    }
    else
    {
        printf("Nao ha dados!\n");
    }
}


void contarEstudantesEmRisco(estudante *e, nmats *nm, int quant)
{
    int estudantesEmRisco = 0;

    if(quant > 0)
    {
        for (int i = 0; i < quant; i++)
        {

            if ((nm[i].nMateria <= 3 && nm[i].ects < 60) ||
                    (nm[i].nMateria <= 4 && nm[i].ects < 120) ||
                    (nm[i].nMateria > 5 && nm[i].ects <= 154))
            {
                printf("\t%4d\t%-20s\t%2d-%2d-%4d\t%s\t%d\t%.2f\n",
                       e[i].id, e[i].nome, e[i].dia, e[i].mes, e[i].ano, e[i].nacionalidade, nm[i].nMateria, nm[i].ects );
                estudantesEmRisco = 1;

            }
        }
        if(!estudantesEmRisco)
        {
            printf("\n\tNAO HA ESTUDANTES EM RISCO\n");
        }
    }
    else
    {
        printf("\n\tnao ha dados\n");
    }



}


void salvar(estudante **e, nmats **nm, int quant, char arq[], char arq2[])
{
    FILE *fileDados = fopen(arq, "w");
    FILE *fileMats = fopen(arq2, "w");
    int i;

    if (fileDados && fileMats)
    {
        for (i = 0; i < quant; i++)
        {
            fprintf(fileDados, "%d\t%s\t%d-%d-%d\t%s\n",
                    (*e)[i].id, (*e)[i].nome, (*e)[i].dia, (*e)[i].mes, (*e)[i].ano, (*e)[i].nacionalidade);

            fprintf(fileMats, "%d\t%d\t%.2f\n", (*e)[i].id, (*nm)[i].nMateria, (*nm)[i].ects);
        }

        fclose(fileDados); // Fechar o arquivo de dados
        fclose(fileMats);  // Fechar o arquivo de matrículas
    }
    else
    {
        printf("Erro ao abrir arquivos para escrita.\n");
    }
}

void removerAluno(estudante **e, nmats **nm, int *quant)
{
    int id = 0, encontrou = 0, posicao = 0;
    printf("\ndigite a id do estudante que deseja remover: ");
    scanf("%d", &id);

    for(int i = 0; i< *quant; i++)
    {
        if((*e)[i].id == id)
        {
            encontrou = 1;
            posicao = i;
            break;
        }
    }
    if(encontrou)
    {
        for( int i = posicao; i < *quant - 1; i++)
        {
            (*e)[i] = (*e)[i+1];
            (*nm)[i] = (*nm)[i+1];
        }
        printf("\nestudante %d removido com sucesso!\n", id);
        *quant -= 1;

    }
    else
    {
        printf("\nEstudante com a ID %d não encontrado.\n", id);
    }

}

void procurarNome(estudante **e, nmats **nm, int quant){

}

int main()
{
    estudante *e = NULL;
    nmats *nm = NULL;
    char arquivodados[] = "dados.txt";
    char arquivodados2[] = "cod nMat ECTS.txt";
    int quant = 0, opcao, remove = 0;
    float mediaMatriculas = 0.00;

    do
    {
        printf("\nEscolha uma opcao:\n1. Ler arquivo de dados\n2. Cadastrar aluno\n3. Imprimir dados\n4. Salvar arquivo\n5. Numero medio de matriculas\n6. Ver finalistas\n7. Ver estudantes em risco de prescrever\n8. Remover aluno\n0. Sair\n");
        scanf("%d", &opcao);
        getchar(); // Para consumir o caractere de nova linha

        switch (opcao)
        {
        case 1:
            quant = lerArquivoDados(&e, &nm, arquivodados, arquivodados2);
            printf("\nArquivos lidos\n");
            break;
        case 2:
            quant+= adicionarEstudante(&e, &nm, quant);
            break;
        case 3:
            imprimirArquivoDados(e, nm, quant);
            printf("\ntotal de estudantes: %d", quant);
            break;
        case 4:
            salvar(&e, &nm, quant, arquivodados, arquivodados2);
            break;
        case 5:
            mediaMatriculas = calcMediaMatriculas(nm, quant);
            printf("media: %.2f\n", mediaMatriculas);
            break;

        case 6:
            verFinalistas(e, nm, quant);
            break;

        case 7:
            contarEstudantesEmRisco(e, nm, quant);
            break;

        case 8:
            removerAluno(&e, &nm, &quant);
            break;

        case 0:
            printf("Saindo...\n");
            break;
        default:
            printf("Opcao invalida!\n");
            break;
        }
    }
    while (opcao != 0);

    free(nm);
    free(e);
    return 0;
}
