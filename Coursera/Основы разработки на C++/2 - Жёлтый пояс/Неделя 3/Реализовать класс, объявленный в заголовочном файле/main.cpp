#include "test_runner.h"
#include "phone_number.h"

void TestAll()
{
  TestRunner tr;
}

int main()
{
  TestAll();

	try
	{
		PhoneNumber pn("+1-2-coursera-cpp");
		std::cout << pn.GetInternationalNumber() << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
  

  return 0;
}
