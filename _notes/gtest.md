

- How to run tests and debug Google Test project in VS Code?
https://stackoverflow.com/questions/62910867/how-to-run-tests-and-debug-google-test-project-in-vs-code



1. clone googletest
```
https://github.com/abseil/googletest.git
```
add googletest to .gitmodules
```
git submodule add <url> googletest
```

2. CMakeLists.txt
```
  cmake_minimum_required(VERSION 3.12) # version can be different

  project(my_cpp_project) #name of your project

  add_subdirectory(googletest) # add googletest subdirectory

  include_directories(googletest/include) # this is so we can #include <gtest/gtest.h>

  add_executable(mytests mytests.cpp) # add this executable

  target_link_libraries(mytests PRIVATE gtest) # link google test to this executable
```

3. Project directory
```
  └─cpp/
      ├─ build/
      ├─ googletest/
      ├─ CMakeLists.txt
      ├─ myfunctions.h
      └─ mytests.cpp
```

4. Run
```
  cd build
  cmake ..
  make 
  ./mytests
```