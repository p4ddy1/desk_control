cmake_minimum_required(VERSION 3.22)
project(desk_control)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Bluetooth
        REQUIRED)

add_executable(desk_control src/main.cpp src/Bluetooth/Service/BluetoothController.cpp src/Bluetooth/Service/BluetoothController.h src/Bluetooth/Service/BluetoothScanner.cpp src/Bluetooth/Service/BluetoothScanner.h src/Bluetooth/Model/Desk.cpp src/Bluetooth/Model/Desk.h src/Gui/MainWindow/MainWindow.cpp src/Gui/MainWindow/MainWindow.h src/Gui/MainWindow/MainWindow.ui src/Gui/DeviceScanWindow/DeviceScanWindow.cpp src/Gui/DeviceScanWindow/DeviceScanWindow.h src/Gui/DeviceScanWindow/DeviceScanWindow.ui src/Gui/Model/DeskModel.cpp src/Gui/Model/DeskModel.h src/Config/Devices/DeviceConfigStorage.cpp src/Config/Devices/DeviceConfigStorage.h)

set_target_properties(desk_control PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist
        )

target_link_libraries(desk_control
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Bluetooth
        )

