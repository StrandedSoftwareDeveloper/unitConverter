APPNAME=unitconverter
RAWDRAWANDROID=rawdrawandroid
CFLAGS:=-I. -ffunction-sections -Os -fvisibility=hidden
LDFLAGS:=-s
PACKAGENAME?=io.github.strandedsoftwaredeveloper.$(APPNAME)
SRC:=main.c

ANDROIDVERSION=30
ANDROIDTARGET=30


TARGETS:=makecapk/lib/x86_64/lib$(APPNAME).so #makecapk/lib/arm64-v8a/lib$(APPNAME).so makecapk/lib/armeabi-v7a/lib$(APPNAME).so # makecapk/lib/x86/lib$(APPNAME).so makecapk/lib/x86_64/lib$(APPNAME).so

include rawdrawandroid/Makefile
