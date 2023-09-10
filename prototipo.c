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
//essa funcao adiciona estudantes, usando a struct estudante e nmats com ponteiro de ponteiro.
int adicionarEstudante(estudante **e, nmats **nm, int quant)
{
    char partedonome[100];
    if (quant)
    {
        //alocacao dinamica de memoria.
        estudante *novo = malloc(sizeof(estudante));
        nmats *cad = malloc(sizeof(nmats));

        if (novo == NULL || cad == NULL)
        {
            printf("\nErro na alocacao de memoria. Nao foi possível realizar o cadastro.\n");
            return 0;
        }
        char input[50];

        do
        {

            printf("\ndigite o codigo do estudante: ");
            fgets(input, sizeof(input), stdin); // Lê a entrada como uma string
            if (sscanf(input, "%d", &novo->id) != 1 || novo->id <= 0) //aqui eu coloco o sscanf para ser verificado e se for diferente de um mumero inteiro da invalido.
            {
                printf("\nCodigo invalido. Por favor, digite um numero inteiro positivo.\n");
            }

        }
        while(sscanf(input, "%d", &novo->id) != 1 || novo->id <= 0 );  //verificando se os dados estao corretos!

        printf("\ndigite o nome do estudante: ");
        scanf(" %99[^\n]", novo->nome); // Adicionado espaço no formato para consumir nova linha
        novo->nome[0] = toupper(novo->nome[0]);
        getchar();

        do
        {
            printf("\nDigite a data de nascimento do estudante (dd-mm-aaaa): ");
            if (scanf("%d-%d-%d", &novo->dia, &novo->mes, &novo->ano) != 3 ||
                    novo->dia < 1 || novo->dia > 31 ||
                    novo->mes < 1 || novo->mes > 12 ||
                    novo->ano < 1900 || novo->ano > 2007)
            {
                printf("Formato de data invalido. Use o formato dd-mm-aaaa.\n");
                while (getchar() != '\n'); // Limpa o buffer de entrada
            }
        }
        while (novo->dia < 1 || novo->dia > 31 ||
                novo->mes < 1 || novo->mes > 12 ||
                novo->ano < 1900 || novo->ano > 2007);

        printf("\ndigite a nacionalidade do estudante: ");
        scanf(" %49[^\n]", novo->nacionalidade); // Adicionado espaço no formato para consumir nova linha
        novo->nacionalidade[0] = toupper(novo->nacionalidade[0]);
        getchar();

        do
        {
            printf("\ndigite o numero de cadeiras inscritas do estudante: ");
            fgets(input, sizeof(input), stdin);
            if (sscanf(input, "%d", &cad->nMateria) != 1 || cad->nMateria <= 0)
            {
                printf("\nnumero invalido. Por favor, digite um numero inteiro positivo.\n");
            }
        }
        while(sscanf(input, "%d", &cad->nMateria) != 1 || cad->nMateria <= 0 );

        do
        {
            printf("\ndigite o total de creditos do estudante: ");
            fgets(input, sizeof(input), stdin);
            if (sscanf(input, "%f", &cad->ects) != 1 || cad->ects <= 0)
            {
                printf("\nnumero invalido. Por favor, digite um numero inteiro positivo.\n");
            }
        }
        while(sscanf(input, "%f", &cad->ects) != 1 || cad->ects <= 0 );

        *e = realloc(*e, sizeof(estudante) * (quant + 1)); //realoco a memoria para o tamanho atualizado.
        *nm = realloc(*nm, sizeof(nmats) * (quant + 1));
        (*e)[quant] = *novo;
        (*nm)[quant] = *cad;

        return 1;
    }
    else
    {
        printf("\nNao foi possível realizar o cadastro.\n");
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
        printf("Erro na realocacao de memoria.\n");
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
        printf("\nNao ha alunos cadastrados.\n");
        return 0.0;
    }
    for (int i = 0; i < quant; i++)
    {
        somaMatriculas += nm[i].nMateria;//adiciono cada cadeira dos alunos a soma.
    }
    printf("\ntotal de matriculas: %.2f\n", somaMatriculas);
    return somaMatriculas/quant; //divido o total de cadeiras pelo numero de alunos.

}

