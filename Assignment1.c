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
  int runTime;
  int completionTime;
  int TAT;
  int startTime;
  int waitTime;
  bool finished;
  bool queued;
}process;

typedef struct gantt{
  process Process;
  int enterTime;
  int leaveTime;
}gantt;

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
  bool qFound = false;

  int finished = 0; //keeps track of the amount of process completed
  int lowestTime = ((unsigned int)~0 >> 1); //lowest burst time
  int location;
  int totalTime = 0; //time that has passed
  bool processFound = false; //finds the next process to run
  int i = 0;
  int x = ((unsigned int)~0 >> 1); //the l
  /*
    this first loop finds the first job to arrival an executes it first
    then the block after that is setting all the values for that
    first job.

  */
  while(i < length){
    if(processArray[i].arrivalTime < lowestTime){
      location = i;
      lowestTime = processArray[i].arrivalTime;
    }
    i++;
  }
  lowestTime = ((unsigned int)~0 >> 1);
  /*////////////////////////////////////////////////////
  these are all the calues for the first processed that is finished

  processArray[location].finished = true;
  processArray[location].queued = true;
  these two lines ensure that no duplicates are added into the qArray
  without these you get unexpected results

  ////////////////////////////////////////////////////*/
  resultArray[finished] = processArray[location];
  resultArray[finished].startTime = resultArray[finished].arrivalTime;
  resultArray[finished].completionTime = resultArray[finished].startTime + resultArray[finished].burstTime;
  resultArray[finished].waitTime = resultArray[finished].startTime - resultArray[finished].arrivalTime; // should always be 0
  resultArray[finished].TAT = resultArray[finished].completionTime - resultArray[finished].arrivalTime; //should be = to BT
  resultArray[finished].finished = true;
  resultArray[finished].queued = true;
  processArray[location].finished = true;
  processArray[location].queued = true;
  totalTime = resultArray[finished].completionTime;
  finished++;


  //fill the queue for the jobs added while last one ran
  for(i = 0; i < length; i++){
    if(processArray[i].arrivalTime <= totalTime && !processArray[i].finished){
      qArray[q++] = processArray[i];
      processArray[i].queued = true;
    }
  }

  /*///////////////////////////////////////////////
  The first while loop makes sure that all process have been completed before
  we exit the program
  ///////////////////////////////////////////////*/
  while(finished != length){
    /*///////////////////////////////////
    this for loop checks to see which process needs to be ran next
    //////////////////////////////////*/
    for(i = 0; i < q; i++){
      if(!qArray[i].finished && qArray[i].burstTime < lowestTime){
        location = i;
        //printf("location -> %d\n", location);
        lowestTime = qArray[location].burstTime;
        processFound = true;
      }
    }
    /*///////////////////////////////////
    adding all of the variables to result array the same way as it was done for the
    first one
    //////////////////////////////////*/
    if(processFound){
      qArray[location].finished = true;
      resultArray[finished] = qArray[location];
      resultArray[finished].startTime = totalTime;
      resultArray[finished].completionTime = resultArray[finished].startTime + resultArray[finished].burstTime;
      resultArray[finished].TAT = resultArray[finished].completionTime - resultArray[finished].arrivalTime;
      resultArray[finished].waitTime = resultArray[finished].TAT - resultArray[finished].burstTime;
      totalTime = resultArray[finished].completionTime;
      finished++;
    }
    /*///////////////////////////////////
    if we enter this else it means that there is currently nothing
    in the queue and we need to just increase the time till
    something is added
    //////////////////////////////////*/
    else{
      totalTime++;
    }
    /*///////////////////////////////////
    Adds any jobs not in the queue yet if they have arrived
    if all jobs in queue it is skipped
    //////////////////////////////////*/
    if(q < length){
      for(i = 0; i < length; i++){
        if(processArray[i].arrivalTime <= totalTime && !processArray[i].queued ){
          qArray[q++] = processArray[i];
          processArray[i].queued =  true;
        }
      }
    }
    //reset these variable before restarting the while loop
    lowestTime = ((unsigned int)~0 >> 1);
    processFound = false;
  }
  free(qArray);
}

