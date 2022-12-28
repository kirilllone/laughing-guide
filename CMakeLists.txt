cmake_minimum_required(VERSION 2.8 FATAL_ERROR) # Проверка версии CMake.
									# Если версия установленой программы
									# старее указаной, произайдёт аварийный выход.
project(hello_world)				# Название проекта

set(SOURCE_EXE test.c)			# Установка переменной со списком исходников

include_directories(libakrypt-0.x)			# Расположение заголовочных файлов

add_executable(mem ${SOURCE_EXE})	# Создает исполняемый файл с именем main#

add_subdirectory(libakrypt-0.x)				# Добавление подпроекта, указывается имя дирректории

target_link_libraries(mem /home/maksim/Desktop/govno/libakrypt-0.x/libakrypt.so.0.9.10)
target_link_libraries(mem /home/maksim/Desktop/govno/libakrypt-0.x/libakrypt-base.so.0.9.10)		# Линковка программы с библиотекой libakrypt.so.0.9.10

