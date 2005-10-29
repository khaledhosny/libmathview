#!/bin/sh

for m in ~/Library/Mail/Mailboxes/Mailing\ Lists/GtkMathView\ Online.mbox/Messages/*.emlx; do
	ID=`fgrep "<real>" "$m" | cut -d '>' -f2 | cut -d '<' -f1`
	LABEL=`fgrep "Date:" "$m" | cut -c7-`
	echo $ID \"$LABEL\"
done

