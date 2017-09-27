#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_ESC 27
#define KEY_ENTER 13
#define KEY_SPACE 32

void MoveCursorTo(int x,int y);
void ClearScreen(void);
int getch(void);
void Credits(void);
void Solution(void);
int Exit(void);
int PlaAgain(void);
void Input(int *matrix);
void DisplayMatrix(int *matrix);
int CheckWin(int *matrix, int _sum);
void Play();
void ClearMatrix(int *matrix);
void Help(void);
void Menu();
void Congratulations(void);
void Presentation(void);

int main(void)
{
    
    srand((unsigned)time(NULL));
    Presentation();
   
    Menu();
    
    return 0;
} 

// Essas funcoes de limpeza de tela,
//mover cursor e a implementação
// de getch para linux são de autoria de Frederico Pissarra.
#if defined(_WIN32)

#include <windows.h>
#include <conio.h>

void MoveCursorTo(int x,int y)
{
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (COORD){x,y});
}

void ClearScreen()
{
    HANDLE hConsole;
    COORD coord = {0, 0};
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConsoleSize, dwWriten;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    dwConsoleSize = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputCharacter(hConsole, (TCHAR)' ', dwConsoleSize, coord, &dwWriten);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConsoleSize, coord, &dwWriten);
    SetConsoleCursorPosition(hConsole, coord);
}

#elif defined (__linux)

#include <termios.h>
#include <unistd.h>

void MoveCursorTo(int x, int y)
{
    printf("\x1b[%d;%dH", ++x, ++y);
}

void ClearScreen()
{
    fputs("\x1b[2J",stdout);
}

int gecth()
{
    struct termios oldt, newt;
    int ch;
 
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
 
    ch = getchar();
  
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        
    return ch;
}

#else 
    
    fputs("Sistema operacional nao suportado",stderr);

#endif

void Credits(void)
{
    Presentation();
    getchar();
    ClearScreen();
    
    printf("Este game foi desenvolvido por Bruno Soares\n");
    getchar();
}

void Solution(void)
{
    ClearScreen();
    
    puts(
        "\n\t  2 |  9 |  4 \n"
        "\t____|____|____\n"
        "\t  7 |  5 |  3 \n"
        "\t____|____|____\n"
        "\t    |    |    \n"
        "\t  6 |  1 |  8 \n");    
    getchar();
}

int Exit(void)
{
    int key_pressed,position=0;
    
    MoveCursorTo(60,20);
    
    do
    {
        if(!position) printf("\r\tDESEJA REALMENTE SAIR [SIM] NAO");
            else if(position) printf("\r\tDESEJA REALMENTE SAIR SIM [NAO]");
        
        key_pressed = getch();

        if(key_pressed == KEY_LEFT || key_pressed == KEY_RIGHT) position = !position;

    }
    while(key_pressed != KEY_ENTER);

    puts("\r                                                                          ");
    
    if(!position)
        return true;
    
    return false;
}

int PlaAgain(void)
{
    int key_pressed,position=0;
    MoveCursorTo(60,20);
    
    do
    {
        if(!position) printf("\r\tDESEJA JOGAR NOVAMENTE [SIM] NAO");
            else if(position) printf("\r\tDESEJA JOGAR NOVAMENTE SIM [NAO]");
            
        key_pressed = getch();

        if(key_pressed == KEY_LEFT || key_pressed == KEY_RIGHT) position = !position;
    }
    while(key_pressed != KEY_ENTER);
    puts("\r                                                                          ");
    
    if(position) return false;
        else return true;
}

