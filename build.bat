@echo off

call cl /O2 /c sexpresso\sexpresso.cpp
call lib sexpresso.obj /OUT:sexpresso.lib
call cl /Isexpresso /O2 /c sexpresso_std\sexpresso_std.cpp
call lib sexpresso_std.obj /OUT:sexpresso_std.lib

