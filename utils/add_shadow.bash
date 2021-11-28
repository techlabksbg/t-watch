PNG=$1
OUT="${PNG%.*}-shadow.png"
LIC=abouticon.txt

echo "Converting $PNG to $OUT"

convert "$PNG" \( +clone -background white -shadow 100x2+1+1 \) +swap -background none -layers flatten +repage -crop 72x72+0+0 "$OUT"

mv "$OUT" "$PNG"

if [ -f $LIC ]
then
	(head -n 2 $LIC; echo "White shadow added by Ivo Blöchliger"; tail -n +2 $LIC) > temp24985u98e.txt
   mv temp24985u98e.txt $LIC
   echo Edited $LIC
fi

