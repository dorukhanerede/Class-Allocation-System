#include <iostream>
#include <vector>

using namespace std;

//COMPONENT
class Component
{
public:
    virtual ~Component(){};
    virtual void add(Component *d) {};
    void display(int indent) {
        for(int i = 1;i <= indent;i++) { cout <<"-";}
        cout <<  "+ " + getName() << endl;
        
        for (unsigned int i= 0; i< components.size(); i++) {
            components[i]->display(indent+2);
        }
    }
    string getName() {return name;};
protected:
    vector<Component*> components;
    Component(string n) {name=n;};
private:
    string name;
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
private:
    bool state = true;
};
//SECURITY PERSONAL
class SecurityPersonal
{
public:
    SecurityPersonal(string name){_name = name;};
    string getSecurityName(){return _name; };
private:
    string _name;
};

//DEPARTMENT CLASS FOR EVERY FLOOR
class Department
{
public:
    Department(string name, SecurityPersonal *toSc) {departmentName = name; sc = toSc;};
    string getName(){return departmentName;};
    string getSecurityPersonalName(){ return sc->getSecurityName();};
private:
    string departmentName;
    SecurityPersonal* sc;
};

//ABSTRACT ITERATOR
class abstractIterator
{
public:
    virtual Component* getCurrent()=0;
    virtual void First(string type)=0;
    virtual bool Done()=0;
    virtual void Next(string type)=0;
    abstractIterator(){}
};

//COMPOSITE CLASS 1
class Campus : public Component
{
public:
    Campus(string name) : Component(name) {};
    
    void add(Component *c) {
        components.push_back(c);
    }
};

//COMPOSITE CLASS 2
class Building : public Component
{
public:
    Building(string name) : Component(name) {};
    
    void add(Component *c) {
        components.push_back(c);
    }
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
    
    string getDepartmentName() {
        return department->getName();
    }
    
private:
    Department* department;
};

//LEAF CLASS
class Classroom : public Component
{
public:
    Classroom(string name) : Component(name) {setAvailibility(true);};
    
    void setAvailibility(bool toSet)
    {
        available=toSet;
    }
    bool getAvailibility()
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
};

//
//CONCRETE ITERATOR FOR CLASSROOMS
//
class ClassroomIterator : public abstractIterator
{
    vector<Classroom*> collection;
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
    }
};

//
//FACADE CLASS
//TODO
class ResourceAllocationDepartment
{
public:
    ResourceAllocationDepartment(ClassroomIterator* i) {classroomIterator=i;};
    ClassroomIterator* setAllAvailabilityOfClassrooms(bool toSet)
    {
        for(classroomIterator->First("all"); !classroomIterator->Done(); classroomIterator->Next("all"))
        {
            classroomIterator->getCurrent()->setAvailibility(toSet);
        }
        return classroomIterator;
    };
    
    void setExamToClassrooms(vector<Classroom*> classrooms)
    {
        ClassroomIterator* ci = new ClassroomIterator(&classrooms);
        for(ci->First("all"); !ci->Done(); ci->Next("all"))
        {
            ci->getCurrent()->setAvailibility(false);
        }
        delete ci;
    };
private:
    ClassroomIterator* classroomIterator;
};

// This function is printing the iterator
void printClassrooms(ClassroomIterator *i, string type) {
    cout << endl << "Iterating classrooms: " << endl;
    for (i->First(type); !i->Done(); i->Next(type)) {
            cout << "Available Classrooms -> " << i->getCurrent()->getName() << endl;
    }
}

//this function is to create the IEU campus and simple out the main
/*Campus* createCampus() {
 Campus* ieu = new Campus("IEU");
 Building* m = new Building("M block");
 Building* c = new Building("C block");
 Floor* firstM = new Floor("1st Floor");
 Floor* secondM = new Floor("2nd Floor");
 Floor* firstC = new Floor("1st Floor");
 Floor* secondC = new Floor("2nd Floor");
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
 return ieu;
 }*/

int main()
{
    vector<Classroom*> classrooms;
    Department* se = new Department("Software Engineering");
    Department* ce = new Department("Computer Engineering");
    Department* cie = new Department("Civil Engineering");
    Campus* ieu = new Campus("IEU");
    Building* m = new Building("M block");
    Building* c = new Building("C block");
    Floor* firstM = new Floor("1st Floor", se);
    Floor* secondM = new Floor("2nd Floor", ce);
    Floor* firstC = new Floor("1st Floor", se);
    Floor* secondC = new Floor("2nd Floor", cie);
    Classroom* m101 = new Classroom("M101");
    Classroom* m102 = new Classroom("M102");
    Classroom* c101 = new Classroom("C101");
    Classroom* c102 = new Classroom("C102");
    Classroom* m201 = new Classroom("M201");
    Classroom* m202 = new Classroom("M202");
    Classroom* c201 = new Classroom("C201");
    Classroom* c202 = new Classroom("C202");
    
    classrooms.push_back(m101);
    classrooms.push_back(m102);
    classrooms.push_back(m201);
    classrooms.push_back(m202);
    classrooms.push_back(c101);
    classrooms.push_back(c102);
    classrooms.push_back(c201);
    classrooms.push_back(c202);
    
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
    
    // To display the campus
    //ieu->display(1);
    
    m101->setAvailibility(false);
    c102->setAvailibility(false);
    c202->setAvailibility(false);
    
    ClassroomIterator* myIterator = new ClassroomIterator(&classrooms);
    printClassrooms(myIterator,"");
    
    ResourceAllocationDepartment* rad = new ResourceAllocationDepartment(myIterator);
    rad->setAllAvailabilityOfClassrooms(true);
    
    printClassrooms(myIterator,"");
    
    cout << endl << "M first floor : " << firstM->getDepartmentName() << endl;
    cout << "M second floor : " << secondM->getDepartmentName() << endl;
    
    vector<Classroom*> toset;
    
    toset.push_back(m101);
    toset.push_back(m102);
    toset.push_back(c202);
    
    rad->setExamToClassrooms(toset);
    
    printClassrooms(myIterator,"");
    
    return 0;
}