void Input(int *matrix)
{
    int key_pressed=0;
    int input;
    static int x = 0,y = 0, i = 2, j = 2;
    
    while(1)
    {
        MoveCursorTo(i,j);
        
        key_pressed = getch();
        
        if(key_pressed == KEY_ENTER)
        {
            MoveCursorTo(0,10);
            puts("-------------------\nDigite o numero desejado");
            
            MoveCursorTo(i,j);
            scanf("%d",&input);
            
            if( input > 9)
                input = 0;
            
            *(matrix + y*3 + x) = input;
                return;
        }

        if(key_pressed == KEY_DOWN && y==2){y=0;j = 2;}
            
        else if(key_pressed == KEY_UP && y == 0) {y = 2; j = 8;}
        
        else if(key_pressed == KEY_LEFT && x == 0){x = 2;i = 12;}
        
        else if(key_pressed == KEY_RIGHT && x == 2){x = 0; i = 2;}
        
        else if(key_pressed == KEY_UP){y--;j -= 3;} 
        
        else if(key_pressed == KEY_DOWN){ y++; j += 3;} 
        
        else if(key_pressed == KEY_LEFT){ x--; i -= 5;} 
        
        else if(key_pressed == KEY_RIGHT){x++; i += 5;}
        
        if(key_pressed == KEY_ESC)
        {
            if(Exit())
                exit(0);
        }            
    }

}


void DisplayMatrix(int *matrix)
{
   
   MoveCursorTo(0,2); 
   printf
    (
    "  %d |  %d |  %d \n"
    "____|____|____\n"
    "    |    |    \n"
    "  %d |  %d |  %d \n"
    "____|____|____\n"
    "    |    |    \n"
    "  %d |  %d |  %d \n\n",
    *(matrix),
    *(matrix + 1),
    *(matrix + 2),
    *(matrix + 3),
    *(matrix + 4),
    *(matrix + 5),
    *(matrix + 6),
    *(matrix + 7),
    *(matrix + 8)
    );
}

int CheckWin(int *matrix,int _sum)
{
    int i;
    int sum = 0;

    for( i = 0; i < 3 ; i++ )//VERIFICA AS SOMAS LINHAS
    {   
        
        sum = ( *(matrix + i*3 ) + *(matrix + i*3 + 1)+ *(matrix + i*3 + 2));

        if(sum != _sum)
            return false;
    }
    
    for(i = 0 ; i < 3 ; i++ )//VERIFICA AS SOMAS DAS COLUNAS
    {
        sum = ( *(matrix + i) + *(matrix + i + 3) + *(matrix + i +6 ) );
        
        if(sum != _sum)
            return false;
    }
    
    if( ( *(matrix) + *(matrix + 4) + *(matrix + 8) )!= _sum)
        return false;
    
     if( ( *(matrix+2) + *(matrix + 4) + *(matrix + 6) )!= _sum)
        return false;
    
    return true;
}

void Play( )
{
    int _sum = 15;
    int matrix[3][3];
    ClearMatrix((int*)matrix);
    
    while(true)
    {   
        ClearScreen();
        printf("A soma eh %d",_sum);
        DisplayMatrix((int*)matrix);
        Input((int*)matrix);
        DisplayMatrix((int*)matrix);
		
        if( CheckWin((int*)matrix,_sum) == true) 
        {
            Congratulations();
            if( PlaAgain())
                continue;
            
            else return;
        }
    }
}

void ClearMatrix(int *matrix)
{
    memset(matrix,0,9 * sizeof(int));
} 

void Help(void)
{
    ClearScreen();
    
    puts(
        "\nO seu objetivo no jogo do quadrado magico eh preenche-lo com numeros de maneira\n"
        "que as somas dos numeros na horizontal, vertical e diagonal seja igual ao valor\n"
        "especificado\n\n"
        "Use as setas para mover o cursor sobre o tabuleiro\n"
        "pressione [ENTER] na celula que deseja inserir o valor\n"
        "insira o valor e pressione [ENTER] novamente\n\n"
        "Para sair do game pressione [ESC]\n"
        );
    
    getchar();
}

