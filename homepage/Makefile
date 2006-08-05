
NULL =

HTML_FILES = \
  index.html \
  form.html \
  news.html \
  qna.html \
  $(NULL)

PNG_FILES = \
  svg.png \
  svg-small.png \
  svgmathml.png \
  font.png \
  abiword.png \
  abiword-macosx.png \
  abiword-small.png \
  t1lib.png \
  t1lib-small.png \
  pango.png \
  logo.png \
  mathmllogo-small.png \
  $(NULL)

HOME_FILES = \
  gtkmathview.css \
  gtkmathview-online.conf.xml \
  $(HTML_FILES) \
  $(PNG_FILES)
  $(NULL)

all: $(HOME_FILES) $(PNG_FILES)

abiword-small.png: abiword.png
	convert -resize 200x $< $@

svg-small.png: svg.png
	convert -resize 200x $< $@

t1lib-small.png: t1lib.png
	convert -crop 200x120+0+0 $< $@

%.svg: %.xml
	./mathmlsvg.sh --font-size=48 $<

%.png: %.svg
	rsvg -d 72 $< $@

stats.png: stats.gp stats.data
	gnuplot $< >$@

stats.data: mkstats.sh /Users/luca/Library/Mail/Mailboxes/Mailing\ Lists/GtkMathView\ Online.mbox/Messages/
	./mkstats.sh >$@

mathmllogo-small.png: mathmllogo.png
	convert -resize 88x $< $@

sync_all: $(HOME_FILES)
	rsync --cvs-exclude -r -v $^ lpadovan@mowgli.cs.unibo.it:/projects/helm/public_html/software/mml-widget
	rsync render.cgi lpadovan@mowgli.cs.unibo.it:/projects/helm/public_html/software/mml-widget/cgi-bin

sync: $(HTML_FILES)
	rsync --cvs-exclude -r -v $^ lpadovan@mowgli.cs.unibo.it:/projects/helm/public_html/software/mml-widget
	rsync render.cgi lpadovan@mowgli.cs.unibo.it:/projects/helm/public_html/software/mml-widget/cgi-bin

