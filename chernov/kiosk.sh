#!/bin/bash

###https://pimylifeup.com/ubuntu-chromium-kiosk/
sudo apt update
sudo apt upgrade -y

sudo apt install -y xorg openbox x11-xserver-utils


mkdir -p ~/.config/openbox


cat <<EOL > ~/.config/openbox/autostart

xset -dpms


xset s off


xset s noblank


xmodmap -e "keycode  24 = NoSymbol NoSymbol NoSymbol NoSymbol"

for i in {67..77}; do xmodmap -e "keycode $i = NoSymbol NoSymbol NoSymbol NoSymbol"; done
for i in {96..107}; do xmodmap -e "keycode $i = NoSymbol NoSymbol NoSymbol NoSymbol"; done

# тут путь к файлу
/home/toor2/txt_edit
EOL

chmod +x ~/.config/openbox/autostart

cat <<EOL > ~/.xinitrc
exec openbox-session
EOL

sudo bash -c 'cat <<EOL > /etc/systemd/system/kiosk.service
[Unit]
Description=Custom Application Kiosk
After=systemd-user-sessions.service

[Service]
User=$USER
Type=simple
ExecStart=/usr/bin/startx

[Install]
WantedBy=multi-user.target
EOL'

sudo systemctl daemon-reload

sudo systemctl enable kiosk.service

echo "Перезагрузите"

