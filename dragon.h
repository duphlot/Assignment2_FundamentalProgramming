/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 2
* Programming Fundamentals Summer 2025
* Date: 28.07.2025
*/

//The library here is concretely set, students are not allowed to include any other libraries.
#ifndef H_DRAGON_WARRIORS_H
#define H_DRAGON_WARRIORS_H

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class MovingObject;
class ArrayMovingObject;
class DragonWarriorsProgram;
class Position;
class Configuration;
class Map;

class DragonLord;
class SmartDragon;
class Warrior;
class FlyTeam;
class GroundTeam;

class BaseItem;
class BaseBag;
class TeamBag;

class TestDragonWar;

// ——— Enums ———
enum ElementType     { PATH, OBSTACLE, GROUND_OBSTACLE };
enum DragonType      { SD1, SD2, SD3 };
enum ItemType        { DRAGONSCALE, HEALINGHERB, TRAPENHANCER };

// ——— MapElement ———
class MapElement {
private:
    ElementType type;
    int         req_dmg; 
public:
    MapElement(ElementType type, int in_req_dmg = 0);
    virtual ~MapElement();
    ElementType getType() const;
    int getReqDmg()       const;
};

// ——— Position ———
class Position {
private:
    int r_, c_;
public:
    static const Position npos;
    Position(int r = 0, int c = 0);
    Position(const string &str_pos);
    int     getRow()  const;
    int     getCol()  const;
    void    setRow(int r);
    void    setCol(int c);
    string  str()     const;
    bool    isEqual(int in_r, int in_c) const;
    int manhattanDistance(const Position &other) const;

};

// ——— Position operators ———
bool operator==(const Position& lhs, const Position& rhs);
bool operator!=(const Position& lhs, const Position& rhs);

// ——— Map ———
class Map {
private:
    int           num_rows;
    int           num_cols;
    MapElement ***grid;

public:
    Map(int rows, int cols,
        int num_obst,       Position *obst,
        int num_gro_obst,   Position *gro_obst);
    ~Map();
    bool isValid(const Position & pos, MovingObject * obj) const;
    int  getNumRows()     const;
    int  getNumCols()     const;
    static const Position npos;
    bool isPath(const Position & pos) const;
    bool isObstacle(const Position & pos) const;
    bool isGroundObstacle(const Position & pos) const;
};

// ——— MovingObject ———
class MovingObject {
protected:
    int      index;
    Position   pos;
    Map     *  map;
    string    name;

public:
    MovingObject(int index, const Position & pos, Map *map, const string & name = "");
    virtual ~MovingObject();
    virtual Position getNextPosition() = 0;
    Position          getCurrentPosition() const;
    virtual void      move()          = 0;
    virtual string    str()    const = 0;
    virtual int getHp() = 0;
    virtual int getDamage() = 0;
    virtual void setHp(int new_hp) = 0;
    virtual void setDamage(int new_damage) = 0;
    virtual bool isDragonLord() const = 0;
    virtual bool isSmartDragon() const = 0;
    virtual void setPosition(const Position &pos) {
        this->pos = pos;
    }
    string       getName() const {
        return name;
    }
    int          getIndex() const {
        return index;
    }
};

// ——— Warrior ———
class Warrior : public MovingObject {
protected:
    int hp;
    int damage;
    BaseBag *bag;

public:
    Warrior(int index, const Position & pos, Map * map,
            const string & name, int hp, int damage);
    virtual ~Warrior();
    int getHp() override;
    int getDamage() override;
    void setHp(int new_hp) override;
    void setDamage(int new_damage) override;
    BaseBag* getBag() const{
        return bag;
    };
    bool isDragonLord() const override;
};

// ——— FlyTeam & GroundTeam ———
class FlyTeam : public Warrior {
private:
    string moving_rule;
    int moving_index;
public:
    FlyTeam(int index, const string & moving_rule,
        const Position & pos, Map * map, int hp, int damage);
    // TODO
    string getMovingRule() const;
    Position getNextPosition() override;
    Position getRereversePosition() const;
    void move() override;
    string str() const override;
    bool attack(DragonLord *dragonlord);
    bool isDragonLord() const override { return false; }
    bool isSmartDragon() const override { return false; }
    string getName() const { return name + to_string(index); }

};

