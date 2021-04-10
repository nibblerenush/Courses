#include "json.h"

#include <cmath>

using namespace std;

namespace Json {

  Document::Document(Node root) : root(move(root)) {
  }

  const Node& Document::GetRoot() const {
    return root;
  }

  Node LoadNode(istream& input);

  Node LoadArray(istream& input) {
    vector<Node> result;

    for (char c; input >> c && c != ']'; ) {
      if (c != ',') {
        input.putback(c);
      }
      result.push_back(LoadNode(input));
    }

    return Node(move(result));
  }

  Node LoadNumber(istream& input) {
    char sign = '+';
    if (input.peek() == '-') {
      sign = input.get();
    }

    int64_t first_part = 0;
    while (isdigit(input.peek())) {
      first_part *= 10;
      first_part += static_cast<int64_t>(input.get()) - '0';
    }
    
    int count = 0;
    int64_t second_part = 0;
    if (input.peek() == '.') {
      input.get();
      while (isdigit(input.peek())) {
        count++;
        second_part *= 10;
        second_part += static_cast<int64_t>(input.get()) - '0';
      }
    }

    double result = first_part + (second_part / pow(10, count));
    if (sign == '-') {
      result = -result;
    }

    return Node(result);
  }

  Node LoadString(istream& input) {
    string line;
    getline(input, line, '\"');
    return Node(move(line));
  }

  Node LoadDict(istream& input) {
    map<string, Node> result;

    for (char c; input >> c && c != '}'; ) {
      if (c == ',') {
        input >> c;
      }

      string key = LoadString(input).AsString();
      input >> c;
      result.emplace(move(key), LoadNode(input));
    }

    return Node(move(result));
  }

  Node LoadBool(istream& input) {
    char c = input.peek();
    if (c == 't') {
      input.ignore(4); // true
      return Node(true);
    }
    else {
      input.ignore(5); // false
      return Node(false);
    }
  }

  Node LoadNode(istream& input) {
    char c;
    input >> c;

    if (c == '[') {
      return LoadArray(input);
    } else if (c == '{') {
      return LoadDict(input);
    } else if (c == '\"') {
      return LoadString(input);
    } else if (c == 't' || c == 'f') {
      input.putback(c);
      return LoadBool(input);
    }
    else {
      input.putback(c);
      return LoadNumber(input);
    }
  }

  Document Load(istream& input) {
    return Document{LoadNode(input)};
  }

}
