/****************************************************************************
** Meta object code from reading C++ file 'imenubar.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "imenubar.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'imenubar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_IMenuBar_t {
    QByteArrayData data[19];
    char stringdata[211];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IMenuBar_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IMenuBar_t qt_meta_stringdata_IMenuBar = {
    {
QT_MOC_LITERAL(0, 0, 8), // "IMenuBar"
QT_MOC_LITERAL(1, 9, 7), // "newFile"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 8), // "loadFile"
QT_MOC_LITERAL(4, 27, 4), // "save"
QT_MOC_LITERAL(5, 32, 6), // "saveAs"
QT_MOC_LITERAL(6, 39, 14), // "printInventory"
QT_MOC_LITERAL(7, 54, 12), // "printHistory"
QT_MOC_LITERAL(8, 67, 15), // "changeToEnglish"
QT_MOC_LITERAL(9, 83, 15), // "changeToChinese"
QT_MOC_LITERAL(10, 99, 15), // "showPreferences"
QT_MOC_LITERAL(11, 115, 12), // "addVegetable"
QT_MOC_LITERAL(12, 128, 10), // "addCompany"
QT_MOC_LITERAL(13, 139, 9), // "addPerson"
QT_MOC_LITERAL(14, 149, 7), // "addUnit"
QT_MOC_LITERAL(15, 157, 13), // "removeCompany"
QT_MOC_LITERAL(16, 171, 12), // "removePerson"
QT_MOC_LITERAL(17, 184, 15), // "removeVegetable"
QT_MOC_LITERAL(18, 200, 10) // "removeUnit"

    },
    "IMenuBar\0newFile\0\0loadFile\0save\0saveAs\0"
    "printInventory\0printHistory\0changeToEnglish\0"
    "changeToChinese\0showPreferences\0"
    "addVegetable\0addCompany\0addPerson\0"
    "addUnit\0removeCompany\0removePerson\0"
    "removeVegetable\0removeUnit"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IMenuBar[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   99,    2, 0x0a /* Public */,
       3,    0,  100,    2, 0x0a /* Public */,
       4,    0,  101,    2, 0x0a /* Public */,
       5,    0,  102,    2, 0x0a /* Public */,
       6,    0,  103,    2, 0x0a /* Public */,
       7,    0,  104,    2, 0x0a /* Public */,
       8,    0,  105,    2, 0x0a /* Public */,
       9,    0,  106,    2, 0x0a /* Public */,
      10,    0,  107,    2, 0x0a /* Public */,
      11,    0,  108,    2, 0x0a /* Public */,
      12,    0,  109,    2, 0x0a /* Public */,
      13,    0,  110,    2, 0x0a /* Public */,
      14,    0,  111,    2, 0x0a /* Public */,
      15,    0,  112,    2, 0x0a /* Public */,
      16,    0,  113,    2, 0x0a /* Public */,
      17,    0,  114,    2, 0x0a /* Public */,
      18,    0,  115,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void IMenuBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IMenuBar *_t = static_cast<IMenuBar *>(_o);
        switch (_id) {
        case 0: _t->newFile(); break;
        case 1: _t->loadFile(); break;
        case 2: _t->save(); break;
        case 3: _t->saveAs(); break;
        case 4: _t->printInventory(); break;
        case 5: _t->printHistory(); break;
        case 6: _t->changeToEnglish(); break;
        case 7: _t->changeToChinese(); break;
        case 8: _t->showPreferences(); break;
        case 9: _t->addVegetable(); break;
        case 10: _t->addCompany(); break;
        case 11: _t->addPerson(); break;
        case 12: _t->addUnit(); break;
        case 13: _t->removeCompany(); break;
        case 14: _t->removePerson(); break;
        case 15: _t->removeVegetable(); break;
        case 16: _t->removeUnit(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject IMenuBar::staticMetaObject = {
    { &QMenuBar::staticMetaObject, qt_meta_stringdata_IMenuBar.data,
      qt_meta_data_IMenuBar,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *IMenuBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IMenuBar::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_IMenuBar.stringdata))
        return static_cast<void*>(const_cast< IMenuBar*>(this));
    return QMenuBar::qt_metacast(_clname);
}

int IMenuBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMenuBar::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
