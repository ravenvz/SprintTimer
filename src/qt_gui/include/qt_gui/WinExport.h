#ifndef WINEXPORT_H
#define WINEXPORT_H

#include <QtCore/QtGlobal>

#if defined(WIN_EXPORT_WORKAROUND)
#define GLIB_EXPORT Q_DECL_EXPORT
#else
#define GLIB_EXPORT Q_DECL_IMPORT
#endif

#endif // WINEXPORT_H
