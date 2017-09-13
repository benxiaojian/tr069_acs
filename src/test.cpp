#include <iostream>
#include <boost/shared_ptr.hpp>
using namespace std;

static const char *url = "/hems-web-ui/ws/cwmp/";
static const char *urn = "urn:examples";
static const char *method = "sayHello";


void fun(int signal_num)
{
	cout << "end!!!" << endl;
	exit(0);
}

int main()
{
	return 0;
}
