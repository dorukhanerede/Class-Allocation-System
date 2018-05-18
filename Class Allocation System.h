#ifndef CLASS_ALLOCATION_SYSTEM_H_
#define CLASS_ALLOCATION_SYSTEM_H_

#include <iostream>
#include <vector>
using namespace std;

class Department;
class Lock;
class abstractIterator;
class Teacher;
class ResourceAllocationDepartment;
class Classroom;

//KEY CLASS FOR SINGLETON OBJECT
class Key
{
public:
    static Key* getKey(){
        if(key == NULL)
            key = new Key();
        return key;
    };
private:
    Key(){};
    Key(Key const&);
    void operator = (Key const&);
    static Key* key;
};

//LOCK FOR CLASSROOMS
class Lock
{
public:
    Lock(){state =true;};

    bool getState()
    {
        return state;
    }

    void openLock(Key *key)
    {
        if(key==NULL)
            cout << "You need key"<<endl;
        else
            state=false;
    }

    void closeLock(Key *key)
    {
        if(state==false)
            state=true;
        else
            cout << "The lock is already closed"<<endl;
    }
private:
    bool state;
};

//SECURITY PERSONAL
class SecurityPersonal
{
public:
    SecurityPersonal(string name);
    void setIterator(abstractIterator* classroomIterator);
    string getSecurityName(){return _name; };
    void askToUnlock(Teacher* teacher, string className);
private:
    abstractIterator* _classroomIterator;
    string _name;
    Key* key;
};

//DEPARTMENT CLASS FOR EVERY FLOOR
class Department
{
/*public:
    Department(string name, abstractIterator* ci) {departmentName = name; classroomIterator=ci;};
    string getName(){return departmentName;};
private:
    abstractIterator* classroomIterator;
    string departmentName;*/
public:
    Department(string name, SecurityPersonal *toSc) {departmentName = name; sc = toSc;};
    string getName(){return departmentName;};
    SecurityPersonal* getSecurityPersonal(){return sc;};
private:
    string departmentName;
    SecurityPersonal* sc;
};

//TEACHER AS OBSERVER
class Teacher
{
public:
    Teacher(string name) {_name=name;}
    string getName() {return _name;}
    void update(Classroom* classroom);
    Classroom* getClassroomWanted(){return _classroom;}
private:
    string _name;
    Classroom* _classroom;
};

//COMPONENT
class Component
{
public:
    virtual ~Component(){};
    virtual void add(Component *d) {};
    virtual void display(int indent) {};
    virtual void attach(Teacher* teacher){};
    virtual void detach(Teacher* teacher){};
    virtual void notify(){};
    virtual bool getAvailability(){};
    virtual void setAvailability(bool){};
    virtual void setResourceAllocationDepartment(ResourceAllocationDepartment* rad){};
    virtual Department* getDepartment(){};
    virtual Lock* getLock(){};
    string getName() {return name;};
protected:
    vector<Component*> components;
    Component(string n) {name=n;};
private:
    string name;
};

//COMPOSITE CLASS 1
class Campus : public Component
{
public:
    Campus(string name) : Component(name) {};

    void add(Component *c) {
        components.push_back(c);
    }

    void display(int indent)
    {
        for(int i = 1;i <= indent;i++) { cout <<"-";}
        cout <<  "+ " + getName() << endl;

        for (unsigned int i= 0; i< components.size(); i++) {
            components[i]->display(indent+2);
        }
    }
};

//COMPOSITE CLASS 2
class Building : public Component
{
public:
    Building(string name) : Component(name) {lock=new Lock();};

    void add(Component *c) {
        components.push_back(c);
    }

    void display(int indent)
    {
        for(int i = 1;i <= indent;i++) { cout <<"-";}
        cout <<  "+ " + getName() << endl;

        for (unsigned int i= 0; i< components.size(); i++) {
            components[i]->display(indent+2);
        }
    }
    void getLockState()
    {
        if(lock->getState())
            cout << "It is locked" << endl;
        else
            cout << "It is not locked" << endl;
    }
private:
    Lock *lock;
};

//COMPOSITE CLASS 3
class Floor : public Component
{
public:
    Floor(string name, Department* dep) : Component(name) {setDepartment(dep);};

    void add(Component *c) {
        components.push_back(c);
    }

