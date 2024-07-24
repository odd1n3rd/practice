#!/bin/bash


sudo apt update
sudo apt upgrade -y


sudo apt install -y xorg openbox x11-xserver-utils


mkdir -p ~/.config/openbox


cat <<EOL > ~/.config/openbox/autostart

xset -dpms


xset s off


xset s noblank

# Disable Alt+F4
xmodmap -e "keycode  24 = NoSymbol NoSymbol NoSymbol NoSymbol"

# Disable Ctrl+Alt+F1 to F12 (console switching)
for i in {67..77}; do xmodmap -e "keycode $i = NoSymbol NoSymbol NoSymbol NoSymbol"; done
for i in {96..107}; do xmodmap -e "keycode $i = NoSymbol NoSymbol NoSymbol NoSymbol"; done


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

# Reload the systemd manager configuration to apply the new service
sudo systemctl daemon-reload

# Enable the new kiosk service so that it starts at boot
sudo systemctl enable kiosk.service

echo "Setup is complete. Please reboot your system to start the kiosk mode."
