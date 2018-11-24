/*  Programa: Jogo Da Senha
       Autor: Rafael Timbó Matos (RA 106228)
       Turma MC102Y
       Versao: a
       Concluída em: 31/05/2010
       Breve descricao:
          Este programa simula o jogo da senha.
*/

#include <stdio.h>
#include <allegro.h>
#include <time.h>

#define AZUL           21
#define AMARELO        22
#define VERMELHO       23
#define VERDE          24
#define PRETO          25
#define LARANJA        26
#define CIANO          27
#define ROXO           28

#define COR_ALEATORIA 21 + rand() % numcores

#define COR_AZUL     makecol( 0  , 0  , 255 )
#define COR_AMARELO  makecol( 255, 255, 0   )
#define COR_LARANJA  makecol( 255, 100, 0   )
#define COR_CIANO    makecol( 0  , 215, 255 )
#define COR_PRETO    makecol( 0  , 0  , 0   )
#define COR_ROXO     makecol( 160, 0  , 200 )
#define COR_VERDE    makecol( 0  , 255, 0   )
#define COR_VERMELHO makecol( 255, 0  , 0   )

#define COR_BRANCO       makecol( 255, 255, 255 )
#define COR_BG COR_BRANCO

#define TRUE -1
#define FALSE 0
#define SUCCESS 1
#define FAILURE 0
#define PRINCIPAL 0
#define SUPORTE 1
#define INVALIDO -5

#define MARGEM 3
#define RAIO 25
#define DIAMETRO (2 * RAIO)
#define ESPACO_FONTES (DIAMETRO + 4 * MARGEM)
#define TELA_LARGURA 640
#define TELA_ALTURA 600

#define MAX_QUANT_PINOS 6

struct Pino
    {
        short int x,y;
        short int cor;
    };

void Screen(BITMAP *buffer)
{
    blit(buffer, screen, 0, 0, 0, 0, TELA_LARGURA,TELA_ALTURA);
}
int Sair()
{
    if (key[KEY_ESC])
        exit(EXIT_SUCCESS);

    return FALSE;
}

void init(BITMAP *buffer[2])
//Carrega as configurações iniciais necessárias para o jogo
{
    allegro_init();
    install_keyboard();
    install_timer();
    srand(time(NULL));
    install_mouse();
    set_color_depth(desktop_color_depth());

    if ( (set_gfx_mode(GFX_AUTODETECT_WINDOWED, TELA_LARGURA, TELA_ALTURA, 0, 0) ) != 0) {
        allegro_message(allegro_error);
        exit(EXIT_FAILURE);
    }
    buffer[PRINCIPAL] = create_bitmap(TELA_LARGURA,TELA_ALTURA);
    buffer[SUPORTE] = create_bitmap(TELA_LARGURA,TELA_ALTURA);
    set_window_title("Jogo Da Senha - Pressione ESC para sair");
}

int ConfereCorIgual(short int senha[MAX_QUANT_PINOS], short int quant_pinos, short int i)
//Confere se a cor definida já nao existe na senha
{
    int m;
    for ( m = 0 ; m != i ; m++)
            if (senha[i] == senha[m])
            return TRUE;
    return FALSE;
}

void CriarSenha(short int senha[MAX_QUANT_PINOS], short int quant_pinos, short int numcores)
//Esta funcao gera aleatoriamente as cores da senha
{
    short int i
    ;

    for ( i = 0 ; i < quant_pinos ; i++)
    {
        senha[i] = COR_ALEATORIA;
        i += ConfereCorIgual(senha, quant_pinos, i); //ANULE ESTA LINHA PARA HABILITAR REPETIÇÂO DE COR NA SENHA
    }
}

void CarregaNivel( short int* quant_pinos, short int* chances, short int* numcores)
//Esta funcao determina a quantidade de pinos na senha, o numero de cores possiveis e quantas chances o usuario tem para acertar a senha
//atraves do nivel, definido pelo usuario.
{
    int nivel;
    printf("Digite o nivel desejado(1,2,3):\n");
    fflush(stdin);
    scanf("%d", &nivel);
    printf("\n");
    while( nivel > 3 || nivel < 1)
    {
        printf("Somente sao niveis validos 1 , 2 ou 3! Por favor digite um nivel valido. \n");
        scanf("%d", &nivel);
        printf("\n");
    }

    switch(nivel)
    {
        case 1:
            *quant_pinos = 4;
            *numcores = 6;
            *chances = 6;
            break;

         case 2:
            *quant_pinos = 5;
            *numcores = 7;
            *chances= 8;
            break;

        case 3:
            *quant_pinos = 6;
            *numcores = 8;
            *chances = 10;
            break;
    }
}