int priority(struct process *processArray, struct process *resultArray, struct gantt *gantArray, int length){
  process *qArray = malloc(length * sizeof(process));
  int q = 0; //tracks the amont of process queued
  bool qFound = false;

  int gantLength = 1;
  int gantPos = 0;
  int finished = 0; //keeps track of the amount of process completed
  int lowTime = ((unsigned int)~0 >> 1); // for the first job
  int highestPriority = -1; //highestPriority
  int location;
  int totalTime = 0; //time that has passed
  bool processFound = false; //finds the next process to run
  bool processFinished = false;
  int i = 0;
  int x = ((unsigned int)~0 >> 1); //the l
  /*
    this first loop finds the first job to arrival an executes it first
    then the block after that is setting all the values for that
    first job.

  */
  while(i < length){
    if(processArray[i].arrivalTime < lowTime){
      location = i;
      lowTime = processArray[i].arrivalTime;
    }
    i++;
  }
  totalTime = processArray[location].arrivalTime;
  // printf("tT -> %d\n", totalTime);
  qArray[q] = processArray[location];
  processArray[location].queued = true;
  gantArray[gantPos].enterTime = totalTime;
  gantArray[gantPos].Process = qArray[q];
  qArray[q++].startTime = totalTime;
  //printf("st q -> %d\n, st pa -> %d\n location -> %d \n", qArray[q -1].startTime, processArray[location].startTime), location;
  /*////////////////////////////////////////////////////
  these are all the values for the first processed that is finished

  processArray[location].finished = true;
  processArray[location].queued = true;
  these two lines ensure that no duplicates are added into the qArray
  without these you get unexpected results

  ////////////////////////////////////////////////////*/
  /*resultArray[finished] = processArray[location];
  resultArray[finished].startTime = resultArray[finished].arrivalTime;
  resultArray[finished].completionTime = resultArray[finished].startTime + resultArray[finished].burstTime;
  resultArray[finished].waitTime = resultArray[finished].startTime - resultArray[finished].arrivalTime; // should always be 0
  resultArray[finished].TAT = resultArray[finished].completionTime - resultArray[finished].arrivalTime; //should be = to BT
  resultArray[finished].finished = true;
  resultArray[finished].queued = true;
  processArray[location].finished = true;
  processArray[location].queued = true;
  totalTime = resultArray[finished].completionTime;
  finished++;*/

  //fill the queue for the jobs added while last one ran

  /*for(i = 0; i < length; i++){
    if(processArray[i].arrivalTime <= totalTime && !processArray[i].queued){
      qArray[q++] = processArray[i];
      processArray[i].queued = true;
    }
  }*/
  /*///////////////////////////////////////////////
  The first while loop makes sure that all process have been completed before
  we exit the program
  ///////////////////////////////////////////////*/
  while(finished != length){
    /*///////////////////////////////////
    this for loop checks to see which process needs to be ran next
    //////////////////////////////////*/
    if(!qArray[location].finished){
      qArray[location].runTime -= 1;
      totalTime++;
      if (qArray[location].runTime == 0){
         processFinished = true;
         qArray[location].finished = true;
      }
      else
        processFinished = false;
    }
    else{
      // printf("hahhahaha\n");

      processFinished = false;
    }
    // printf("q %d finished -> %d total time -> %d\n", q, finished, totalTime);
    if(q < length){
      for(i = 0; i < length; i++){
        if(processArray[i].arrivalTime <= totalTime && !processArray[i].queued ){
          // printf("\nhere %d\n", i);
          qArray[q++] = processArray[i];
          processArray[i].queued =  true;
        }
      }
    }
    for(i = 0; i < q; i++){
      if(!processFinished && !qArray[i].finished && qArray[i].priority > highestPriority){
        location = i;
        //printf("gl -> %d q -> %d i-> %d\n",location, q ,i);
        highestPriority = qArray[location].priority;
        processFound = true;
      }
    }
    if(processFound){
      if(qArray[location].ID != gantArray[gantPos].Process.ID){
        gantLength += 1;
        //gantArray = (gantt *)realloc(gantArray, gantLength * 5 * sizeof(gantt));
        gantArray[gantPos].leaveTime = totalTime;
        gantArray[++gantPos].Process = qArray[location];
        gantArray[gantPos].enterTime = totalTime;
        if(qArray[location].startTime == ((unsigned int)~0 >> 1))
          qArray[location].startTime = totalTime;
        }


    }
    else{
      if(!qArray[location].finished)
        processFinished = true;
      else
        totalTime++;
    }
    //printf("total time %d\n", totalTime);
    /*///////////////////////////////////
    adding all of the variables to result array the same way as it was done for the
    first one
    //////////////////////////////////*/
    if(processFinished){
      qArray[location].finished = true;
      resultArray[finished] = qArray[location];
      resultArray[finished].startTime = qArray[location].startTime;
      resultArray[finished].completionTime = resultArray[finished].startTime + resultArray[finished].burstTime;
      resultArray[finished].TAT = resultArray[finished].completionTime - resultArray[finished].arrivalTime;
      resultArray[finished].waitTime = resultArray[finished].TAT - resultArray[finished].burstTime;
      totalTime = resultArray[finished].completionTime;
      finished++;
      if(finished == length)
        gantArray[gantPos].leaveTime = totalTime;
    }
    /*///////////////////////////////////
    if we enter this else it means that there is currently nothing
    in the queue and we need to just increase the time till
    something is added
    //////////////////////////////////*/

    /*///////////////////////////////////
    Adds any jobs not in the queue yet if they have arrived
    if all jobs in queue it is skipped
    //////////////////////////////////*/
    if(q < length){
      for(i = 0; i < length; i++){
        if(processArray[i].arrivalTime <= totalTime && !processArray[i].queued ){
          qArray[q++] = processArray[i];
          processArray[i].queued =  true;
        }
      }
    }
    //reset these variable before restarting the while loop
    //if(q != length)
      highestPriority = -1;
    //processFinished = false;
    processFound = false;
  }
  free(qArray);
  return gantLength;
}

