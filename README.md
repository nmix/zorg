# Zorg
Zorg - платформа для разработки процесс-ориентированного программного комплекса в Linux на языке C++. Используя классы платформы, можно относительно быстро запустить распределенную по процессам программную систему, например, систему управления периферийным оборудованием.

В основу платформы заложены следующие тезисы:

* Компоненты программной системы выполняются в независимом контексте - процессе.

* Процессы взаимодействуют между собой посредствам сообщений.

* Система обмена сообщениями содержит одиин *старший* процесс и один или более *младших* процессов. Старший процесс может передавать сообщения любому младшему процессу, младший - только старшему.

* Программный комплекс **допускает** задержку доставки сообщений до 10 мс.


## Назначение и состав
Zorg представляет собой набор классов для быстрого прототипирования и создания многопроцессного программного комплекса. В основе набора лежат следующие классы:

* `Logger` - класс журналирования. Представляет собой "тонкую" прослойку между пользовательским кодом и `syslog`. Предоставляет возможность вести логирование работы программы в контексте процесса.

* `Daemon` - абстрактный класс процесса (демона). Этот класс представляет собой "обертку" вокруг метода `fork()`. Позволяет классам-наследникам исполняться в контексте новго процесса.

* `Node` - абстрактный класс приема/передачи сообщений. Класс "оборачивает" методы библиотеки [zmq](http://zeromq.org). Текущая редакция системы сообщений предполагает (пока) единственный вариант обмена сообщений - централизованный: один "старший" процесс, один или много "младших" процессов; "старший" передает сообщение любому "младшему", а "младший" - только "старшему".

## Установка

### Boost (v. 1.61)
[Boost C++ Libraries](http://www.boost.org)

```bash
mkdir /tmp/boost
cd /tmp/boost
curl -L -o boost.tar.gz https://sourceforge.net/projects/boost/files/boost/1.61.0/boost_1_61_0.tar.gz
tar -xf boost.tar.gz -C /usr/local
cd /usr/local/boost*
./bootstrap.sh
./b2 install
```


### ZeroMQ (v. 4.1.5)
[ZeroMQ Distributed Messaging](http://zeromq.org)

```bash
mkdir /tmp/zmq
cd /tmp/zmq
curl -L -o zmq.tar.gz https://github.com/zeromq/zeromq4-1/releases/download/v4.1.5/zeromq-4.1.5.tar.gz
tar -xf zmq.tar.gz
cd zeromq-*
./configure
make
make install
```

### Zorg

```bash
git clone git@github.com:nmix/zorg.git
cd zorg
mkdir build
cd build
cmake ..
make
make install
ldconfig
```


## Использование

### Класс Logger
#### Пример
Файл `main.cc`
```cpp
#include <zorg/logger.h>
int main(int, char*[])
{
  Logger logger("LOGGER-EX");
  logger.log(Logger::Level::debug, "Debug message");
  logger.log(Logger::Level::info, "Info message");
  logger.log(Logger::Level::warning, "Warning message");
  logger.log(Logger::Level::error, "Error message");
  logger.log(Logger::Level::fatal, "Fatal message");
  return 0;
}
```
Компиляция и сборка
```bash
g++ main.cc -o lgr -std=c++11 -lzlogger
```
В другой консоли запускаем чтение из `syslog`
```bash
tailf /var/log/syslog | grep LOGGER
```
Запускаем пример
```bash
./lgr
# в другой консоли
# tailf /var/log/syslog | grep LOGGER
Sep 28 15:39:09 udvb LOGGER-EX[20258]:     debug : Debug message
Sep 28 15:39:09 udvb LOGGER-EX[20258]:      info : Info message
Sep 28 15:39:09 udvb LOGGER-EX[20258]:   warning : Warning message
Sep 28 15:39:09 udvb LOGGER-EX[20258]:     error : Error message
Sep 28 15:39:09 udvb LOGGER-EX[20258]:     fatal : Fatal message
```
#### Описание
Класс `Logger` представляет собой "тонкую" обертку вокруг системного вызова `syslog`.
В конструкторе класса задается строка, которая будет ставится в начало всех сообщений в журнале. Это сделано для удобного парсинга строк в общем журнале сообщений.
Класс определяет 5 уровней сообщений:
```cpp
enum class Level { debug = 0, info = 1, warning = 2, error = 3, fatal = 4 };
```
которые сопоставляются соответствующим уровням в `syslog`. Уровни транслируются в соответствующие уровни `syslog`: `LOG_DEBUG`, `LOG_INFO`, `LOG_WARNING`, `LOG_ERR` и `LOG_CRIT`.

Метод
```cpp
void log(Logger::Level, std::string);
```
выполняет запись сообщения в журанл `/var/log/syslog`.



### Класс Daemon

### Класс Node

### DLN