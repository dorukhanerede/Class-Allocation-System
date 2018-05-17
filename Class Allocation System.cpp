#include <iostream>
#include <vector>

using namespace std;

class Department;

//COMPONENT
class Component
{
public:
    virtual ~Component(){};
    virtual void add(Component *d) {};
    virtual void display(int indent) {};
    virtual bool getAvailability(){};
    virtual void setAvailability(bool){};
    virtual Department* getDepartment(){};
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

Key *Key::key = NULL;

//LOCK FOR CLASSROOMS
class Lock
{
public:
    Lock(){};

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

    void closeLock()
    {
        if(state==false)
            state=true;
        else
            cout << "The lock is already closed"<<endl;
    }
private:
    bool state = true;
};

//COMPOSITE CLASS 2
class Building : public Component
{
public:
    Building(string name) : Component(name) {};

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
    Lock *lock= new Lock();
};

//SECURITY PERSONAL
class SecurityPersonal
{
public:
    SecurityPersonal(string name){_name = name;};
    void addDepartmentClassrooms(Classroom* classroom)
    string getSecurityName(){return _name; };
    void askToUnlock(Teacher* teacher,string name){key.getKey();}
private:
    ClassroomCollection* collection;
    string _name;
    Key* key;
};

//DEPARTMENT CLASS FOR EVERY FLOOR
class Department
{
public:
    Department(string name, SecurityPersonal *toSc) {departmentName = name; sc = toSc;};
    string getName(){return departmentName;};
    SecurityPersonal* getSecurityPersonal(){return sc;};
private:
    string departmentName;
    SecurityPersonal* sc;
};

//TEACHER
class Teacher
{
public:
    Teacher(string name) {_name=name;}
    string getName() {return _name;}
private:
    string _name;
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
    Classroom(string name) : Component(name) {setAvailability(true);};

    void setAvailability(bool toSet)
    {
        available=toSet;
    }
    bool getAvailability()
    {
        return available;
    }
    void add(Component *c) {
        cout<<"Classroom is leaf";
    }
    void display(int indent) {
        for(int i = 1;i <= indent;i++) { cout <<"-";}
            cout << " " << getName() << endl;
    }
private:
    bool available;
    Lock *lock = new Lock();
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
    ClassroomIterator(vector<Classroom*> *a){collection = *a;};
	void First();
    void Next();
    Classroom* getCurrent()const;
	bool Done()const;
private:
    vector<Classroom*> collection;
	const ClassroomCollection *_classroomCollection;
	int _current;

    /*vector<Classroom*> collection;
    int currentIndex;
public:
    ClassroomIterator(vector<Classroom*> *a)
    {
        collection=*a;//copy vector
    }
    bool Done()
    {
        return currentIndex>=collection.size();
    }
    void Next(string type)
    {
        if(type=="")
        {
            for(;;)
            {
                if(currentIndex==(collection.size())-1)
                {
                    currentIndex++;
                    break;
                }
                currentIndex++;
                if(getCurrent()->getAvailibility())
                {
                    break;
                }
            }
        }else
        {
            currentIndex++;
        }

    }
    void First(string type)
    {
        if(type=="")
        {
            currentIndex=0;
            if(!getCurrent()->getAvailibility())
            {
                for(currentIndex = 1; currentIndex >= collection.size();currentIndex++)
                {
                    if(getCurrent()->getAvailibility())
                    {
                        break;
                    }
                }
            }
        }
        else
            currentIndex=0;
    }
    Classroom* getCurrent()
    {
        return collection.at(currentIndex);
    }*/
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

ClassroomIterator::ClassroomIterator(const ClassroomCollection *classroomCollection) :
	_classroomCollection(classroomCollection) {
}
void ClassroomIterator::First() {
	_current = 0;
}
void ClassroomIterator::Next() {
	_current++;
}
bool ClassroomIterator::Done()const {
	return _current >= _classroomCollection->getCount();
}
Classroom* ClassroomIterator::getCurrent()const {
	return (Done()?NULL:_classroomCollection->get(_current));
}

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

FloorIterator::FloorIterator(const FloorCollection *floorCollection) :
	_floorCollection(floorCollection) {
}
void FloorIterator::First() {
	_current = 0;
}
void FloorIterator::Next() {
	_current++;
}
bool FloorIterator::Done()const {
	return _current >= _floorCollection->getCount();
}
Floor* FloorIterator::getCurrent()const {
	return (Done()?NULL:_floorCollection->get(_current));
}

//
//FACADE CLASS
//TODO
class ResourceAllocationDepartment
{
public:

