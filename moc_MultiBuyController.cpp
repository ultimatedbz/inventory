/****************************************************************************
** Meta object code from reading C++ file 'MultiBuyController.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MultiBuyController.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MultiBuyController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MultiBuyController_t {
    QByteArrayData data[6];
    char stringdata[72];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MultiBuyController_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MultiBuyController_t qt_meta_stringdata_MultiBuyController = {
    {
QT_MOC_LITERAL(0, 0, 18), // "MultiBuyController"
QT_MOC_LITERAL(1, 19, 15), // "vegeDropChanged"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 8), // "newIndex"
QT_MOC_LITERAL(4, 45, 10), // "addElement"
QT_MOC_LITERAL(5, 56, 15) // "subtractElement"

    },
    "MultiBuyController\0vegeDropChanged\0\0"
    "newIndex\0addElement\0subtractElement"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MultiBuyController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x0a /* Public */,
       4,    0,   32,    2, 0x0a /* Public */,
       5,    0,   33,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MultiBuyController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MultiBuyController *_t = static_cast<MultiBuyController *>(_o);
        switch (_id) {
        case 0: _t->vegeDropChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->addElement(); break;
        case 2: _t->subtractElement(); break;
        default: ;
        }
    }
}

const QMetaObject MultiBuyController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MultiBuyController.data,
      qt_meta_data_MultiBuyController,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MultiBuyController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MultiBuyController::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MultiBuyController.stringdata))
        return static_cast<void*>(const_cast< MultiBuyController*>(this));
    return QObject::qt_metacast(_clname);
}

int MultiBuyController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE