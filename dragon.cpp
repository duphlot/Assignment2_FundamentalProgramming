#include "dragon.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// 3.1 

MapElement::MapElement(ElementType type, int in_req_dmg): type(type), req_dmg(in_req_dmg) {   
}
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
bool Map::isValid(const Position & pos, MovingObject * obj) const {
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
        string working_str = str_pos;
        
        if (!working_str.empty() && working_str[0] == '(') {
            working_str = working_str.substr(1);
            comma_pos--; 
        }
        if (!working_str.empty() && working_str.back() == ')') {
            working_str = working_str.substr(0, working_str.length() - 1);
        }
        
        comma_pos = working_str.find(',');
        if (comma_pos != string::npos) {
            r_ = stoi(working_str.substr(0, comma_pos));
            c_ = stoi(working_str.substr(comma_pos + 1));
        } else {
            r_ = 0;
            c_ = 0;
        }
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

const Position Position::npos(-1, -1);

bool operator!=(const Position& lhs, const Position& rhs) {
    return lhs.getRow() != rhs.getRow() || lhs.getCol() != rhs.getCol();
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
    : MovingObject(index, pos, map, name), hp(hp), damage(damage) {
        this->hp = max(this->hp, 0);
        this->hp = min(this->hp, 500);

        this->damage = max(this->damage, 0);
        this->damage = min(this->damage, 900);
    }
Warrior::~Warrior() {}
int Warrior::getHp() const { return hp; }
int Warrior::getDamage() const { return damage; }
void Warrior::setHp(int new_hp) { 
    if (new_hp < 0) hp = 0;
    else if (new_hp > 500) hp = 500;
    else hp = new_hp;
}
void Warrior::setDamage(int new_damage) { 
    if (new_damage < 0) damage = 0;
    else if (new_damage > 900) damage = 900;
    else damage = new_damage;
}


BaseBag* Warrior::getBag() const {
    // TODO: Implement this method
    return nullptr;
}

// 3.6 
FlyTeam::FlyTeam(int index, const string & moving_rule,
                 const Position & pos, Map * map, int hp, int damage)
    : Warrior(index, pos, map, "FlyTeam", hp, damage), moving_rule(moving_rule), moving_index() {
        hp = max(hp, 0);
        hp = min(hp, 500);

        damage = max(damage, 0);
        damage = min(damage, 900);
    }
string FlyTeam::getMovingRule() const { return moving_rule; }
Position FlyTeam::getNextPosition() {
    if (moving_rule.empty()) return Position::npos;
    
    char direction = moving_rule[moving_index];
    //xxcout<<moving_index<<" "<<direction<<endl;
    Position next_pos = pos;
    if (direction == 'U') {
        next_pos.setRow(pos.getRow() - 1);
    } else if (direction == 'D') {
        next_pos.setRow(pos.getRow() + 1);
    } else if (direction == 'L') {
        next_pos.setCol(pos.getCol() - 1);
    } else if (direction == 'R') {
        next_pos.setCol(pos.getCol() + 1);
    } else return Position::npos; 
    return next_pos;
}
void FlyTeam::move() {
    for (int i = 0; i < moving_rule.size(); ++i) {
        moving_index %= moving_rule.size();
        Position next_pos = getNextPosition();
        if (map->isValid(next_pos, this)) {
            pos = next_pos;
            moving_index = (moving_index + 1) % moving_rule.size();
        }
    }
}
string FlyTeam::str() const {
    int tempIndex = max(index,1);
    tempIndex = min(tempIndex, 2);
    return "FlyTeam" + to_string(tempIndex) + "[index=" + to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule + "]";
}
bool FlyTeam::attack(DragonLord *dragonlord) {
    if (dragonlord == nullptr) return false;
    
    Position dragonlord_pos = dragonlord->getPosition();
    Position flyteam_pos = getCurrentPosition();
    
    if (flyteam_pos.isEqual(dragonlord_pos.getRow(), dragonlord_pos.getCol())) {
        return true;
    }
    return false;
}

// 3.7
GroundTeam::GroundTeam(int index, const string & moving_rule,
                       const Position & pos, Map * map, int hp, int damage)
    : Warrior(index, pos, map, "GroundTeam", hp, damage), moving_rule(moving_rule), moving_index() {
        this->hp = max(this->hp, 0);
        this->hp = min(this->hp, 500);

        this->damage = max(this->damage, 0);
        this->damage = min(this->damage, 900);
    }
Position GroundTeam::getNextPosition() {
    if (moving_rule.empty()) return Position::npos;
    
    char direction = moving_rule[moving_index];
    Position next_pos = pos;
    if (direction == 'U') {
        next_pos.setRow(pos.getRow() - 1);
    } else if (direction == 'D') {
        next_pos.setRow(pos.getRow() + 1);
    } else if (direction == 'L') {
        next_pos.setCol(pos.getCol() - 1);
    } else if (direction == 'R') {
        next_pos.setCol(pos.getCol() + 1);
    } else return Position::npos; 
    return next_pos;
}
void GroundTeam::move() {
    for (int i = 0; i < moving_rule.size(); ++i) {
        moving_index %= moving_rule.size();
        Position next_pos = getNextPosition();
        if (map->isValid(next_pos, this)) {
            pos = next_pos;
            moving_index = (moving_index + 1) % moving_rule.size();
        }
    }
}
string GroundTeam::str() const {
    int tempIndex = max(index,1);
    tempIndex = min(tempIndex, 2);
    return "GroundTeam[index=" + to_string(tempIndex) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule + "]";
}

bool GroundTeam::trap(DragonLord *dragonlord) {
    if (dragonlord == nullptr) return false;

    Position dragonlord_pos = dragonlord->getPosition();
    Position groundteam_pos = getCurrentPosition();

    if (groundteam_pos.isEqual(dragonlord_pos.getRow(), dragonlord_pos.getCol())) {
        return true;
    }
    return false;
}

int GroundTeam::getTrapTurns() const {
    return moving_index;
}
void GroundTeam::setTrapTurns(int turns) {
    moving_index = max(turns, 0);
}

// 3.8
DragonLord::DragonLord(int index, const Position & pos, Map * map,
                       FlyTeam *flyteam1, FlyTeam *flyteam2, GroundTeam *ground_team)
    : MovingObject(index, pos, map, "DragonLord"), flyteam1(flyteam1), flyteam2(flyteam2) {}

Position DragonLord::getPosition() const {
    int x = abs(flyteam1->getCurrentPosition().getRow() - flyteam2->getCurrentPosition().getRow());
    int y = abs(flyteam1->getCurrentPosition().getCol() - flyteam2->getCurrentPosition().getCol());
    return Position(x, y);
}

int DragonLord::manhattanDistance() const {
    Position pos1 = flyteam1->getCurrentPosition();
    Position pos2 = flyteam2->getCurrentPosition();
    return abs(pos1.getRow() - pos2.getRow()) + abs(pos1.getCol() - pos2.getCol());
}

Position DragonLord::getNextPosition() {
    if (manhattanDistance() > 5) {
        Position next_pos = getPosition();
        return next_pos;
    } else {
        Position cur_pos = getPosition();
        Position next_pos(cur_pos.getCol(), cur_pos.getRow());
        return next_pos;
    }
}

// cai nay sai nha, de cho do trong
void DragonLord::move() {
    Position next_pos = getNextPosition();
    if (map->isValid(next_pos, this)) {
        pos = next_pos;
    }
}

string DragonLord::str() const {
    return "DragonLord[index=" + to_string(index) + ";pos=" + pos.str() + "]";
}

// 3.9
ArrayMovingObject::ArrayMovingObject(int capacity)
    : count(0), capacity(capacity) {
    arr_mv_objs = new MovingObject*[capacity];
}
ArrayMovingObject::~ArrayMovingObject() {
    delete[] arr_mv_objs;
}
bool ArrayMovingObject::isFull() const {
    return count >= capacity;
}
bool ArrayMovingObject::add(MovingObject *mv_obj) {
    if (isFull()) {
        return false;
    }
    arr_mv_objs[count++] = mv_obj;
    return true;
}
string ArrayMovingObject::str() const {
    string result = "ArrayMovingObject[";
    result += "count=" + to_string(count) + ";capacity=" + to_string(capacity) + ";";
    for (int i = 0; i < count; ++i) {
        result += arr_mv_objs[i]->str();
        if (i < count - 1) {
            result += ";";
        }
    }
    result += "]";
    return result;
}

// 3.10
Configuration::Configuration(const string & filepath): map_num_rows(0), map_num_cols(0), max_num_moving_objects(0),
    num_obstacles(0), arr_obstacles(nullptr), num_ground_obstacles(0),
    arr_ground_obstacles(nullptr), flyteam1_moving_rule(""), flyteam1_init_pos(),
    flyteam1_init_hp(0), flyteam1_init_damage(0), flyteam2_moving_rule(""),
    flyteam2_init_pos(), flyteam2_init_hp(0), flyteam2_init_damage(0),
    groundteam_moving_rule(""), groundteam_init_pos(), groundteam_init_hp(0),
    groundteam_init_damage(0), dragonlord_init_pos(), num_steps(0) {
    
    ifstream file(filepath);
    
    if (!file.is_open()) {
        cerr << "Error opening configuration file: " << filepath << endl;
        return;
    }
    string line;
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        size_t equal_pos = line.find('=');
        if (equal_pos == string::npos) continue;
        
        string key = line.substr(0, equal_pos);
        string value = line.substr(equal_pos + 1);
        
        if (key == "MAP_NUM_ROWS") {
            if (!value.empty()) map_num_rows = stoi(value);
        } else if (key == "MAP_NUM_COLS") {
            if (!value.empty()) map_num_cols = stoi(value);
        } else if (key == "MAX_NUM_MOVING_OBJECTS") {
            if (!value.empty()) max_num_moving_objects = stoi(value);
        } else if (key == "NUM_OBSTACLE") {
            if (!value.empty()) num_obstacles = stoi(value);
        } else if (key == "ARRAY_OBSTACLE") {
            if (num_obstacles == 0) {
                num_obstacles = countPositionsInArray(value);
            }
            if (num_obstacles > 0) {
                arr_obstacles = new Position[num_obstacles];
                parsePositionArray(value, arr_obstacles, num_obstacles);
            }
        } else if (key == "NUM_GROUND_OBSTACLE") {
            if (!value.empty()) num_ground_obstacles = stoi(value);
        } else if (key == "ARRAY_GROUND_OBSTACLE") {
            if (num_ground_obstacles == 0) {
                num_ground_obstacles = countPositionsInArray(value);
            }
            if (num_ground_obstacles > 0) {
                arr_ground_obstacles = new Position[num_ground_obstacles];
                parsePositionArray(value, arr_ground_obstacles, num_ground_obstacles);
            }
        } else if (key == "FLYTEAM1_MOVING_RULE") {
            flyteam1_moving_rule = value;
        } else if (key == "FLYTEAM1_INIT_POS") {
            flyteam1_init_pos = Position(removeParentheses(value));
        } else if (key == "FLYTEAM1_INIT_HP") {
            if (!value.empty()) flyteam1_init_hp = stoi(value);
        } else if (key == "FLYTEAM1_INIT_DMG") {
            if (!value.empty()) flyteam1_init_damage = stoi(value);
        } else if (key == "FLYTEAM1_INIT_DAMAGE") {
            if (!value.empty()) flyteam1_init_damage = stoi(value);
        } else if (key == "FLYTEAM2_MOVING_RULE") {
            flyteam2_moving_rule = value;
        } else if (key == "FLYTEAM2_INIT_POS") {
            flyteam2_init_pos = Position(removeParentheses(value));
        } else if (key == "FLYTEAM2_INIT_HP") {
            if (!value.empty()) flyteam2_init_hp = stoi(value);
        } else if (key == "FLYTEAM2_INIT_DMG") {
            if (!value.empty()) flyteam2_init_damage = stoi(value);
        } else if (key == "FLYTEAM2_INIT_DAMAGE") {
            if (!value.empty()) flyteam2_init_damage = stoi(value);
        } else if (key == "GROUNDTEAM_MOVING_RULE") {
            groundteam_moving_rule = value;
        } else if (key == "GROUNDTEAM_INIT_POS") {
            groundteam_init_pos = Position(removeParentheses(value));
        } else if (key == "GROUNDTEAM_INIT_HP") {
            if (!value.empty()) groundteam_init_hp = stoi(value);
        } else if (key == "GROUNDTEAM_INIT_DMG") {
            if (!value.empty()) groundteam_init_damage = stoi(value);
        } else if (key == "GROUNDTEAM_INIT_DAMAGE") {
            if (!value.empty()) groundteam_init_damage = stoi(value);
        } else if (key == "DRAGONLORD_INIT_POS") {
            dragonlord_init_pos = Position(removeParentheses(value));
        } else if (key == "NUM_STEPS") {
            if (!value.empty()) num_steps = stoi(value);
        }
    }
    
    file.close();
}

int Configuration::countPositionsInArray(const string& value) {
    string processed = value;
    
    if (!processed.empty() && processed[0] == '[') {
        processed = processed.substr(1);
    }
    if (!processed.empty() && processed.back() == ']') {
        processed = processed.substr(0, processed.length() - 1);
    }
    
    if (processed.empty()) {
        return 0;
    }
    
    int count = 1;
    for (char c : processed) {
        if (c == ';') {
            count++;
        }
    }
    return count;
}

void Configuration::parsePositionArray(const string& value, Position* arr, int count) {
    string processed = value;

    if (!processed.empty() && processed[0] == '[') {
        processed = processed.substr(1);
    }
    if (!processed.empty() && processed.back() == ']') {
        processed = processed.substr(0, processed.length() - 1);
    }
    
    int pos_index = 0;
    size_t start = 0;
    
    while (start < processed.length() && pos_index < count) {
        size_t end = processed.find(';', start);
        if (end == string::npos) {
            end = processed.length();
        }
        
        string pos_str = processed.substr(start, end - start);
        
        if (!pos_str.empty() && pos_str[0] == '(') {
            pos_str = pos_str.substr(1);
        }
        if (!pos_str.empty() && pos_str.back() == ')') {
            pos_str = pos_str.substr(0, pos_str.length() - 1);
        }
        
        size_t comma_pos = pos_str.find(',');
        if (comma_pos != string::npos) {
            int row = stoi(pos_str.substr(0, comma_pos));
            int col = stoi(pos_str.substr(comma_pos + 1));
            arr[pos_index] = Position(row, col);
        }
        
        pos_index++;
        start = end + 1;
    }
}

string Configuration::removeParentheses(const string& value) {
    string result = value;
    if (!result.empty() && result[0] == '(') {
        result = result.substr(1);
    }
    if (!result.empty() && result.back() == ')') {
        result = result.substr(0, result.length() - 1);
    }
    return result;
}

Configuration::~Configuration() {
    delete[] arr_obstacles;
    delete[] arr_ground_obstacles;
}
string Configuration::str() const {
    string result = "Configuration[\n";
    result += "MAP_NUM_ROWS=" + to_string(map_num_rows) + "\n";
    result += "MAP_NUM_COLS=" + to_string(map_num_cols) + "\n";
    result += "MAX_NUM_MOVING_OBJECTS=" + to_string(max_num_moving_objects) + "\n";
    result += "NUM_OBSTACLE=" + to_string(num_obstacles) + "\n";
    result += "ARRAY_OBSTACLE=[";
    for (int i = 0; i < num_obstacles; ++i) {
        result += arr_obstacles[i].str();
        if (i < num_obstacles - 1) {
            result += ";";
        }
    }
    result += "]\n";
    result += "NUM_GROUND_OBSTACLE=" + to_string(num_ground_obstacles) + "\n";
    result += "ARRAY_GROUND_OBSTACLE=[";
    for (int i = 0; i < num_ground_obstacles; ++i) {
        result += arr_ground_obstacles[i].str();
        if (i < num_ground_obstacles - 1) {
            result += ";";
        }
    }
    result += "]\n";
    result += "FLYTEAM1_MOVING_RULE=" + flyteam1_moving_rule + "\n";
    result += "FLYTEAM1_INIT_POS=" + flyteam1_init_pos.str() + "\n";
    result += "FLYTEAM1_INIT_HP=" + to_string(flyteam1_init_hp) + "\n";
    result += "FLYTEAM1_INIT_DAMAGE=" + to_string(flyteam1_init_damage) + "\n";
    result += "FLYTEAM2_MOVING_RULE=" + flyteam2_moving_rule + "\n";
    result += "FLYTEAM2_INIT_POS=" + flyteam2_init_pos.str() + "\n";
    result += "FLYTEAM2_INIT_HP=" + to_string(flyteam2_init_hp) + "\n";
    result += "FLYTEAM2_INIT_DAMAGE=" + to_string(flyteam2_init_damage) + "\n";
    result += "GROUNDTEAM_MOVING_RULE=" + groundteam_moving_rule + "\n";
    result += "GROUNDTEAM_INIT_POS=" + groundteam_init_pos.str() + "\n";
    result += "GROUNDTEAM_INIT_HP=" + to_string(groundteam_init_hp) + "\n";
    result += "GROUNDTEAM_INIT_DAMAGE=" + to_string(groundteam_init_damage) + "\n";
    result += "DRAGONLORD_INIT_POS=" + dragonlord_init_pos.str() + "\n";
    result += "NUM_STEPS=" + to_string(num_steps)+ "\n";
    result += "]";
    return result;
}
//3.11

// 3.12
SmartDragon::SmartDragon(int index, const Position & init_pos, Map * map, 
                         DragonType type, MovingObject *obj, int damage)
    : MovingObject(index, init_pos, map, "SmartDragon"), smartdragon_type(type), 
      damage(damage), item(nullptr), target(obj), target_pos(init_pos) {
        this->damage = max(this->damage, 0);
        this->damage = min(this->damage, 900);
}
Position SmartDragon::getNextPosition() {
    if (smartdragon_type == SD1 || smartdragon_type == SD2) {
        int dx[] = {0, 1, 0, -1};
        int dy[] = {1, 0, -1, 0};
        for (int i = 0; i < 4; ++i) {
            Position next_pos(pos.getRow() + dx[i], pos.getCol() + dy[i]);
            if (map->isValid(next_pos, target)) {
                return next_pos;
            }
        }
        return pos; 
    } else if (smartdragon_type == SD3) {
        return target->getCurrentPosition(); 
    }
    return pos; // Default case
}

void SmartDragon::move() {
    Position next_pos = getNextPosition();
    if (map->isValid(next_pos, target) && next_pos != Position::npos) {
        pos = next_pos;
    }
}
string SmartDragon::str() const {
    string temp;
    if (smartdragon_type == SD1) temp = "SD1";
    else if (smartdragon_type == SD2) temp = "SD2";
    else if (smartdragon_type == SD3) temp = "SD3";
    string results = "smartdragon[pos=" + pos.str() + ";type=" + temp + ";tg=" + target->str() + "]";
    return results;
}

// BaseItem implementation
BaseItem::BaseItem(ItemType type, int value) : type(type), value(value) {}
BaseItem::~BaseItem() {}

// Base Bag implementation
BaseBag::BaseBag(int capacity) : capacity(capacity), count(0) {
        items = new BaseItem*[capacity];
        for (int i = 0; i < capacity; ++i) items[i] = nullptr;
    }
BaseBag::~BaseBag() {
    for (int i = 0; i < capacity; ++i) {
        delete items[i];
    }
    delete[] items;
}
bool BaseBag::insert(BaseItem* item) {
    if (item == nullptr) {
        return false;
    }

    for (int i = 0; i < capacity; ++i) {
        if (items[i] == nullptr) {
            items[i] = item;
            return true;
        }
    }
    return false;
}

BaseItem* BaseBag::get() {
    Warrior* owner = (Warrior*)(this); 
    int index_first_item = -1;
    // item dau danh sach
    for (int i = 0; i < capacity; ++i){
        if (items[i] != nullptr) {
            index_first_item = i;
            break;
        }
    }

    for (int i = 0; i < capacity; ++i) {
        if (items[i] != nullptr && items[i]->canUse(owner)) {
            BaseItem* item = items[i];
            swap(items[i],items[index_first_item]);
            items[index_first_item] = nullptr; 
            return item;
        }
    }
    return nullptr; 
}

BaseItem* BaseBag::get(ItemType itemType) {
    Warrior* owner = (Warrior*)(this); 
    int index_first_item = -1;
    // item dau danh sach
    for (int i = 0; i < capacity; ++i){
        if (items[i] != nullptr) {
            index_first_item = i;
            break;
        }
    }

    for (int i = 0; i < capacity; ++i) {
        if (items[i] != nullptr && items[i]->getType() == itemType && items[i]->canUse(owner)) {
            BaseItem* item = items[i];
            swap(items[i],items[index_first_item]);
            items[index_first_item] = nullptr; 
            return item;
        }
    }
    return nullptr;
}

string BaseBag::str() const {
    string result = "Bag[count=" + to_string(count) + ";";
    for (int i = 0; i < capacity; ++i) {
        if (items[i] != nullptr) {
            result += items[i]->str() + ", ";
        }
    }
    result += "]";
    return result;
}


//TeamBag - chua biet lam gi

// DragonWarriorsProgram implementation
DragonWarriorsProgram::DragonWarriorsProgram(const string &config_file_path) {
    // Initialize all pointers to nullptr
    config = nullptr;
    flyteam1 = nullptr;
    flyteam2 = nullptr;
    groundteam = nullptr;
    dragonlord = nullptr;
    map = nullptr;
    arr_mv_objs = nullptr;
    
    // Load configuration from file
    config = new Configuration(config_file_path);
    
    // Create map with obstacles from configuration
    map = new Map(config->map_num_rows, config->map_num_cols, 
                  config->num_obstacles, config->arr_obstacles,
                  config->num_ground_obstacles, config->arr_ground_obstacles);
    
    // Create FlyTeam1
    flyteam1 = new FlyTeam(1, config->flyteam1_moving_rule, 
                           config->flyteam1_init_pos, map,
                           config->flyteam1_init_hp, config->flyteam1_init_damage);
    
    // Create FlyTeam2  
    flyteam2 = new FlyTeam(2, config->flyteam2_moving_rule,
                           config->flyteam2_init_pos, map,
                           config->flyteam2_init_hp, config->flyteam2_init_damage);
    
    // Create GroundTeam
    groundteam = new GroundTeam(3, config->groundteam_moving_rule,
                                config->groundteam_init_pos, map,
                                config->groundteam_init_hp, config->groundteam_init_damage);
    
    // Create DragonLord
    dragonlord = new DragonLord(4, config->dragonlord_init_pos, map,
                                flyteam1, flyteam2, groundteam);

}

DragonWarriorsProgram::~DragonWarriorsProgram() {
    delete config;
    delete flyteam1;
    delete flyteam2;
    delete groundteam;
    delete dragonlord;
    delete map;
    delete arr_mv_objs;
}

bool DragonWarriorsProgram::isStop() const {
    if (!flyteam1 || !flyteam2 || !groundteam || !dragonlord) return true;
    if (flyteam1->getHp() <= 1 && flyteam2->getHp() <= 1 && groundteam->getHp() <= 1) return true;
    return false;
}


////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////