class GroundTeam : public Warrior {
private:
    string moving_rule;
    int moving_index; 
    int trap_turns;
    bool isTrapped;
    int trap_duration;

public:
    GroundTeam(int index, const string & moving_rule,
        const Position & pos, Map * map, int hp, int damage);
    // TODO
    Position getNextPosition() override;
    Position getRereversePosition() const;
    void move() override;
    string str() const override;
    void trap(DragonLord *dragonlord);
    int getTrapTurns() const;
    void setTrapTurns(int turns);
    bool isDragonLord() const override { return false; }
    bool processTrap(); 
    bool swapPosition();
    void setIsTrapped(bool trapped) { isTrapped = trapped; }
    bool isSmartDragon() const override { return false; }
};

// ——— DragonLord ———
class DragonLord : public MovingObject {
private:
    FlyTeam *flyteam1;
    FlyTeam *flyteam2;
    int hp;
    int damage;

public:
    DragonLord(int index, const Position & pos, Map * map,
               FlyTeam *flyteam1, FlyTeam *flyteam2, GroundTeam *ground_team);
    Position getNextPosition() override;
    void move() override;
    string str() const override;
    Position getPosition() const;
    int manhattanDistance(const Position pos1, const Position pos2) const;
    int getHp() override;
    int getDamage() override;
    void setHp(int new_hp) override;
    void setDamage(int new_damage) override;
    bool isDragonLord() const override { return true; }
    bool isSmartDragon() const override { return false; }
};

// ...................
// ——— SmartDragon ———
class SmartDragon : public MovingObject {
private:
    DragonType smartdragon_type;
    int hp;
    int damage;
    BaseItem* item;
    MovingObject *target;
    Position   target_pos;
public:
    SmartDragon(int index, const Position & init_pos, Map * map, 
                        DragonType type, MovingObject *obj, int damage);
    Position getNextPosition() override;
    void move() override;
    string str() const override;
    int getHp() override;
    int getDamage() override;
    void setHp(int new_hp) override;
    void setDamage(int new_damage) override;
    DragonType getDragonType() const { return smartdragon_type; }
    bool isDragonLord() const override { return false; }
    bool isSmartDragon() const override { return true; }
};

// ——— BaseItem ———
class BaseItem {
private:
    ItemType type;
    int      value;
public:
    BaseItem(ItemType type, int value);
    virtual ~BaseItem();
    virtual bool canUse(Warrior* w) = 0;
    virtual void use(Warrior* w) = 0;
    int getValue() const { return value; }
    ItemType getType() const { return type; }
    string str() const {
        if (type == DRAGONSCALE) return "DragonScale";
        else if (type == HEALINGHERB) return "HealingHerb";
        else if (type == TRAPENHANCER) return "TrapEnhancer";
        return "";
    }
};
class DragonScale : public BaseItem {
public:
    DragonScale() : BaseItem(DRAGONSCALE, 25) {}
    bool canUse(Warrior* w) override {
        return w->getDamage() <= 400;
    }
    void use(Warrior* w) override {
        if (canUse(w)) {
            int current_dmg = w->getDamage();
            w->setDamage(current_dmg + current_dmg * 25 / 100);
        }
    }
};

class HealingHerb : public BaseItem {
public:
    HealingHerb() : BaseItem(HEALINGHERB, 20) {}
    bool canUse(Warrior* w) override {
        return w->getHp() <= 100;
    }

    void use(Warrior* w) override {
        if (canUse(w)) {
            int current_hp = w->getHp();
            w->setHp(round(current_hp + current_hp * 20 / 100.0));
        }
    }
};


class TrapEnhancer : public BaseItem {
public:
    TrapEnhancer() : BaseItem(TRAPENHANCER, 1) {}
    bool canUse(Warrior* w) override {
        return true; 
    }

    void use(Warrior* w) override {
        GroundTeam* ground_team = (GroundTeam*)(w);
        if (ground_team) {
            ground_team->setTrapTurns(ground_team->getTrapTurns() + 1);
        } 
    }
};


// ——— BaseBag ———
class BaseBag {
protected:
    int capacity;
    int count;
    BaseItem **items;
public:
    BaseBag(int capacity);
    virtual ~BaseBag();
    virtual bool insert(BaseItem* item);
    virtual BaseItem* get();
    virtual BaseItem* get(ItemType itemType);
    virtual string str() const;

};

