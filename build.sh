#!/bin/bash

usage() { echo "Usage: $0 [-t <0|1>] [-c <0|1>]" 1>&2; exit 1; }

BUILD_PATH="./tsn_app"

while getopts ":t:c:" o; do
    case "${o}" in
        t)
            t=${OPTARG}
            ((t == 0 || t == 1)) || usage
            ;;
	c)
            c=${OPTARG}
	    ((c == 0 || c == 1)) || usage
	    ;;
        *)
            usage
            ;;
    esac
done
shift $((OPTIND-1))

if [ -z "${t}" || [ -z "${c}" ]; then
    usage
else
    if [ ${c} == "1" ]; then
	if [ ${t} == "1" ]; then
	    make -C ${BUILD_PATH} cleantest
	else
	    make -C ${BUILD_PATH} clean
	    make -C ${BUILD_PATH} cleantest
	fi
    else
        mkdir -p ./tsn_app/mkdir
        make -C ${BUILD_PATH} V=s -j2
        if [ ${t} == "1" ]; then
            make -C ${BUILD_PATH} test V=s -j2
        fi
    fi
fi

