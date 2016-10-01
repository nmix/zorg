# Zorg
Zorg - платформа для разработки процесс-ориентированного программного комплекса в Linux на языке C++. Используя классы платформы, можно относительно быстро запустить распределенную по процессам программную систему, например, систему управления периферийным оборудованием.

В основу платформы заложены следующие тезисы:

* Компоненты программной системы выполняются в независимом контексте - процессе.

* Процессы взаимодействуют между собой посредством сообщений.

* Система обмена сообщениями содержит один *старший* процесс и один или более *младших* процессов. Старший процесс может передавать сообщения любому младшему процессу, младший - только старшему.

* Программный комплекс **допускает** задержку доставки сообщений до 10 мс.


## Назначение и состав
Zorg представляет собой набор классов для быстрого прототипирования и создания многопроцессного программного комплекса. В основе набора лежат следующие классы:

* `Logger` - класс логирования. Представляет собой "тонкую" прослойку между пользовательским кодом и  `syslog`. Предоставляет возможность вести логирование работы программы в контексте процесса.

* `Daemon` - абстрактный класс процесса (демона). Этот класс представляет собой "обертку" вокруг метода `fork()`. Позволяет классам-наследникам исполняться в контексте нового процесса.

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
#### Порядок использования класса


1) Наследование от класса `Daemon` с передачей ему адреса. Адрес используется при сохранении идентификатора процесса, а также при обмене сообщениями.

2) Определение метода `loop()`. Данный метод реализует основной контекст исполнения процесса.

3) Определение методов `init()` и `finalize()` для инициализации и финализации соответственно. Методы вызываются однократно до или после основного контекста.

#### Пример - тестовый процесс
Файл `exdaemon.h`.
```cpp
#ifndef _EXDAEMON_H_
#define _EXDAEMON_H_

#include <zorg/daemon.h>

class ExDaemon : public Daemon {
public:
	ExDaemon(uint addr);
protected:
	void init();
	void loop();
	void finalize();
private:
	int log_counter;
};
#endif // _EXDAEMON_H_
```

Файл `exdaemon.cc`
```cpp
#include "exdaemon.h"
#include <boost/format.hpp>

ExDaemon::ExDaemon(uint addr) : Daemon(addr)
{
  log_counter = 0;
}

/*
 * Метод init() вызывается однократно в контексте исполнения процесса
 */
void ExDaemon::init()
{
	debug("init()");
    // чтение файла конфигурации (см. описание ниже)
	load_config();
	std::string val = config_tree.get<std::string>("foo");
	int test = config_tree.get("test", 0);
	debug("config file: foo = " + val + " test = " + std::to_string(test));
}

/*
 * Основной контекст исполнения программы - бизнес-логика процесса.
 * Метод вызывается в бесконечном цикле в классе `Daemon`
 */
void ExDaemon::loop()
{
	log_counter += 1;
	debug((boost::format("Debug message %s # %d") % progname() % log_counter).str());
	if (log_counter > 9) stop();
	sleep(1);
}

/*
 * Финализация - вызывается однократно перед завершением работы демона.
 */
void ExDaemon::finalize()
{
	debug("finalize()");
}
```

