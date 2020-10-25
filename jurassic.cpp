#include <QThread>
#include <QMutex>
#include <QSemaphore>
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

QSemaphore* carsAvailable;
QSemaphore carsTaken;
QSemaphore visitorsWaiting(0);
int N;
//QSemaphore visitorsRemaining;


int* arrOfWaitingVisitors;
int indexOfWaitingVisitors = 0;
int indexOfNextVisitor = 0;
int visitorsServed = 0;

QMutex ctrl1, ctrl2, ctrl3, ctrl4, ctrl5, ctrl6, ctrl7, ctrl8;



class Car : public QThread{
private:
    int ID;
    int VID;
    int waitingTime;
public:
    Car(int ID, int waitingTime){     
        this -> ID = ID + 1;
        this -> waitingTime = waitingTime;

    }
    void run(){

    	while(1){

    		if(!visitorsWaiting.tryAcquire()){
	    		ctrl2.lock();
	    		cout << "Car " << ID << " is waiting for a visitor\n";
	    		ctrl2.unlock();
	    		visitorsWaiting.acquire(); // the car waits until it can aquire a visitor

	    		ctrl6.lock();
	    		VID = arrOfWaitingVisitors[indexOfNextVisitor];
	    		indexOfNextVisitor++;
	    		ctrl6.unlock();
    		}
    		else {
    			// if it can aquire immediatly, then
    			ctrl7.lock();
    			VID = arrOfWaitingVisitors[indexOfNextVisitor];
    			indexOfNextVisitor++;
    			ctrl7.unlock();
    		}


    		ctrl8.lock();
    		cout << "Car " << ID << " got visitor " << VID << " as a passanger\n";
    		visitorsServed++;
    		visitorsWaiting.release();
    		ctrl8.unlock();
    		sleep((long) waitingTime); // to simulate the drive around

    		ctrl4.lock();
    		carsAvailable -> release();
    		cout << "Car " << ID << " returned\n";
    		cout << "Visitor " << VID << " is leaving the park\n";
    		ctrl4.unlock();

    		// break once all visitors have been served
    		if(visitorsServed == N) 
    			break;

    	}
    	

    }
};


class Visitor : public QThread{
private:
    int ID;
    int waitingTime;
public:
    Visitor(int ID, int waitingTime){
    	this -> ID = ID + 1;					// IDs start at 1
    	this -> waitingTime = waitingTime;

    	ctrl1.lock();
    	cout << "Visitor " << ID << " Entered the park\n";
    	ctrl1.unlock();

    } 
    void run(){
    	sleep((long) waitingTime);

    	// a visitor is added to the array after they wake up, visitorsWaiting semaphore is also incremented
    	ctrl5.lock();
    	arrOfWaitingVisitors[indexOfWaitingVisitors] = ID;
    	indexOfWaitingVisitors++;
    	visitorsWaiting.release();
    	ctrl5.unlock();


    	if(!carsAvailable -> tryAcquire()){
    		ctrl2.lock();
    		cout << "Visitor " << ID << " is waiting for a car\n";
    		ctrl2.unlock();
    		carsAvailable -> acquire(); // wait to acquire a car
    	}
    	// else {


    	// }
 
    }
};

//================================================
int main(int argc, char** argv){

    srand(time(NULL)^getpid());

     if (argc != 3){
        std::cout << "Incorrect Number of Arguments!" <<std::endl;
        return 0; 
    }
    
    N = atoi(argv[1]);					// number of visitors
    int M = atoi(argv[2]);                 	// number of cars

    Visitor* v[N];
    Car* c[M];
    arrOfWaitingVisitors = new int[N];
    int waitingTime;
    carsAvailable = new QSemaphore(M);

    for(int i = 0; i < N; i++){
    	waitingTime = 1 + rand() % 5;    // a visitor will roam for 1-5 seconds	
    	v[i] = new Visitor(i, waitingTime);
    	v[i] -> start();
    }

    for(int i = 0; i < M; i++){
    	waitingTime = 1 + rand() % 3;	// a car will drive for 1-3 seconds
    	c[i] = new Car(i, waitingTime);
    	c[i] -> start();
    }

    for(int i = 0; i < N; i++)
        v[i] -> wait();

    for(int i = 0; i < M; i++)
    	c[i] -> wait();

  
    return 0;
}