#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

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

void menu_reboot() {
    int sub_sel = 0;
    int sub_total = 6;
    int sub_key;
    char *reboot_options[] = {
        "Back",
        "Reboot System",
        "Reboot to Recovery",
        "Reboot to Bootloader",
        "Reboot to Sideload",
        "Power off",
    };

    while(1) {
        system("clear");
        printf("\033[37;44m %-48s \033[0m\n", "          SFAP - REBOOT OPTIONS");
        printf("\n");

        for(int i = 0; i < sub_total; i++) {
            if(i == sub_sel) printf("\033[1;37;44m > %-45s \033[0m\n", reboot_options[i]);
            else printf("   \033[0;34m%-45s \033[0m\n", reboot_options[i]);
        }

        sub_key = getch();
        if(sub_key == 27) {
            getch(); 
            switch(getch()) {
                case 'A': if(sub_sel > 0) sub_sel--; break;
                case 'B': if(sub_sel < sub_total - 1) sub_sel++; break;
            }
        } else if(sub_key == 10) {
            if(sub_sel == 0) return;
            
            system("clear");
            if(sub_sel == 1) system("adb reboot");
            else if(sub_sel == 2) system("adb reboot recovery");
            else if(sub_sel == 3) system("adb reboot bootloader");
            else if(sub_sel == 4) system("adb reboot sideload");
            else if(sub_sel == 5) system("adb shell reboot -p");
            
            printf("\n\033[1;33mDone! Press any key...\033[0m");
            getch();
        }
    }
}

int main() {
    printf("\e[?25l");
    int selected = 0;
    int total = 6; // Reduced to 6 options
    int key;

    char *options[] = {
        "Exit",
        "Reboot Options",
        "Baterry Info",
        "CPU Info",
        "Mount /system/data",
        "Check Devices"
    };

    while(1) {
        system("clear");
        printf("\033[37;44m %-48s \033[0m\n", "                      SFAP");
        printf("\033[0;34mSFAP ADB Panel -- Connected to: ");
        fflush(stdout);
        system("adb shell getprop ro.product.model");
        printf("\n\033[0;33mUse ARROW KEYS to navigate and press ENTER:\033[0m\n");
        printf("\033[0;33mVersion-1.6.4 - SFAP - Simple and Fast ADB Panel\n");
        printf("\033[9m                                                  \033[0m\n");

        for(int i = 0; i < total; i++) {
            if(i == selected) printf("\033[1;37;44m > %-45s \033[0m\n", options[i]);
            else printf("   \033[0;34m%-45s \033[0m\n", options[i]);
        }

        key = getch();
        if(key == 27) {
            getch();
            switch(getch()) {
                case 'A': if(selected > 0) selected--; break;
                case 'B': if(selected < total - 1) selected++; break;
            }
        } else if(key == 10) {
            if (selected == 0) { system("clear"); break; }
            if (selected == 1) {
                menu_reboot();
            } else {
                system("clear");
                printf("\033[1;32m[EXECUTING]: %s...\033[0m\n", options[selected]);
                
                // Fixed mapping for the new main menu order
                if (selected == 2) { system("adb shell dumpsys battery"); }
                else if (selected == 3) { system("adb shell cat /proc/cpuinfo"); }
                else if (selected == 4) { system("adb shell mount | grep -E '/system|/data'"); }
                else if (selected == 5) { system("adb devices"); }

                printf("\n\033[1;33mOperation completed! Press any key...\033[0m");
                getch();
            }
        }
    }
    printf("\e[?25h");
    return 0;
}
