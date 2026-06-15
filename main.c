#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct cliente
{
    char nome[30];
    char sexo;
    int idade;
};
typedef struct cliente cliente;

struct compra
{
    cliente cl;
    int numItens;
    char franquia;
    float valorTotal;
};
typedef struct compra compra;

cliente cadastrarCliente();

compra cadastrarCompra(cliente c);

void guardarCompras(compra *lista, int totalCompras);

int recuperarCompras(compra **lista, int *totalCompras, cliente **clientes, int *totalClientes,cliente *maisNovo, cliente *maiorNome);


int main()
{
    compra *cpr;
    cliente *cli;
    cliente novo = {0};
    cliente maior = {0};

    cpr = malloc(sizeof(compra));
    if (cpr == NULL)
    {
        printf("Erro");
        return 1;
    }
    cli = malloc(sizeof(cliente));
    if (cli == NULL)
    {
        printf("Erro");
        return 1;
    }
    int encontrou = 0, opcao, numcompras = 0, numClientes = 0, opCliente, num = 0, opinfo, cliM = 0, cliF = 0, optotal;
    char nome[30];

    opcao = 0;

    // Carregar compras do arquivo ao iniciar
    recuperarCompras(&cpr, &numcompras, &cli, &numClientes, &novo, &maior);

    do
    {
        printf("\n========================================\n");
        printf("  BEM-VINDO A LAURO STORE!  \n");
        printf("========================================\n");

        printf("\n===== MENU PRINCIPAL =====\n");
        printf("  [1] Cadastrar novas compras\n");
        printf("  [2] Pesquisar compra por cliente\n");
        printf("  [3] Ver informações de todas as compras\n");
        printf("  [4] Pesquisar uma compra especifica\n");
        printf("  [5] Encerrar\n");
        printf("  Opção: ");

        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            int opCompras = 0;

            // Recebe o valor que vai ser cadastrado
            printf("\nOpcao 1 selecionada: Cadastrar novas compras\n");
            printf("Digite quantas compras deseja cadastrar:\n");
            scanf("%d", &opCompras);
            getchar();
            // Soma com as compras anteriores para não sobrescrever os dados já existentes
            numcompras += opCompras;
            cpr = realloc(cpr, numcompras * sizeof(compra));
            if (cpr == NULL)
            {
                printf("Erro");
                return 1;
            }

            // Recebe os dados dos clientes [i]
            for (int i = numcompras - opCompras; i < numcompras; i++)
            {
                if (i > 0)
                {
                    printf("Escolha uma opção:\n");
                    printf("[1] Deseja cadastrar um cliente\n");
                    printf("[2] Deseja usar um cliente já cadastrado\n");
                    scanf("%d", &opCliente);
                    getchar();

                    switch (opCliente)
                    {
                        // Cadstra um cliente
                    case 1:
                        numClientes++;
                        cli = realloc(cli, numClientes * sizeof(cliente));
                        cli[numClientes - 1] = cadastrarCliente();
                        cpr[i] = cadastrarCompra(cli[numClientes - 1]);
                        if (cli[numClientes - 1].idade < novo.idade ||numClientes - 1 == 0);
                        {
                            novo = cli[numClientes - 1];
                        }
                        if (strlen(cli[numClientes - 1].nome) > strlen(maior.nome))
                        {
                            maior = cli[numClientes - 1];
                        }

                        break;
                        // Verifica se o cliente existe
                    case 2:
                        printf("Digite o nome do cliente que fez a compra:\n");
                        fgets(nome, sizeof(nome), stdin);
                        nome[strcspn(nome, "\n")] = '\0';
                        for (int j = 0; j < numClientes; j++)
                        {
                            // Se encontar cadastra compra com cliete já existente
                            if (strcmp(nome, cli[j].nome) == 0)
                            {
                                cpr[i] = cadastrarCompra(cli[j]);
                                encontrou = 1;
                            }
                        }
                        if (encontrou == 0)
                        {
                            printf("Cliente ainda não cadastrado:\n");
                        }
                        else if (encontrou == 1)
                        {
                            encontrou--;
                            break;
                        }
                        break;
                    default:
                        printf("Escolha uma opcao valida");
                        i--;
                    }
                    // Caso seja o primeiro cliente ele cadastra direto
                }
                else
                {
                    numClientes++;
                    cli = realloc(cli, numClientes * sizeof(cliente));
                    cli[numClientes - 1] = cadastrarCliente();
                    cpr[i] = cadastrarCompra(cli[numClientes - 1]);
                    novo = cli[numClientes - 1];
                    maior = cli[numClientes - 1];
                }
            }

            // Salvar compras no arquivo após cadastrar
            guardarCompras(cpr, numcompras);

            break;
        case 2:
            printf("\nOpcao 2 selecionada: Pesquisar compra por cliente\n");
            // Caso não tenha cadastro ele fecha o caso
            if (numcompras == 0)
            {
                printf("Nenhuma compra foi cadastrada ainda.\n");
                break;
            }
            //  1 verdadeiro Se pesquisarNovamente for verdadeiro ira fazer a pesquisa no nome
            int pesquisarNovamente = 1;
            while (pesquisarNovamente == 1)
            {
                getchar();
                printf("Digite o nome do cliente: \n");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';

                int achouCliente = 0;
                int totalComprasCliente = 0;
                float somaValores = 0.0;
                // Verifica se achou algum cliente
                for (int i = 0; i < numcompras; i++)
                {
                    // Se sim mostrar todas as compras do clientes
                    if (strcmp(nome, cpr[i].cl.nome) == 0)
                    {
                        if (achouCliente == 0)
                        {
                            printf("\n======= Compras de %s ========\n", cpr[i].cl.nome);
                        }
                        achouCliente = 1;
                        totalComprasCliente++;

                        printf("\n---Compra %d---\n", totalComprasCliente);
                        printf("    Franquia: %c\n", cpr[i].franquia);
                        printf("    Quantidade de itens: %d\n", cpr[i].numItens);
                        printf("    Valor total: R$ %.2f\n", cpr[i].valorTotal);

                        somaValores += cpr[i].valorTotal;
                    }
                }
                // Se não retorna que não existe compras cadastradas
                if (achouCliente == 0)
                {
                    printf("Não há compras cadastradas para o nome'%s'.\n", nome);
                }
                else
                {
                    float media = somaValores / totalComprasCliente;
                    printf("\n================================\n");
                    printf("    Total de compras encontradas: %d\n", totalComprasCliente);
                    printf("    Media do valor das compras: R$ %.2f\n", media);
                    printf("\n================================\n");
                }
                printf("\nO que deseja fazer?\n");
                printf("[1] Pesquisar outro cliente\n");
                printf("[2] Retornar ao menu principal\n");
                printf("Opção: ");

                int opPesquisa;
                scanf("%d", &opPesquisa);

                if (opPesquisa == 2)
                {
                    pesquisarNovamente = 0;
                }
                else if (opPesquisa != 1)
                {
                    printf("Opção inválida, retornando ao menu principal.\n");
                    pesquisarNovamente = 0;
                }
            }

            break;

        case 3:
            float somaPreco = 0;
            float mediaPreco = 0;

            printf("\nOpcao 3 selecionada: Ver informacoes de todas as compras\n");

            for (int i = 0; i < numcompras; i++)
            {
                printf("%s\n", cpr[i].cl.nome);
                printf("%c\n", cpr[i].cl.sexo);
                printf("%d\n", cpr[i].cl.idade);
                printf("%d\n", cpr[i].numItens);
                printf("%c\n", cpr[i].franquia);
                printf("%.2f\n", cpr[i].valorTotal);
                somaPreco += cpr[i].valorTotal;
            }
            mediaPreco = somaPreco / numcompras;
            printf("Valor total de todas as compras: R$%.2f\n ", somaPreco);
            printf("A media de valor gasto em compras: R$%.2f\n", mediaPreco);

            printf("1)Deseja voltar para o menu principal\n");
            printf("2)Deseja encerrar programa\n");
            scanf("%d", &optotal);
            if (optotal == 2)
            {
                printf("\n");
                printf("Encerrando...");
                opcao = 5;
            }
            break;

        case 4:
            if (numcompras == 0)
            {
                printf("Não há compras cadastradas");
                break;
            }

            printf("\nOpção 4 selecionada: Ver informações das compras\n");
            printf("Digite a opcao desejada:\n");
            printf("1)Ver todas as compras com valor abaixo do valor de referencia\n");
            printf("2)Ver todas as compras pelo numero de itens\n");
            printf("3)Ver a quantidade de compras por franquia\n");
            printf("4)Ver quantidade de clientes e compras por sexo\n");
            printf("5)Ver os dados do cliente com maior nome\n");
            printf("6)Ver os dados do cliente mais novo\n");
            scanf("%d", &opinfo);

            switch (opinfo)
            {
            case 1:
                float ref;
                int comprasAbaixo = 0;

                printf("Digite o valor que você deseja ter como referencia:\n");
                scanf("%f", &ref);

                for (int i = 0; i < numcompras; i++)
                {
                    if (cpr[i].valorTotal < ref)
                    {
                        comprasAbaixo++;
                    }
                }

                printf("Quantidade de compras abaixo de R$ %.2f: %d\n", ref, comprasAbaixo);

                break;
            case 2:
                int comprasDoisItens = 0;

                for (int i = 0; i < numcompras; i++)
                {
                    if (cpr[i].numItens == 2)
                    {
                        comprasDoisItens++;
                    }
                }

                printf("Quantidade de compras com exatamente 2 itens: %d\n", comprasDoisItens);

                break;
            case 3:
                int comprasFranquiaC = 0;

                for (int i = 0; i < numcompras; i++)
                {
                    if (cpr[i].franquia == 'C')
                    {
                        comprasFranquiaC++;
                    }
                }

                printf("Franquia C: %d\n", comprasFranquiaC);

                break;
            case 4:

                int compradoresSemSexo = 0;
                float totalMulheres = 0.0;

                for (int i = 0; i < numcompras; i++)
                {
                    if (cpr[i].cl.sexo == 'O')
                    {
                        compradoresSemSexo++;
                    }

                    if (cpr[i].cl.sexo == 'F')
                    {
                        totalMulheres += cpr[i].valorTotal;
                    }
                }

                printf("Compradores que não informaram o sexo: %d\n", compradoresSemSexo);
                printf("Valor total comprado por mulheres: R$ %.2f\n", totalMulheres);

                break;
            case 5:
                printf("%s é o cliente com o maior nome:\n", maior.nome);
                printf("Aqui estão todas as informações de compras:\n");
                for (int i = 0; i < numcompras; i++)
                {
                    if (strcmp(maior.nome, cli[i].nome) == 0)
                    {
                        printf("%s\n", cpr[i].cl.nome);
                        printf("%c\n", cpr[i].cl.sexo);
                        printf("%d\n", cpr[i].cl.idade);
                        printf("%d\n", cpr[i].numItens);
                        printf("%c\n", cpr[i].franquia);
                        printf("%f\n", cpr[i].valorTotal);
                    }
                }
                break;

            case 6:
                printf("%s e o cliente mais novo:\n", novo.nome);
                printf("Aqui estão todas as informações de compras:\n");
                for (int i = 0; i < numcompras; i++)
                {
                    if (novo.idade == cli[i].idade)
                    {
                        printf("%s\n", cpr[i].cl.nome);
                        printf("%c\n", cpr[i].cl.sexo);
                        printf("%d\n", cpr[i].cl.idade);
                        printf("%d\n", cpr[i].numItens);
                        printf("%c\n", cpr[i].franquia);
                        printf("%f\n", cpr[i].valorTotal);
                    }
                }

                break;
            default:
                printf("Opção invalida\n");
            }

            break;

        case 5:
            printf("\nEncerrando o programa. Obrigado por usar a Lauro Store!\n");
            free(cpr);
            free(cli);
            break;

        default:
            printf("\nOpcao invalida. Por favor, escolha uma opcao entre 1 e 4.\n");
        }

    } while (opcao != 5);

    return 0;
}

