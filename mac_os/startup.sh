#!/bin/sh

SPRINT_TIMER_BUNDLE="`echo "$0" | sed -e 's/\/Contents\/MacOS\/SprintTimer//'`"
SPRINT_TIMER_RESOURCES="$SPRINT_TIMER_BUNDLE/Contents/Resources"
SPRINT_TIMER_TEMP="/tmp/SprintTimer/$UID"
SPRINT_TIMER_ETC="$SPRINT_TIMER_TEMP/etc"
SPRINT_TIMER_PANGO_RC_FILE="$SPRINT_TIMER_ETC/pango/pangorc"

echo "running $0"
echo "SPRINT_TIMER_BUNDLE: $SPRINT_TIMER_BUNDLE"
echo "SPRINT_TIMER_RESOURCES: $SPRINT_TIMER_RESOURCES"

# # Start X11 ...
# ps -wx -ocommand | grep -e '[X]11.app' > /dev/null
# if [ "$?" != "0" -a ! -f ~/.xinitrc ]; then
#     echo "rm -f ~/.xinitrc" > ~/.xinitrc
#     sed 's/xterm/# xterm/' /usr/X11R6/lib/X11/xinit/xinitrc >> ~/.xinitrc
# fi
#
# mkdir -p $SPRINT_TIMER_TEMP
# cat << __END_OF_GETDISPLAY_SCRIPT__ > "$SPRINT_TIMER_TEMP/getdisplay.sh"
# #!/bin/sh
# mkdir -p "$SPRINT_TIMER_TEMP"
#
# if [ "\$DISPLAY"x == "x" ]; then
#     echo :0 > "$SPRINT_TIMER_TEMP/display"
# else
#     echo \$DISPLAY > "$SPRINT_TIMER_TEMP/display"
# fi
# __END_OF_GETDISPLAY_SCRIPT__
# chmod +x "$SPRINT_TIMER_TEMP/getdisplay.sh"
# rm -f $SPRINT_TIMER_TEMP/display
# open-x11 $SPRINT_TIMER_TEMP/getdisplay.sh || \
# open -a XDarwin $SPRINT_TIMER_TEMP/getdisplay.sh || \
# echo ":0" > $SPRINT_TIMER_TEMP/display
#
# while [ "$?" == "0" -a ! -f $SPRINT_TIMER_TEMP/display ];
# do
#   #echo "Waiting for display $SPRINT_TIMER_TEMP/display"
#   sleep 1;
# done
# export "DISPLAY=`cat $SPRINT_TIMER_TEMP/display`"
#
# ps -wx -ocommand | grep -e '[X]11' > /dev/null || exit 11
#
# # Setup temporary runtime files
# rm -rf "$SPRINT_TIMER_TEMP"
#
# # Because the bundle could be located anywhere at runtime, we have to
# # create temporary copies of the Pango configuration files that
# # reflect our current location
# mkdir -p "$SPRINT_TIMER_ETC/pango"
# sed -e 's|/opt/local/etc|'"$SPRINT_TIMER_ETC|g" "$SPRINT_TIMER_RESOURCES/etc/pango/pangorc" > "$SPRINT_TIMER_ETC/pango/pangorc"
# sed -e 's|/opt/local|\"'"$SPRINT_TIMER_RESOURCES|g" -e "s/\.so/.so\"/g" "$SPRINT_TIMER_RESOURCES/etc/pango/pango.modules" > "$SPRINT_TIMER_ETC/pango/pango.modules"
# cp -f "$SPRINT_TIMER_RESOURCES/etc/pango/pangox.aliases" "$SPRINT_TIMER_ETC/pango/pangox.aliases"

export "DYLD_LIBRARY_PATH=$SPRINT_TIMER_RESOURCES/lib"
# export "FONTCONFIG_PATH=$SPRINT_TIMER_RESOURCES/etc/fonts"
# export "PANGO_RC_FILE=$SPRINT_TIMER_PANGO_RC_FILE"
export "PATH=$SPRINT_TIMER_RESOURCES/bin:$PATH"

exec "$SPRINT_TIMER_RESOURCES/bin/sprint_timer"
