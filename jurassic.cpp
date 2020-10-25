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



class Car : public QThread{
private:
    int ID;
public:
    Car(int ID){     
        this -> ID = ID + 1;


    }
    void run(){
           
    }
};


class Visitor : public QThread{
private:
    int ID;
    int waitingTime;
public:
    Visitor(int ID, int waitingTime){
    	this -> ID = ID;
    	this -> waitingTime = waitingTime;

    	cout << "Visitor " << ID << " Entered the park\n";

    	
    } 
    void run(){
    	sleep((long) waitingTime);
    	cout << "Visitor " << ID << " slept for " << waitingTime << " seconds\n";
 
    }
};

//================================================
int main(int argc, char** argv){

    srand(time(NULL)^getpid());

     if (argc != 3){
        std::cout << "Incorrect Number of Arguments!" <<std::endl;
        return 0; 
    }
    
    int N = atoi(argv[1]);					// number of visitors
    int M = atoi(argv[2]);                 	// number of cars

    Visitor* v[N];
    Car* c[M];
    int waitingTime;
    carsAvailable = new QSemaphore(M);


    for(int i = 0; i < N; i++){
    	waitingTime = rand() % 5;    	
    	v[i] = new Visitor(i+1, waitingTime);

    }

  
    return 0;
}