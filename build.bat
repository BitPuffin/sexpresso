@echo off

call cl /O2 /c sexpresso.cpp
call lib sexpresso.obj /OUT:sexpresso.lib
