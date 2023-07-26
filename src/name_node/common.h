#include<iostream>
#include<string>
// std::string split(std::string s,std::string& ret)//返回目录不包含/
// {   //去除前面/
//     if(s[0]=='/')
//     {
//         s=s(s.begin()+1,s.end());
//     }
//     if(s.size()==0)
//     {
//         return ""; 
//     }

//     if((size_t r=s.find('/'))==s.end())
//     {//ret为null说明已经结束
//         return s;
//     }
//     else
//     {
//         ret=s(s.begin()+r,s.end());
//         return s(s.begin(),s.begin()+r);
//     }
    
// }
//最后不要带/

std::string GetFileName(std::string s)
{
    string ret=string(s.begin()+s.rfind("/")+1,s.end());
    return ret;
}

int getLastIndex(ServiceImpl* sp) {
	return sp->logs.size() - 1;
}

// 获取最后的任期(快照版本
 int getLastTerm(ServiceImpl* sp)  {
	// 因为初始有填充一个，否则最直接len == 0
	if (sp->logs.size()-1 == 0) {
		return 0;
	} else {
        
		return sp->logs[sp->logs.size()-1].term();
	}
}
int ihash(string s)
{
   uint64_t ret=0;
   for(auto& x:s)
   {
      ret=ret*31+x-'a';
   }
   return ret&0x7fffffff;

}
int GetTime()
{
   srand((unsigned)time(NULL));//srand()函数产生一个以当前时间开始的随机种子.应该放在for等循环语句前面 不然要很长时间等待
	int ret=rand()%100;

   return ret; 
}
unsigned long long Getms(timeval* tv){
   return tv->tv_sec*1000 + tv->tv_usec/1000;
}