// ...................


// ——— TeamBag ———
class TeamBag : public BaseBag {
private:
    BaseBag **arrBaseBag;
    Warrior *owner;
    int count;
public:
    TeamBag(Warrior *owner);
    ~TeamBag();
    bool insert(BaseBag* item);
    BaseBag* get(int index);
};


// ——— ArrayMovingObject ———
class ArrayMovingObject {
private:
    MovingObject **arr_mv_objs;
    int count;
    int capacity;

public:
    ArrayMovingObject(int capacity);
    ~ArrayMovingObject() ;
    // TODO
    bool isFull() const;
    bool add(MovingObject *mv_obj);
    string str() const;
    int size() const { return count; }
    MovingObject* get(int index) const {
        if (index < 0 || index >= count) {
            return nullptr;
        }
        return arr_mv_objs[index];
    }
};

// ——— Configuration ———
class Configuration {
    friend class DragonWarriorsProgram;
private:
    // TODO
    int map_num_rows, map_num_cols;
    int max_num_moving_objects;
    int num_obstacles;
    Position *arr_obstacles;
    int num_ground_obstacles;
    Position *arr_ground_obstacles;
    string flyteam1_moving_rule;
    Position flyteam1_init_pos;
    int flyteam1_init_hp;
    int flyteam1_init_damage;
    string flyteam2_moving_rule;
    Position flyteam2_init_pos;
    int flyteam2_init_hp;
    int flyteam2_init_damage;
    string groundteam_moving_rule;
    Position groundteam_init_pos;
    int groundteam_init_hp;
    int groundteam_init_damage;
    Position dragonlord_init_pos;
    int num_steps;
    
    void parsePositionArray(const string& value, Position* arr, int count);
    string removeParentheses(const string& value);
    int countPositionsInArray(const string& value);

public:
    Configuration(const string & filepath);
    ~Configuration();
    string str() const;
    int getFlyTeam1InitHP() const { return flyteam1_init_hp; }
};

// ——— DragonWarriorsProgram ———
class Path {
private:
    Position* positions;
    int capacity;
    int idx;
    int filled;
    int totalMoves;
public:
    Path(int cap = 100) : capacity(cap), idx(0), filled(0), totalMoves(0) {
        positions = new Position[capacity];
    }
    Path(const Path& other) : capacity(other.capacity), idx(other.idx), filled(other.filled), totalMoves(other.totalMoves) {
        positions = new Position[capacity];
        for (int i = 0; i < filled; ++i) {
            positions[i] = other.positions[i];
        }
    }
    ~Path() {
        delete[] positions;
    }
    void add(const Position& pos) {
        if (filled < capacity) {
            positions[filled++] = pos;
        }
        totalMoves++;
    }

    int getTotalMoves() const {
        return totalMoves;
    }

    bool checkLoop() const {
        if (filled < 4) return false;
        const Position& a = positions[filled - 5];
        const Position& b = positions[filled - 4];
        const Position& c = positions[filled - 3];
        const Position& d = positions[filled - 2];
        const Position& e = positions[filled - 1];
        if (a == c && b == d && c == e) {
            return true;
        }
        return false;
    }
};

class MovementHistory {
private:
    Path **history;
    int count;
    int capacity;

public:
    MovementHistory(int capacity){
        this->capacity = capacity;
        this->count = 0;
        history = new Path*[capacity];
    }
    ~MovementHistory(){
        for (int i = 0; i < count; ++i) {
            delete history[i];
        }
        delete[] history;
    }
    void addPath(const Path *path){
        if (count < capacity) {
            history[count++] = new Path(*path);
        }
    }
    Path* getPath(int index) const {
        if (index < 0 || index >= count) {
            return nullptr;
        }
        return history[index];
    }
};

class DragonWarriorsProgram {
private:
    Configuration      *config;
    Map                *map;
    ArrayMovingObject  *arr_mv_objs;
    
public:
    FlyTeam            *flyteam1;
    FlyTeam            *flyteam2;
    GroundTeam         *groundteam;
    DragonLord         *dragonlord;
    MovementHistory    *movement_history;
    int countSD1, countSD2, countSD3;
    int num_smart_dragons;

    DragonWarriorsProgram(const string &config_file_path);

    bool   isStop() const;

