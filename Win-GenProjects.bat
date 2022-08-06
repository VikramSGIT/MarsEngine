@echo off
prompt $P$G 
call Premake\premake5.exe vs2019
call git submodule init
call git submodule update
popd
PAUSE
