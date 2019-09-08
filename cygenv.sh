#!/usr/bin/env bash
#
# the "PROJ_ROOT" environment variable must be defined before sourcing this file
#PROJ_ROOT=$HOME/projects/gprs

# Project name
export PROJ=$1

export PROJ_WORKDIR=${GPRS_PROJ_ROOT}/$PROJ

# Soft directory location
export SOFT_WORKDIR=${PROJ_WORKDIR}

# Soft environment setup
if [ -f ${SOFT_WORKDIR}/env/set_env.sh ]; then
	source ${SOFT_WORKDIR}/env/set_env.sh
fi

if [ -d ${SOFT_WORKDIR} ]; then
	cd ${SOFT_WORKDIR}
	echo "Project switched to $PROJ"
else
	echo "!!!!!!!==error==!!!!!!!!"
	echo "-->Project \"${SOFT_WORKDIR}\" NOT exist!!"
fi	
