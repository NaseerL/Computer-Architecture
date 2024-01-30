/*
ECGR 5181
Assignment 1: Event Driven Simulator
*/

#include <iostream>
#include <bits/stdc++.h>
#include <time.h>
#include <chrono>
#include <vector>

using namespace std;

//This class was created in order to make a an event
//Event consists of a system time and a random integer
//Later in the code this event will be inserted into a vector and then be sorted
class event{
    
    private:
        double time_t;
        int rand_int_v;

    public:
        event(){}

        event(int time_t,int rand_int_v){

            this->time_t = time_t;
            this->rand_int_v = rand_int_v;
        }
            
        void set_time_t(int time_t){
            this->time_t = time_t;
        }

        void set_rand_int_v(int rand_int_v){
            this->rand_int_v = rand_int_v;
        }

        int get_time_t(){
            return time_t;
        }

        int get_rand_int_v(){
            return rand_int_v;
        }

};

//Retuns true or false for (event 1 time)>(event 2 time)
bool compare_event_t(event event1, event event2){

    return (event1.get_time_t() < event2.get_time_t());
}

//Function used to sort vector consisting of events and then print them on screen
//The vector is sorted by time and then radom integer
vector<event> sort_events(vector<event> MEQ){


    //This code sorts the time first and then random integer. It is a normal bubble sort but modified to 
    //sort events 
    int temp1, temp2;

    for(int i=0 ; i<MEQ.size() ; i++){

        for(int j=0 ; j<MEQ.size() ; j++){

            if( ((MEQ[j].get_time_t() == MEQ[j+1].get_time_t()) && (MEQ[j].get_rand_int_v() > MEQ[j+1].get_rand_int_v()))){

                temp1 = MEQ[j].get_rand_int_v();
                temp2 = MEQ[j].get_time_t();

                MEQ[j].set_rand_int_v(MEQ[j+1].get_rand_int_v());
                MEQ[j].set_time_t(MEQ[j+1].get_time_t());

                MEQ[j+1].set_rand_int_v(temp1);
                MEQ[j+1].set_time_t(temp2);
            }
        }
    }

    return MEQ;

}


int main(){

    vector<event> MEQ, FinalMEQ;
    int rand_int_v;
    double timet;

    //Used to kep track of the time program begins
    //Will be used for time t
    auto start = chrono::steady_clock::now();

    cout << endl << "20 Radomly generated Events" << endl << endl;
    
    //Generates 20 random events
    for(int i=0 ; i<20 ; i++){

        //generates a random number v
        rand_int_v = rand()%1000 + 1;

        //Helps keep track and update time t
        auto endone = chrono::steady_clock::now();
        timet = double(chrono::duration_cast < chrono::seconds > (endone - start).count());

        //This delay is used to show the code works.
        //If the delay is not added the code runs faster than a seconds
        //and all time t will be 0
        for(int delay=0; delay<100000000 ; delay++);

        cout << "Event " << i << " - (" << timet << " , " << rand_int_v << ")" << endl;

        event events(timet,rand_int_v);
        MEQ.push_back(events);
    }

    //Sort these 20 events
    sort_events(MEQ);

    //Generates 80 random events
    for(int j=0 ; j<80 ; j++){

        //generates a random number v
        rand_int_v = rand()%1000 + 1;

        timet = MEQ[0].get_rand_int_v() + timet;

        //Helps keep track and update time t
        auto endtwo = chrono::steady_clock::now();
        timet = double(chrono::duration_cast < chrono::seconds > (endtwo - start).count());

        //This delay is used to show the code works.
        //If the delay is not added the code runs faster than a seconds
        //and all time t will be 0
        for(int delay=0; delay<100000000 ; delay++);

        event events(timet,rand_int_v);
        MEQ.push_back(events);
    }

    cout << endl << endl << "100 events generated and sorted by time" << endl << endl;

    //FinalMEQ holds the sorted array
    FinalMEQ = sort_events(MEQ);

    //Display the sorted array
    for(int j=0; j<MEQ.size(); j++){

        cout << "Event " << j << " - (" << FinalMEQ[j].get_time_t() << " , " << FinalMEQ[j].get_rand_int_v() << ")" << endl;
    }

    return 0;
}