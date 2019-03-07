# Build a static library (libggk.a)
noinst_LIBRARIES = libggk.a
libggk_a_CXXFLAGS = -fPIC -Wall -Wextra -std=c++14 -lcrypto -lm -L/usr/include/curl/lib -lcurl -lmysqlclient  $(GLIB_CFLAGS) $(GIO_CFLAGS) $(GOBJECT_CFLAGS)
libggk_a_SOURCES = ../../ArchivedLocation.cpp \
		   ../../ArchivedLocation.h \
		   ../../ArchiveLibrary.cpp \
		   ../../ArchiveLibrary.h \
		   ../../CoordinateInformation.cpp \
		   ../../CoordinateInformation.h \
		   ../../CURLplusplus.cpp \
		   ../../CURLplusplus.h \
		   ../../Database.cpp \
                   ../../Database.h \
                   ../../DataDepositor.cpp \
                   ../../DataDepositor.h \
                   ../../DataDepositor.o \
                   ../../EncryptionModule.cpp \
                   ../../EncryptionModule.h \
		   ../../fetchReport.cpp \
		   ../../fetchReport.h \
		   ../../googleAPI.cpp \
		   ../../googleAPI.h \
		   ../../hashfunc.cpp \
		   ../../hashfunc.h \
		   ../../json.hpp \
		   ../../LocationInformation.cpp \
		   ../../LocationInformation.h \
		   ../../placesAPI.cpp \
		   ../../placesAPI.h \
		   ../../squasher.cpp \
		   ../../squasher.h \
		   ../../StringParser.h \
		   ../../UserController.cpp \
		   ../../UserController.h \
                   ../../md5.cpp \
                   ../../md5.h \
                   ../../RawDataRepository.cpp \
                   ../../RawDataRepository.h \
                   ../../SystemTimeManager.cpp \
                   ../../SystemTimeManager.h \
                   ../../UserController.cpp \
                   ../../UserController.h \
                   ../../user.cpp \
                   ../../user.h \
                   ../../UserLibrary.cpp \
                   ../../UserLibrary.h \
		   ../../UserVisitInfo.cpp \
		   ../../UserVisitInfo.h \
		   ../../VisitLog.cpp \
		   ../../VisitLog.h \
		   DBusInterface.cpp \
                   DBusInterface.h \
                   DBusMethod.cpp \
                   DBusMethod.h \
                   DBusObject.cpp \
                   DBusObject.h \
                   DBusObjectPath.h \
                   GattCharacteristic.cpp \
                   GattCharacteristic.h \
                   GattDescriptor.cpp \
                   GattDescriptor.h \
                   GattInterface.cpp \
                   GattInterface.h \
                   GattProperty.cpp \
                   GattProperty.h \
                   GattService.cpp \
                   GattService.h \
                   GattUuid.h \
                   Globals.h \
                   Gobbledegook.cpp \
                   ../include/Gobbledegook.h \
                   HciAdapter.cpp \
                   HciAdapter.h \
                   HciSocket.cpp \
                   HciSocket.h \
                   Init.cpp \
                   Init.h \
                   Logger.cpp \
                   Logger.h \
                   Mgmt.cpp \
                   Mgmt.h \
                   Server.cpp \
                   Server.h \
                   ServerUtils.cpp \
                   ServerUtils.h \
                   standalone.cpp \
                   TickEvent.h \
                   Utils.cpp \
                   Utils.h
# Build our standalone server (linking statically with libggk.a, linking dynamically with GLib)
standalone_CXXFLAGS = -fPIC -Wall -Wextra -std=c++14 -lm -L/usr/include/curl/lib -lcurl -lmysqlclient -lcrypto
noinst_PROGRAMS = standalone
standalone_SOURCES = standalone.cpp
standalone_LDADD = libggk.a
standalone_LDLIBS = $(GLIB_LIBS) $(GIO_LIBS) $(GOBJECT_LIBS)
