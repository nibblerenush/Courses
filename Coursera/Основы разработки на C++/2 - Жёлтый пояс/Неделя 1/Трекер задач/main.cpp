#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

enum class TaskStatus
{
  NEW,
  IN_PROGRESS,
  TESTING,
  DONE
};

using TasksInfo = std::map<TaskStatus, int>;

class TeamTasks
{
public:
  const TasksInfo& GetPersonTasksInfo(const std::string& person) const
  {
    return m_person_tasksinfo.at(person);
  }
  
  void AddNewTask(const std::string& person)
  {
    ++m_person_tasksinfo[person][TaskStatus::NEW];
  }

  std::tuple<TasksInfo, TasksInfo> PerformPersonTasks(const std::string& person, int task_count)
  {
    if (m_person_tasksinfo.count(person))
    {
      TasksInfo update_tasks_info;
      TasksInfo not_update_tasks_info;

      for (const auto& tasksinfo : m_person_tasksinfo.at(person))
      {
        if (task_count < 1)
        {
          break;
        }

        if (tasksinfo.first != TaskStatus::DONE)
        {
          int new_status_count { 0 };
          int diff { task_count - tasksinfo.second };
          if (diff >= 0)
          {
            new_status_count = tasksinfo.second;
            task_count = diff;
          }
          else
          {
            new_status_count = task_count;
            task_count = 0;
          }
          
          TaskStatus new_status = static_cast<TaskStatus>(static_cast<int>(tasksinfo.first) + 1);
          update_tasks_info[new_status] = new_status_count;
        }
      }

      for (const auto& tasksinfo : update_tasks_info)
      {
        TaskStatus old_status = static_cast<TaskStatus>(static_cast<int>(tasksinfo.first) - 1);
        m_person_tasksinfo[person][old_status] -= tasksinfo.second;
        m_person_tasksinfo[person][tasksinfo.first] += tasksinfo.second;
      }
      ClearPersonTasksInfo(person);

      for (const auto& tasksinfo : m_person_tasksinfo[person])
      {
        if (tasksinfo.first != TaskStatus::DONE)
        {
          if (update_tasks_info.count(tasksinfo.first))
          {
            int temp = tasksinfo.second - update_tasks_info.at(tasksinfo.first);
            if (temp) { not_update_tasks_info[tasksinfo.first] = temp; }
          }
          else
          {
            not_update_tasks_info[tasksinfo.first] = tasksinfo.second;
          }
        }
      }

      return std::tie(update_tasks_info, not_update_tasks_info);
    }
    else
    {
      return {};
    }
  }

private:
  void ClearPersonTasksInfo(const std::string& person)
  {
    std::vector<TaskStatus> vector_clear;
    for (const auto& taskinfo : m_person_tasksinfo[person])
    {
      if (taskinfo.second == 0)
      {
        vector_clear.push_back(taskinfo.first);
      }
    }

    for (const auto& key_clear : vector_clear)
    {
      m_person_tasksinfo[person].erase(key_clear);
    }
  }

private:
  std::map<std::string, TasksInfo> m_person_tasksinfo;
};

int main()
{
  TeamTasks team_tasks;
  team_tasks.AddNewTask("Alice");
  team_tasks.AddNewTask("Alice");
  team_tasks.AddNewTask("Alice");
  team_tasks.AddNewTask("Alice");
  team_tasks.AddNewTask("Alice");
  team_tasks.PerformPersonTasks("Alice", 5);
  team_tasks.PerformPersonTasks("Alice", 5);
  team_tasks.PerformPersonTasks("Alice", 1);
  return 0;
}
