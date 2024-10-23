//Adapted from the rawdrawandroidexample test.c
//Copyright (c) 2011-2020 <>< Charles Lohr - Under the MIT/x11 or NewBSD License you choose.
// NO WARRANTY! NO GUARANTEE OF SUPPORT! USE AT YOUR OWN RISK
// Super basic test - see rawdrawandroid's thing for a more reasonable test.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <string.h>
#include "os_generic.h"
#include <GLES3/gl3.h>
#include <asset_manager.h>
#include <asset_manager_jni.h>
#include <android_native_app_glue.h>
#include <android/log.h>
#include <android/sensor.h>
#include "CNFGAndroid.h"

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define CNFG3D
#define CNFG_IMPLEMENTATION
#include "CNFG.h"

typedef struct {
    char *name;
    double coefficient;
    double bias;
} Unit;

typedef struct {
    char *name;
    Unit *units;
} Category;

unsigned frames = 0;
unsigned long iframeno = 0;

volatile int suspended;

short screenx, screeny;
int lastbuttonx = 0;
int lastbuttony = 0;
int lastmotionx = 0;
int lastmotiony = 0;
int lastbid = 0;
int lastmask = 0;
int lastkey, lastkeydown;

static int keyboard_up;

Category *categories = NULL;
size_t currentCategoryIndex = 0;

void HandleKey( int keycode, int bDown ) {
	lastkey = keycode;
	lastkeydown = bDown;
	if( keycode == 4 ) { AndroidSendToBack( 1 ); }
}

void HandleButton( int x, int y, int button, int bDown ) {
	lastbid = button;
	lastbuttonx = x;
	lastbuttony = y;
}

void HandleMotion( int x, int y, int mask ) {
	lastmask = mask;
	lastmotionx = x;
	lastmotiony = y;
}

extern struct android_app * gapp;


int HandleDestroy() {
    for (int c=0; c<arrlen(categories); c++) {
        free(categories[c].name);
        for (int u=0; u<arrlen(categories[c].units); u++) {
            free(categories[c].units[u].name);
        }
        arrfree(categories[c].units);
    }
    arrfree(categories);
}

void HandleSuspend() {
	suspended = 1;
}

void HandleResume() {
	suspended = 0;
}

void loadUnits() {
    FILE *unitsFile = fopen("/storage/emulated/0/Documents/units.txt", "r");
    if (unitsFile == NULL) {
        fprintf(stderr, "Failed to open units.txt!\n");
        return;
    }

    fseek(unitsFile, 0, SEEK_END);
    size_t size = ftell(unitsFile);
    fseek(unitsFile, 0, SEEK_SET);

    char *data = malloc(size+1);

    fread(data, 1, size, unitsFile);
    data[size] = '\0';
    fclose(unitsFile);

    char *line = strtok(data, "\n");
    while (line) {
        if (line[0] == ' ' || line[0] == '\t') { //Unit listing
            Unit unit;

            for (; *line != '\0' && *line != '\n' && *line == ' '; line++) {} //Quick and dirty whitespace trim

            char *nameStart = line;
            for (; *line != '\0' && *line != '\n' && *line != '='; line++) {} //find the first '='
            size_t len = line-nameStart;

            unit.name = malloc(len+1);
            memcpy(unit.name, nameStart, len);
            unit.name[len] = '\0';
            if (*(++line) != '*') {
                fprintf(stderr, "Expected '*', found '%c'\n", *line);
            }

            char *end;
            unit.coefficient = strtod(++line, &end);
            line = end;
            if (*line != '+') {
                fprintf(stderr, "Expected '+', found '%c'\n", *line);
            }

            unit.bias = strtod(++line, &end);
            arrput(categories[currentCategoryIndex].units, unit);
        } else { //Category listing
            Category c = {.units = NULL};
            c.name = malloc(strlen(line)+1);
            memcpy(c.name, line, strlen(line));
            c.name[strlen(line)] = '\0';
            arrput(categories, c);
            currentCategoryIndex = arrlen(categories)-1;
        }
        line = strtok(NULL, "\n");
    }

    free(data);
}

int main() {
    fprintf(stderr, "Starting Unit Converter\n");
	int i, x, y;
	double ThisTime;
	double LastFPSTime = OGGetAbsoluteTime();
	double LastFrameTime = OGGetAbsoluteTime();
	double SecToWait;
	int linesegs = 0;

	CNFGBGColor = 0x000080ff;
	CNFGSetupFullscreen("Unit Converter", 0);

    fprintf(stderr, "Loading units...\n");
    loadUnits();
    fprintf(stderr, "Done loading units!\n");

    for (int c=0; c<arrlen(categories); c++) {
        fprintf(stderr, "%s:\n", categories[c].name);
        for (int u=0; u<arrlen(categories[c].units); u++) {
            fprintf(stderr, "\t%s: coefficient: %lf, bias: %lf\n", categories[c].units[u].name, categories[c].units[u].coefficient, categories[c].units[u].bias);
        }
    }

	while(1)
	{
		int i, pos;
		float f;
		iframeno++;
		RDPoint pto[3];

		CNFGHandleInput();

		if( suspended ) { usleep(50000); continue; }

		CNFGClearFrame();
		CNFGColor( 0xffffffff );
		CNFGGetDimensions( &screenx, &screeny );



		frames++;
		CNFGSwapBuffers();

		ThisTime = OGGetAbsoluteTime();
		if( ThisTime > LastFPSTime + 1 )
		{
			printf( "FPS: %d\n", frames );
			frames = 0;
			linesegs = 0;
			LastFPSTime+=1;
		}
	}

	return(0);
}


