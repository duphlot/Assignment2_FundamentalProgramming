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
    cout << "----- Sample Testcase 11 -----" << endl;
Configuration * config = new Configuration("sa_tc_01_config");
cout << config->getFlyTeam1InitHP() << endl;
cout << config->str() << endl;
delete config;
    return 0;
}