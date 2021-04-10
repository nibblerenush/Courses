#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

struct Object
{
  int cost;
  int weight;
  double cost_of_one_weight;
};

int main()
{
  int number_of_objects;
  int weight_of_bag;
  std::cin >> number_of_objects >> weight_of_bag;

  std::vector<Object> objects(number_of_objects);
  for (int i = 0; i < number_of_objects; ++i)
  {
    std::cin >> objects[i].cost >> objects[i].weight;
    objects[i].cost_of_one_weight = static_cast<double>(objects[i].cost) / objects[i].weight;
  }

  std::sort(objects.begin(), objects.end(), [](const Object& lhs, const Object& rhs) {
    return lhs.cost_of_one_weight > rhs.cost_of_one_weight;
  });
  
  double cost_of_bag = 0.0;
  int temp_weight_of_bag = 0;
  int i = 0;
  
  while (i < number_of_objects)
  {
    if (temp_weight_of_bag < weight_of_bag)
    {
      if (objects[i].weight != 0)
      {
        cost_of_bag += objects[i].cost_of_one_weight;
        objects[i].weight--;
        temp_weight_of_bag++;
      }
      else
      {
        i++;
      }
    }
    else
    {
      break;
    }
  }

  std::cout << std::fixed << std::setprecision(3) << cost_of_bag << std::endl;
  return 0;
}