int CalculaCor(int pigmento)
//esta funcao manipula a cor para que o programa possa processá-la
//Devolve o makecol correspondente a cor.
{
    switch(pigmento)
        {
            case AZUL:
                return COR_AZUL;
                break;

            case AMARELO:
                return COR_AMARELO;
                break;

            case LARANJA:
                return COR_LARANJA;
                break;

            case CIANO:
                return COR_CIANO;
                break;

            case PRETO:
                return COR_PRETO;
                break;

            case ROXO:
                return COR_ROXO;
                break;

            case VERDE:
                return COR_VERDE;
                break;

            case VERMELHO:
                return COR_VERMELHO;
                break;
        }
    return FALSE;
}
int DesenhaTelaJogo(short int quant_pinos, short int chances, short int numcores, BITMAP *buffer)
//Esta funcao desenha a tela inicial do  jogo
{
    short int i , j, pigmento;

    rectfill(buffer, 0 , 0, TELA_LARGURA, TELA_ALTURA, COR_BG);
    //____________DESENHARA OS BURACOS AONDE SERAO POSTOS OS PINOS PELO USUARIO_______________
    for (j = 0 ; j < quant_pinos ; j++) //quantidade de colunas definida pela quant de pinos numa rodada
        for ( i = 0 ; i < chances ; i++) //quantidade de linhas definida pela quantidade de chances do usuario
            circle(buffer,ESPACO_FONTES + MARGEM + j*DIAMETRO + RAIO + j * 2 * MARGEM, MARGEM + i * DIAMETRO + RAIO + i * MARGEM, RAIO, COR_PRETO);
          //ESPACO_FONTES PARA DESENHAR AS FONTES A ESQUERDA ; MARGEM para ter margem da borda da JANELA DO JOGO ;j*DIAMETRO contara quantos espaços(buracos) ja passaram ;
            // + RAIO para desenhar no centro ; j*MARGEM para ter espaço entre os buracos

    //j = quant_pinos e i = chances

    //                  (buffer, font,                     x                              ,    y      , COR_FONT , COR_BG,     TEXTO    );
    textprintf_centre_ex(buffer, font, ESPACO_FONTES + 4 * MARGEM + j*DIAMETRO + 3 * RAIO + j * 2 * MARGEM,     MARGEM, COR_PRETO, COR_BG,"Cores certas");
    textprintf_centre_ex(buffer, font, ESPACO_FONTES + 7 * MARGEM + j*DIAMETRO +     RAIO + j * 2 * MARGEM, 5 * MARGEM, COR_PRETO, COR_BG,"lugar certo ");
    textprintf_centre_ex(buffer, font, ESPACO_FONTES + 7 * MARGEM + j*DIAMETRO + 5 * RAIO + j * 2 * MARGEM, 5 * MARGEM, COR_PRETO, COR_BG,"/ lugar errado");

    //____________DESENHARA A FONTE DOS PINOS_______________________________________
    pigmento = AZUL;
    for (i = 0 ; i < numcores ; i++)
    {
        circlefill(buffer, MARGEM + RAIO , MARGEM + i * DIAMETRO + RAIO + i * MARGEM, RAIO, CalculaCor(pigmento) );
        pigmento++;
    }
    textprintf_centre_ex(buffer, font, MARGEM + RAIO, 3 * MARGEM + i * DIAMETRO+ i * MARGEM, COR_PRETO, COR_BRANCO,"Cores");
    vline(buffer, MARGEM + DIAMETRO + 2 * MARGEM, 0, TELA_ALTURA, COR_PRETO); //Linha vertical que divide as fontes das posicoes dos pinos para o jogador


    Screen(buffer);
    return SUCCESS;
}

int MouseNoIntervalo(int xmin, int xmax, int ymin, int ymax, int x_mouse, int y_mouse)
//Detecta se o mouse está na area xmin até xmax e ymin até ymax
{
    if (x_mouse == -1 || y_mouse == -1 ) //se for necessario atualizar o mouse, passa-se como os 2 ultimos parametros -1
    {
        poll_mouse();
        x_mouse = mouse_x;
        y_mouse = mouse_y;
    }

    if ( x_mouse >= xmin && x_mouse <= xmax)
    {
        if (y_mouse >= ymin && y_mouse <= ymax)
            return TRUE;
    }
    else return FALSE;
    return FALSE;
}

