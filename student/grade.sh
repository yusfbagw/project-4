#!/bin/bash
set -eo pipefail

function show_header() {
    echo =========================================================
    echo "Running $1 test cases..."
}
function run_lc3() {
    show_header "LC3"
    sleep 1
    python3 suites/autograder.py "$@"
}
function display_lc3_html() {
    if [[ -f "report.html" ]]; then
        OS_NAME=$(uname)
        if [ "$OS_NAME" = "Darwin" ]; then
            open report.html
        elif [ "$OS_NAME" = "Linux" ]; then
            xdg-open "report.html" &
        fi
    fi
}

function run_c() {
    show_header "C"
    sleep 1
    make clean
    make run-tests "TEST=$@"
}

if [[ -z "$CS2110_IMAGE_VERSION" ]]; then
    # If run outside the Docker container (preferred):
    imageName="gtcs2110/cs2110docker-c:stable"

    if ! docker -v >/dev/null; then
        >&2 echo "ERROR: Please install Docker before running this script. Refer to the CS 2110 Docker Guide."
        exit 1
    fi

    if ! docker container ls >/dev/null; then
        >&2 echo "ERROR: Docker is not currently running. Please start Docker before running this script."
        exit 1
    fi

    echo "Attempting to pull down most recent image of $imageName..."

    if ! docker pull "$imageName"; then
        >&2 echo "WARNING: Unable to pull down the most recent image of $imageName"
    fi

    if command -v docker-machine &> /dev/null; then
        # We're on legacy Docker Toolbox
        # pwd -W doesn't work with Docker Toolbox
        # Extra '/' fixes some mounting issues
        curDir="/$(pwd)"
        else
        # pwd -W should correct path incompatibilites on Windows for Docker Desktop users
        curDir="/$(pwd -W 2>/dev/null || pwd)"
    fi

    rm -f report.html
    # If run in the Docker container:
    if [[ "$1" == "c" ]]; then
        show_header "C"
        docker run --rm -it -v "$curDir":/cs2110/host/ --entrypoint "make" "$imageName" "run-tests" "TEST=${@:2}"
    else
        show_header "C"
        docker run --rm -it -v "$curDir":/cs2110/host/ --entrypoint "make" "$imageName" "run-tests"
        
    fi
else
    # If run in the Docker container:
    if [[ "$1" == "c" ]]; then
        run_c "${@:2}"
    else
        run_c
    fi
fi