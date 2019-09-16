/*
 * Troy Seide
 * Lab 1 3207
 * Event Simulator
 * 09/16/19
 */
/* 
 * File:   main.cpp
 * Author: Owner
 *
 * Created on August 29, 2019, 12:40 AM
 */

#include <cstdlib>
#include <iostream>
#include <queue>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;
struct Process{
    int id;
    int current_state; //(state value continues to increment)
    //0:CPU Queue
    //1:CPU
    //2:Disk1Queue
    //3:Disk1
    //4:Disk2Queue
    //5:Disk2
    //6:simulation finished event
    int time;
};

/*
 * 
 */
void text_file_config();
int random_number(int min, int max);
void task_initializer();
void Statistics();
fstream logfile;
int t[13];
int Seed=t[0], Arrive_Min=t[3], Arrive_Max=t[4], CPU_Min=t[6], CPU_Max=t[7], Disk1_Min=t[8], Disk1_Max=t[9], Disk2_Min=t[10], Disk2_Max=t[11], CPU_tp=0, Disk1_tp=0, Disk2_tp=0;
int Quit_Prob=t[5], init_time=t[1], Fin_time=t[2], CPU_head=1, CPU_tail=0, Disk1_head=random_number(1,7), Disk1_tail=0, Disk2_head=random_number(1,7), Disk2_tail=0, Priority_head=1, Priority_tail=0; //(whatever event is currently running it will go back in the CPU queue)
struct Process CPU[10000], Disk1[10000], Disk2[10000], Priority[10000]; 
int main(int argc, char** argv) {
    text_file_config();   
    task_initializer();
    Statistics();
    return 0; 
}

void task_initializer(){ 
    srand(Seed); 
    int time=0; //(increments after the while loop runs a cycle)
    struct Process process1= {1, 0, 0};
    int nextprocessid=2;
    CPU[CPU_head]=process1;
    CPU_head++;
    
    struct Process process2= {0,6,Fin_time}; //Simulation finished events goes into priority queue
    Priority[Priority_head]=process2;
    Priority_head++;
    
    int timeOfNextprocess=time+random_number(Arrive_Min, Arrive_Max);
    bool CPUfree=true;
    bool Disk1free=true;
    bool Disk2free=true;  
    while(Priority_head>Priority_tail){
        if(time==timeOfNextprocess){
            struct Process process3= {nextprocessid,0, 0};
            nextprocessid++;
            CPU[CPU_head]=process3;
            CPU_head++;
            timeOfNextprocess=time+random_number(Arrive_Min, Arrive_Max); //helps to determine when the next event is going to arrive   
        }
        if(CPUfree && CPU_head>CPU_tail){
            Priority[Priority_head].id=CPU_tail; //(not sure about this line of code)
            Priority[Priority_head].current_state=1;
            Priority[Priority_head].time=time+random_number(CPU_Min, CPU_Max);
            CPU_tail++;
            Priority_head++;
            CPUfree=false;
            CPU_tp++;
        }
        if(Disk1free && Disk1_head>Disk1_tail){
            Priority[Priority_head].id=Disk1_tail; //(not sure about this line of code)
            Priority[Priority_head].current_state=3;
            Priority[Priority_head].time=time+random_number(Disk1_Min, Disk1_Max);
            Disk1_tail++;
            Priority_head++;
            Disk1free=false;
        }
        if(Disk2free && Disk2_head>Disk2_tail){
            Priority[Priority_head].id=Disk2_tail; //(not sure about this line of code)
            Priority[Priority_head].current_state=5;
            Priority[Priority_head].time=time+random_number(Disk2_Min, Disk2_Max);
            Disk2_tail++;
            Priority_head++;
            Disk2free=false;
        } 
        for(int i=0; i<(sizeof(Priority)/sizeof(Priority[0])); i++){ //(for loop that goes through the elements of the priority queue to check if the current state == the time)
            if(Priority[i].time==time){
                if(Priority[i].current_state==1){
                    CPUfree=true;
                    int t=random_number(0,10); //(code is getting stuck here) 
                    //cout<<"this is : "<<t<<"\n";
                    //cout<<"This is disk1 head:"<<Disk1_head<<"\n";
                    //cout<<"This is disk2 head:"<<Disk2_head<<"\n";
                    
                    if(t<Quit_Prob){
                        ofstream logfile;
                        logfile.open("logfile.txt", std::ios::app);
                        logfile<<"\nBased off the quit probability the system has exited."; 
                        logfile.close();
                    }
                    else{
                        if((Disk1_head-Disk1_tail)>(Disk2_head-Disk2_tail)){
                            struct Process process4={Priority[i].id, 2, 0}; 
                            Disk1[Disk1_head]=process4;
                            Disk1_head++;
                            Priority_tail++;
                            ofstream logfile;
                            logfile.open("logfile.txt", std::ios::app);
                            logfile<<"\nA new process has entered Disk 1 Queue at time: "<<time;
                            logfile.close();
                        }
                        else if((Disk1_head-Disk1_tail)==(Disk2_head-Disk2_tail)){
                            int between=(rand() < RAND_MAX/2) ? 2 : 4;
                            cout<<"This value = between: "<< between<<"\n";
                            struct Process process4={Priority[i].id, between, 0}; //trying to only choose between 2 and 4 because Disk1 Queue and Disk2 Queue
                            Priority_tail++;
                            
                            if(between==2){
                                ofstream logfile;
                                logfile.open("logfile.txt", std::ios::app);
                                logfile<<"\nBased off of the random probability the process entered the Disk 1 Queue"<<time;
                                logfile.close();
                           }
                            else{
                                ofstream logfile;
                                logfile.open("logfile.txt", std::ios::app);
                                logfile<<"\nBased off of the random probability the process entered the Disk 2 Queue"<<time; 
                                logfile.close();
                           }
                        }
                        else{
                            struct Process process4={Priority[i].id, 4, 0}; 
                            Disk2[Disk2_head]=process4;
                            Disk2_head++;
                            Priority_tail++;
                            ofstream logfile;
                            logfile.open("logfile.txt", std::ios::app);
                            logfile<<"\nA new process has entered Disk 2 Queue at time: "<<time;
                            logfile.close();
                        }
                    } 
                }
                
                if(Priority[i].current_state==3){
                    ofstream logfile;
                    logfile.open("logfile.txt", std::ios::app);
                    logfile<<"\nA new process has entered Disk 1 queue and plans on moving into Disk1 at time = "<<time;
                    struct Process process5={Priority[i].id, 0, 0}; 
                    CPU[CPU_head]=process5;
                    CPU_head++;
                    Priority_tail++;
                    Disk1free=true;
                    Disk1_tp++;
                }
                if(Priority[i].current_state==4){
                    ofstream logfile;
                    logfile.open("logfile.txt", std::ios::app);
                    logfile<<"\nA new process has entered Disk 2 queue and plans on moving into Disk 2 at time = "<<time;
                    struct Process process6={Priority[i].id, 0, 0}; 
                    CPU[CPU_head]=process6;
                    CPU_head++;
                    Priority_tail++;
                    Disk2free=true;
                    Disk2_tp++;
                }
                if(Priority[i].current_state==6){
                    //logfile.open("logfile.txt");
                    FILE *t;
                    t=fopen("logfile.txt", "a");
                    fprintf(t, "%s","The process has entered the simulation finished event at time = ");
                    fprintf(t, "%d", time);
                    //logfile<<"The process has entered the simulation finished event, the current state= "<<Priority[i].current_state;
                    Priority_tail++;
                }
                
            }
        }
        //cout<<"This is disk1 head:"<<Disk1_head<<"\n";
        //cout<<"This is disk2 head:"<<Disk2_head<<"\n";
        time++;
        init_time++;
        if(init_time==Fin_time){
            exit(0);
        }
    }
}
int random_number(int min, int max){
    //cout<<"T";
    return (rand()%max)+min;
}