int ClickFonte(short int numcores)
//Esta funcao verifica se foi clicado na area da fonte das cores e em qual cor foi clicada
{
    int i;
    for (i = 1 ; i <= numcores ; i++)
    if(MouseNoIntervalo( MARGEM, MARGEM + DIAMETRO, MARGEM, MARGEM + i * DIAMETRO + i * MARGEM, -1, -1) )
        return (i - 1);

    return INVALIDO;
}

void RevelaSenha(short int senha[MAX_QUANT_PINOS], short int quant_pinos)
//imprime a senha no canto inferior esquerdo da tela
{
    short int j;
    textprintf_centre_ex(screen, font, MARGEM + RAIO, TELA_ALTURA - MARGEM - RAIO, COR_PRETO, COR_BRANCO,"Senha:");
    for ( j = 0 ; j < quant_pinos ; j++)
        circlefill(screen,ESPACO_FONTES + MARGEM + j * DIAMETRO + RAIO + j * 2 * MARGEM, TELA_ALTURA - MARGEM - RAIO, RAIO, CalculaCor(senha[j]) );
}

void Vitoria(short int senha[MAX_QUANT_PINOS], short int quant_pinos)
// Apresenta a senha no caso de vitória e finaliza o programa.
{
    RevelaSenha(senha, quant_pinos);
    allegro_message("Parabens Detetive!! Voce acertou a senha!!");
    rest(1800);
    exit(EXIT_SUCCESS);
}

void NaoConseguiu(short int senha[MAX_QUANT_PINOS], short int quant_pinos)
// Apresenta a senha no caso de derrota e finaliza o programa.
{
    RevelaSenha(senha, quant_pinos);
    allegro_message("Que Pena!! Voce nao conseguiu acertar a senha!!");
    rest(1700);
    exit(EXIT_SUCCESS);
}

int ConfereSenha(short int senha[MAX_QUANT_PINOS], short int tentativa[MAX_QUANT_PINOS], short int quant_pinos, BITMAP *buffer, short int i)
//funcao responsavel por checar quanto o usuario acertou e responder apropriadamante
{
    short int n, m, lugarcerto = 0, lugarerrado = 0, computou[MAX_QUANT_PINOS], marcou;

    for (n = 0 ; n < quant_pinos ; n++) computou[n] = FALSE;

    for (n = 0 ; n < quant_pinos ; n++)
    {
        marcou = FALSE;
        if (tentativa[n] == senha[n] && computou[n] == FALSE)
        {
            lugarcerto++;
            computou[n] = TRUE;
        }
        else for ( m = n + 1 ; m != n && !marcou; m = ( m + 1) % quant_pinos)
            if (tentativa[n] == senha[m] && computou[m] == FALSE && tentativa[m] != senha[m])
            {
                lugarerrado++;
                computou[m] = TRUE;
                marcou = TRUE;
            }
    }
    textprintf_centre_ex(buffer, font, ESPACO_FONTES + 7 * MARGEM + quant_pinos * DIAMETRO +     RAIO + quant_pinos * 2 * MARGEM, MARGEM + i * DIAMETRO + RAIO + i * MARGEM, COR_PRETO, COR_BG,"%d",lugarcerto);
    textprintf_centre_ex(buffer, font, ESPACO_FONTES + 7 * MARGEM + quant_pinos * DIAMETRO + 5 * RAIO + quant_pinos * 2 * MARGEM, MARGEM + i * DIAMETRO + RAIO + i * MARGEM, COR_PRETO, COR_BG,"%d",lugarerrado);
    printf("Voce posicionou corretamente %d cores certas!! \n", lugarcerto);
    printf("Voce acertou %d cores, mas estao no lugar errado!! \n", lugarerrado);

    if (lugarcerto == quant_pinos) return TRUE;
    else return FALSE;
}

