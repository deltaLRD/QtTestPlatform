#include <iostream>
#include"Datastruct.h"
#include"ai_pve.cpp"
#include"ai_pvp_ai1.cpp"
#include"ai_pvp_ai2.cpp"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
int main(int argc, char** argv) {
	DataStruct data;
    KeyStruct key;
    ai_pve(&data,&key);
    ai_pvp_ai1(&data,&key);
    ai_pvp_ai2(&data,&key);
	return 0;
}