    void setDepartment(Department *_department)
    {
        department = _department;
    }

    Department* getDepartment()
    {
        return department;
    }

    string getDepartmentName() {
        return department->getName();
    }

    void display(int indent)
    {
        for(int i = 1;i <= indent;i++) { cout <<"-";}
        cout <<  "+ " + getName() << endl;

        for (unsigned int i= 0; i< components.size(); i++) {
            components[i]->display(indent+2);
        }
    }

private:
    Department* department;
};

//LEAF CLASS
class Classroom : public Component
{
public:
    Classroom(string name) : Component(name) {setAvailability(true);lock = new Lock();};

    void setAvailability(bool toSet)
    {
        if(toSet)
            notify();
        available=toSet;
    }
    bool getAvailability()
    {
        return available;
    }
    void add(Component *c) {
        cout<<"Classroom is leaf";
    }
    Lock* getLock()
    {
        return lock;
    }
    void getLockState()
    {
        if(lock->getState())
            cout << "It is locked" << endl;
        else
            cout << "It is not locked" << endl;
    }
    void display(int indent) {
        for(int i = 1;i <= indent;i++) { cout <<"-";}
            cout << " " << getName() << endl;
    }

    Department* getDepartment()
    {
        return _floor->getDepartment();
    }

    void setFloor(Floor* floor)
    {
        _floor=floor;
    }

    void attach(Teacher* teacher){teachers.push_back(teacher);}
    void detach(Teacher* teacher){
        for (unsigned int i= 0; i< teachers.size(); i++) {
            if (teachers[i]->getName() == teacher->getName()) {
                teachers.erase(teachers.begin()+i);
                return;
            }
        }
    }
    void notify();

    void setResourceAllocationDepartment(ResourceAllocationDepartment* rad){_rad=rad;}

private:
    ResourceAllocationDepartment* _rad;
    vector<Teacher*> teachers;
    Floor *_floor;
    bool available;
    Lock *lock;
};

//ABSTRACT ITERATOR
class abstractIterator
{
public:
    virtual Component* getCurrent() const =0;
    virtual void First()=0;
    virtual bool Done()const=0;
    virtual void Next()=0;
    abstractIterator(){}
};

class FloorCollection;

class FloorIterator : public abstractIterator
{
public:
    FloorIterator(const FloorCollection *floorCollection);
	void First();
    void Next();
    Floor* getCurrent()const;
	bool Done()const;
private:
	const FloorCollection *_floorCollection;
	int _current;
};

class ClassroomCollection;
//
//CONCRETE ITERATOR FOR CLASSROOMS
//
class ClassroomIterator : public abstractIterator
{
public:
    ClassroomIterator(const ClassroomCollection *classroomCollection);
    //ClassroomIterator(vector<Classroom*> *a){collection = *a;};
	void First();
    void Next();
    Classroom* getCurrent()const;
	bool Done()const;
private:
    //vector<Classroom*> collection;
	const ClassroomCollection *_classroomCollection;
	int _current;
};

//ABSTRACT COLLECTION
class AbstractCollection
{
public:
	 virtual ~AbstractCollection(){};
	 virtual abstractIterator* CreateIterator() = 0;
	 virtual void add(Component*){}; 		// Not needed for iteration.
	 virtual int getCount () const = 0; // Needed for iteration.
	 virtual Component* get(int) const = 0; // Needed for iteration.
protected:
	AbstractCollection(){};
};

//CONCRETE COLLECTION FOR CLASSROOMS
class ClassroomCollection : public AbstractCollection
{
private:
	 vector<Classroom*> classrooms;
public:
	ClassroomIterator* CreateIterator() {
		return new ClassroomIterator(this);
    };
    int getCount () const {return classrooms.size(); };
    Classroom* get(int index) const{ return classrooms[index];};
    void add(Classroom* classroom) {classrooms.push_back(classroom);};
};

class FloorCollection : public AbstractCollection
{
private:
	 vector<Floor*> floors;
public:
	FloorIterator* CreateIterator() {
		return new FloorIterator(this);
    };
    int getCount () const {return floors.size(); };
    Floor* get(int index) const{ return floors[index];};
    void add(Floor* floor) {floors.push_back(floor);};
};

#endif