void imprimirArquivoDados(estudante *e, nmats *nm, int quant)
{
    //essa funcao apenas mostra os alunos lidos.
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

//essa funcao conta estudantes em risco de prescrever.
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
        printf("\nDados Salvos com sucesso!!\n");
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

    do
    {
        printf("\nDigite a ID do estudante que deseja remover (um numero inteiro positivo): ");
        if (scanf("%d", &id) != 1 || id <= 0)
        {
            printf("\nEntrada invalida. Por favor, insira um numero inteiro positivo.\n");
            while (getchar() != '\n'); // Limpar o buffer de entrada
        }
        else
        {
            break;
        }
    }
    while (1);

    for(int i = 0; i < *quant; i++)
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
        for(int i = posicao; i < *quant - 1; i++)
        {
            (*e)[i] = (*e)[i+1];
            (*nm)[i] = (*nm)[i+1];
        }
        printf("\nEstudante %d removido com sucesso!\n", id);
        *quant -= 1;
    }
    else
    {
        printf("\nEstudante com a ID %d nao encontrado, tente novamente.\n", id);
    }
}

//essa funcao procura partes do nome e devolve os achados.
void procurarNome(estudante **e, nmats **nm, int quant, char parteDoNome[])
{
    int encontrados = 0;
    for (int i = 0; i < quant; i++)
    {
        // Verifica se a parteDoNome está contida no nome do estudante (procura por substrings)
        if (strstr((*e)[i].nome, parteDoNome) != NULL)
        {
            printf("\n\tCodigo\tNome\t\tData Nasc\tNacionalidade\tCadeiras\tECTS\n");
            printf("\t-----------------------------------------------\n");
            printf("\t%4d\t%-20s\t%2d-%2d-%4d\t%s\t%d\t%.2f\n",
                   (*e)[i].id, (*e)[i].nome, (*e)[i].dia, (*e)[i].mes, (*e)[i].ano, (*e)[i].nacionalidade, (*nm)[i].nMateria, (*nm)[i].ects);
            encontrados = 1;
        }
    }
    if (!encontrados)
    {
        printf("\nNenhum estudante encontrado com o nome que contenha \"%s\".\n", parteDoNome);
    }
}


void buscarNacionalidade(estudante **e, int quant, char nacionalidade[])
{
    int numEstudante = 0;
    int encontrados = 0;
    char *mostrador;

    for (int i = 0; i < quant; i++)
    {
        // Compara a nacionalidade do estudante (em e[i].nacionalidade) com a nacionalidade fornecida.
        if (strstr((*e)[i].nacionalidade, nacionalidade) != NULL)
        {
            mostrador = (*e)[i].nacionalidade;
            printf("\n\tCodigo\tNome\t\tData Nasc\tNacionalidade\n");
            printf("\t-----------------------------------------------\n");
            printf("\t%4d\t%-20s\t%2d-%2d-%4d\t%s\n",
                   (*e)[i].id, (*e)[i].nome, (*e)[i].dia, (*e)[i].mes, (*e)[i].ano, (*e)[i].nacionalidade);
            encontrados = 1;// Marcamos que encontramos pelo menos um estudante com a nacionalidade buscada
            numEstudante++;
        }
    }

    if (!encontrados)
    {
        printf("\nNenhum estudante encontrado com a nacionalidade %s.\n", nacionalidade);
    }
    printf("\ntotal de estudantes %s = %d\n", mostrador, numEstudante);
}
//essa funcao me da o total de matriculas e a media para cada nacionalidade.
float matriculaNacionalidade(estudante **e, nmats *nm, int quant, char nacionalidade[])
{
    float somaMatriculas = 0.0;
    char *mostrador;
    int encontrados = 0, contador = 0;
    for (int i = 0; i < quant; i++)
    {
        // Compara a nacionalidade do estudante (em e[i].nacionalidade) com a nacionalidade fornecida.
        if (strstr((*e)[i].nacionalidade, nacionalidade) != NULL)
        {
            somaMatriculas += nm[i].nMateria;
            mostrador = (*e)[i].nacionalidade;
            encontrados = 1;
            contador++;
        }
    }

    if (!encontrados)
    {
        printf("\nNenhum estudante encontrado com a nacionalidade %s.\n", nacionalidade);
    }
    printf("\ntotal de matriculas dos %s: %.2f\n",mostrador, somaMatriculas);
    return somaMatriculas/contador;
}



