PNG=$1
OUT="${PNG%.*}-shadow.png"

echo "Converting $PNG to $OUT"

convert "$PNG" \( +clone -background white -shadow 70x4+2+2 \) +swap -background none -layers flatten +repage "$OUT"



