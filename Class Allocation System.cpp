#include "Class Allocation System.h"

#include <iostream>
#include <vector>

using namespace std;

class Department;
class Lock;
class abstractIterator;
class Teacher;
class Department;

Key *Key::key = NULL;

SecurityPersonal::SecurityPersonal(string name){
    _name = name;
}
void SecurityPersonal::setIterator(abstractIterator* classroomIterator){
    _classroomIterator=classroomIterator;
}
void SecurityPersonal::askToUnlock(Teacher* teacher, string className){
    for(_classroomIterator->First(); !_classroomIterator->Done(); _classroomIterator->Next())
    {
        if(_classroomIterator->getCurrent()->getName()==className && _classroomIterator->getCurrent()->getDepartment()->getSecurityPersonal()->getSecurityName()==_name)
            _classroomIterator->getCurrent()->getLock()->openLock(Key::getKey());
    }
}

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

void Teacher::update(Classroom* classroom){_classroom=classroom;cout << endl << "Notified "+this->getName()+
    " about the classroom that he wanted to make an exam,\nthe previous exam was cancelled so you have been moved to have the classroom."<<endl;
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
        SecurityPersonal *mc = new SecurityPersonal("Murat Celik");
        SecurityPersonal *dn = new SecurityPersonal("Dorukhan Nerede");
        SecurityPersonal *ac = new SecurityPersonal("Aytac Celiktug");
        Department* se = new Department("Software Engineering",mc);
        Department* ce = new Department("Computer Engineering",dn);
        Department* cie = new Department("Civil Engineering",ac);
        Floor* firstM = new Floor("M 1st Floor", se);
        Floor* secondM = new Floor("M 2nd Floor", ce);
        Floor* firstC = new Floor("C 1st Floor", se);
        Floor* secondC = new Floor("C 2nd Floor", cie);
        Classroom* m101 = new Classroom("M101");
        m101->setFloor(firstM);
        Classroom* m102 = new Classroom("M102");
        m102->setFloor(firstM);
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
        mc->setIterator(classroomIterator);
        dn->setIterator(classroomIterator);
        ac->setIterator(classroomIterator);
        setResourceAllocationDepartmentofClassrooms();
    };

    void setResourceAllocationDepartmentofClassrooms(){
        for(classroomIterator->First(); !classroomIterator->Done(); classroomIterator->Next())
        {
            classroomIterator->getCurrent()->setResourceAllocationDepartment(this);
        }
    }

    void setAllAvailabilityOfClassrooms(bool toSet)
    {
        for(classroomIterator->First(); !classroomIterator->Done(); classroomIterator->Next())
        {
            if(!toSet){
                classroomIterator->getCurrent()->setAvailability(toSet);
                classroomIterator->getCurrent()->getLock()->openLock(Key::getKey());
            }else
            {
                classroomIterator->getCurrent()->setAvailability(toSet);
                classroomIterator->getCurrent()->getLock()->closeLock(Key::getKey());
            }
        }
    }

    void setExamToClassrooms(string className, Teacher* teacher)
    {
        for(classroomIterator->First(); !classroomIterator->Done(); classroomIterator->Next())
        {
            if(classroomIterator->getCurrent()->getName()==className)
            {
                if(classroomIterator->getCurrent()->getAvailability())
                {
                    classroomIterator->getCurrent()->setAvailability(false);
                    return;
                }
                else
                {
                    classroomIterator->getCurrent()->attach(teacher);
                    return;
                }
            }
        }
    }

    void printClassrooms()
    {
        cout << endl << "Iterating classrooms: " << endl;
        for (classroomIterator->First(); !classroomIterator->Done(); classroomIterator->Next()) {
            if(classroomIterator->getCurrent()->getAvailability())
                cout << "Available Classrooms -> " << classroomIterator->getCurrent()->getName() << endl ;
        }
        cout << endl;
     }

    void getClassLockState(string className)
    {
        for(classroomIterator->First(); !classroomIterator->Done(); classroomIterator->Next())
        {
            if(classroomIterator->getCurrent()->getName()==className)
                {
                    if(!classroomIterator->getCurrent()->getLock()->getState())
                        cout<< className +" is not locked, you can enter"<<endl;
                    else
                        cout << className +" is locked, you cannot enter"<<endl;
                    return;
                }
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

    void cancelExam(string className, Teacher* teacher)
    {
        for(classroomIterator->First(); !classroomIterator->Done(); classroomIterator->Next())
        {
            if(classroomIterator->getCurrent()->getName()==className)
                if(!classroomIterator->getCurrent()->getAvailability())
                    {
                        classroomIterator->getCurrent()->setAvailability(true);
                        return;
                    }
        }
    }

    void displaySchool()
    {
        ieu->display(1);
    }

    void setClassroomAvailability(string className)
    {
        for (classroomIterator->First(); !classroomIterator->Done(); classroomIterator->Next()) {
                if(classroomIterator->getCurrent()->getName()==className)
                    classroomIterator->getCurrent()->setAvailability(false);
        }
    }


private:
    abstractIterator *classroomIterator;
    abstractIterator *floorIterator;
    Campus *ieu;
};

void Classroom::notify(){
    for (unsigned int i = 0; i < teachers.size(); i++) {
        teachers[i]->update(this);
        _rad->setExamToClassrooms(teachers[i]->getClassroomWanted()->getName(),this->teachers[i]);
        _rad->setClassroomAvailability(teachers[i]->getClassroomWanted()->getName());
        this->detach(teachers[i]);
        return;
    }
}


int main()
{
    Teacher *uc = new Teacher("Ufuk Celikkan");
    Teacher *kk = new Teacher("Kaan Kurtel");
    ResourceAllocationDepartment* rad = new ResourceAllocationDepartment();
    rad->displaySchool(); // -> this is displaying the IEU campus
    rad->printClassrooms(); // before setting an exam displaying the available rooms
    rad->setExamToClassrooms("M101",uc);//setting an exam
    rad->printClassrooms(); // after setting an exam displaying the available rooms
    SecurityPersonal* sp = rad->getSecurityPersonalOfFloor("M 1st Floor");
    sp->askToUnlock(uc,"M101"); // Teacher is asking the correct security guard for the keys
    rad->getClassLockState("M101");
    rad->getClassLockState("M102"); // this is the classroom's lock state
    rad->setExamToClassrooms("M101",kk); // second teacher is saying that he wants the same class as the previous teacher so this teacher will be observer
    rad->cancelExam("M101",uc); // the first teacher is canceling the exam and this will result with the notification to the second teacher that wanted this class

    ResourceAllocationDepartment* rad2 = new ResourceAllocationDepartment();
    cout<<endl;
    rad2->setAllAvailabilityOfClassrooms(false); //on a second ieu setting all classrooms for history,foreing language exams
    rad2->getClassLockState("M101");
    rad2->setAllAvailabilityOfClassrooms(true);
    rad2->getClassLockState("M101");

    return 0;
}
