PNG=$1
OUT=`basename "$1"`
OUT="${OUT%.*}"
OUTC="${PNG%.*}.c"

echo "Converting $PNG to $OUT"

curl -F "img_file=@$PNG" -F "name=$OUT" -F cf=true_color_alpha -F format=c_array -F submit=Convert https://lvgl.io/tools/img_conv_core.php > $OUTC


