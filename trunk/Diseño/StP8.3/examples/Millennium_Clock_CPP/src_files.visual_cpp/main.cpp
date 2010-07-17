#include <stdio.h>
#include <string.h>

//#ACD# M(UDIF) Include Files

#include "Weight.h"

//#end ACD#

void Applic_Init() {

    //#ACD# M(UDSC) StartUp Code

        // user defined code to be added here ...

    //#end ACD#
}

int main(int argc, char * argv) {

    Applic_Init();

    //#ACD# M(UDMC) Main Program Code

    Weight * start_class = new Weight();
	start_class->takeEvent(Weight::raise);

    //#end ACD#
    return 0;
}
