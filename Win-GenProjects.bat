@echo off
prompt $P$G 
call Premake\premake5.exe vs2022
call git submodule init
call git submodule update
popd
PAUSE
