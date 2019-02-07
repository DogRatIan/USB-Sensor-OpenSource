@Echo [Updating app_icon.png]
magick -background "#00000000" -size 128x128    app_icon.svg   ..\assets\app_icon.png
magick -background "#00000000" -size 72x72      app_icon.svg   ..\assets\app_icon.ico
copy app_icon.svg ..\assets\

@Echo [ALL DONE]
:END
