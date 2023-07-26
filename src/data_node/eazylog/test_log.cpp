#include "./src/easylogging++.h"
INITIALIZE_EASYLOGGINGPP 
#include<iostream>
#include<sstream>
void aa()
{
    LOG(INFO)<<"wqpotseld;ldgs";
}
int main()
{
    el::Configurations conf("./config/log.conf");
    el::Loggers::reconfigureAllLoggers(conf);
    // std::string s = "shakfjhfasjk";
    // std::stringstream ss(s);
    const char *test = "sfkla;fjal;sjfklasjl";
    LOG(INFO)<<test<<2;
    LOG(TRACE)<<test;
    aa();
    return 0;
}

