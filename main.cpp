/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 2
* Programming Fundamentals Summer 2025
* Date: 28.07.2025
*/

//The library here is concretely set, students are not allowed to include any other libraries.

#include "dragon.h"

using namespace std;

int main(int argc, const char * argv[]) {
	
    cout << "----- Sample Testcase 15 -----" << endl;
    DragonWarriorsProgram * myProgram = new DragonWarriorsProgram("sa_tc_01_config");
    cout << myProgram->flyteam1->str() << endl;
    cout << myProgram->flyteam2->str() << endl;
    delete myProgram;
    return 0;
}