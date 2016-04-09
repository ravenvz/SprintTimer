set -e

old_name=$1
new_name=$2

usage="Usage: ./globalRename.sh OLD_NAME NEW_NAME"

if [[ ! -z $old_name ]] && [[ ! -z $new_name ]];
then
    (cd src && grep -lr $old_name | xargs sed -i "s/$old_name/$new_name/g")
    (cd tests && grep -lr $old_name | xargs sed -i "s/$old_name/$new_name/g")
else
    echo $usage
fi
