#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using std::vector;
using std::cin;
using std::cout;

struct Processor
{
  int index;
  long long all_duration;
};

bool operator > (const Processor& lhs, const Processor& rhs)
{
  if (lhs.all_duration == rhs.all_duration) {
    return lhs.index > rhs.index;
  }
  else {
    return lhs.all_duration > rhs.all_duration;
  }
}

class ProcessorCompare
{
public:
  bool operator () (const Processor& lhs, const Processor& rhs) const {
    return lhs > rhs;
  }
};

class JobQueue {
 private:
  int num_workers_;
  vector<int> jobs_;

  vector<int> assigned_workers_;
  vector<long long> start_times_;

  void WriteResponse() const {
    for (int i = 0; i < jobs_.size(); ++i) {
      cout << assigned_workers_[i] << " " << start_times_[i] << "\n";
    }
  }

  void ReadData() {
    int m;
    cin >> num_workers_ >> m;
    jobs_.resize(m);
    for(int i = 0; i < m; ++i)
      cin >> jobs_[i];
  }

  void AssignJobs() {
    // TODO: replace this code with a faster algorithm.
    /*assigned_workers_.resize(jobs_.size());
    start_times_.resize(jobs_.size());
    vector<long long> next_free_time(num_workers_, 0);
    for (int i = 0; i < jobs_.size(); ++i) {
      int duration = jobs_[i];
      int next_worker = 0;
      for (int j = 0; j < num_workers_; ++j) {
        if (next_free_time[j] < next_free_time[next_worker])
          next_worker = j;
      }
      assigned_workers_[i] = next_worker;
      start_times_[i] = next_free_time[next_worker];
      next_free_time[next_worker] += duration;
    }*/

    std::priority_queue<Processor, std::vector<Processor>, ProcessorCompare> heap;
    for (int i = 0; i < num_workers_; ++i) {
      heap.push(Processor{ i, 0 });
    }

    for (int i = 0; i < (int)jobs_.size(); ++i)
    {
      int time = jobs_[i];

      auto processor = heap.top();
      heap.pop();

      assigned_workers_.push_back(processor.index);
      start_times_.push_back(processor.all_duration);

      processor.all_duration += time;
      heap.push(processor);
    }
  }

 public:
  void Solve() {
    ReadData();
    AssignJobs();
    WriteResponse();
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  JobQueue job_queue;
  job_queue.Solve();
  return 0;
}