Файл `zdex.conf` - файл конфигурации. По-умолчанию метод `load_config()` загружает конфигурацию в формате `json` из файла `<имя-программы>.conf`, находящегося в текущей директории, в объект config_tree класса [boost::property_tree::ptree](http://www.boost.org/doc/libs/1_62_0/doc/html/property_tree.html). Можно задать путь к файлу конфигурации методом `set_config()`.
```json
{
  "foo": "bar",
  "test": 1  
}
```

Файл `main.cc`. Процесс запускается методом `start()`, останавливается - методом `stop()`. Для удобства определен метод `exec()`, который принимает в качестве аргумента строковые аналоги команд.
```cpp
#include "exdaemon.h"
#include <boost/format.hpp>
#include <iostream>
#include <exception>
#include <string>

int main(int argc, char* argv[])
{
	int address = 1;
	std::string cmd = "start";
	if (argc == 2)
	{
		cmd = argv[1];
	}
	else if (argc == 3)
	{
		cmd = argv[1];
		address = std::stoi(argv[2]);
	}
	ExDaemon d1(address);
	d1.set_log_prefix((boost::format("MYDAEMON<%d>") % address).str());
	try
	{
		d1.exec(cmd);
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
}
```
Компилируем и собираем
```bash
 g++ main.cc exdaemon.cc -o zdex -std=c++11 -lzlogger -lzdaemon -lboost_filesystem
 ```
В другой консоли запускаем парсинг системного журнала
```bash
tailf /var/log/syslog | grep MYDAEMON
```
В первой консоли запускаем исполняемый файл
```bash
./zdex start
# ... консоль с парсером
# tailf /var/log/syslog | grep MYDAEMON
Sep 28 20:57:41 udvb MYDAEMON<1>[6684]:      info : Daemon 1 started
Sep 28 20:57:41 udvb MYDAEMON<1>[6684]:     debug : init()
Sep 28 20:57:41 udvb MYDAEMON<1>[6684]:     debug : config file: foo = bar test = 1
Sep 28 20:57:41 udvb MYDAEMON<1>[6684]:     debug : Debug message zdex # 1
Sep 28 20:57:42 udvb MYDAEMON<1>[6684]:     debug : Debug message zdex # 2
Sep 28 20:57:43 udvb MYDAEMON<1>[6684]:     debug : Debug message zdex # 3
Sep 28 20:57:44 udvb MYDAEMON<1>[6684]:     debug : Debug message zdex # 4
Sep 28 20:57:45 udvb MYDAEMON<1>[6684]:     debug : Debug message zdex # 5
Sep 28 20:57:46 udvb MYDAEMON<1>[6684]:     debug : Debug message zdex # 6
Sep 28 20:57:47 udvb MYDAEMON<1>[6684]:     debug : Debug message zdex # 7
Sep 28 20:57:48 udvb MYDAEMON<1>[6684]:     debug : Debug message zdex # 8
Sep 28 20:57:49 udvb MYDAEMON<1>[6684]:     debug : Debug message zdex # 9
Sep 28 20:57:50 udvb MYDAEMON<1>[6684]:     debug : Debug message zdex # 10
Sep 28 20:57:50 udvb MYDAEMON<1>[6684]:     debug : finalize()
Sep 28 20:57:50 udvb MYDAEMON<1>[6684]:      info : Daemon stopped
```
#### Методы класса Daemon
Метод|Описание
-----|-------
**public**|
bool config_exists()| Возвращает `true` если файл конфигурации существует.
void exec(std::string)|Команда процессу, варианты: "start", "stop", "status", "restart", "kill".
uint get_address()|Получение адреса процесса
void kill_process()|Сброс процесса. Метод finalize() не вызывается.
void load_config()| Загружает файл конфигурации в объект `boost::property_tree::ptree config_tree`
void restart()|Перезапуск процесса
void set_config(std::string)|Задать файл конфигурации в аболютном или относительном формате. Пример: "/tmp/foo.cfg"
void set_log_prefix(std::string)|Установить префикс для записей журнала
void start()|Запуск процесса
void status()|Отображение статуса процесса: запущен/остановлен
void stop()|Останов процесса. Перед завершением процесса вызывается метод finalize().
**protected**|
virtual void check_messages()|Виртуальный метод. Прием и отправка сообщений. Вызывается перед каждым проходом основного цикла.
void debug(std::string)|Вывод строки в журнал с уровнем Logger::Level::debug
void error(std::string)|Вывод строки в журнал с уровнем Logger::Level::error
void fatal(std::string)|Вывод строки в журнал с уровнем Logger::Level::fatal
virtual void finalize()|Виртаульный метод. Вызывается после выхода из основного цикла процесса.
std::string get_config_path()|Возвращает абсолютный путь к файлу конфигурации.
std::string get_pidfile_path()|Возвращает абсолютный путь к файлу с идентификатором процесса
void info(std::string)|Вывод строки в журнал с уровнем Logger::Level::ingo
virtual void init()|Виртуальный метод. Вызывается перед запуском основного цикла процесса.
void log(Logger::Level, std::string)|Вывод строки в журнал с заданным уровнем (debug, info, warning, error, fatal)
virtual void loop() = 0|Абстрактный метод. Основной цикл процесса.
std::string progname()|Возвращает символическое имя процесса.
void warning(std::string)|Вывод строки в журнал с уровнем Logger::Level::warning

### Класс Node
#### Тривиальный пример
Файл `main.cc`
```cpp
#include <iostream>
#include <string>
#include <unistd.h>
#include <zorg/master_node.h>
#include <zorg/slave_node.h>

int main(int, char*[])
{
	MasterNode master_node; // "старший" узел
	SlaveNode slave_node(10); // "младщий" узел с адресом 10
	sleep(1);	// задержка для переходных процессов в ZMQ (создание сокета)
	int counter = 0;
	while (true)
	{
		// --- прием и отправка сообщений на старшем узле
        usleep(10);
		if (master_node.check_messages())
		{
			std::cout << "master_node: " << master_node.recv() << std::endl;
		}
		master_node.send_to(10, "hello " + std::to_string(counter));
		// --- прием и отправка сообщений на младшем узле
        usleep(10);
		if (slave_node.check_messages())
		{
			std::cout << " slave_node: " << slave_node.recv() << std::endl;
			slave_node.send("world " + std::to_string(counter));
		}
		// ---
		counter += 1;
		sleep(1);
	}
	return 0;
}
```
Компиляция и сборка
```bash
g++ main.cc -o nd -std=c++11 -lznode -lboost_filesystem -lzmq
```
Запуск
```bash
./nd
 slave_node: 10#hello 0
master_node: 10#world 0
 slave_node: 10#hello 1
master_node: 10#world 1
 slave_node: 10#hello 2
master_node: 10#world 2
 slave_node: 10#hello 3
...
```

#### Описание
В библиотеке libznode реализован централизованный метод обмена сообщениями: один старший узел и один или более младших узлов. Старший узел передает сообщение в адрес младшего узла, младшие узлы передают сообщения старшему.

Чтение сообщений происходят в неблокирующем режиме. Это сделано для того, чтобы была возможность корректного использования узлов в контексте процессов. Блокированный какой-либо операцией процесс затруднительно корректно завершить.

Прием и передача сообщений ведется через буферы входящий и исходящий буферы обмена соответственно. Методы передачи `send`, `send_to` и метод приема `recv` помещают и извлекают сообщения из буфера, при том как метод `check_messages` производит непосредственную передачу сообщения из буфера в сокет и прием сообщения из сокета с помещением во входящий буфер. Буферы сообщений реализованы классом `std::queue<std::string>`.
#### Методы
Метод|Описание
-----|-----
**public**|
std::string recv()|Получение первого сообщения из буфера входящих сообщений. При этом полученное сообщение из буфера удаляется.  Если перед вызовом `recv()` буфер пустой, то генерируется исключение `NodeException` с кодом `NO_DATA_AVAILABLE`.
bool has_messages()|Возвращает `true`, если во входящем буфере имеются сообщения.
uint awating_messages_count()|Возвращает количество сообщений во буфере входящих сообщений.
std::string format_message(uint addr, std::string data)|Формирование строки отправляемого сообщения: `addr` - адрес получателя или отправителя, `data` - данные сообщения.


### DLN
DLN (Daemon-Logger-Node) - это библиотека, обеспечивающая создание и взаимодействие независимых контекстов (процессов) исполнения программой системы.

#### Пример
Ниже приведен тривиальный пример, в котором 1 "старший" и 3 "младших" процесса взаимодействуют между собой.
##### Старший процесс
Файл `main_daemon.h`
```cpp
#ifndef _MAIN_DAEMON_H_
#define _MAIN_DAEMON_H_

#include <zorg/master_node_daemon.h>

class MainDaemon : public MasterNodeDaemon
{
public:
	MainDaemon();
private:
	void process_message_from(uint, std::string);
	// ---
	void init();
	void loop();
	// ---
	int loop_counter;
};

#endif // _MAIN_DAEMON_H_
```
Файл `main_daemon.cc`
```cpp
#include "main_daemon.h"
#include <boost/format.hpp>

#define ADDRESS 		101

MainDaemon::MainDaemon() : MasterNodeDaemon(ADDRESS)
{
}

void MainDaemon::init()
{
	// обязательный вызов метода init() родительского класса для инициализации
    // классов обмена сообщений в контексте исполнения процесса
	MasterNodeDaemon::init();
	loop_counter = 0;
}

// основной цикл процесса
// последовательная отправка трех сообщений трем процессам с адресами 105, 106 и 111
void MainDaemon::loop()
{
	if (loop_counter < 3)
	{
		send_to(105, "hello 105 " + std::to_string(loop_counter));
		send_to(106, "hello 106 " + std::to_string(loop_counter));
		send_to(111, "hello 111 " + std::to_string(loop_counter));
	}
	loop_counter += 1;
}

// переопределяем виртуальный абстрактный метод `process_message()` 
// родительского класса
void MainDaemon::process_message_from(uint, std::string)
{
}
```
##### Младший процесс
Файл `ex_daemon.h`
```cpp
#ifndef _EX_DAEMON_H_
#define _EX_DAEMON_H_

#include <zorg/slave_node_daemon.h>

class ExDaemon : public SlaveNodeDaemon
{
public:
	ExDaemon(uint);	
private:
	void process_message(std::string);
	// ---
	void init();
	void loop();
	// ---
	int loop_counter;
};
#endif // _EX_DAEMON_H_
```
Файл `ex_daemon.cc`
```cpp
#include "ex_daemon.h"

ExDaemon::ExDaemon(uint addr) : SlaveNodeDaemon(addr)
{
}

void ExDaemon::init()
{
	// обязательный вызов метода init() родительского класса для инициализации
    // классов обмена сообщений в контексте исполнения процесса
	SlaveNodeDaemon::init();
	loop_counter = 0;
}

void ExDaemon::loop()
{
}

// обработка входящего сообщения
// добавляем в ответное сообщение данные из входящего
void ExDaemon::process_message(std::string message)
{
	send("world! " + message);
}
```
##### Стартер
Файл `main.cc`
```cpp
#include "main_daemon.h"
#include "ex_daemon.h"

#include <iostream>
#include <exception>
#include <string>
#include <unistd.h>

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cerr << "Usage: dln start|stop|status" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::string cmd = argv[1];
    // --- старший процесс
	MainDaemon d;
    // --- 3 экземпляра младшего процесса
	ExDaemon d1(105);
	ExDaemon d2(106);
	ExDaemon d3(111);
	try
	{
		d.exec(cmd);
		d1.exec(cmd);
		d2.exec(cmd);
		d3.exec(cmd);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}
```
##### Компиляция, сборка и запуск
```bash
g++ main.cc main_daemon.cc ex_daemon.cc -o dln -std=c++11 -lzdaemon -lzlogger -lznode -lzdln -lzmq
```

В другой консоли запускаем просмотр журнала работы
```bash
tailf /var/log/syslog | grep ZORG
```
Запуск процессов
```bash
./dln start
# tailf /var/log/syslog | grep ZORG
Sep 29 15:47:13 udvb ZORG-106 [7947]:      info : Daemon 106 started
Sep 29 15:47:13 udvb ZORG-111 [7948]:      info : Daemon 111 started
Sep 29 15:47:13 udvb ZORG-105 [7946]:      info : Daemon 105 started
Sep 29 15:47:13 udvb ZORG-101 [7945]:      info : Daemon 101 started
Sep 29 15:47:14 udvb ZORG-101 [7945]:      info : msg out: 105#hello 105 0
Sep 29 15:47:14 udvb ZORG-105 [7946]:      info :  msg in: 105#hello 105 0
Sep 29 15:47:14 udvb ZORG-105 [7946]:      info : msg out: world! 105#hello 105 0
Sep 29 15:47:14 udvb ZORG-101 [7945]:      info : msg out: 106#hello 106 0
Sep 29 15:47:14 udvb ZORG-106 [7947]:      info :  msg in: 106#hello 106 0
Sep 29 15:47:14 udvb ZORG-106 [7947]:      info : msg out: world! 106#hello 106 0
Sep 29 15:47:14 udvb ZORG-101 [7945]:      info : msg out: 111#hello 111 0
Sep 29 15:47:14 udvb ZORG-111 [7948]:      info :  msg in: 111#hello 111 0
Sep 29 15:47:14 udvb ZORG-111 [7948]:      info : msg out: world! 111#hello 111 0
Sep 29 15:47:14 udvb ZORG-101 [7945]:      info :  msg in: 105#world! 105#hello 105 0
Sep 29 15:47:14 udvb ZORG-101 [7945]:      info :  msg in: 106#world! 106#hello 106 0
Sep 29 15:47:14 udvb ZORG-101 [7945]:      info :  msg in: 111#world! 111#hello 111 0
Sep 29 15:47:14 udvb ZORG-101 [7945]:      info : msg out: 105#hello 105 1
Sep 29 15:47:14 udvb ZORG-105 [7946]:      info :  msg in: 105#hello 105 1
Sep 29 15:47:14 udvb ZORG-105 [7946]:      info : msg out: world! 105#hello 105 1
Sep 29 15:47:14 udvb ZORG-101 [7945]:      info : msg out: 106#hello 106 1
Sep 29 15:47:14 udvb ZORG-106 [7947]:      info :  msg in: 106#hello 106 1
Sep 29 15:47:14 udvb ZORG-106 [7947]:      info : msg out: world! 106#hello 106 1
Sep 29 15:47:14 udvb ZORG-101 [7945]:      info : msg out: 111#hello 111 1
Sep 29 15:47:14 udvb ZORG-111 [7948]:      info :  msg in: 111#hello 111 1
Sep 29 15:47:14 udvb ZORG-111 [7948]:      info : msg out: world! 111#hello 111 1
Sep 29 15:47:14 udvb ZORG-101 [7945]:      info :  msg in: 105#world! 105#hello 105 1
Sep 29 15:47:14 udvb ZORG-101 [7945]:      info :  msg in: 106#world! 106#hello 106 1
Sep 29 15:47:14 udvb ZORG-101 [7945]:      info :  msg in: 111#world! 111#hello 111 1
Sep 29 15:47:14 udvb ZORG-101 [7945]:      info : msg out: 105#hello 105 2
Sep 29 15:47:14 udvb ZORG-105 [7946]:      info :  msg in: 105#hello 105 2
Sep 29 15:47:14 udvb ZORG-105 [7946]:      info : msg out: world! 105#hello 105 2
Sep 29 15:47:14 udvb ZORG-101 [7945]:      info : msg out: 106#hello 106 2
Sep 29 15:47:14 udvb ZORG-106 [7947]:      info :  msg in: 106#hello 106 2
Sep 29 15:47:14 udvb ZORG-106 [7947]:      info : msg out: world! 106#hello 106 2
Sep 29 15:47:14 udvb ZORG-101 [7945]:      info : msg out: 111#hello 111 2
Sep 29 15:47:14 udvb ZORG-111 [7948]:      info :  msg in: 111#hello 111 2
Sep 29 15:47:14 udvb ZORG-111 [7948]:      info : msg out: world! 111#hello 111 2
Sep 29 15:47:14 udvb ZORG-101 [7945]:      info :  msg in: 105#world! 105#hello 105 2
Sep 29 15:47:14 udvb ZORG-101 [7945]:      info :  msg in: 106#world! 106#hello 106 2
Sep 29 15:47:14 udvb ZORG-101 [7945]:      info :  msg in: 111#world! 111#hello 111 2
```

Проверка статуса процссов
```bash
./dln status
Daemon started [7945]
Daemon started [7946]
Daemon started [7947]
Daemon started [7948]

```
Останов процессов
```bash
./dln stop
# tailf /var/log/syslog | grep ZORG
# ...
Sep 29 15:54:35 udvb ZORG-106 [7947]:      info : Daemon stopped
Sep 29 15:54:35 udvb ZORG-101 [7945]:      info : Daemon stopped
Sep 29 15:54:35 udvb ZORG-105 [7946]:      info : Daemon stopped
Sep 29 15:54:35 udvb ZORG-111 [7948]:      info : Daemon stopped
```
#### Описание

Для реализации программной системы нескольких независимых контекстов необходимо наследоваться от класса `MasterNodeDaemon`, который реализует функционал "старшего" процесса, и наследоваться от класса `SlaveNodeDaemon`, реализующего функции "младшего" процесса.
Для корректного функционирования в системе может существовать только один старший процесс, и произвольное количество младших. Кроме того, в текущей реализации проекта, проверялась работа одного набора контекстов в операционной системе, т.е. запуск разных программных систем, в каждой из которых существуют свои старшие и младшие процессы в пределах одной операционной системы не проводился.

При наследовании от классов `MasterNodeDaemon` и `SlaveNodeDaemon` необходимо, кроме метода `loop()`, переопределить метод `process_message()`. Указанный метод будет вызываться всякий раз, когда процесс получит сообщение, адрессованное ему. Также обязательным условием корректного использования родительских классов является вызов родительских `init()` и `finalize()` при определении собственных методов инициализации и деинициализации.

Для отправки сообщения "младшему" процессу используется метод класса `send_to(uint addr, std::string data)`, где `addr` - адресат, `data` - данные сообщения. Отправка сообщения "старшему" процессу осуществляется методом `send(std::string data)`, при этом в начало сообщения автоматически добавляется адрес отправителя.


**КОНЕЦ**
