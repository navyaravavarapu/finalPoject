#include <scheduling.h>
#include <fstream>
#include <iostream>
#include <list>
#include <queue>
#include <string>

using namespace std;

pqueue_arrival read_workload(string filename)
{
  pqueue_arrival workload;

  ifstream fd1;
  fd1.open(filename);
  if (!fd1.is_open())
  {
    cout << "ERROR: file not found: " + filename << endl;
  }

  for (int arrival, duration; fd1 >> arrival >> duration;)
  {
    Process p;
    p.arrival = arrival;
    p.duration = duration;
    p.first_run = -1;
    workload.push(p);
  }
  return workload;
}

void show_workload(pqueue_arrival workload)
{
  pqueue_arrival xs = workload;
  cout << "Workload:" << endl;
  while (!xs.empty())
  {
    Process p = xs.top();
    cout << '\t' << p.arrival << ' ' << p.duration << endl;
    xs.pop();
  }
}

void show_processes(list<Process> processes)
{
  list<Process> xs = processes;
  cout << "Processes:" << endl;
  while (!xs.empty())
  {
    Process p = xs.front();
    cout << "\tarrival=" << p.arrival << ", duration=" << p.duration
         << ", first_run=" << p.first_run << ", completion=" << p.completion
         << endl;
    xs.pop_front();
  }
}

list<Process> fifo(pqueue_arrival workload)
{
  list<Process> complete;
  int time_acc = 0;
  while (!workload.empty())
  {
    Process p = workload.top();
    p.first_run = time_acc;
    time_acc += p.duration;
    p.completion = time_acc;
    complete.push_back(p);
    workload.pop();
  }
  return complete;
}

list<Process> sjf(pqueue_arrival workload)
{
  list<Process> complete;
  int time_stamp = workload.top().arrival;
  pqueue_duration durations;
  while (!workload.empty() || !durations.empty())
  {
    while (!workload.empty() && workload.top().arrival <= time_stamp)
    {
      durations.push(workload.top());
      workload.pop();
    }

    if (durations.empty())
    {
      time_stamp = workload.top().arrival;
      continue;
    }
    Process d = durations.top();
    d.first_run = time_stamp;
    d.completion = d.first_run + d.duration;
    time_stamp = d.completion;
    complete.push_back(d);
    durations.pop();
  }
  return complete;
}

list<Process> stcf(pqueue_arrival workload)
{
  pqueue_duration duration_queue;
  list<Process> complete;
  int time_acc = 0;
  while (!workload.empty() || !duration_queue.empty())
  {
    while (workload.top().arrival <= time_acc && !workload.empty())
    {
      duration_queue.push(workload.top());
      workload.pop();
    }
    Process p = duration_queue.top();
    duration_queue.pop();
    if (p.first_run == -1)
    {
      p.first_run = time_acc;
    }
    // Execute process for 1 time quanta
    p.duration -= 1;
    time_acc += 1;

    if (p.duration == 0)
    {
      p.completion = time_acc;
      complete.push_back(p);
    }
    else
    {
      duration_queue.push(p);
    }
  }
  return complete;
}

list<Process> rr(pqueue_arrival workload)
{
  list<Process> executing;
  list<Process> complete;
  int time_acc = 0;
  while (!workload.empty() || !executing.empty())
  {
    while (workload.top().arrival <= time_acc && !workload.empty())
    {
      executing.push_back(workload.top());
      workload.pop();
    }
    Process p = executing.front();
    executing.pop_front();
    if (p.first_run == -1)
    {
      p.first_run = time_acc;
    }
    p.duration -= 1;
    time_acc += 1;
    if (p.duration == 0)
    {
      p.completion = time_acc;
      complete.push_back(p);
    }
    else
    {
      executing.push_back(p);
    }
  }
  return complete;
}

list<Process> mlfq(pqueue_arrival workload)
{
  const int numLevels = 4;                         
  const int timeQuantum[numLevels] = {1, 4, 3, 7}; //time for each level defined
  list<Process> complete;
  queue<Process> queues[numLevels];
  int time_acc = 0;

  while (!workload.empty() || !queues[numLevels - 1].empty())
  {
    // Add new processes to the lowest priority queue
    while (!workload.empty() && workload.top().arrival <= time_acc)
    {
      Process p = workload.top();
      workload.pop();
      p.first_run = -1;
      p.level = 0;
      queues[0].push(p);
    }
    bool procExec = false;
    // Process the queues with round-robin
    for (int i = 0; i < numLevels; i++)
    {
      if (!queues[i].empty())
      {
        Process &p = queues[i].front();
        // Execute the process for the defined time above
        int quantum = min(timeQuantum[p.level], p.duration);
        p.duration -= quantum;
        time_acc += quantum;
        if (p.first_run == -1)
        {
          p.first_run = time_acc - quantum;
        }
        if (p.duration == 0)
        {
          p.completion = time_acc;
          complete.push_back(p);
          queues[i].pop();
        }
        else
        {
          // Boost the process to the next higher priority level
          p.level = min(p.level + 1, numLevels - 1);
        }
        procExec = true;
        break;
      }
    }
    if (!procExec)
    {
      time_acc++;
    }
  }
  return complete;
}

list<Process> lifo(pqueue_arrival workload) {
  // Initialize the completed processes list
  list<Process> complete;

  // Process the workload in LIFO order
  while (!workload.empty()) {
    Process p = workload.top();
    workload.pop();
    p.first_run = p.duration;  
    p.completion = p.arrival + p.duration;  

    complete.push_front(p); 
  }

  return complete;
}

float avg_turnaround(list<Process> processes)
{
  int acc_turnaround_time = 0;
  for (auto process : processes)
  {
    acc_turnaround_time += process.completion - process.arrival;
  }
  return (float)acc_turnaround_time / (float)processes.size();
}

float avg_response(list<Process> processes)
{
  int acc_response_time = 0;
  for (auto process : processes)
  {
    acc_response_time += process.first_run - process.arrival;
  }
  return (float)acc_response_time / (float)processes.size();
}

void show_metrics(list<Process> processes)
{
  float avg_t = avg_turnaround(processes);
  float avg_r = avg_response(processes);
  show_processes(processes);
  cout << '\n';
  cout << "Average Turnaround Time: " << avg_t << endl;
  cout << "Average Response Time:   " << avg_r << endl;
}