cliente cadastrarCliente()
{
    cliente c;
    printf("==========Cadastrando Cliente ==========\n");
    do
    {
        printf("Digite o nome do cliente:\n");
        fgets(c.nome, sizeof(c.nome), stdin);
        c.nome[strcspn(c.nome, "\n")] = '\0';
        if (strlen(c.nome) < 4)
        {
            printf("Nome inválido deve ter no minimo 4 caracteres\n");
        }

    } while (strlen(c.nome) < 4);
    ;
    do
    {
        printf("Digite o sexo do cliente usando apenas 1 caracter maiusculo:\n");
        scanf(" %c", &c.sexo);
        if (c.sexo != 'M' && c.sexo != 'F' && c.sexo != 'O')
        {
            printf("Sexo inválido digite um valido('M','F','O')\n");
        }

    } while (c.sexo != 'M' && c.sexo != 'F' && c.sexo != 'O');
    do
    {
        printf("Digite a idade do cliente:\n");
        scanf("%d", &c.idade);

        if (c.idade < 0 || c.idade > 120)
        {
            printf("Idade inválida\n");
        }
    } while (c.idade <= 0 || c.idade > 120);
    getchar();

    return c;
}

compra cadastrarCompra(cliente c)
{
    compra cpr;
    cpr.cl = c;
    do
    {
        printf("Digite a quantidade de Itens da compra:\n");
        scanf("%d", &cpr.numItens);
        if (cpr.numItens < 1)
        {
            printf("Quantidade invalida:\n");
        }
    } while (cpr.numItens < 1);
    do
    {
        printf("Digite a franquia da loja ('A','B' ou 'C'):\n");
        scanf(" %c", &cpr.franquia);
        if (cpr.franquia != 'A' && cpr.franquia != 'B' && cpr.franquia != 'C')
        {
            printf("Franquia invalida:\n");
        }
    } while (cpr.franquia != 'A' && cpr.franquia != 'B' && cpr.franquia != 'C');

    do
    {
        printf("Digite o valor total da compra:\n");
        scanf("%f", &cpr.valorTotal);
        if (cpr.valorTotal < 0)
        {
            printf("Valor Invalido\n");
        }
    } while (cpr.valorTotal < 0);
    getchar();

    printf("Compra cadastrada com sucesso!\n");

    return cpr;
}
void guardarCompras(compra *lista, int totalCompras)
{
    FILE *arquivo = fopen("camisas.txt", "w");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo para salvar!\n");
        return;
    }
    for (int i = 0; i < totalCompras; i++)
    {
        fprintf(arquivo, "%s|%c|%d|%d|%c|%.2f\n",
                lista[i].cl.nome,
                lista[i].cl.sexo,
                lista[i].cl.idade,
                lista[i].numItens,
                lista[i].franquia,
                lista[i].valorTotal);
    }
    fclose(arquivo);
}

