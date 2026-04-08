#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

// Reads key press without needing ENTER
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
    // \e[?25l hides the cursor
    printf("\e[?25l");

    int selected = 0;
    int total = 6;
    int key;

    char *options[] = {
        "1. Reboot System",
        "2. Reboot to Bootloader",
        "3. Reboot to Recovery Mode",
        "4. Power Off",
	"5. Battery info",
        "6. Exit"
    };

    char *scripts[] = {
        "C: system(\"adb reboot\");",
        "C: system(\"adb reboot bootloader\");",
        "C: system(\"adb reboot recovery\");",
        "C: system(\"adb shell reboot -p\");",
	"C: system(\"adb shell dumpsys battery\");",
        "C: break;"
    };

    while(1) {
	system("clear");

        printf("\033[37;44m %-48s \033[0m\n", " ANDROID DEBUG BRIDGE PANEL - ADB ");
        printf("\n\033[0;33mUse ARROW KEYS to navigate and press ENTER:\033[0m\n"); 
	printf("\033[0;33mVersion-1.6.4 - SFAP - Simple and Fast ADB Panel\n");
        printf("\033[9m                                                  \033[0m\n");

        for(int i = 0; i < total; i++) {
            if(i == selected) {
                printf("\033[1;37;44m > %-45s \033[0m\n", options[i]);
            } else {
                printf("   \033[0;34m%-45s \033[0m\n", options[i]);
            }
        }
        // Moves print to line 20, column 1
        printf("\033[20;1H");

        printf("\n\n\033[1;30;47m %-48s \033[0m\n", " [ COMMAND SCRIPT: ] ");
        // Prints the script based on current selection in Bold White
        printf("\033[1;37m >> %s \033[0m\n", scripts[selected]);
        key = getch();

        if(key == 27) { // ESC sequence for arrow keys
            getch(); // skip [
            switch(getch()) {
                case 'A': if(selected > 0) selected--; break; // UP
                case 'B': if(selected < total - 1) selected++; break; // DOWN
            }
        }
	else if(key == 10) { // ENTER
            system("clear");
            printf("\033[1;32m[EXECUTING]: %s...\033[0m\n", options[selected]);

            if (selected == 0) {
                system("adb reboot");
            } else if (selected == 1) {
                system("adb reboot bootloader");
            } else if (selected == 2) {
                system("adb reboot recovery");
            } else if (selected == 3) {
                system("adb shell reboot -p");
            } else if (selected == 4) {
                system("adb shell dumpsys battery");
	    } else if (selected == 5) { 
                printf("Exiting system...\n");
                break;
            }

	  printf("\n\033[1;33mOperation completed! Press any key to continue...\033[0m");
            getch();
        } 
    }

    printf("\e[?25h");
    return 0;
}
