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
};

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
};

// ——— Warrior ———
class Warrior : public MovingObject {
protected:
    int hp;
    int damage;

public:
    Warrior(int index, const Position & pos, Map * map,
            const string & name, int hp, int damage);
    virtual ~Warrior();
    int getHp() const;
    int getDamage() const;
    void setHp(int new_hp);
    void setDamage(int new_damage);
    BaseBag* getBag() const;
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
    void move() override;
    string str() const override;
    bool attack();

};

class GroundTeam : public Warrior {
private:
    string moving_rule;
    int moving_index; 

public:
    GroundTeam(int index, const string & moving_rule,
        const Position & pos, Map * map, int hp, int damage);
    // TODO
    Position getNextPosition() override;
    void move() override;
    string str() const override;
    bool trap() const;
    int getTrapTurns() const;
    void setTrapTurns(int turns);
};

// ——— DragonLord ———
class DragonLord : public MovingObject {
private:
    FlyTeam *flyteam1;
    FlyTeam *flyteam2;

public:
    DragonLord(int index, const Position & pos, Map * map,
               FlyTeam *flyteam1, FlyTeam *flyteam2);
    Position getNextPosition() override;
    void move() override;
    string str() const override;
    Position getPosition() const;
    int manhattanDistance() const;
};

// ...................
// ——— SmartDragon ———
class SmartDragon : public MovingObject {
private:
    DragonType smartdragon_type;
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
};
class DragonScale : public BaseItem {
public:
    DragonScale() : BaseItem(DRAGONSCALE, 25) {}
    bool canUse(Warrior* w) override {
        return w->getHp() <= 400;
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
            w->setHp(current_hp + current_hp * 20 / 100);
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
        GroundTeam* ground_team = dynamic_cast<GroundTeam*>(w);
        if (ground_team) {
            ground_team->setTrapTurns(ground_team->getTrapTurns() + 1);
        } 
    }
};


// ——— BaseBag ———
class BaseBag {
private:
    int capacity;
    BaseItem **items;
public:
    BaseBag(int capacity);
    virtual ~BaseBag();
    virtual bool insert(BaseItem* item);
    virtual BaseItem* get();
    virtual BaseItem* get(ItemType itemType);
    virtual bool str() const;
};
// ...................


// ——— TeamBag ———
class TeamBag : public BaseBag {
private:
    Warrior *owner;
public:
    TeamBag(int capacity, Warrior *owner);
    ~TeamBag();
    bool insert(BaseItem* item) override;
    BaseItem* get() override;
    BaseItem* get(ItemType itemType) override;
    bool str() const override;
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
    
    // Helper function to parse position arrays
    void parsePositionArray(const string& value, Position* arr, int count);
    // Helper function to remove parentheses from position strings
    string removeParentheses(const string& value);

public:
    Configuration(const string & filepath);
    ~Configuration();
    string str() const;
};

// ——— DragonWarriorsProgram ———
class DragonWarriorsProgram {
private:
    // Sample attributes

    Configuration      *config;

    FlyTeam            *flyteam1;
    FlyTeam            *flyteam2;
    GroundTeam         *groundteam;
    DragonLord         *dragonlord;

    Map                *map;
    ArrayMovingObject  *arr_mv_objs;
    
public:
    DragonWarriorsProgram(const string &config_file_path);

    bool   isStop() const;

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

    void run(bool verbose) {
        // Note: This is a sample code. You can change the implementation as you like.
        // TODO
        for (int istep = 0; istep < config->num_steps; ++istep) {
            for (int i = 0; i < arr_mv_objs->size(); ++i) {
                arr_mv_objs->get(i)->move();
                if (isStop()) {
                    printStep(istep);
                    break;
                }
                if (verbose) {
                    printStep(istep);
                }
            }
        }
        printResult();
    }

    ~DragonWarriorsProgram();
};

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* H_DRAGON_WARRIORS_H */
