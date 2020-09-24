set terminal png
#
set output "orbit.png"
set size square
plot "earth.txt" w l notitle, "orbital.txt" u 3:4 w l notitle
#