int main()
{

    estudante *e = NULL;
    nmats *nm = NULL;
    char nacionalidade[50];
    char arquivodados[] = "dados.txt";
    char arquivodados2[] = "cod nMat ECTS.txt";
    int quant = 0, opcao, remove = 0, arquivosLidos = 0;
    float mediaMatriculas = 0.00;
    float mediaMatriculasNacionalidade = 0.00;

    do
    {
        printf("\n---------------------------------------------------------------------------");
        printf("\nEscolha uma opcao:\n1. Ler arquivo de dados\n2. Cadastrar aluno\n3. Imprimir dados\n4. Salvar arquivo\n5. Numero medio de matriculas\n6. Ver finalistas\n7. Ver estudantes em risco de prescrever\n8. Remover aluno\n9. Procurar aluno por nome\n10. Listar por nacionalidade\n11. Numero medio de matriculas por nacionalidade\n\n0. Sair\n\n");
        scanf("%d", &opcao);
        getchar(); // Para consumir o caractere de nova linha

        switch (opcao)
        {
        case 1:
            quant = lerArquivoDados(&e, &nm, arquivodados, arquivodados2);
            printf("\nArquivos lidos\n");
            arquivosLidos = 1;

            break;
        case 2:
            if(arquivosLidos == 1)
            {
                quant+= adicionarEstudante(&e, &nm, quant);
            }
            else
                printf("\nNao e possivel cadastrar sem ler os arquivos antes\n");
            break;
        case 3:
            imprimirArquivoDados(e, nm, quant);
            printf("\ntotal de estudantes: %d", quant);
            break;

        case 4:
            if(arquivosLidos == 1)
            {
                salvar(&e, &nm, quant, arquivodados, arquivodados2);
            }
            else
                printf("\nNao e possivel salvar sem ler os arquivos antes\n");
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
            if(arquivosLidos == 1)
            {
                removerAluno(&e, &nm, &quant);
            }
            else
            {
                printf("primeiro leia os arquivos, depois tente excluir\n");
            };
            break;

        case 9:
            if(arquivosLidos == 1)
            {
                printf("Digite a parte do nome que deseja pesquisar: ");
                char parteDoNome[100];
                scanf(" %99[^\n]", parteDoNome);
                parteDoNome[0] = toupper(parteDoNome[0]);
                procurarNome(&e, &nm, quant, parteDoNome);
            }
            else
            {
                printf("\nNao e possivel buscar alunos sem ler os arquivos antes\n");

            }
            break;

        case 10:
            if(arquivosLidos == 1)
            {
                printf("\nDigite a nacionalidade que deseja listar: ");
                scanf(" %49[^\n]", nacionalidade);
                nacionalidade[0] = toupper(nacionalidade[0]);
                buscarNacionalidade(&e, quant, nacionalidade);
            }
            else
            {
                printf("leia os arquivos primeiro e depois tente listar!\n");
            }
            break;

        case 11:
             if(arquivosLidos == 1)
            {
            printf("\nDigite a nacionalidade que deseja listar: ");
            scanf(" %49[^\n]", nacionalidade);
            nacionalidade[0] = toupper(nacionalidade[0]);
            mediaMatriculasNacionalidade = matriculaNacionalidade(&e, nm, quant, nacionalidade);
            printf("media: %.2f\n", mediaMatriculasNacionalidade);}
            else{printf("Primeiro leia os dados, depois tente buscar a media de matriculas!!\n");}
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
