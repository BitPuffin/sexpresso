@echo off

cl /I../sexpresso /EHa /Fe_test-sexpresso.exe test_sexpresso.cpp ..\sexpresso\sexpresso.cpp
call _test-sexpresso.exe
del _test-sexpresso.exe
