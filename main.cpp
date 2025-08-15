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

void g_satc_02() { // global
    cout << "----- Sample Testcase 02 -----" << endl;
    Configuration * config = new Configuration("sa_tc_02_config");
    cout << config->str() << endl;
    delete config;
}

class TestDragonWar {
public:
    TestDragonWar() = default;

    static void satc_19() {
        cout << "\n----- Sample Testcase 19 -----" << endl;
        string config_path = "sa_tc_06_config";
        bool verbose = true;
        DragonWarriorsProgram * program = new DragonWarriorsProgram(config_path);
        cout << "------------------------------------------------------!BEGIN!------------------------------------------------------" << endl;
        cout << "Step:START--" << program->flyteam1->str() << "--|--" << program->flyteam2->str() << "--|--" << program->groundteam->str() << "--|--" << program->dragonlord->str() << endl;
        program->run(verbose);
        cout << "-------------------------------------------------------!END!-------------------------------------------------------" << endl;
        delete program;
    }

    static void satc_20() {
        cout << "\n----- Sample Testcase 20 -----" << endl;
        string config_path = "sa_tc_07_config";
        bool verbose = true;
        DragonWarriorsProgram * program = new DragonWarriorsProgram(config_path);
        cout << "------------------------------------------------------!BEGIN!------------------------------------------------------" << endl;
        cout << "Step:START--" << program->flyteam1->str() << "--|--" << program->flyteam2->str() << "--|--" << program->groundteam->str() << "--|--" << program->dragonlord->str() << endl;
        program->run(verbose);
        cout << "-------------------------------------------------------!END!-------------------------------------------------------" << endl;
        delete program;
    }

    static void satc_21() {
        cout << "\n----- Sample Testcase 21 -----" << endl;
        string config_path = "sa_tc_08_config";
        bool verbose = true;
        DragonWarriorsProgram * program = new DragonWarriorsProgram(config_path);
        cout << "------------------------------------------------------!BEGIN!------------------------------------------------------" << endl;
        cout << "Step:START--" << program->flyteam1->str() << "--|--" << program->flyteam2->str() << "--|--" << program->groundteam->str() << "--|--" << program->dragonlord->str() << endl;
        program->run(verbose);
        cout << "-------------------------------------------------------!END!-------------------------------------------------------" << endl;
        delete program;
    }

    static void satc_22() {
        cout << "\n----- Sample Testcase 22 -----" << endl;
        string config_path = "sa_tc_09_config";
        bool verbose = true;
        DragonWarriorsProgram * program = new DragonWarriorsProgram(config_path);
        cout << "------------------------------------------------------!BEGIN!------------------------------------------------------" << endl;
        cout << "Step:START--" << program->flyteam1->str() << "--|--" << program->flyteam2->str() << "--|--" << program->groundteam->str() << "--|--" << program->dragonlord->str() << endl;
        program->run(verbose);
        cout << "-------------------------------------------------------!END!-------------------------------------------------------" << endl;
        delete program;
    }
};

int main(int argc, const char * argv[]) {
    // cout << "----- Sample Testcase 01 -----" << endl;
    // MapElement * myMapCell_1 = new MapElement(ElementType::PATH, 0);
    // cout << "Cell_1 [type=" << myMapCell_1->getType() << ",req_dmg=" << myMapCell_1->getReqDmg() << "]" << endl;
    // delete myMapCell_1;
    // cout << "----- Sample Testcase 07 -----" << endl;
    // Map * map = new Map(5, 5, 0, nullptr, 0, nullptr);
    // FlyTeam * flyteam = new FlyTeam(1, "RRD", Position(0, 0), map, 100, 100);

    // cout << "Initial positions:" << endl;
    // cout << flyteam->getCurrentPosition().str() << endl;
    // cout << "After first move:" << endl;
    // flyteam->move();
    // cout << flyteam->getCurrentPosition().str() << endl;
    // cout << "After second move:" << endl;
    // flyteam->move();
    // cout << flyteam->getCurrentPosition().str() << endl;
    // cout << "Final Information" << endl;
    // cout  << flyteam->str() << endl;
        
    // delete flyteam;
    // delete map;
    // cout << "----- Sample Testcase 15 -----" << endl;
    // DragonWarriorsProgram * myProgram = new DragonWarriorsProgram("sa_tc_02_config");
    // cout << myProgram->flyteam1->str() << endl;
    // cout << myProgram->flyteam2->str() << endl;
    // cout << myProgram->groundteam->str() << endl;
    // cout << myProgram->dragonlord->str() << endl;
    // myProgram->run(true);
    // delete myProgram;

    TestDragonWar::satc_21();
    return 0;
}