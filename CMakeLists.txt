project(kwin4_effect_scaleup)

cmake_minimum_required(VERSION 2.4)
add_definitions( ${QT_DEFINITIONS} )
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules )
find_package(KDE4)
add_definitions( ${KDE4_DEFINITIONS} )

find_package(X11)
include(KDE4Defaults)
include(MacroLibrary)
include_directories (${KDE4_INCLUDES})


kde4_add_plugin(kwin4_effect_scaleup scaleup.cpp)
target_link_libraries(kwin4_effect_scaleup ${KDE4_KDEUI_LIBS} kwineffects ${X11_LIBRARIES})
install(TARGETS kwin4_effect_scaleup DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES scaleup.desktop DESTINATION ${SERVICES_INSTALL_DIR}/kwin )


project (kcm_kwin4_effect_scaleup)
set( kcm_kwin4_effect_scaleup_SOURCES scaleup_config.cpp )
kde4_add_ui_files( kcm_kwin4_effect_scaleup_SOURCES scaleup_config.ui )
kde4_add_plugin (kcm_kwin4_effect_scaleup ${kcm_kwin4_effect_scaleup_SOURCES} )
target_link_libraries(kcm_kwin4_effect_scaleup ${KDE4_KDEUI_LIBS} kwineffects )
install (TARGETS kcm_kwin4_effect_scaleup DESTINATION ${PLUGIN_INSTALL_DIR})
install( FILES scaleup_config.desktop DESTINATION ${SERVICES_INSTALL_DIR}/kwin )