void Menu(int _sum)
{
    int key_pressed,position=0,exit=false;
	
    while(!exit)
    {
        do
        {
            ClearScreen();
		
            if(!position)
                puts(
                    "\t\t\t JOGAR <-\n"
                    "\t\t\t AJUDA\n"
                    "\t\t\t CREDITOS\n"
                    "\t\t\t SOLUCAO\n"
                    "\t\t\t SAIR\n"
                    );
                    
            if(position == 1)
                puts(
                    "\t\t\t JOGAR\n"
                    "\t\t\t AJUDA <- \n"
                    "\t\t\t CREDITOS\n"
                    "\t\t\t SOLUCAO \n"
                    "\t\t\t SAIR   \n"
                    );
                    
            if(position == 2)
                puts(
                    "\t\t\t JOGAR \n"
                    "\t\t\t AJUDA \n"
                    "\t\t\t CREDITOS <-\n"
                    "\t\t\t SOLUCAO \n"
                    "\t\t\t SAIR   \n"
                    );
                    
            if(position == 3)
                puts(
                    "\t\t\t JOGAR \n"
                    "\t\t\t AJUDA \n"
                    "\t\t\t CREDITOS\n"
                    "\t\t\t SOLUCAO <- \n"
                    "\t\t\t SAIR   \n"
                    );
					
            if(position == 4)
                puts(
                    "\t\t\t JOGAR \n"
                    "\t\t\t AJUDA \n"
                    "\t\t\t CREDITOS\n"
                    "\t\t\t SOLUCAO \n"
                    "\t\t\t SAIR <-\n"
                    );
            
            key_pressed = getch();
		
            if(position == 0 && key_pressed == KEY_UP) position = 4;
            
            else if(position == 4 && key_pressed == KEY_DOWN) position = 0;
            
            else if(key_pressed == KEY_UP) position--;
            
            else if(key_pressed == KEY_DOWN) position++;
        }
        while(key_pressed != KEY_ENTER);
    
        switch(position)
        {
            case 0:
                Play(_sum);
                exit = 0;
                break;
            
            case 1:
                Help();
                break;
            
            case 2:
                Credits();
                break;
                
            case 3:
                Solution();
                break;
            
            case 4:
                exit = Exit();
                break;
        }
    }

}

void Congratulations(void)
{
    ClearScreen();
    printf("Parabens voce conseguiu \\0/ !\n");
}

void Presentation(void)
{
    ClearScreen();
    puts("                                   ___                            ___\n"         
    "                                  (   )                          (   )\n"        
    "  .--.     ___  ___    .---.    .-.| |   ___ .-.      .---.    .-.| |    .--.\n"   
    " /    \\   (   )(   )  / .-, \\  /   \\ |  (   )   \\    / .-, \\  /   \\ |   /    \\\n"
    "|  .-. '   | |  | |  (__) ; | |  .-. |   | ' .-. ;  (__) ; | |  .-. |  |  .-. ;\n"
    "| |  | |   | |  | |    .'`  | | |  | |   |  / (___)   .'`  | | |  | |  | |  | |\n"
    "| |  | |   | |  | |   / .'| | | |  | |   | |         / .'| | | |  | |  | |  | |\n"
    "| |  | |   | |  | |  | /  | | | |  | |   | |        | /  | | | |  | |  | |  | |\n"
    "| '  | |   | |  ; '  ; |  ; | | '  | |   | |        ; |  ; | | '  | |  | '  | |\n"
    "' `-'  |   ' `-'  /  ' `-'  | ' `-'  /   | |        ' `-'  | ' `-'  /  '  `-' /\n"
    " `._ / |    '.__.'   `.__.'_.  `.__,'   (___)       `.__.'_.  `.__,'    `.__.' \n"
    "    | |   \n"                                                                     
    "   (___) \n"                                                                      
        
        
    "                                 .-.\n"
    " ___ .-. .-.    .---.     .--.   ( __)   .--.      .--.\n"
    "(   )   '   \\  / .-, \\  /     \\  ('')  /     \\   /     \\\n"  
    "|  .-.  .-. ; (__) ; | ;  ,-. '  | |  |  .-. ;  |  .-. ;\n"
    "| |  | |  | |   .'`  | | |  | |  | |  |  |(___) | |  | |\n"
    "| |  | |  | |  / .'| | | |  | |  | |  |  |      | |  | |\n"
    "| |  | |  | | | /  | | | |  | |  | |  |  | ___  | |  | |\n"
    "| |  | |  | | ; |  ; | | '  | |  | |  |  '(   ) | '  | |\n"
    "| |  | |  | | ' `-'  | '  `-' |  | |  '  `-' |  '  `-' /\n"
    "(___)(___)(___)`.__.'_.  `.__. | (___)  `.__,'    `.__.'\n"
    "                        ( `-' ;\n"
    "                         `.__.\n"                       
    );   
     
    getchar();
}