# Сборка
Минимально необходимая конфигурация Linux для сборки (на примере Ubuntu:bionic):

apt-get update
apt-get install
apt-get install build-essential cmake git

После установки минимальной конфигурации скачиваем исходники из репозитория:


git -c http.sslVerify=false clone https://github.com/iignat/ElementaryHttpServer.git

Далее запускаем сборку:

cmake -S ./ -B ./ElementaryHttpServer
make