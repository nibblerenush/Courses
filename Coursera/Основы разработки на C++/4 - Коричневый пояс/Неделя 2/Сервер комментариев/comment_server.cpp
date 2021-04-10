#include "test_runner.h"

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <utility>
#include <map>
#include <optional>
#include <unordered_set>
#include <tuple>

using namespace std;

struct HttpRequest {
  string method, path, body;
  map<string, string> get_params;
};

pair<string, string> SplitBy(const string& what, const string& by) {
  size_t pos = what.find(by);
  if (by.size() < what.size() && pos < what.size() - by.size()) {
    return { what.substr(0, pos), what.substr(pos + by.size()) };
  }
  else {
    return { what, {} };
  }
}

template<typename T>
T FromString(const string& s) {
  T x;
  istringstream is(s);
  is >> x;
  return x;
}

pair<size_t, string> ParseIdAndContent(const string& body) {
  auto [id_string, content] = SplitBy(body, " ");
  return { FromString<size_t>(id_string), content };
}

struct LastCommentInfo {
  size_t user_id, consecutive_count;
};

struct HttpHeader {
  string name, value;
};

enum class HttpCode {
  Ok = 200,
  NotFound = 404,
  Found = 302,
};

class HttpResponse
{
public:
  explicit HttpResponse(HttpCode code) : m_code(move(code))
  {}

  HttpResponse& AddHeader(string name, string value)
  {
    m_headers.push_back({ move(name), move(value) });
    return *this;
  }

  HttpResponse& SetContent(string a_content)
  {
    m_content = move(a_content);
    return *this;
  }

  HttpResponse& SetCode(HttpCode a_code)
  {
    m_code = move(a_code);
    return *this;
  }

  friend ostream& operator << (ostream& output, const HttpResponse& resp);
  friend bool operator != (const HttpResponse& lhs, const HttpResponse& rhs);
  friend bool operator == (const HttpResponse& lhs, const HttpResponse& rhs);

private:
  HttpCode m_code;
  vector<HttpHeader> m_headers;
  string m_content;
};

ostream& operator << (ostream& output, const HttpResponse& resp)
{
  output << "HTTP/1.1 " << static_cast<int>(resp.m_code) << ' ';
  switch (resp.m_code)
  {
  case HttpCode::Ok:
    output << "OK";
    break;
  case HttpCode::NotFound:
    output << "Not found";
    break;
  case HttpCode::Found:
    output << "Found";
    break;
  }
  output << '\n';

  for (const auto& header : resp.m_headers)
  {
    if (header.name != "Content-Length")
    {
      output << header.name << ": " << header.value << '\n';
    }
  }
  
  if (!resp.m_content.empty())
  {
    output << "Content-Length: " << resp.m_content.length() << '\n';
  }

  output << '\n' << resp.m_content;
  return output;
}

bool operator != (const HttpResponse& lhs, const HttpResponse& rhs)
{
  return tie(lhs.m_code, lhs.m_content, lhs.m_headers) != tie(rhs.m_code, rhs.m_content, rhs.m_headers);
}

bool operator == (const HttpResponse& lhs, const HttpResponse& rhs)
{
  return tie(lhs.m_code, lhs.m_content, lhs.m_headers) == tie(rhs.m_code, rhs.m_content, rhs.m_headers);
}

class CommentServer {
private:
  vector<vector<string>> comments_;
  std::optional<LastCommentInfo> last_comment;
  unordered_set<size_t> banned_users;

public:
  HttpResponse ServeRequest(const HttpRequest& req)
  {
    if (req.method == "POST")
    {
      // add_user
      if (req.path == "/add_user")
      {
        comments_.emplace_back();
        string response = to_string(comments_.size() - 1);
        return HttpResponse(HttpCode::Ok).SetContent(response);
      }
      // add_comment
      else if (req.path == "/add_comment")
      {
        auto [user_id, comment] = ParseIdAndContent(req.body);

        if (!last_comment || last_comment->user_id != user_id) {
          last_comment = LastCommentInfo{ user_id, 1 };
        }
        else if (++last_comment->consecutive_count > 3) {
          banned_users.insert(user_id);
        }

        if (banned_users.count(user_id) == 0) {
          comments_[user_id].push_back(string(comment));
          return HttpResponse(HttpCode::Ok);
        }
        else {
          return HttpResponse(HttpCode::Found).AddHeader("Location", "/captcha");
        }
      }
      // checkcaptcha
      else if (req.path == "/checkcaptcha")
      {
        if (auto [id, response] = ParseIdAndContent(req.body); response == "42") {
          banned_users.erase(id);
          if (last_comment && last_comment->user_id == id) {
            last_comment.reset();
          }
          return HttpResponse(HttpCode::Ok);
        }
        else {
          return HttpResponse(HttpCode::Found).AddHeader("Location", "/captcha");
        }
      }
      else {
        return HttpResponse(HttpCode::NotFound);
      }
    }
    // GET
    else if (req.method == "GET")
    {
      if (req.path == "/user_comments") {
        auto user_id = FromString<size_t>(req.get_params.at("user_id"));
        string response;
        for (const string& c : comments_[user_id]) {
          response += c + '\n';
        }
        return HttpResponse(HttpCode::Ok).SetContent(response);
      }
      else if (req.path == "/captcha") {
        string response = "What's the answer for The Ultimate Question of Life, the Universe, and Everything?";
        return HttpResponse(HttpCode::Ok).SetContent(response);
      }
      else {
        return HttpResponse(HttpCode::NotFound);
      }
    }
  }
};

