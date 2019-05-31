#define BATT_NOW        "/sys/class/power_supply/BAT0/energy_now"
#define BATT_FULL       "/sys/class/power_supply/BAT0/energy_full"
#define BATT_STATUS       "/sys/class/power_supply/BAT0/status"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

char *
smprintf(char *fmt, ...)
{
	va_list fmtargs;
	char *buf = NULL;

	va_start(fmtargs, fmt);
	if (vasprintf(&buf, fmt, fmtargs) == -1){
		fprintf(stderr, "malloc vasprintf\n");
		exit(1);
    }
	va_end(fmtargs);

	return buf;
}

char *
getbattery(){
    long lnum1, lnum2 = 0;
    char *status = malloc(sizeof(char)*12);
    char s = '?';
    char c = '\x01';
    FILE *fp = NULL;
    if ((fp = fopen(BATT_NOW, "r"))) {
        fscanf(fp, "%ld\n", &lnum1);
        fclose(fp);
        fp = fopen(BATT_FULL, "r");
        fscanf(fp, "%ld\n", &lnum2);
        fclose(fp);
        fp = fopen(BATT_STATUS, "r");
        fscanf(fp, "%s\n", status);
        fclose(fp);
        if (strcmp(status,"Charging") == 0)
        {
            s = '+';
            c = '\x06';
        }
        if (strcmp(status,"Discharging") == 0)
        {
            s = '-';
            c = '\x05';
        }
        if (strcmp(status,"Full") == 0)
        {
            c = '\x06';
            s = '=';
        }
        return smprintf("%c \xF0\x9F\x94\x8B%ld%%", c, (lnum1/(lnum2/100)));
    }
    else return smprintf("");
}



