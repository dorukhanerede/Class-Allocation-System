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

//ABSTRACT ITERATOR
class abstractIterator
{
public:
    virtual Component* getCurrent()=0;
    virtual void First()=0;
    virtual bool Done()=0;
    virtual void Next()=0;
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
	Floor(string name) : Component(name) {};

	void add(Component *c) {
		components.push_back(c);
	}
};

//LEAF CLASS
class Classroom : public Component
{
public:
	Classroom(string name) : Component(name) {};

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
    bool available = true;
};

//
//CONCRETE ITERATOR FOR AVAILABLE CLASSROOMS
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
    void Next()
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
    }
    void First()
    {
        currentIndex=0;
        if(!getCurrent()->getAvailibility())
            Next();
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
    void setAllAvailabilityOfClassrooms()
    {
        for(classroomIterator->First(); !classroomIterator->Done(); classroomIterator->Next())
    {
        classroomIterator->getCurrent()->setAvailibility(false);
    }
    };
private:
    ClassroomIterator* classroomIterator;
};

void printClassrooms(ClassroomIterator *i) {
    cout << "Iterating classrooms: " << endl;
    for (i->First(); !i->Done(); i->Next()) {
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

    ieu->display(1);

    m101->setAvailibility(false);
    c102->setAvailibility(false);
    c202->setAvailibility(false);

    ClassroomIterator* myIterator = new ClassroomIterator(&classrooms);
    printClassrooms(myIterator);

    ResourceAllocationDepartment* rad = new ResourceAllocationDepartment(myIterator);
    rad->setAllAvailabilityOfClassrooms();

    printClassrooms(myIterator);
}
