#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

// FUNCAO DO SALTOS PARA LER TECLA SEM ENTER
int getch() {
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

int main() {
// O código \e[?25l desliga o cursor!
printf("\e[?25l");

    int selecionado = 0;
    int total = 5; // Aumentamos para 5 opcoes agora!
    int tecla;
    char *opcoes[] = { 
        "1. Reiniciar",
        "2. Reiniciar no Bootloader",
        "3. Reiniciar no Recovery Mode",
        "4. Desligar ",
        "5. Sair"
    };

	char *scripts[] = {
    "C: system(\"adb reboot\");",
    "C: system(\"adb reboot bootloader\");",
    "C: system(\"adb reboot recovery\");",
    "C: system(\"adb shell reboot -p\");",
    "C: break;"
};

    while(1) {
        system("clear"); 
        
        printf("\033[37;44m %-48s \033[0m\n", " ANDROID DEBUG BRIDGE - Version - 1.5.0 ");
        printf("\n Navegue com as SETAS e confirme com ENTER:\n\n");

        for(int i = 0; i < total; i++) {
            if(i == selecionado) {
                // LINHA SELECIONADA (Fundo Azul, Texto Branco para dar contraste)
                printf("\033[1;37;44m > %-45s \033[0m\n", opcoes[i]);
            } else {
                // LINHAS NORMAIS (Fundo Preto, TEXTO AZUL!)
                printf("   \033[1;34m%-45s \033[0m\n", opcoes[i]);
            }
        }
printf("\033[20;1H");  //isso deixa o print do script na linhs 20 da coluna 1!

	printf("\n\n\033[1;30;47m %-48s \033[0m\n", " [ SCRIPT DO COMANDO: ] "); 
// Imprime a descrição baseada no item que você selecionou agora!
	// O código \033[1;37m deixa o texto Branco em Negrito!
	printf("\033[1;37m >> %s \033[0m\n", scripts[selecionado]);

        tecla = getch();

        if(tecla == 27) { 
            getch(); // pula o [
            switch(getch()) {
                case 'A': if(selecionado > 0) selecionado--; break; // Cima
                case 'B': if(selecionado < total - 1) selecionado++; break; // Baixo
            }
        } 
        else if(tecla == 10) { // ENTER
            system("clear");
            printf("\033[1;32m[EXECUTANDO]: %s...\033[0m\n", opcoes[selecionado]);
            
            // LOGICA DOS COMANDOS ADB
            if (selecionado == 0) {
                system("adb reboot");
            } else if (selecionado == 1) {
                system("adb reboot bootloader");
            } else if (selecionado == 2) {
                system("adb reboot recovery");
            } else if (selecionado == 3) {
                system("adb shell reboot -p");
            } else if (selecionado == 4) {
                printf("Saindo do sistema...\n");
                break; 
            }
            
            printf("\n\033[1;33mOperacao concluida! Pressione qualquer tecla...\033[0m");
            getch(); 
        }
    }
// O código \e[?25h liga o cursor de volta!
printf("\e[?25h");
    return 0;
}
