PNG=$1
OUT="${PNG%.*}-shadow.png"
LIC=abouticon.txt

echo "Converting $PNG to $OUT"

convert "$PNG" \( +clone -background white -shadow 70x3+1+1 \) +swap -background none -layers flatten +repage -crop 72x72+7+7 "$OUT"

mv "$OUT" "$PNG"

if [ -f $LIC ]
	(head -n 2 $LIC; echo "White shadow added by Ivo Blöchliger"; tail -n +2 $LIC) > temp24985u98e.txt
   mv temp24985u98e.txt $LIC
   echo Edited $LIC
fi




