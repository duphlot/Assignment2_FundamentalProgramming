#include "dragons.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// 3.1 
MapElement::MapElement(ElementType t, int r): type(t), req_dmg(r) {}
MapElement::~MapElement() {}
ElementType MapElement::getType() const { return type; }
int MapElement::getReqDmg() const {return req_dmg; }

// 3.2
Map::Map(int rows, int cols, int num_obst, Position *obst, int num_gro_obst, Position *gro_obst): num_rows(rows), num_cols(cols) {
    grid = new MapElement**[num_rows];
    for (int i = 0; i < num_rows; ++i) {
        grid[i] = new MapElement*[num_cols];
        for (int j = 0; j < num_cols; ++j) {
            grid[i][j] = new MapElement(PATH);
        }
    }
    
    for (int i = 0; i < num_obst; ++i) {
        grid[obst[i].getRow()][obst[i].getCol()] = new MapElement(OBSTACLE);
    }
    
    for (int i = 0; i < num_gro_obst; ++i) {
        grid[gro_obst[i].getRow()][gro_obst[i].getCol()] = new MapElement(GROUND_OBSTACLE);
    }
}
Map::~Map() {
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            delete grid[i][j];
        }
        delete[] grid[i];
    }
    delete[] grid;
}
bool Map::isValid(const Position & pos, MovingObject * map) const {
    if (pos.getRow() < 0 || pos.getRow() >= num_rows || pos.getCol() < 0 || pos.getCol() >= num_cols) {
        return false;
    }
    ElementType type = grid[pos.getRow()][pos.getCol()]->getType();
    return (type == PATH || type == GROUND_OBSTACLE);
}
int Map::getNumRows() const { return num_rows; }
int Map::getNumCols() const { return num_cols; }


// 3.3
Position::Position(int r, int c): r_(r), c_(c) {}
Position::Position(const string &str_pos) {
    size_t comma_pos = str_pos.find(',');
    if (comma_pos != string::npos) {
        r_ = stoi(str_pos.substr(0, comma_pos));
        c_ = stoi(str_pos.substr(comma_pos + 1));
    } else {
        r_ = 0;
        c_ = 0;
    }
}
int Position::getRow() const { return r_; }
int Position::getCol() const { return c_; }
void Position::setRow(int r) { r_ = r; }
void Position::setCol(int c) { c_ = c; }
string Position::str() const {
    return "(" + to_string(r_) + "," + to_string(c_) + ")";
}
bool Position::isEqual(int in_r, int in_c) const {
    return (r_ == in_r && c_ == in_c);
}

// 3.4
MovingObject::MovingObject(int index, const Position & pos, Map *map, const string & name)
    : index(index), pos(pos), map(map), name(name) {}
MovingObject::~MovingObject() {}
Position MovingObject::getCurrentPosition() const { return pos; }
Position MovingObject::getNextPosition() { return Position::npos; }
void MovingObject::move() {
    Position next_pos = getNextPosition();
    if (map->isValid(next_pos, this)) {
        pos = next_pos;
    }
}
string MovingObject::str() const {
    return name + " at " + pos.str();
}

// 3.5
Warrior::Warrior(int index, const Position & pos, Map * map,
                 const string & name, int hp, int damage)
    : MovingObject(index, pos, map, name), hp(hp), damage(damage) {}
Warrior::~Warrior() {}
int Warrior::getHp() const { return hp; }
int Warrior::getDamage() const { return damage; }
void Warrior::setHp(int new_hp) { hp = new_hp; }
void Warrior::setDamage(int new_damage) { damage = new_damage; }

// 3.6 
FlyTeam::FlyTeam(int index, const string & moving_rule,
                 const Position & pos, Map * map, int hp, int damage)
    : Warrior(index, pos, map, "FlyTeam", hp, damage), moving_rule(moving_rule) {}
string FlyTeam::getMovingRule() const { return moving_rule; }
Position FlyTeam::getNextPosition() const {
    Position next_pos = pos;
    //
}
void FlyTeam::move() {
    Position next_pos = getNextPosition();
    if (map->isValid(next_pos, this)) {
        pos = next_pos;
    }
}
string FlyTeam::str() const {
    return "FlyTeam[index=" + to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule + "]";
}
bool FlyTeam::attack() {
    // TODO

}

// 3.7
GroundTeam::GroundTeam(int index, const string & moving_rule,
                       const Position & pos, Map * map, int hp, int damage)
    : Warrior(index, pos, map, "GroundTeam", hp, damage), moving_rule(moving_rule) {}
Position GroundTeam::getNextPosition() const {

}
void GroundTeam::move() {
    Position next_pos = getNextPosition();
    if (map->isValid(next_pos, this)) {
        pos = next_pos;
    }
}
string GroundTeam::str() const {

}
int GroundTeam::getTrapTurns() const {
    // TODO
}
void GroundTeam::setTrapTurns(int turns) {

}

// 3.8
DragonLord::DragonLord(int index, const Position & pos, Map * map,
                       FlyTeam *flyteam1, FlyTeam *flyteam2)
    : MovingObject(index, pos, map, "DragonLord"), flyteam1(flyteam1), flyteam2(flyteam2) {}
Position DragonLord::getNextPosition() const {

}
void DragonLord::move() {
    Position next_pos = getNextPosition();
    if (map->isValid(next_pos, this)) {
        pos = next_pos;
    }
}
string DragonLord::str() const {
}
////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////