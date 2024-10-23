//Copyright (c) 2011-2020 <>< Charles Lohr - Under the MIT/x11 or NewBSD License you choose.
// NO WARRANTY! NO GUARANTEE OF SUPPORT! USE AT YOUR OWN RISK
// Super basic test - see rawdrawandroid's thing for a more reasonable test.

#include <stdio.h>
#include <stdlib.h>
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

#define CNFG3D
#define CNFG_IMPLEMENTATION
#include "CNFG.h"

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
}

void HandleSuspend() {
	suspended = 1;
}

void HandleResume() {
	suspended = 0;
}

int main() {
	int i, x, y;
	double ThisTime;
	double LastFPSTime = OGGetAbsoluteTime();
	double LastFrameTime = OGGetAbsoluteTime();
	double SecToWait;
	int linesegs = 0;

	CNFGBGColor = 0x000080ff;
	CNFGSetupFullscreen("Unit Converter", 0);

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


