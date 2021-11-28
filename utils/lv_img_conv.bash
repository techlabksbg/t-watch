PNG=$1
OUT=`basename "$1"`
OUT="${OUT%.*}"
OUTC="${PNG%.*}.c"

echo "Converting $PNG to $OUT"

echo "The Webconverter has changed. Change this script or use the offline converter found here"
echo "https://github.com/lvgl/lv_img_conv"
echo

echo "The online converter can be found here: https://lvgl.io/tools/imageconverter"

# curl -F "img_file=@$PNG" -F "name=$OUT" -F cf=true_color_alpha -F format=c_array -F submit=Convert https://lvgl.io/tools/img_conv_core.php > $OUTC


