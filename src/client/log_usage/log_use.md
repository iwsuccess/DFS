## 快速使用

> [官方地址](https://github.com/amrayn/easyloggingpp)，easylogging 是 header-only 的，下载好 easylogging++.h， easylogging.c 文件，放置进自己工程对应目录即可使用。

- 包含 easylogging++.h 头文件

- 使用宏INITIALIZE_EASYLOGGINGPP初始化

```cpp
#include "easylogging++.h"  
// 文件名，hello_log.cc  
INITIALIZE_EASYLOGGINGPP  
  
int main(int argv, char* argc[]) {  
   LOG(INFO) << "My first info log using default logger";  
   return 0;  
}
```

- 编译命令：`g++ easylogging.c hello_log.cc`（注意 easylogging.c 文件路径；以及指定 include 搜索目录，头文件 easylogging.h 应在其中） 

  例如：`g++ try_log.cc ../easylogging++.cc -I ../../../include/`

### 自定义日志格式

编辑配置文件，例如命名为`log.conf`：

```shell
* GLOBAL:  
    ENABLED                 =   true  
    TO_FILE                 =   true  
    TO_STANDARD_OUTPUT      =   true  
    FORMAT                  =   "[%level | %datetime] | %msg"  
    FILENAME                =   "log/log_%datetime{%Y%M%d}.log"  
    MILLISECONDS_WIDTH      =   3  
    PERFORMANCE_TRACKING    =   false  
    MAX_LOG_FILE_SIZE       =   1048576  ## 1MB - Comment starts with two hashes (##)
    LOG_FLUSH_THRESHOLD     =   0  
      
* TRACE:  
    FILENAME                =   "log/trace_log_%datetime{%Y%M%d}.log"  
      
* DEBUG:  
    FILENAME                =   "log/debug_log_%datetime{%Y%M%d}.log"  
      
* FATAL:  
    ENABLED                 =   false  
      
* ERROR:  
    FILENAME                =   "log/error_log_%datetime{%Y%M%d}.log"  
      
* WARNING:  
    FILENAME                =   "log/warning_log_%datetime{%Y%M%d}.log"  
      
* INFO:  
    FILENAME                =   "log/info_log_%datetime{%Y%M%d}.log"  
      
* VERBOSE:  
    ENABLED                 =   false  

```

代码文件中指定配置文件路径，使之生效即可。

```cpp
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
  // ....
  return 0;
}
```



