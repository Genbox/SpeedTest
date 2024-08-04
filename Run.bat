@echo off

if exist "bin" (
    del /F /Q bin\*
) else (
    mkdir bin
)

set GOARCH=amd64
set GOOS=windows

REM We first compile all the languages

csc -nologo -optimize -out:bin\SpeedTest-cs.exe src\SpeedTest.cs
javac -d bin\ -g:none src\SpeedTest.java
go build -ldflags "-s -w" -o bin\SpeedTest-go.exe src\SpeedTest.go
cl /EHsc /nologo /O2 /Fe:bin\SpeedTest-cpp.exe src\SpeedTest.cpp > nul

REM Then we execute them in order

echo.
echo ### C# ###
bin\SpeedTest-cs.exe
echo.
echo ### Java ###
java -cp bin\ SpeedTest
echo.
echo ### Go ###
bin\SpeedTest-go.exe
echo.
echo ### C++ ###
bin\SpeedTest-cpp.exe