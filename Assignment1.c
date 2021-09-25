#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

struct process{
  int ID;
  int priority;
  int arrivalTime;
  int burstTime;
  int completionTime;
  int TAT;
  int startTime;
  int waitTime;
  bool finished;
};

void FCFS(struct process *processArray, struct process *resultArray, int length){
  int finished = 0; //keeps track of the amount of process completed
  int lowestTime = ((unsigned int)~0 >> 1);
  int location = ((unsigned int)~0 >> 1);
  int totalTime = 0;
  int i = 0;
  //int x = 0;
  while(finished != length){
    while(i < length){
      if(processArray[i].finished == false && processArray[i].arrivalTime < lowestTime){
        lowestTime = processArray[i].arrivalTime;
        location = i;
      }
      i++;
    }
    resultArray[finished] = processArray[location];
    processArray[location].finished = true;

    resultArray[finished].startTime = totalTime;
    totalTime = totalTime + resultArray[finished].burstTime;
    resultArray[finished].completionTime = totalTime;
    resultArray[finished].TAT = resultArray[finished].completionTime - resultArray[finished].arrivalTime;
    resultArray[finished].waitTime = resultArray[finished].startTime - resultArray[finished].arrivalTime;

    //these variables need to be reset before going back into the loop
    lowestTime = ((unsigned int)~0 >> 1);
    finished++;
    i = 0;
  }
}

void setprocess(struct process *input, int length, int maxRand){
  int i = 0;
  while(i < length){
    input[i].ID = i;
    input[i].priority = (rand() % maxRand);
    input[i].arrivalTime = (rand() % maxRand);
    input[i].burstTime = (rand() % maxRand);
    input[i].finished = false;
    i++;
  }
}



int main(){
  srand(time(0)); //seed with time for random results everytime

  int maxRand = 20;
  int arrayLength = 10; //needs to come from input
  int i = 0;

  float avgWait;

  struct process processArray[arrayLength];
  struct process FCFSArray[arrayLength];

  setprocess(processArray, arrayLength, maxRand);
  printf("Entered Info\n");
  while(i < arrayLength){
    printf("Process id: %d priority: %d arrivalTime: %d burstTime: %d\n", processArray[i].ID, processArray[i].priority, processArray[i].arrivalTime, processArray[i].burstTime);
    i++;
  }

  printf("\n");
  i=0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// FCFS
/////////////////////////////////////////////////////////////////////////////////////////////////////////
  FCFS(processArray, FCFSArray, arrayLength);
  printf("FCFS results\n");
  while(i < arrayLength){
    printf("ID: %d priority: %d AT: %d BT: %d CT: %d TAT: %d\n", FCFSArray[i].ID, FCFSArray[i].priority, FCFSArray[i].arrivalTime, FCFSArray[i].burstTime, FCFSArray[i].completionTime, FCFSArray[i].TAT);
    i++;
  }
  i = 0;
  while(i < arrayLength){
    avgWait = avgWait + FCFSArray[i].waitTime;
    i++;
  }

  i=0;
  avgWait = avgWait/arrayLength;

  printf("AVG wait time for FCFS (first come first serve) = %f\n", avgWait);
  return 0;
}
