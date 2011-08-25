set(kdroid_SRCS
   view/contactitemdelegate.cpp
   view/smsitemdelegate.cpp
   view/kdroidxmlgui.cpp
   view/sendview.cpp
   view/kdroidview.cpp
   contact/contact.cpp
   contact/contactlist.cpp
   sms/smsmessage.cpp
   sms/smslist.cpp
   net/packet.cpp
   net/port.cpp
   kdroid.cpp
   xmlhandler.cpp
   main.cpp
 )

kde4_add_ui_files(kdroid_SRCS view/kdroidview_base.ui view/prefs_base.ui view/sendview_base.ui)

kde4_add_kcfg_files(kdroid_SRCS settings.kcfgc )

kde4_add_executable(kdroid ${kdroid_SRCS})

target_link_libraries(kdroid ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} )

install(TARGETS kdroid ${INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( PROGRAMS kdroid.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES kdroid.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES kdroidui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kdroid )
install( FILES kdroid.notifyrc DESTINATION ${DATA_INSTALL_DIR}/kdroid  )
