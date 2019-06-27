#! /bin/sh

hash=`git rev-parse --short=8 HEAD`
echo GitHash=$hash
if [ "_$1_" != "__" ]
then
echo $hash > "$1/git_hash.txt"
fi
