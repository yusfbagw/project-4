@echo off

set imageName="gtcs2110/cs2110docker-c:stable"

docker container ps > nul 2>&1
if "%errorlevel%" neq "0" (
    echo ERROR: Docker not found. Ensure that Docker is installed and is running before running this script. Refer to the CS 2110 Docker Guide.
    exit /b 1
)

echo Attempting to pull down most recent image of %imageName%...
docker pull %imageName%
if "%errorlevel%" neq "0" (
    echo ERROR: Unable to pull down the most recent image of %imageName%
    exit /b 1
)

set curDir=%cd%


docker run --rm -i -v "%curDir%:/cs2110/host/" --entrypoint "bash" %imageName% "grade.sh" %*

if %ERRORLEVEL% NEQ 125 (
    if EXIST "report.html" (
        start "" "report.html"
    )
)