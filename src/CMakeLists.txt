set(kdroid_SRCS
   util/phonenumberutil.cpp
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
   net/tcpport.cpp
   net/tcpclientport.cpp
   net/tcpserverport.cpp
   net/dispatcher.cpp
   dbus/messagedbushandler.cpp
   kdroid.cpp
   xmlhandler.cpp
   main.cpp
 )

kde4_add_ui_files(kdroid_SRCS view/kdroidview_base.ui view/prefs_base.ui view/sendview_base.ui)

kde4_add_kcfg_files(kdroid_SRCS settings.kcfgc )

#qt4_generate_dbus_interface(dbus/messagedbushandler.h)

qt4_add_dbus_adaptor( kdroid_SRCS dbus/org.kde.kdroid.Message.xml dbus/messagedbushandler.h MessageDBusHandler MessageDBusHandlerAdaptor MessageDBusHandlerAdaptor)

kde4_add_executable(kdroid ${kdroid_SRCS})

target_link_libraries(kdroid ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KNOTIFYCONFIG_LIBRARY} )

install(TARGETS kdroid ${INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( PROGRAMS kdroid.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES kdroid.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES kdroidui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kdroid )
install( FILES kdroid.notifyrc DESTINATION ${DATA_INSTALL_DIR}/kdroid  )