# ������
���������� ����������� ������������ Linux ��� ������ (�� ������� Ubuntu:bionic):

apt-get update
apt-get install
apt-get install build-essential cmake git

����� ��������� ����������� ������������ ��������� ��������� �� �����������:


git -c http.sslVerify=false clone https://github.com/iignat/ElementaryHttpServer.git

����� ��������� ������:

cmake -S ./ -B ./ElementaryHttpServer
make