include ( ../../config.mak )
include ( ../../settings.pro )
include ( ../../version.pro )
 
TEMPLATE = lib
TARGET = mythupnp-$$LIBVERSION
CONFIG += thread dll
target.path = $${LIBDIR}
INSTALLS = target

setting.path = $${PREFIX}/share/mythtv/
setting.files += CDS_scpd.xml CMGR_scpd.xml MSRR_scpd.xml MXML_scpd.xml

INSTALLS += setting

QMAKE_CLEAN += $(TARGET) $(TARGETA) $(TARGETD) $(TARGET0) $(TARGET1) $(TARGET2)

# Input

HEADERS += httprequest.h upnp.h ssdp.h taskqueue.h  
HEADERS += upnpdevice.h upnptasknotify.h upnptasksearch.h threadpool.h upnputil.h
HEADERS += httpserver.h upnpcds.h upnpcdsobjects.h bufferedsocketdevice.h upnpmsrr.h
HEADERS += eventing.h upnpcmgr.h upnptaskevent.h upnptaskcache.h ssdpcache.h
HEADERS += upnpimpl.h multicast.h broadcast.h configuration.h
HEADERS += soapclient.h mythxmlclient.h

SOURCES += httprequest.cpp upnp.cpp ssdp.cpp taskqueue.cpp upnputil.cpp
SOURCES += upnpdevice.cpp upnptasknotify.cpp upnptasksearch.cpp threadpool.cpp
SOURCES += httpserver.cpp upnpcds.cpp upnpcdsobjects.cpp bufferedsocketdevice.cpp
SOURCES += eventing.cpp upnpcmgr.cpp upnpmsrr.cpp upnptaskevent.cpp ssdpcache.cpp
SOURCES += configuration.cpp soapclient.cpp mythxmlclient.cpp

INCLUDEPATH += ../libmyth
INCLUDEPATH += ../..
DEPENDPATH += ../libmyth

LIBS += -L../libmyth
LIBS += -lmyth-$$LIBVERSION
LIBS += $$EXTRA_LIBS

isEmpty(QMAKE_EXTENSION_SHLIB) {
  QMAKE_EXTENSION_SHLIB=so
}
isEmpty(QMAKE_EXTENSION_LIB) {
  QMAKE_EXTENSION_LIB=a
}

cygwin {
  QMAKE_EXTENSION_SHLIB=$${QMAKE_EXTENSION_SHLIB}$${QMAKE_EXTENSION_CYGWIN}
}

TARGETDEPS += ../libmyth/libmyth-$${LIBVERSION}.$${QMAKE_EXTENSION_SHLIB}

inc.path = $${PREFIX}/include/mythtv/upnp/

inc.files  = httprequest.h upnp.h ssdp.h taskqueue.h bufferedsocketdevice.h
inc.files += upnpdevice.h upnptasknotify.h upnptasksearch.h threadpool.h upnputil.h
inc.files += httpserver.h httpstatus.h upnpcds.h upnpcdsobjects.h
inc.files += eventing.h upnpcmgr.h upnptaskevent.h upnptaskcache.h ssdpcache.h
inc.files += upnpimpl.h multicast.h broadcast.h configuration.h
inc.files += soapclient.h mythxmlclient.h

INSTALLS += inc

cygwin:HEADERS += darwin-sendfile.h
cygwin:SOURCES += darwin-sendfile.c

freebsd:HEADERS += darwin-sendfile.h 
freebsd:SOURCES += darwin-sendfile.c 

macx {
    HEADERS += darwin-sendfile.h
    SOURCES += darwin-sendfile.c

    # This lib depends on libmythtv which depends on some stuff in libmythui.
    LIBS += -L../libmythui -lmythui-$$LIBVERSION
    #QMAKE_LFLAGS_SHLIB += -flat_namespace -undefined warning
}
