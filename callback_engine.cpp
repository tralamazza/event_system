// Example program
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <mutex>

/**
 * Example subject class to demonsrate observer pattern
 */
class Subject
{

    std::vector<class Observer *> observers;
    std::mutex mtx;
    int value;

public:
    /**
     * Add observer to observers vector. If already registered or nullptr throw
     * runtime error
     *
     * @param obs pointer to observer object
     */
    void add_observer(Observer *obs)
    {
        // local lock_guard to lock mtx for guaranted destruction/exception
        std::lock_guard<std::mutex> lock(mtx);

        // Check if nullptr
        if (nullptr == obs)
        {
            throw std::runtime_error("Observer is nullptr");
        }

        // Check if already present in vector
        auto it = std::find(observers.begin(), observers.end(), obs);
        if (it != observers.end())
        {
            throw std::runtime_error("Observer already added");
        }

        observers.emplace_back(obs);
    }

    /**
     * Remove observer from vector
     *
     * @param obs pointer to observer object
     */
    void remove_observer(Observer *obs)
    {
        // local lock_guard to lock mtx for guaranted destruction/exception
        std::lock_guard<std::mutex> lock(mtx);

        // Check if nullptr
        if (nullptr == obs)
        {
            throw std::runtime_error("Observer is nullptr");
        }

        // Check if observer is present in vector
        auto it = std::find(observers.begin(), observers.end(), obs);
        if (it != observers.end())
        {
            observers.erase(it);
        }
        else
        {
            throw std::runtime_error("Trying to remove unregistered observer");
        }
    }

    /**
     * Example value set and notify as callback that value has been set
     */
    void setVal(int val)
    {
        value = val;
        notify();
    }

    /**
     * Example value get
     */
    int getVal()
    {
        return value;
    }

    /**
     * Notify method for observer pattern
     */
    void notify();
};

/**
 * Observer base class
 *
 */
class Observer
{

    /**
     * Observed object
    */
    Subject *model;

public:
    /**
     * Observer constructor
     */
    Observer(Subject *mod)
    {

        model = mod;
        model->add_observer(this);
    }

    /**
     * Abstract observer update method
     */
    virtual void update() = 0;

protected:
    Subject *getSubject() { return model; }
};

/**
 * Notify method for observed object
 */
void Subject::notify()
{
    // local lock_guard to lock mtx for guaranted destruction/exception
    std::lock_guard<std::mutex> lock(mtx);

    for (int i = 0; i < observers.size(); i++)
    {
        observers[i]->update();
    }
}

/**
 * Example observer method
 */
class ObserverA : public Observer
{
public:
    ObserverA(Subject *subj) : Observer(subj){};
    /**
     * Callback method for observed object
     */
    void update() override
    {
        int v = getSubject()->getVal();
        std::cout << "Subject value " << v << std::endl;
        std::cout << "Observer A: Subject value multipled by 2 " << v * 2 << std::endl;
    }
};

/**
 * Example observer method
 */
class ObserverB : public Observer
{
public:
    ObserverB(Subject *subj) : Observer(subj){};
    /**
     * Callback method for observed object
     */
    void update() override
    {
        int v = getSubject()->getVal();

        std::cout << "Subject value " << v << std::endl;
        std::cout << "Observer B: Subject value multipled by 2 " << v * 2 << std::endl;
    }
};

int main()
{

    Subject subj;

    std::cout << "Add observers" << std::endl;
    ObserverA aObs2(&subj);
    ObserverB bObs1(&subj);

    subj.setVal(14);

    std::cout << "Remove observer A" << std::endl;
    subj.remove_observer(&aObs2);

    subj.setVal(10);
}