ostream& operator<<(ostream& output, const HttpHeader& h) {
  return output << h.name << ": " << h.value;
}

bool operator==(const HttpHeader& lhs, const HttpHeader& rhs) {
  return lhs.name == rhs.name && lhs.value == rhs.value;
}

struct ParsedResponse {
  int code;
  vector<HttpHeader> headers;
  string content;
};

istream& operator >>(istream& input, ParsedResponse& r) {
  string line;
  getline(input, line);

  {
    istringstream code_input(line);
    string dummy;
    code_input >> dummy >> r.code;
  }

  size_t content_length = 0;

  r.headers.clear();
  while (getline(input, line) && !line.empty()) {
    if (auto [name, value] = SplitBy(line, ": "); name == "Content-Length") {
      istringstream length_input(value);
      length_input >> content_length;
    }
    else {
      r.headers.push_back({ std::move(name), std::move(value) });
    }
  }

  r.content.resize(content_length);
  input.read(r.content.data(), r.content.size());
  return input;
}

void Test(CommentServer& srv, const HttpRequest& request, const ParsedResponse& expected) {
  HttpResponse httpResponse = srv.ServeRequest(request);
  HttpResponse expectedHttpResponse(static_cast<HttpCode>(expected.code));
  for (const auto& header : expected.headers)
  {
    expectedHttpResponse.AddHeader(header.name, header.value);
  }
  expectedHttpResponse.SetContent(expected.content);
  ASSERT_EQUAL(httpResponse, expectedHttpResponse);
}

template <typename CommentServer>
void TestServer() {
  CommentServer cs;

  const ParsedResponse ok{ 200 };
  const ParsedResponse redirect_to_captcha{ 302, {{"Location", "/captcha"}}, {} };
  const ParsedResponse not_found{ 404 };

  Test(cs, { "POST", "/add_user" }, { 200, {}, "0" });
  Test(cs, { "POST", "/add_user" }, { 200, {}, "1" });
  Test(cs, { "POST", "/add_comment", "0 Hello" }, ok);
  Test(cs, { "POST", "/add_comment", "1 Hi" }, ok);
  Test(cs, { "POST", "/add_comment", "1 Buy my goods" }, ok);
  Test(cs, { "POST", "/add_comment", "1 Enlarge" }, ok);
  Test(cs, { "POST", "/add_comment", "1 Buy my goods" }, redirect_to_captcha);
  Test(cs, { "POST", "/add_comment", "0 What are you selling?" }, ok);
  Test(cs, { "POST", "/add_comment", "1 Buy my goods" }, redirect_to_captcha);
  Test(
    cs,
    { "GET", "/user_comments", "", {{"user_id", "0"}} },
    { 200, {}, "Hello\nWhat are you selling?\n" }
  );
  Test(
    cs,
    { "GET", "/user_comments", "", {{"user_id", "1"}} },
    { 200, {}, "Hi\nBuy my goods\nEnlarge\n" }
  );
  Test(
    cs,
    { "GET", "/captcha" },
    { 200, {}, {"What's the answer for The Ultimate Question of Life, the Universe, and Everything?"} }
  );
  Test(cs, { "POST", "/checkcaptcha", "1 24" }, redirect_to_captcha);
  Test(cs, { "POST", "/checkcaptcha", "1 42" }, ok);
  Test(cs, { "POST", "/add_comment", "1 Sorry! No spam any more" }, ok);
  Test(
    cs,
    { "GET", "/user_comments", "", {{"user_id", "1"}} },
    { 200, {}, "Hi\nBuy my goods\nEnlarge\nSorry! No spam any more\n" }
  );

  Test(cs, { "GET", "/user_commntes" }, not_found);
  Test(cs, { "POST", "/add_uesr" }, not_found);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestServer<CommentServer>);
  HttpResponse res(HttpCode::Ok);
  res.SetContent("string");
  cout << res;
}
