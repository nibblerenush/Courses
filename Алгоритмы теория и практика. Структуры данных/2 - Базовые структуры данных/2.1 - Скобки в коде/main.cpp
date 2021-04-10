#include <iostream>
#include <stack>
#include <string>
#include <utility>

struct Frame
{
  char bracket;
  std::size_t index;
};

std::pair<std::size_t, bool> Solution(const std::string& input_str)
{
  std::stack<Frame> my_stack;
  for (std::size_t i = 0; i < input_str.length(); ++i)
  {
    const char& bracket = input_str.at(i);
    if (bracket == '(' || bracket == '[' || bracket == '{')
    {
      my_stack.push({bracket, i});
    }
    else if (bracket == ')' || bracket == ']' || bracket == '}')
    {
      if (my_stack.empty())
      {
        return {i, false};
      }
      
      Frame open_bracket = my_stack.top();
      my_stack.pop();

      if
      (
        (open_bracket.bracket == '(' && bracket != ')') ||
        (open_bracket.bracket == '[' && bracket != ']') ||
        (open_bracket.bracket == '{' && bracket != '}')
      )
      {
        return {i, false};
      }
    }
  }
  if (!my_stack.empty())
  {
    return {my_stack.top().index, false};
  }
  return {0, true};
}

int main()
{
  std::string input_str;
  std::cin >> input_str;

  auto result = Solution(input_str);
  if (result.second) {
    std::cout << "Success";
  }
  else {
    std::cout << result.first + 1;
  }
  std::cout << std::endl;

  return 0;
}
