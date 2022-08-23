# script pra iniciar o jogo

gnome-terminal -- bash -c "./program 0 127.0.0.1 7875 127.0.0.1 7876 1; exec bash"
gnome-terminal -- bash -c "./program 1 127.0.0.1 7876 127.0.0.1 7877 0; exec bash"
gnome-terminal -- bash -c "./program 2 127.0.0.1 7877 127.0.0.1 7878 0; exec bash"
gnome-terminal -- bash -c "./program 3 127.0.0.1 7878 127.0.0.1 7875 0; exec bash"