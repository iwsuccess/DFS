#include<stdio.h>
#include<bits/stdc++.h>
using namespace std;
 
class MySemaphore{
private:
	int count;
	std::mutex mx;
	std::condition_variable cv;
public:
	MySemaphore(int val = 1) :count(val) {}
	void P(){
		std::unique_lock<std::mutex> lock(mx);
		if (--count < 0)
		{
			cv.wait(lock);
		}
	}
	void V(){
		std::unique_lock<std::mutex> lock(mx);
		if (++count <= 0){
			cv.notify_one();
		}
	}
};
//int g_num = 0;
//MySemaphore semp(1);//����
//MySemaphore sems(0);//����
//
//void P(int id)
//{
//	for (int i = 0; i < 10; ++i)
//	{
//		semp.P();
//		g_num = i;
//		cout << "P " << g_num << endl;
//		sems.V();
//	}
//}
//void S(int id)
//{
//	for (int i = 0; i < 10; ++i)
//	{
//		sems.P();
//		cout << "S" << g_num << endl;
//		semp.V();
//	}
//}
/*int main()
{
	std::thread thp(P,1);
	std::thread ths(S,1);
	thp.join();
	ths.join();
	return 0;
}*/

