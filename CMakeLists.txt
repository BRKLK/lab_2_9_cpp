cmake_minimum_required(VERSION 3.15) # Проверка версии CMake.
									 # Если версия установленой программы
									 # старее указаной, произайдёт аварийный выход.

set(PROJECT_NAME lab_2_9)				 # имя проекта		 
project("${PROJECT_NAME}")

set(CMAKE_CURRENT_LIST_DIR "D:/VSCode_projects/BFU/CppLabs/lab_2_9_cpp")

add_executable("${PROJECT_NAME}" main.cpp)


set(CMAKE_CXX_STANDARD 20)			 # устанавливаем 17 стандарт языка
# set(CMAKE_CXX_EXTENSIONS OFF)
# set(CMAKE_CXX_STANDARD_REQUIRED TRUE)


add_definitions(-DSFML_STATIC)		 # дефайны	


include_directories(headers /usr/include "ext/SFML/include")
link_directories("ext/SFML/lib")

set(SFML_ROOT c:/SFML)
set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}/ext/SFML")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML REQUIRED system window graphics)
if(SFML_FOUND)
	include_directories(${SFML_INCLUDE_DIR})
    include_directories("C:\\Windows\\Fonts")
	target_link_libraries("${PROJECT_NAME}" ${SFML_LIBRARIES})
endif()


# file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})


if(WIN32)
    file(GLOB BINARY_DEP_DLLS "${SFML_INCLUDE_DIR}/../bin/*.dll")
    file(COPY ${BINARY_DEP_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
    file(GLOB MINGW_DEP_DLLS "C:/mingw64/bin/*.dll")
    file(COPY ${MINGW_DEP_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
endif()


											
