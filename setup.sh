#!/usr/bin/env bash
#
# Setting up the CSDTK environment
#
# edit .profile .bashrc .zshrc

function help()
{
    echo "Usage:"
    echo "./setup.sh [compile software dir] [projects dir]"
    echo "e.g. ./setup.sh ~/sofware/CSDTK ~/projects"
}


if [[ "$1x" == "helpx" || "$1x" == "-hx" || "$1x" == "-helpx" || "$1x" == "--helpx" ]]; then
    echo "=========================="
    help
    echo "=========================="
    exit 0
fi

if [[ "$1x" == "x" || "$2x" == "x" ]]; then
    echo "=========================="
    echo "!! parameters error!!"
    help
    echo "=========================="
    exit 0
fi

[ -f $HOME/.profile ] || {
    echo >&2 "$HOME/.profile: No such file in your home directory !"
    exit 1
}


export GPRS_SOFTWARE_ROOT=`cd $1; pwd`
export GPRS_PROJ_ROOT=`cd $2; pwd`

# Root directorty for all the projects
[ -z "${GPRS_PROJ_ROOT}" ] && {
  [ -d $HOME/projects ] || mkdir -p $HOME/projects
  GPRS_PROJ_ROOT='$HOME/projects'
}



# Backup the previous $HOME/.profile
echo "======setup start======"
echo "Setting up the CSDTK environment ..."

# Type work <project_name> to switch the environment to the required project
cp -f $HOME/.bashrc $HOME/.bashrc_$(date +%F_%H%M)
sed -i '/###############GPRS CSDTK#############/,/############GPRS CSDTK END############/d' $HOME/.bashrc
echo "###############GPRS CSDTK#############" >> $HOME/.bashrc
echo "alias work='source ${GPRS_SOFTWARE_ROOT}/cygenv.sh'" >> $HOME/.bashrc
cat $HOME/.bashrc | \
egrep '^export GPRS_PROJ_ROOT' > /dev/null || {
  cat >> $HOME/.bashrc <<EOF
export GPRS_PROJ_ROOT=${GPRS_PROJ_ROOT}
export PATH=\$PATH:${GPRS_SOFTWARE_ROOT}/bin:${GPRS_SOFTWARE_ROOT}/mingw32/usr/bin:${GPRS_SOFTWARE_ROOT}/mips-rda-elf/bin:${GPRS_SOFTWARE_ROOT}/rv32-elf/bin
export PATH=\$PATH:${GPRS_SOFTWARE_ROOT}/cooltools
export LD_LIBRARY_PATH=\${LD_LIBRARY_PATH}:${GPRS_SOFTWARE_ROOT}/lib:${GPRS_SOFTWARE_ROOT}/mingw32/usr/lib
EOF
  [ "$?" -eq "0" ] && echo -e "success!\nexport GPRS_PROJ_ROOT=${GPRS_PROJ_ROOT}" || echo "failed!"
}
echo "############GPRS CSDTK END############" >> $HOME/.bashrc
source $HOME/.bashrc


#for zsh
if [ -f "$HOME/.zshrc" ];then
echo "edit .zshrc"
cp -f $HOME/.zshrc $HOME/.zshrc_$(date +%F_%H%M)
sed -i '/###############GPRS CSDTK#############/,/############GPRS CSDTK END############/d' $HOME/.zshrc
echo "###############GPRS CSDTK#############" >> $HOME/.zshrc
echo "alias work='source ${GPRS_SOFTWARE_ROOT}/cygenv.sh'" >> $HOME/.zshrc
cat $HOME/.zshrc | \
egrep '^export GPRS_PROJ_ROOT' > /dev/null || {
  cat >> $HOME/.zshrc <<EOF
export GPRS_PROJ_ROOT=${GPRS_PROJ_ROOT}
export PATH=\$PATH:${GPRS_SOFTWARE_ROOT}/bin:${GPRS_SOFTWARE_ROOT}/mingw32/usr/bin:${GPRS_SOFTWARE_ROOT}/mips-rda-elf/bin:${GPRS_SOFTWARE_ROOT}/rv32-elf/bin
export PATH=\$PATH:${GPRS_SOFTWARE_ROOT}/cooltools
export LD_LIBRARY_PATH=\${LD_LIBRARY_PATH}:${GPRS_SOFTWARE_ROOT}/lib:${GPRS_SOFTWARE_ROOT}/mingw32/usr/lib
EOF
  [ "$?" -eq "0" ] && echo -e "success!\nexport GPRS_PROJ_ROOT=${GPRS_PROJ_ROOT}" || echo "failed!"
}
echo "############GPRS CSDTK END############" >> $HOME/.zshrc
source $HOME/.zshrc
fi
echo "=======setup end======="


