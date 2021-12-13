#! /bin/bash

PROJECT_NAME=USB-Sensor_Desktop

rm -Rf output
mkdir output
cp ${PROJECT_NAME} output || exit 1
cp ${PROJECT_NAME}.desktop output || exit 1
cp app_icon.png output || exit 1


project_root=$(head -n 1 project_root.txt)
if [ "_$project_root" = "_" ]; then
    echo "Unable to read project_root.txt."
    exit 1
fi

if [ ! -e $project_root ]; then
    echo "$project_root not exist."
    exit 1
fi

qmake_bin=$(head -n 1 qmake_bin.txt)
if [ "_$qmake_bin" = "_" ]; then
    echo "Unable to read qmake_bin.txt."
    exit 1
fi

if [ ! -e $qmake_bin ]; then
    echo "$qmake_bin not exist."
    exit 1
fi


echo "Project root: $project_root"
echo "qmake binary: $qmake_bin"

cd output && \
    /opt/Qt/linuxdeployqt-continuous-x86_64.AppImage ${PROJECT_NAME} -qmake=$qmake_bin -qmldir=$project_root/qml -appimage -verbose=2
