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

	void add(Component *c) {
		cout<<"Classroom is leaf";
	}
	void display(int indent) {
		for(int i = 1;i <= indent;i++) { cout <<"-";}
		cout << " " << getName() << endl;
	}
};

//this function is to create the IEU campus and simple out the main
Campus* createCampus() {

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
}

int main()
{
    createCampus()->display(1);
}
