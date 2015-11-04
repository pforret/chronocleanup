#!/bin/sh
# just a wrapper to start the php program
DIR=$(dirname $0)
PROG=$(basename $0 .sh)
REAL=$(cd $DIR; pwd)

if [ -f "$REAL/$PROG.php" ] ; then
	PARAMS=""
	cd $REAL
	while [ -n "$1" ] ; do
		WORD=$1
		if [ "${WORD}" == "${WORD// /}" ] ; then
			PARAMS="$PARAMS $1"
		else
			PARAMS="$PARAMS \"$1\""
		fi
		shift
	done
	php "$REAL/$PROG.php" $PARAMS
else
	echo "ERROR: cannot find program [$REAL/$PROG.php]" >&2
fi