int recuperarCompras(compra **lista, int *totalCompras, cliente **clientes, int *totalClientes, cliente *maisNovo, cliente *maiorNome)
{
    FILE *arquivo = fopen("camisas.txt", "r");
    if (arquivo == NULL)
    {
        // Arquivo ainda não existe
        return 0;
    }

    char linha[200];
    int encontradas = 0;

    while (fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        linha[strcspn(linha, "\n")] = '\0';
        if (strlen(linha) < 3)
            continue;

        compra c;
        char sexoStr[4];
        char franquiaStr[4];

        // cada campo separado por | na linha do arquivo
        char *parte = strtok(linha, "|");
        if (!parte)
            continue;
        strncpy(c.cl.nome, parte, 29);
        c.cl.nome[29] = '\0';

        parte = strtok(NULL, "|");
        if (!parte)
            continue;
        c.cl.sexo = parte[0];

        parte = strtok(NULL, "|");
        if (!parte)
            continue;
        c.cl.idade = atoi(parte);

        parte = strtok(NULL, "|");
        if (!parte)
            continue;
        c.numItens = atoi(parte);

        parte = strtok(NULL, "|");
        if (!parte)
            continue;
        c.franquia = parte[0];

        parte = strtok(NULL, "|");
        if (!parte)
            continue;
        c.valorTotal = atof(parte);

        // adiciona a compra lida no vetor
        (*totalCompras)++;
        *lista = realloc(*lista, (*totalCompras) * sizeof(compra));
        if (*lista == NULL)
        {
            printf("Erro de memória!\n");
            fclose(arquivo);
            return -1;
        }
        (*lista)[*totalCompras - 1] = c;

        // checa se o cliente dessa compra já foi adicionado antes
        int clienteJaExiste = 0;
        for (int j = 0; j < *totalClientes; j++)
        {
            if (strcmp((*clientes)[j].nome, c.cl.nome) == 0)
            {
                clienteJaExiste = 1;
                break;
            }
        }
        if (!clienteJaExiste)
        {
            (*totalClientes)++;
            *clientes = realloc(*clientes, (*totalClientes) * sizeof(cliente));
            if (*clientes == NULL)
            {
                printf("Erro de memória!\n");
                fclose(arquivo);
                return -1;
            }
            (*clientes)[*totalClientes - 1] = c.cl;

            // atualiza o mais novo e o de maior nome conforme vai lendo
            if (*totalClientes == 1)
            {
                *maisNovo = c.cl;
                *maiorNome = c.cl;
            }
            else
            {
                if (c.cl.idade < maisNovo->idade)
                    *maisNovo = c.cl;
                if (strlen(c.cl.nome) > strlen(maiorNome->nome))
                    *maiorNome = c.cl;
            }
        }
        encontradas++;
    }

    fclose(arquivo);

    if (encontradas > 0)
        printf("\n[ARQUIVO] %d compra(s) carregada(s) de 'camisas.txt'.\n", encontradas);

    return encontradas;
}
