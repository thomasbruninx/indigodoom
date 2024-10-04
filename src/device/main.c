#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>


#include "doomdef.h"
#include "m_argv.h"
#include "d_main.h"

#define GPIO_DEVICE "/sys/class/gpio"

FILE *fp;

void initGPIO(int pinsc, char** pinsv) {
    char filename[256];
    int fd;

    printf("Initializing GPIO pin inputs\n");

    sprintf(filename, "%s/export", GPIO_DEVICE);

    fd = open(filename, O_WRONLY);
    if (fd == -1) {
        fprintf(stderr, "Unable to open %s\n", GPIO_DEVICE);
        exit(1);
    }

    for (int i = 0; i < pinsc; ++i) {
        int pinsv_len = strlen(pinsv[i]);
        if (write(fd, pinsv[i], pinsv_len) != pinsv_len) {
            fprintf(stderr, "Error exporting GPIO pin %s\n", pinsv[i]);
            exit(1);
        }
    }

    close (fd);

    for (int i = 0; i < pinsc; ++i) {
	sprintf(filename, "%s/gpio%s/direction", GPIO_DEVICE, pinsv[i]);

	fd = open(filename, O_WRONLY);
    	if (fd == -1){
        	printf("Cannot open direction of GPIO pin %s\n", pinsv[i]);
        	exit(1);
    	}

	if (write(fd, "in", 2) != 2) {
            fprintf(stderr, "Error setting direction of GPIO pin %s\n", pinsv[i]);
            exit(1);
        }
	close(fd);

    }

}

char getGPIO(char* pin) {
    char pinv = 0;
    char buffer[10];
    char filename[256];

    sprintf(filename, "%s/gpio%s/value", GPIO_DEVICE, pin);

    if ((fp = fopen(filename, "rb")) == NULL){
        printf("Cannot open value of GPIO pin %s\n", pin);
        exit(1);
    }

    fread(buffer, sizeof(char), sizeof(buffer)-1, fp);
    pinv = atoi(buffer);

    fclose(fp);

    return pinv;
}

void cleanGPIO(int pinsc, char** pinsv) {
    char filename[256];
    int fd;

    sprintf(filename, "%s/unexport", GPIO_DEVICE);

    fd = open(filename, O_WRONLY);
    if (fd == -1) {
        fprintf(stderr, "Unable to open %s\n", GPIO_DEVICE);
        exit(1);
    }

    for (int i = 0; i < pinsc; ++i) {
        int pinsv_len = strlen(pinsv[i]);
        if (write(fd, pinsv[i], pinsv_len) != pinsv_len) {
            fprintf(stderr, "Error unexporting GPIO pin %s\n", pinsv[i]);
            exit(1);
        }
    }

    close (fd);
}

static char* gpio_pins[5] = {"37", "38", "40", "41", "99"};
static char inputs[5] = {0, 0, 0, 0, 0};

int main(int argc, const char** argv)
{
    myargc = argc;
    myargv = argv;

    initGPIO(5, gpio_pins);

    // struct termios initial_settings, new_settings;
    // tcgetattr(0,&initial_settings);

    // new_settings = initial_settings;
    // new_settings.c_lflag &= ~ICANON;
    // new_settings.c_lflag &= ~ECHO;
    // new_settings.c_lflag &= ~ISIG;
    // new_settings.c_cc[VMIN] = 0;
    // new_settings.c_cc[VTIME] = 0;

    // tcsetattr(0, TCSANOW, &new_settings);

    D_DoomMain();

    // tcsetattr(0, TCSANOW, &initial_settings);

    return 0;
}

void I_StartTic (void)
{
    event_t event = {0,0,0,0};
    char keys[5] = {KEY_UPARROW, KEY_LEFTARROW, KEY_RIGHTARROW, KEY_ENTER, KEY_RCTRL};
    char temp_input = 0;

    // char key = getchar();
    // if (key != EOF)
    // {
    //          if (key == 'a') key = KEY_LEFTARROW;
    //     else if (key == 'd') key = KEY_RIGHTARROW;
    //     else if (key == 'w') key = KEY_UPARROW;
    //     else if (key == 's') key = KEY_DOWNARROW;

    //     event.type = ev_keydown;
    //     event.data1 = key;
    //     D_PostEvent(&event);

    //     event.type = ev_keyup;
    //     event.data1 = key;
    //     D_PostEvent(&event);
    // }


    for (int i = 0; i < 5; ++i) {
	temp_input = getGPIO(gpio_pins[i]);
    	if (inputs[i] != temp_input) {
		inputs[i] = temp_input;
		if (temp_input == 1) {
			event.type = ev_keydown;
		} else {
			event.type = ev_keyup;
		}
		event.data1 = keys[i];
		D_PostEvent(&event);

		// Button used to simulate ENTER also simulates SPACE
		if (keys[i] == KEY_ENTER) {
			event.data1 = 32;
			D_PostEvent(&event);
		}
	}
    }

}
