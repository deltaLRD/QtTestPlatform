#include <iostream>
#include "core.h"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char** argv) {
	time_t gameTime=60;
    Core* core=new Core();
    core->initial(Core::PVP,2,true,gameTime);
	return 0;
}
