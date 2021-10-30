HEADERS += \
    $$PWD/i_operation.h \
    $$PWD/i_operation_factory.h \
    $$PWD/i_operation_invoker.h \
    $$PWD/impl/async_operation_invoker.h \
    $$PWD/impl/console_util_operation.h \
    $$PWD/impl/copy_by_wildcard_operation.h \
    $$PWD/operation_factory.h

SOURCES += \
    $$PWD/impl/async_operation_invoker.cpp \
    $$PWD/impl/console_util_operation.cpp \
    $$PWD/impl/copy_by_wildcard_operation.cpp \
    $$PWD/operation_factory.cpp

