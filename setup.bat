@echo off

echo Setup skript pro stazeni arm-none-eabi

set TOOLCHAIN_VERSION=arm-gnu-toolchain-13.3.rel1-mingw-w64-i686-arm-none-eabi
set TOOLCHAIN_URL=https://developer.arm.com/-/media/Files/downloads/gnu/13.3.rel1/binrel/%TOOLCHAIN_VERSION%.zip
set TOOLCHAIN_ZIP=arm-gnu-toolchain.zip
set TEMP_DIR=.cache\arm-toolchain
set DEST_DIR=%~dp0build\arm-none-eabi
if not exist "%TEMP_DIR%" mkdir "%TEMP_DIR%"

echo.
if exist "%TEMP_DIR%\%TOOLCHAIN_ZIP%" (
    echo Soubor "%TEMP_DIR%\%TOOLCHAIN_ZIP%" jiz existuje, preskakuji stazeni souboru.
) else (
    echo Stahuji arm-none-eabi toolchain...
    curl -L -o "%TEMP_DIR%\%TOOLCHAIN_ZIP%" "%TOOLCHAIN_URL%"
    if %errorlevel% neq 0 (
        echo Chyba pri stahovani arm-none-eabi toolchainu.
        exit /b 1
    )
)

echo.
if exist "%DEST_DIR%" (
    echo Toolchain jiz byl rozbalen v %DEST_DIR%, preskakuji rozbalovani.
) else (
    echo Rozbaluji arm-none-eabi toolchain do docasneho adresare %DEST_DIR%...
    tar -xf "%TEMP_DIR%\%TOOLCHAIN_ZIP%" -C "%TEMP_DIR%"
    if %errorlevel% neq 0 (
        echo Chyba pri rozbalovani stazeneho souboru.
        exit /b 1
    )

    echo Kopiruji arm-none-eabi toolchain do %DEST_DIR%...
    mkdir "%DEST_DIR%"
    xcopy /E /I /Y "%TEMP_DIR%\%TOOLCHAIN_VERSION%" "%DEST_DIR%" >nul 2>nul
    if %errorlevel% neq 0 (
        echo Chyba pri kopirovani souboru!
        exit /b 1
    )
)

echo.
set TOOLS_DIR=%~dp0build\_tools\windows
echo Pridavam toolchain do PATH pro aktualniho uzivatele...
echo %DEST_DIR%\bin
echo %TOOLS_DIR%

echo.
echo %PATH% | findstr /C:"%DEST_DIR%\bin" >nul
if %errorlevel% equ 0 (
    echo Toolchain jiz byl pridan do PATH, preskakuji pridani.
) else (
    setx PATH "%TOOLS_DIR%;%DEST_DIR%\bin;%PATH%"
    set "PATH=%TOOLS_DIR%;%DEST_DIR%\bin;%PATH%"
)

echo.
echo Testuji instalaci arm-none-eabi toolchainu...
echo.
arm-none-eabi-gcc --version
if %errorlevel% neq 0 (
    echo Instalace arm-none-eabi toolchainu selhala!
    exit /b 1
)

echo.
echo Testuji instalaci PicoLibSDK tools...
echo.
make --version
if %errorlevel% neq 0 (
    echo Vlozeni PicoLibSDK tools do PATH selhala!
    exit /b 1
)

echo Inicializace projektu probehla...

echo Mazu docasne soubory...
rmdir /S /Q "%TEMP_DIR%\%TOOLCHAIN_VERSION%"

exit /b 0
