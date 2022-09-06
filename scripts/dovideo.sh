#!/bin/bash 

ffmpeg -i frm_%6d.png -c:v libx264 -r $2 -pix_fmt yuv420p $1-$2fps.mp4
#ffmpeg -i frm_%7d.jpg -c:v libx264 -vf fps=$2 -pix_fmt yuv420p $1-$2fps.mp4