int PosicaoPinoValida(short int i, short int quant_pinos)
//Esta funcao verifica se o pino foi largado em um buraco da rodada. Se sim, retorna qual foi o buraco, se não, retorna INVALIDO.
{
    int j;
    if (MouseNoIntervalo(ESPACO_FONTES + MARGEM, ESPACO_FONTES + MARGEM + (quant_pinos)*DIAMETRO + (quant_pinos - 1)* 2 * MARGEM, MARGEM + i * DIAMETRO + i * MARGEM, MARGEM + (i + 1) * DIAMETRO + i * MARGEM, mouse_x, mouse_y) )
    //Se o mouse estiver(ESQUERDA primeiro buraco,                       Direita ultimo buraco                                  ,Margem superior da linha da vez    , Margem inferior da linha da vez
    {
        for ( j = 0 ; j < quant_pinos ; j++)
            if (mouse_x >= ESPACO_FONTES + MARGEM + j * DIAMETRO + j * 2 * MARGEM && mouse_x <= ESPACO_FONTES + MARGEM + (j + 1)*DIAMETRO + j * 2 * MARGEM)
            return j;
    }
    else return INVALIDO;
}

int GerenciaPinos(short int numcores, BITMAP *buffer[2], short int i, short int quant_pinos, short int tentativa[MAX_QUANT_PINOS])
//Esta funcao e responsavel por associar o clique a cor do pino correta e coloca-lo na posicao certa
{
    struct Pino pino;
    short int pigmento, x1, y1, dx, dy, x = 0, y = 0, pinned = FALSE, j
    ;

    for ( j = 0 ; j < quant_pinos ; j++) tentativa[j] = 0;

    for ( j = 0 ; j < quant_pinos ; j++)
    {
        pino.x = 0;
        pino.y = 0;
        x = 0;
        y = 0;
        pinned = FALSE;
        while (!pinned)
        {
            poll_mouse();
            Sair();
            if (mouse_b & 1  && MouseNoIntervalo( MARGEM, MARGEM + DIAMETRO, MARGEM, MARGEM + (numcores) * DIAMETRO + RAIO + (numcores) * MARGEM, -1, -1))
            {
                pigmento = ClickFonte(numcores);
                if (pigmento != INVALIDO)
                {
                    pino.cor = AZUL + pigmento;
                    while (mouse_b & 1)
                    {
                        x1 = mouse_x;
                        y1 = mouse_y;
                        dx = x - pino.x;
                        dy = y - pino.y;
                        pino.x = x1 - dx;
                        pino.y = y1 - dy;
                        x = x1;
                        y = y1;

                        circlefill( buffer[SUPORTE], (int)pino.x, (int)pino.y, RAIO, CalculaCor(pino.cor));
                        Screen(buffer[SUPORTE]);
                        blit(buffer[PRINCIPAL], buffer[SUPORTE], 0, 0, 0, 0, TELA_LARGURA, TELA_ALTURA);
                        poll_mouse();
                    }
                    Screen(buffer[PRINCIPAL]);
                    x1 = PosicaoPinoValida(i, quant_pinos);
                    if ( x1 != INVALIDO)
                    {
                        if (tentativa[x1] >= AZUL && tentativa[x1] <= ROXO) j--;
                        tentativa[x1] = pino.cor;
                        circlefill(buffer[PRINCIPAL],ESPACO_FONTES + MARGEM + x1 * DIAMETRO + RAIO + x1 * 2 * MARGEM, MARGEM + i * DIAMETRO + RAIO + i * MARGEM, RAIO, CalculaCor(pino.cor));
                        pinned = TRUE;

                    }
                }
            }
        }
        Screen(buffer[PRINCIPAL]);
    }
}

int main(void)
{
    short int quant_pinos, numcores, chances, senha[MAX_QUANT_PINOS], n, i,tentativa[MAX_QUANT_PINOS];
    ; //N  = quant_pinos ; M = numcores ; L  = chances

    BITMAP *buffer[2];

    CarregaNivel(&quant_pinos, &chances, &numcores);
    init(buffer);
    CriarSenha(senha, quant_pinos, numcores);
    DesenhaTelaJogo(quant_pinos, chances, numcores, buffer[PRINCIPAL]);
    show_mouse(screen);
    for ( i = 0 ; i < chances; i++)
    {
        GerenciaPinos(numcores, buffer, i, quant_pinos, tentativa);
        if (ConfereSenha(senha , tentativa, quant_pinos, buffer[PRINCIPAL], i) ) Vitoria(senha, quant_pinos);
        else
        {
        blit(buffer[PRINCIPAL], buffer[SUPORTE], 0, 0, 0, 0, TELA_LARGURA, TELA_ALTURA);
        Screen(buffer[PRINCIPAL]);
        }
    }
    NaoConseguiu(senha, quant_pinos);
    return 0;
}
END_OF_MAIN();