int test(){
    logfile.open("logfile.txt"); 
}
void text_file_config(){
    //cout<<"T";
    ifstream file ("GDEfile(3207).txt");
    ofstream logfile;   
    logfile.open("logfile.txt");
    string words;
    string info= "insert text here";
    int value;
    int i=0;
    int t[]={Seed,init_time,Fin_time, Arrive_Min, Arrive_Max, Quit_Prob, CPU_Min, CPU_Max, Disk1_Min, Disk1_Max, Disk2_Min, Disk2_Max};
    while(file>>words>>value){
        t[i]=value;
       //cout<<value<<"\n";
       i++;
    }
    Seed=t[0];
    init_time=t[1];
    Fin_time=t[2];
    Arrive_Min=t[3];
    Arrive_Max=t[4];
    Quit_Prob=t[5];
    CPU_Min=t[6];
    CPU_Max=t[7];
    Disk1_Min=t[8];
    Disk1_Max=t[9];
    Disk2_Min=t[10];
    Disk2_Max=t[11];
    logfile<<"Seed: "<<Seed<<"\n";
    logfile<<"Init_Time: "<<init_time<<"\n";
    logfile<<"Fin_Time: "<<Fin_time<<"\n";
    logfile<<"Arrive_Min: "<<Arrive_Min<<"\n";
    logfile<<"Arrive_Max: "<<Arrive_Max<<"\n";
    logfile<<"Quit_Prob: "<<Quit_Prob<<"\n";
    logfile<<"CPU_Min: "<<CPU_Min<<"\n";
    logfile<<"CPU_Max: "<<CPU_Max<<"\n";
    logfile<<"Disk1_Min: "<<Disk1_Min<<"\n";
    logfile<<"Disk1_Max: "<<Disk1_Max<<"\n";
    logfile<<"Disk2_Min: "<<Disk2_Min<<"\n";
    logfile<<"Disk2_Max: "<<Disk2_Max<<"\n";
    
    logfile.close();
}
void Statistics(){
    ofstream statistics;
    statistics.open("Statistics.txt");
    int CPU_Average=(CPU_Min+CPU_Max)/2;
    int Disk1_Average=(Disk1_Min+Disk1_Max)/2;
    int Disk2_Average=(Disk2_Min+Disk2_Max)/2;
    int Time_of_server=Fin_time-init_time;
    statistics<< "The CPU Max is: "<<CPU_Max<<"\n";
    statistics<< "The CPU Average is: "<<CPU_Average<<"\n";
    statistics<< "The CPU throughput is: "<<CPU_tp<<"\n";
    statistics<< "The Disk1 Max is: "<<Disk1_Max<<"\n";
    statistics<<"The Disk1 Average is: "<<Disk1_Average<<"\n";
    statistics<< "The Disk1 throughput is: "<<Disk1_tp<<"\n";
    statistics<< "The Disk2 Max is: "<<Disk2_Max<<"\n";
    statistics<<"The Disk2 Average is: "<<Disk2_Average<<"\n";
    statistics<< "The Disk2 throughput: "<<Disk2_tp<<"\n";
    statistics<<"The utilization of the server is: "<<Time_of_server;
}
