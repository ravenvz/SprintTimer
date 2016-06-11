set -e

old_name=$1
new_name=$2

usage="Usage: ./globalRename.sh OLD_NAME NEW_NAME"

if [[ ! -z $old_name ]] && [[ ! -z $new_name ]];
then
    grep -lr $old_name core src qt_gui qt_storage_impl | xargs sed -i "s/$old_name/$new_name/g"
else
    echo $usage
fi
