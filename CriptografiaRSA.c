#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

int primo(long long int x) // Verifica se os números p e q são primos
{
    if (x <= 1)
    {
        return 0;
    }
    for (int i = 2; i <= sqrt(x); i++)
    {
        if (x % i == 0)
        {
            return 0;
        }
    }
    return 1;
}
long long int mdc(long long int n, long long int i) // Ve o mdc do possivel 'e' e totiente
{
    if (n % i == 0)
    {
        return i;
    }
    else
    {
        return mdc(i, n % i);
    }
}
long long int coprimo(long long int n) // Ver os numeros que são coprimos para a formação da chave 'e'
{
    long long int i;
    printf("Digite um numero relativamente primo a %lld:", n);
    scanf("%lld", &i);
    if (i > n)
    {
        printf("Numero invalido, digite novamente\n");
        return coprimo(n);
    }
    else
    {
        if (mdc(n, i) == 1)
        {
            return i;
        }
        else
        {
            printf("Numero invalido, digite novamente\n");
            return coprimo(n);
        }
    }
}
void gerar() // Funcao que gera a chave pública
{
    long long int p, q, e, n, totiente;
    printf("Digite dois números:");
    scanf("%lld%lld", &p, &q);
    if (primo(p) == 1 && primo(q) == 1)
    {
        n = p * q;
        totiente = (p - 1) * (q - 1);
        char *ChavePublica = "ChavePublica.txt";
        FILE *fp = fopen(ChavePublica, "w");
        fprintf(fp, "A chave publica é %lld %lld\n", coprimo(totiente), n);
        fclose(fp);
        printf("A chave publica foi salva no diretorio do programa\n");
        return;
    }
    else
    {
        printf("Numero invalido, digite novamente\n");
        gerar();
    }
}
int exp_mod_rapida(int mensagem, int e, int n) // funcao que usa a exponenciacao modular rapida para encriptar ou desencriptar a mensagem
{
    int res = 1;
    while (e > 0)
    {
        if (e % 2 == 1)
        {
            res = (res * mensagem) % n;
        }
        mensagem = (mensagem * mensagem) % n;
        e = e / 2;
    }
    return res;
}
int criptografar(char palavra[], int cont, int i, int e, int n, int convertido[]) // criptografa a mensagem e salva no diretorio
{
    if (cont == i)
    {
        char *criptografia = "criptografia.txt";

        FILE *fp = fopen(criptografia, "w");
        for (int j = 0; j < i; j++)
        {
            int list = exp_mod_rapida(convertido[j], e, n);
            fprintf(fp, "%d ", list);
        }
        fclose(fp);
        printf("A mensagem criptografada foi salva no diretorio do programa\n");
        return 0;
    }
    else
    {
        convertido[cont] = palavra[cont];
        return criptografar(palavra, cont + 1, i, e, n, convertido);
    }
}
void frase(char palavra[]) // le a mensagem que o usuario quer encriptar
{
    scanf("%[^\n]", palavra);
    int e, n;
    printf("Digite a chave publica: ");
    scanf("%d %d", &e, &n);
    int convertido[strlen(palavra)];
    criptografar(palavra, 0, strlen(palavra), e, n, convertido);
    return;
}
long long int euclidesExtendido(long long int a, long long int b, long long int *s, long long int *t) // Achar o 'd'
{
    if (b == 0)
    {
        *s = 1;
        *t = 0;
        return a;
    }
    long long int s1, t1;
    int mdc = euclidesExtendido(b, a % b, &s1, &t1);
    *s = t1;
    *t = s1 - (a / b) * t1;
    return mdc;
}
long long int chavePrivada(long long int a, long long int m) // Enviar o 'd' da chave privada
{
    long long int s, t;
    int mdc = euclidesExtendido(a, m, &s, &t);
    if (mdc != 1)
    {
        return -1;
    }
    return (s % m + m) % m;
}
void descriptografar(int d, int n) // Desencriptar a mensagem encriptada
{

    FILE *file;
    FILE *arquivo;
    file = fopen("criptografia.txt", "r");
    arquivo = fopen("mensagem.txt", "w");
    int i = 0;
    fscanf(file, "%d", &i);
    while (!feof(file))
    {
        char letra;
        i = exp_mod_rapida(i, d, n);
        letra = i;
        fprintf(arquivo, "%c", letra);
        fscanf(file, "%d", &i);
    }
    printf("A mensagem descriptografada está no diretorio do programa.\n");
    fclose(file);
    fclose(arquivo);
}
void selecao() // Escolha de função
{
    int escolha;
    char palavra[255];

    printf("1 - Gerar chave publica\n");
    printf("2 - Encriptar\n");
    printf("3 - Desencriptar\n");
    scanf("%d", &escolha);
    getchar();
    if (escolha == 1)
    {
        gerar();
    }
    else if (escolha == 2)
    {
        printf("Digite a frase que voce quer criptografar: ");
        frase(palavra);
    }
    else if (escolha == 3)
    {
        int e, p, q;
        printf("Digite o 'p', 'q' e 'e': ");
        scanf("%d %d %d", &p, &q, &e);
        int d = chavePrivada(e, ((p - 1) * (q - 1)));
        descriptografar(d, p * q);
    }
    else
    {
        printf("Opcao invalida, por favor, digite novamente\n");
        selecao();
    }
}
int main()
{
    printf("-----------------------------------\n");
    printf("|      Bem vindo ao projeto de    |\n");
    printf("|         Criptografia RSA        |\n");
    printf("-----------------------------------\n");
    printf("\n");
    printf("Para comecar, escolha a opcao desejada:\n");
    selecao();
    return 0;
}