    // Helper functions for bag management
    void useAllAvailableItems(Warrior* warrior);
    void useItemIfPossible(BaseItem* item, Warrior* warrior);
    BaseItem* createItemFromSmartDragon(MovingObject* warrior, DragonType type);

    void printResult() const {
        if (flyteam1->getCurrentPosition().isEqual(
                dragonlord->getCurrentPosition().getRow(),
                dragonlord->getCurrentPosition().getCol())) {
            cout << "FlyTeam1 defeated the DragonLord!" << endl;
        }
        else if (flyteam2->getCurrentPosition().isEqual(dragonlord->getCurrentPosition().getRow(),
                dragonlord->getCurrentPosition().getCol())) {
            cout << "FlyTeam2 defeated the DragonLord!" << endl;
        }
        else {
            cout << "The warrior lost the battle!" << endl;
        }
    }

    void printStep(int si) const {
        cout << "Step: " << setw(4) << setfill('0') << si
            << "--"
            << flyteam1->str() << "--|--" << flyteam2->str()<< "--|--" << groundteam->str() << "--|--" << dragonlord->str() << endl;
    }
    bool meeting(int istep, int idx) {
        MovingObject *currentTeam = arr_mv_objs->get(idx);

        for (int i = 0; i < arr_mv_objs->size(); ++i) {
            if (i == idx) continue; 
            MovingObject *otherTeam = arr_mv_objs->get(i);
            if (otherTeam->isSmartDragon() && currentTeam->getCurrentPosition() == otherTeam->getCurrentPosition()) {
                SmartDragon *smartDragon = dynamic_cast<SmartDragon*>(otherTeam);
                if (currentTeam->getDamage() > otherTeam->getDamage()){
                    Warrior* warrior = dynamic_cast<Warrior*>(currentTeam);
                    if (warrior) {
                        BaseItem* droppedItem = createItemFromSmartDragon(currentTeam, smartDragon->getDragonType());
                        if (droppedItem) {
                            if (warrior->getBag() && warrior->getBag()->insert(droppedItem)) {
                                cout << "MSG: " << warrior->getName() << " added " << droppedItem->str() << " to bag from defeated SmartDragon" << endl;
                            } else {
                                useItemIfPossible(droppedItem, warrior);
                            }
                        }
                    }
                    
                    if (smartDragon->getDragonType() == SD1) {
                        countSD1--;
                    } else if (smartDragon->getDragonType() == SD2) {
                        countSD2--;
                    } else if (smartDragon->getDragonType() == SD3) {
                        countSD3--;
                    }
                    cout << "MSG: FlyTeam1 encounters SmartDragon" << endl;

                } else {
                    currentTeam->setHp(currentTeam->getHp() - 100);
                }
            }
        }

        if (currentTeam != nullptr && currentTeam->getCurrentPosition() == dragonlord->getCurrentPosition() && !currentTeam->isDragonLord()) {
            if (currentTeam == flyteam1) {
            cout << "MSG: FlyTeam1 encounters DragonLord" << endl;
            printStep(istep);
            cout << "FlyTeam1 defeated the DragonLord!" << endl;
            return true;
            } else if (currentTeam == flyteam2) {
            cout << "MSG: FlyTeam2 encounters DragonLord" << endl;
            printStep(istep);
            cout << "FlyTeam2 defeated the DragonLord!" << endl;
            return true;
            } else if (currentTeam == groundteam) {
            cout << "MSG: GroundTeam encounters DragonLord" << endl;
            groundteam->trap(dragonlord);
            }
        } else {
            for (int i = 0; i < arr_mv_objs->size(); ++i) {
                if (i == idx) continue; 
                MovingObject *otherTeam = arr_mv_objs->get(i);
                if (currentTeam->getCurrentPosition() == otherTeam->getCurrentPosition()) {
                    bool check = false;
                    string name1 = currentTeam->getName();
                    string name2 = otherTeam->getName();
                    if (name1 == name2 &&  currentTeam->getCurrentPosition() == Position::npos) check = true;
                    if (name1 == "FlyTeam")  name1 += to_string(currentTeam->getIndex());
                    if (name2 == "FlyTeam")  name2 += to_string(otherTeam->getIndex());
                    cout << "MSG: " << name1 << " meets " << name2 << endl;
                    if (check) {
                        printStep(istep);
                        printResult();
                        return true;
                    }
                }
            }
        }
        return false;
    }
    void run(bool verbose) {
        // TODO
        for (int istep = 0; istep < config->num_steps; ++istep) {
            bool createSmartDragon = false;
            Position smartDragon;
            DragonType smartDragonType;
            MovingObject *smartDragonTarget;
            int arrSize = arr_mv_objs->size();
            for (int i = 0; i < arrSize; ++i) {
                if (arr_mv_objs->get(i)->getHp() <= 1) continue;

                if (arr_mv_objs->get(i)->isDragonLord() && movement_history->getPath(i)->getTotalMoves() % 5 == 0 && movement_history->getPath(i)->getTotalMoves() != 0) {
                    Position prevPos = arr_mv_objs->get(i)->getCurrentPosition();
                    DragonType newType = SD1;
                    MovingObject *target = flyteam1;
                    if (num_smart_dragons == 0) {
                        newType = SD1;
                    } else {
                        // Compute distances
                        int dist1 = prevPos.manhattanDistance(flyteam1->getCurrentPosition());
                        int dist2 = prevPos.manhattanDistance(flyteam2->getCurrentPosition());
                        int dist3 = prevPos.manhattanDistance(groundteam->getCurrentPosition());
                        if (dist1 == dist2 && dist2 == dist3) newType = countSD1 > countSD2 ? (countSD2 > countSD3 ? SD3 : SD2) : (countSD1 > countSD3 ? SD2 : SD1);
                        else newType = dist1 > dist2 ? (dist2 > dist3 ? SD3 : SD2) : (dist1 > dist3 ? SD2 : SD1);
                    }
                    if (newType == SD1) {
                        countSD1++;
                        target = flyteam1;
                    } else if (newType == SD2) {
                        countSD2++;
                        target = flyteam2;
                    } else if (newType == SD3) {
                        countSD3++;
                        target = groundteam;
                    }
                    num_smart_dragons++;
                    createSmartDragon = true;
                    smartDragon = prevPos;
                    smartDragonType = newType;
                    smartDragonTarget = target;
                }
                if (arr_mv_objs->get(i)->isDragonLord() && groundteam->processTrap()) {
                    continue;
                }
                
                // Before moving, use all available items if it's a warrior
                Warrior* warrior = dynamic_cast<Warrior*>(arr_mv_objs->get(i));
                if (warrior) {
                    useAllAvailableItems(warrior);
                }
                
                arr_mv_objs->get(i)->move();
                // saving step
                movement_history->getPath(i)->add(arr_mv_objs->get(i)->getCurrentPosition());
                if (!arr_mv_objs->get(i)->isDragonLord() && arr_mv_objs->get(i)->getHp() > 1 && movement_history->getPath(i)->checkLoop()) {
                    cout<<"MSG: "<<arr_mv_objs->get(i)->getName()<<" eliminated due to being stuck for 3 similar patterns!"<<endl;
                    arr_mv_objs->get(i)->setHp(1);
                    arr_mv_objs->get(i)->setPosition(Position::npos);
                }
                if (meeting(istep, i)) return ;
                if (createSmartDragon) {
                    createSmartDragon = false;
                    int damage = (smartDragon.getRow() * 257 + smartDragon.getCol() * 139 + 89) % 900 + 1;
                    SmartDragon *objectSmartDragon = new SmartDragon(
                            0, smartDragon, map,
                            smartDragonType, smartDragonTarget, damage);
                    if (!arr_mv_objs->add(objectSmartDragon)) {
                        cout<<"MSG: ArrayMovingObjects is full. Cannot create more SmartDragon"<<endl;
                        continue;
                    }
                    string stringType;
                    if (smartDragonType == SD1) stringType = "SD1";
                    else if (smartDragonType == SD2) stringType = "SD2";
                    else if (smartDragonType == SD3) stringType = "SD3";
                    cout<< "MSG : "<<stringType <<" created at "<<smartDragon.str()<<endl;
                    Path *path = new Path();
                    path->add(smartDragonType);
                    movement_history->addPath(path);
                }
            }
            if (verbose) {
                printStep(istep);
            }
            cout<<"-------------------------------------------------------------------------------------------------------------------\n";
        }
        printResult();
    }
    ~DragonWarriorsProgram();
};

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* H_DRAGON_WARRIORS_H */