    ResourceAllocationDepartment()
    {
        ieu = new Campus("IEU");
        Building* m = new Building("M block");
        Building* c = new Building("C block");
        Department* se = new Department("Software Engineering",new SecurityPersonal("Murat Celik"));
        Department* ce = new Department("Computer Engineering",new SecurityPersonal("Dorukhan Nerede"));
        Department* cie = new Department("Civil Engineering",new SecurityPersonal("Aytac Celiktug"));
        Floor* firstM = new Floor("M 1st Floor", se);
        Floor* secondM = new Floor("M 2nd Floor", ce);
        Floor* firstC = new Floor("C 1st Floor", se);
        Floor* secondC = new Floor("C 2nd Floor", cie);
        Classroom* m101 = new Classroom("M101");
        Classroom* m102 = new Classroom("M102");
        Classroom* c101 = new Classroom("C101");
        Classroom* c102 = new Classroom("C102");
        Classroom* m201 = new Classroom("M201");
        Classroom* m202 = new Classroom("M202");
        Classroom* c201 = new Classroom("C201");
        Classroom* c202 = new Classroom("C202");

        firstC->add(c101);
        firstC->add(c102);
        secondC->add(c201);
        secondC->add(c202);

        firstM->add(m101);
        firstM->add(m102);
        secondM->add(m201);
        secondM->add(m202);

        m->add(firstM);
        m->add(secondM);

        c->add(firstC);
        c->add(secondC);

        ieu->add(m);
        ieu->add(c);

        ClassroomCollection *myCollection = new ClassroomCollection();
        FloorCollection *myCollection2 = new FloorCollection();
        myCollection->add(m101);
        myCollection->add(m102);
        myCollection->add(m201);
        myCollection->add(m202);
        myCollection->add(c101);
        myCollection->add(c102);
        myCollection->add(c201);
        myCollection->add(c202);
        myCollection2->add(firstM);
        myCollection2->add(secondM);
        myCollection2->add(firstC);
        myCollection2->add(secondC);
        classroomIterator = myCollection->CreateIterator();
        floorIterator = myCollection2->CreateIterator();
    };

    void setAllAvailabilityOfClassrooms(bool toSet)
    {
        for(classroomIterator->First(); !classroomIterator->Done(); classroomIterator->Next())
        {
            classroomIterator->getCurrent()->setAvailability(toSet);
        }
    }

    void setExamToClassrooms(string className)
    {
        for(classroomIterator->First(); !classroomIterator->Done(); classroomIterator->Next())
        {
            if(classroomIterator->getCurrent()->getName()==className)
                classroomIterator->getCurrent()->setAvailability(false);
        }
    }

    void printClassrooms()
    {
        cout << endl << "Iterating classrooms: " << endl;
        for (classroomIterator->First(); !classroomIterator->Done(); classroomIterator->Next()) {
            if(classroomIterator->getCurrent()->getAvailability())
                cout << "Available Classrooms -> " << classroomIterator->getCurrent()->getName() << endl;
        }
    }

    SecurityPersonal* getSecurityPersonalOfFloor(string floorName)
    {
        for(floorIterator->First(); !floorIterator->Done(); floorIterator->Next())
        {
            if(floorIterator->getCurrent()->getName()==floorName)
                return floorIterator->getCurrent()->getDepartment()->getSecurityPersonal();
        }
    }

    void displaySchool()
    {
        ieu->display(1);
    }
private:
    abstractIterator *classroomIterator;
    abstractIterator *floorIterator;
    Campus *ieu;
};

int main()
{
    ResourceAllocationDepartment* rad = new ResourceAllocationDepartment();
    rad->displaySchool();
    rad->setExamToClassrooms("M101");
    rad->printClassrooms();
    SecurityPersonal* sp = rad->getSecurityPersonalOfFloor("M 1st Floor");
    sp->

    return 0;
}
