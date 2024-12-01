@echo off
echo Detecting number of CPU cores...

:: Use wmic to get the number of logical processors (cores)
for /f "skip=1" %%x in ('wmic cpu get NumberOfLogicalProcessors') do (
    if not defined NUM_CORES set NUM_CORES=%%x
)

if not defined NUM_CORES (
    echo Defaulting to 1 if unknown system...
    set NUM_CORES=1
)

echo Using %NUM_CORES% cores...

echo Cleaning project...
make.exe -f Makefile.sfml clean

echo Compilation with %NUM_CORES% cores...
make.exe -f Makefile.sfml --jobs=%NUM_CORES% all
