@ECHO OFF
echo building (command: g++ test_main.cpp -o test.exe ../filereader.cpp ../coff_file.cpp -I..)
g++ test_main.cpp -o test.exe ../filereader.cpp ../coff_file.cpp -I..
echo running test.exe
test.exe
echo linking (command: golink /console /entry main .\test_build.obj kernel32.dll)
golink /console /entry main .\test_build.obj kernel32.dll
echo ----------Final Result----------
test_build.exe
