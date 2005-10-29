#!/bin/sh

MAILS=`fgrep "Date:" ~/.evolution/mail/local/Misc.sbd/GtkMathView\ online | cut -c7- | tr ' ' '_'`

for m in $MAILS; do
	LABEL=`echo $m | tr '_' ' '`
	ID=`date -d "$LABEL" +%s`
	echo $ID \"$LABEL\"
done

