# Запуск
Docker image: https://hub.docker.com/r/iignat/elhttpsrv

Запуск: 
```
docker run -t --rm -p 80:80 iignat/elhttpsrv
```
далее в браузере: http://localhost


# Сборка
Минимально необходимая конфигурация Linux для сборки (на примере Ubuntu:bionic):
```
apt-get update
apt-get install
apt-get install build-essential cmake git
```
После установки минимальной конфигурации скачиваем исходники из репозитория:
```
git -c http.sslVerify=false clone https://github.com/iignat/ElementaryHttpServer.git
```
Далее запускаем сборку:
```
cmake -S ./ -B ./ElementaryHttpServer
make
```