void gant(struct process *gantArray, int length){
  printf("|");
  for(int i=0; i<length; i++){
    printf("%d P%d %d|", gantArray[i].startTime, gantArray[i].ID, gantArray[i].completionTime);
  }
  printf("\n");
}

void ganttNP(struct gantt *gantArray, int length){
  printf("|");
  for(int i=0; i<length; i++){
    printf("%d P%d %d|", gantArray[i].enterTime, gantArray[i].Process.ID, gantArray[i].leaveTime);
  }
  printf("\n");
}

void setprocess(struct process *input, int length, int maxRand){
  int i = 0;
  while(i < length){
    input[i].ID = i;
    input[i].priority = (rand() % maxRand);
    input[i].arrivalTime = (rand() % maxRand);
    input[i].burstTime = (rand() % maxRand + 1);
    input[i].runTime = input[i].burstTime;
    input[i].finished = false;
    input[i].queued = false;
    input[i].startTime = ((unsigned int)~0 >> 1);
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
  int maxRand = 20; // could get this from user input
  int arrayLength; //needs to come from input
  int i = 0;
  float avgWait;

  /*//////////////////////////////////
  gets the user input for the amount of process to run
  //////////////////////////////////*/
  printf("How many process would you like: ");
  scanf("%d", &arrayLength);

  /*///////////////////////////////////////////////////////////
  processArray holds all the base information about Process

  Each specific array for the scheduling algorithims holds
  the results of the algorithm in the order in which they would run

  all must be free before main is finished
  //////////////////////////////////////////////////////////*/
  process *processArray = malloc(arrayLength * sizeof(process));
  process *FCFSArray = malloc(arrayLength * sizeof(process));
  process *SJFArray = malloc(arrayLength * sizeof(process));
  process *priorityArray = malloc(arrayLength * sizeof(process));
  gantt *gantArray = malloc(10000 * sizeof(process)); //for non pre emptive

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
  /*FCFS(processArray, FCFSArray, arrayLength);
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
  printf("gant Array\n");
  gant(FCFSArray, arrayLength);*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// SJF
/////////////////////////////////////////////////////////////////////////////////////////////////////////

  //reset process must be ran before calling any new scheduling algotithm
  //just sets finished and queued back to false
  resetProcess(processArray, arrayLength);
  SJF(processArray, SJFArray, arrayLength);
  printf("SJF results\n");
  while(i < arrayLength){
    printf("ID: %d st: %d priority: %d AT: %d BT: %d CT: %d TAT: %d WT: %d\n", SJFArray[i].ID, SJFArray[i].startTime, SJFArray[i].priority, SJFArray[i].arrivalTime, SJFArray[i].burstTime, SJFArray[i].completionTime, SJFArray[i].TAT, SJFArray[i].waitTime);
    i++;
  }
  i = 0;
  while(i < arrayLength){
    avgWait = avgWait + SJFArray[i].waitTime;
    i++;
  }

  i=0;
  avgWait = avgWait/arrayLength;
  printf("AVG wait time for SJF (Shortest Job First) = %f\n", avgWait);
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// SJF
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//reset process must be ran before calling any new scheduling algotithm
//just sets finished and queued back to false
  int gantLength;
  resetProcess(processArray, arrayLength);
  gantLength = priority(processArray, priorityArray, gantArray, arrayLength);
  printf("\nPriority results\n");
  while(i < arrayLength){
    printf("ID: %d st: %d priority: %d AT: %d BT: %d CT: %d TAT: %d WT: %d\n", priorityArray[i].ID, priorityArray[i].startTime, priorityArray[i].priority, priorityArray[i].arrivalTime, priorityArray[i].burstTime, priorityArray[i].completionTime, priorityArray[i].TAT, priorityArray[i].waitTime);
    i++;
  }
  i = 0;
  while(i < arrayLength){
    avgWait = avgWait + priorityArray[i].waitTime;
    i++;
  }

  i=0;
  avgWait = avgWait/arrayLength;
  printf("AVG wait time for priorityArray = %f\n", avgWait);
  printf("gantt chart\n");
  ganttNP(gantArray, gantLength); //dont comment out
  free(SJFArray);
  free(processArray);
  free(FCFSArray);
  free(priorityArray);
  free(gantArray);
  return 0;
}
