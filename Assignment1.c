#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef struct process{
  int ID;
  int priority;
  int arrivalTime;
  int burstTime;
  int completionTime;
  int TAT;
  int startTime;
  int waitTime;
  bool finished;
  bool queued;
}process;

void FCFS(struct process *processArray, struct process *resultArray, int length){
  int finished = 0; //keeps track of the amount of process completed
  int lowestTime = ((unsigned int)~0 >> 1); // lowest arrival time
  int location;
  int totalTime = 0;
  bool processFound = false;
  int i = 0;
  //int x = 0;
  while(finished != length){
    while(i < length){
      if(processArray[i].finished == false && processArray[i].arrivalTime < lowestTime && processArray[i].arrivalTime <= totalTime){
        lowestTime = processArray[i].arrivalTime;
        location = i;
        processFound = true;
      }
      i++;
    }
    if(processFound){
      resultArray[finished] = processArray[location];
      processArray[location].finished = true;

      resultArray[finished].startTime = totalTime;
      totalTime = totalTime + resultArray[finished].burstTime;

      resultArray[finished].completionTime = totalTime;
      resultArray[finished].TAT = resultArray[finished].completionTime - resultArray[finished].arrivalTime;
      resultArray[finished].waitTime = resultArray[finished].startTime - resultArray[finished].arrivalTime;
      finished++;
    }
    else
      totalTime += 1;
    //these variables need to be reset before going back into the loop
    lowestTime = ((unsigned int)~0 >> 1);
    i = 0;
    processFound = false;
  }
}

void SJF(struct process *processArray, struct process *resultArray, int length){
  process *qArray = malloc(length * sizeof(process));
  int q = 0; //tracks the amont of process queued

  int finished = 0; //keeps track of the amount of process completed
  int lowestTime = ((unsigned int)~0 >> 1); //lowest burst time
  int location;
  int totalTime = 0;
  bool processFound = false;
  int i = 0;
  int x = ((unsigned int)~0 >> 1);
  /*
    this first loop finds the first job to arrival an executes it first
    then the block after that is setting all the values for that
    first job.

  */
  while(i < length){
    if(processArray[i].arrivalTime < x){
      location = i;
      x = processArray[i].arrivalTime;
    }
    i++;
  }
  qArray[q++] = processArray[location];
  resultArray[finished] = qArray[0];
  resultArray[finished].startTime = resultArray[finished].arrivalTime;
  resultArray[finished].completionTime = resultArray[finished].startTime + resultArray[finished].burstTime;
  resultArray[finished].waitTime = resultArray[finished].startTime - resultArray[finished].arrivalTime; // should always be 0
  resultArray[finished].TAT = resultArray[finished].completionTime - resultArray[finished].arrivalTime; //should be = to BT
  resultArray[finished].finished = true;
  resultArray[finished].queued = true;
  finished++;

  totalTime = resultArray[0].completionTime;
  //fill the queue for the jobs added while last on ran
  for(i = 0; i < length; i++){
    if(processArray[i].arrivalTime <= totalTime && !processArray[i].queued){
      qArray[q++] = processArray[i];
      processFound = true;
    }
  }
  printf("q -> %d\n", q);
  for(i=0; i< q; i++){
    printf("ID: %d  AT: %d BT: %d \n", qArray[i].ID, qArray[i].arrivalTime, qArray[i].burstTime);
  }

  while(finished != length){
    for(i = 0; i < q; i++){
      printf("q->%d\n", q);
      printf("\n");
      if(!qArray[i].finished && qArray[i].burstTime < lowestTime){
        location = i;
        //printf("location -> %d\n", location);
        lowestTime = qArray[location].burstTime;
        processFound = true;
      }
    }
    if(processFound){
      qArray[location].finished = true;
      resultArray[finished] = qArray[location];
      resultArray[finished].startTime = resultArray[finished].arrivalTime;
      resultArray[finished].completionTime = resultArray[finished].startTime + resultArray[finished].burstTime;
      resultArray[finished].waitTime = resultArray[finished].startTime - resultArray[finished].arrivalTime;
      resultArray[finished].TAT = resultArray[finished].completionTime - resultArray[finished].arrivalTime;
      totalTime = resultArray[finished].burstTime;
      finished++;
    }
    else{
      totalTime += 100;
      printf("SJF didnt find job\n\n\n\n\n\n");
      //finished++;
    }

    //Adds any jobs not in the queue yet if they have arrived
    //if all jobs in queue it is skipped
    if(q < length){
      //printf("adding\n");
      for(i = 0; i < length; i++){
        if(processArray[i].arrivalTime <= totalTime && (!processArray[i].queued || !processArray[i].finished)){
          //printf("added %d\n",i);
          qArray[q++] = processArray[i];
        }
      }
    }
    lowestTime = ((unsigned int)~0 >> 1);
    processFound = false;
  }
  //free(qArray);
}

void setprocess(struct process *input, int length, int maxRand){
  int i = 0;
  while(i < length){
    input[i].ID = i;
    input[i].priority = (rand() % maxRand);
    input[i].arrivalTime = (rand() % maxRand);
    input[i].burstTime = (rand() % maxRand);
    input[i].finished = false;
    input[i].queued = false;
    i++;
  }
}

void resetProcess(struct process *input, int length){
  for(int i =0; i < length; i++){
    input[i].finished = false;
    input[i].queued = false;
  }
}

int main(){
  srand(time(0)); //seed with time for random results everytime
  int maxRand = 20;
  int arrayLength; //needs to come from input
  int i = 0;
  float avgWait;
  printf("How many process would you like: ");
  scanf("%d", &arrayLength);
  process *processArray = malloc(arrayLength * sizeof(process));
  process *FCFSArray = malloc(arrayLength * sizeof(process));
  process *SJFArray = malloc(arrayLength * sizeof(process));

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
    printf("ID: %d st: %d priority: %d AT: %d BT: %d CT: %d TAT: %d\n", FCFSArray[i].ID, FCFSArray[i].startTime, FCFSArray[i].priority, FCFSArray[i].arrivalTime, FCFSArray[i].burstTime, FCFSArray[i].completionTime, FCFSArray[i].TAT);
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// SJF
/////////////////////////////////////////////////////////////////////////////////////////////////////////
  resetProcess(processArray, arrayLength);
  /*SJF(processArray, SJFArray, arrayLength);
  printf("SJF results\n");
  while(i < arrayLength){
    printf("ID: %d st: %d priority: %d AT: %d BT: %d CT: %d TAT: %d\n", SJFArray[i].ID, SJFArray[i].startTime, SJFArray[i].priority, SJFArray[i].arrivalTime, SJFArray[i].burstTime, SJFArray[i].completionTime, SJFArray[i].TAT);
    i++;
  }
  i = 0;
  while(i < arrayLength){
    avgWait = avgWait + SJFArray[i].waitTime;
    i++;
  }

  i=0;
  avgWait = avgWait/arrayLength;

  printf("AVG wait time for SJF (Shortest Job First) = %f\n", avgWait);*/
  free(SJFArray);
  free(processArray);
  free(FCFSArray);
  return 0;
}
