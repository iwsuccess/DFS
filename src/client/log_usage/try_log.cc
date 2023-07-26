#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[]) {
  el::Configurations conf("log.conf");
  el::Loggers::reconfigureAllLoggers(conf);
  LOG(TRACE) << "***** trace log  *****";
  LOG(DEBUG) << "***** debug log  *****";
  LOG(ERROR) << "***** error log  *****";
  LOG(WARNING) << "***** warning log  *****";
  LOG(INFO) << "***** info log  *****";
  // LOG(INFO) << "My first info log using default logger";
  return 0;
}